#include "esphome.h"

#include <Wire.h>
#include <VL53L1X.h>

VL53L1X tof_sensor;

class MyCustomSensor : public PollingComponent, public Sensor {
 public:
  // constructor
  MyCustomSensor() : PollingComponent(5000) {} // polling every 5s

  void setup() override {
    // This will be called by App.setup()
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    tof_sensor.setTimeout(1000);
    tof_sensor.setAddress(0x29);
    if (!tof_sensor.init()) {
      ESP_LOGE("VL53L1X custom sensor", "Failed to detect and initialize sensor!");
      while (1);
    }

    tof_sensor.setDistanceMode(VL53L1X::Short);
    tof_sensor.setMeasurementTimingBudget(500000);

    tof_sensor.startContinuous(50);
  }

  void update() override {
    uint16_t mm = tof_sensor.read();
    
    if (!tof_sensor.timeoutOccurred()) {
      publish_state(mm);
    } else {
      ESP_LOGE("VL53L1X custom sensor", "Timeout during read().");
    }
  }
};