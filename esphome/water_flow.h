#include "esphome.h"

class WaterFlowSensor : public Component, public CustomAPIDevice {
 public:

  /** USER SPECIFIC SETTINGS **/
  const int PIN_NUMBER = 7; // Pin number to detect pulses on
  double USAGE_PER_PULSE = 62; // Amount of pulses that must flow for the water sensor to send one Liter or Gallon
  const double MINIMUM_FLOW_RATE = .01; // .01 Minimum flow rate (in Gallons or Liters per minute) that the flow sensor can recognize. Technically, the slowest consistent flow that the EKM HD meter can read is .22 gal/min. But mine was able to read a flow as slow as .08 gal/min fairly consistently (pulse every ~55 seconds: (60 / (.08/0.075) ).
  const double MAXIMUM_FLOW_RATE = 20;
  const double NEXT_PULSE_TIMEOUT_BUFFER_SEC = 2; // Buffer for waiting for next pulse to tell if water is still running. For example, if we expect the next pulse in 5 seconds, and we don't get a pulse in 5 + NEXT_PULSE_TIMEOUT_BUFFER_SEC seconds, it will report a flow rate of 0 to Home Assistant. If your USAGE_PER_PULSE value is much hight than .075, you will probably want to increase this a bit.
  /****************************/
  const int SENSOR_DEBOUNCE_MS = 10;  //50
  double pulse_timeout_sec = 0;
// Define a constant for the maximum publishing interval
  const int PUBLISH_INTERVAL_MS = 300;
  unsigned long last_publish_time = 0;
  bool pulse_timed_out = false;
  int current_pin_state = 0;
  double flow_rate = 0; // Used to calculate the current flow rate in Gallons or Liters per minute
  unsigned long last_pulse = 0;
  double delta_minutes = 0;
  int flowTriggerState = 0;
  unsigned long time = 0;
  Sensor *clack_flow_rate = new Sensor();

  void setup() override {
    pinMode(PIN_NUMBER, INPUT_PULLUP);

    // Publish the initial state of totalWaterUsage on boot
    id(clack_watermeter).publish_state(id(totalWaterUsage));
    clack_flow_rate->publish_state(0);
  }

  void loop() override {    
    int pinState = digitalRead(PIN_NUMBER);
    
    if (id(water_meter_freeze) == false) {
      // Don't do anything if sensor has not pulsed
      if (pinState == current_pin_state) { 
        if (!pulse_timed_out && ((millis() - last_pulse) > (pulse_timeout_sec * 1000))) {
          // Publish no flow after pulses have stopped
          clack_flow_rate->publish_state(0);
          flowTriggerState = !current_pin_state; // to calculate the flow rate sooner, we will use the next pulse as the "first" pulse, whether it's high or low
          pulse_timed_out = true; // set this so we only publish once when the flow stops
//          id(clack_watermeter).publish_state(id(totalWaterUsage));
        }
        return; 
      }
    }
    pulse_timed_out = false;
    StateChanged(pinState);
  }

  // Pulse from meter
  void StateChanged(int pinState) {
    // Add a debounce filter
    time = millis();
    while (millis() - time < SENSOR_DEBOUNCE_MS) {
      if (digitalRead(PIN_NUMBER) != pinState) return; 
    }

    if (id(water_meter_freeze) == false) {
    // Only increment the total_water_used on HIGH pulse
      if (!pinState) { 
        id(totalWaterUsage) += 1/id(pulse_per_gallon); 
      }

      if (pinState == flowTriggerState) {
        // Calculate flow rate
        delta_minutes = ((time - last_pulse) / 60000.0);
        flow_rate = (1/id(pulse_per_liter) / delta_minutes);
        last_pulse = time;

        // If the calculated flow rate is less the the minimum flow that the meter can read, we wont say there is a real flow just yet.
        // This could be the first of a set of new pulses, or sometimes water just slowly moves back and forth in the pipes. 
        if (flow_rate >= MINIMUM_FLOW_RATE) {
          // publish flow_rate and total_water_used to Home Assistant
          // Check if 500 ms have passed since the last publish
          if (millis() - last_publish_time >= PUBLISH_INTERVAL_MS) {
            // Publish flow_rate and total_water_used to Home Assistant
            clack_flow_rate->publish_state(flow_rate);
            id(clack_watermeter).publish_state(id(totalWaterUsage));
            // Update the last publish time
            last_publish_time = millis();
          }
          // Set a timeout for the next expected pulse + a buffer, depending of the current flow rate.
          // For example, if the water is flowing at 1GPM, that is 13.33PPM, or a pulse every 4.5 seconds.
          // We would set the pulse_timeout_sec to 4.5 + NEXT_PULSE_TIMEOUT_BUFFER_SEC and if we don't 
          // get a pulse by then, we say the flow has stopped.
          pulse_timeout_sec = (60/((1/(1/id(pulse_per_galon))) * flow_rate)) + NEXT_PULSE_TIMEOUT_BUFFER_SEC;
        }
      }
    current_pin_state = pinState;
    }
  }
};
