#include "water_flow_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace clack_water_flow {

static const char *const TAG = "clack_water_flow";

void WaterFlowSensor::setup() {
  pin_->setup();
  // Ensure input + pullup regardless of YAML pin mode specification
  pin_->pin_mode(gpio::FLAG_INPUT | gpio::FLAG_PULLUP);

  // Publish the initial total on boot (mirrors original setup() behaviour)
  if (watermeter_sensor_ != nullptr && total_water_usage_ != nullptr) {
    watermeter_sensor_->publish_state(total_water_usage_->value());
  }
  this->publish_state(0.0f);
}

void WaterFlowSensor::dump_config() {
  LOG_SENSOR("", "Clack Water Flow Sensor", this);
  LOG_PIN("  Pin: ", pin_);
}

void WaterFlowSensor::loop() {
  bool pin_state = pin_->digital_read();
  bool frozen = (water_meter_freeze_ != nullptr) && water_meter_freeze_->value();

  if (!frozen) {
    if (pin_state == current_pin_state_) {
      // No state change — check whether flow has timed out
      if (!pulse_timed_out_ && (millis() - last_pulse_) > (uint32_t)(pulse_timeout_sec_ * 1000)) {
        this->publish_state(0.0f);
        // Prime flow_trigger_state_ so the *next* pulse restarts timing immediately
        flow_trigger_state_ = !current_pin_state_;
        pulse_timed_out_ = true;
      }
      return;
    }
    pulse_timed_out_ = false;
    handle_state_changed_(pin_state);
  }
  // When frozen: do nothing.  current_pin_state_ is NOT updated so that the
  // first pulse after unfreeze is correctly treated as a state change.
}

void WaterFlowSensor::handle_state_changed_(bool pin_state) {
  // Software debounce: the pin must hold the new state for SENSOR_DEBOUNCE_MS
  uint32_t t = millis();
  while (millis() - t < SENSOR_DEBOUNCE_MS) {
    if (pin_->digital_read() != pin_state)
      return;
  }

  // Accumulate total usage on the falling edge (LOW pulse), matching the
  // original "if (!pinState)" logic.
  if (!pin_state && total_water_usage_ != nullptr && pulse_per_gallon_ != nullptr) {
    total_water_usage_->value() += 1.0f / pulse_per_gallon_->value();
  }

  // Calculate flow rate on whichever edge matches flow_trigger_state_
  if (pin_state == flow_trigger_state_) {
    double delta_minutes = (t - last_pulse_) / 60000.0;
    if (pulse_per_gallon_ != nullptr && delta_minutes > 0.0) {
      flow_rate_ = (1.0 / pulse_per_gallon_->value()) / delta_minutes;
    }
    last_pulse_ = t;

    if (flow_rate_ >= MINIMUM_FLOW_RATE) {
      if (millis() - last_publish_time_ >= PUBLISH_INTERVAL_MS) {
        this->publish_state((float) flow_rate_);
        if (watermeter_sensor_ != nullptr && total_water_usage_ != nullptr) {
          watermeter_sensor_->publish_state(total_water_usage_->value());
        }
        last_publish_time_ = millis();
      }
      // Set timeout = expected seconds per pulse + buffer
      if (pulse_per_gallon_ != nullptr && flow_rate_ > 0.0) {
        pulse_timeout_sec_ = (60.0 / (pulse_per_gallon_->value() * flow_rate_)) +
                             NEXT_PULSE_TIMEOUT_BUFFER_SEC;
      }
    }
  }

  current_pin_state_ = pin_state;
}

}  // namespace clack_water_flow
}  // namespace esphome
