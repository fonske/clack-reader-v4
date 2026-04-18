#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/hal.h"
#include "esphome/components/globals/globals_component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace clack_water_flow {

class WaterFlowSensor : public sensor::Sensor, public Component {
 public:
  void set_pin(InternalGPIOPin *pin) { pin_ = pin; }
  void set_total_water_usage(globals::RestoringGlobalsComponent<float> *g) { total_water_usage_ = g; }
  void set_pulse_per_gallon(globals::RestoringGlobalsComponent<float> *g) { pulse_per_gallon_ = g; }
  void set_water_meter_freeze(globals::RestoringGlobalsComponent<bool> *g) { water_meter_freeze_ = g; }
  void set_watermeter_sensor(sensor::Sensor *s) { watermeter_sensor_ = s; }

  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }

 protected:
  void handle_state_changed_(bool pin_state);

  InternalGPIOPin *pin_{nullptr};
  globals::RestoringGlobalsComponent<float> *total_water_usage_{nullptr};
  globals::RestoringGlobalsComponent<float> *pulse_per_gallon_{nullptr};
  globals::RestoringGlobalsComponent<bool> *water_meter_freeze_{nullptr};
  sensor::Sensor *watermeter_sensor_{nullptr};

  // Tuning constants (matching original water_flow_gallons.h)
  static constexpr double MINIMUM_FLOW_RATE = 0.01;
  static constexpr double NEXT_PULSE_TIMEOUT_BUFFER_SEC = 2.0;
  static constexpr uint32_t SENSOR_DEBOUNCE_MS = 10;
  static constexpr uint32_t PUBLISH_INTERVAL_MS = 300;

  // Runtime state
  double pulse_timeout_sec_{0.0};
  uint32_t last_publish_time_{0};
  bool pulse_timed_out_{false};
  bool current_pin_state_{false};
  double flow_rate_{0.0};
  uint32_t last_pulse_{0};
  bool flow_trigger_state_{false};
};

}  // namespace clack_water_flow
}  // namespace esphome
