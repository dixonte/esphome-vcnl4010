#pragma once

#include "Adafruit_VCNL4010.h"

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
  namespace vcnl4010_i2c_sensor {

    class Vcnl4010I2CSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
      public:
        void setup() override;
        void update() override;
        void dump_config() override;

        void set_proximity_sensor(sensor::Sensor *proximity_sensor) { proximity_sensor_ = proximity_sensor; }
      
      protected:
        sensor::Sensor *proximity_sensor_{nullptr};
    };

  }  // namespace Vcnl4010I2CSensor
}  // namespace esphome