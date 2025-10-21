#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

/** Registers */
#define _VCNL4010_COMMAND 0x80          ///< Command
#define _VCNL4010_PRODUCTID 0x81        ///< Product ID Revision
#define _VCNL4010_PROXRATE 0x82         ///< Proximity rate
#define _VCNL4010_IRLED 0x83            ///< IR LED current
#define _VCNL4010_AMBIENTPARAMETER 0x84 ///< Ambient light parameter
#define _VCNL4010_AMBIENTDATA 0x85      ///< Ambient light result (16 bits)
#define _VCNL4010_PROXIMITYDATA 0x87    ///< Proximity result (16 bits)
#define _VCNL4010_INTCONTROL 0x89       ///< Interrupt control
#define _VCNL4010_LOWTHRESHOLD 0x8A     ///< Low threshold value (16 bits)
#define _VCNL4010_HITHRESHOLD 0x8C      ///< High threshold value (16 bits)
#define _VCNL4010_INTSTAT 0x8E          ///< Interrupt status
#define _VCNL4010_MODTIMING 0x8F ///< Proximity modulator timing adjustment

/** Values for command register */
#define _VCNL4010_MEASUREPROXIMITY                                              \
  0x08 ///< Start a single on-demand proximity measurement
#define _VCNL4010_MEASUREAMBIENT                                                \
  0x10 ///< Start a single on-demand ambient light measurement
#define _VCNL4010_PROXIMITYREADY                                                \
  0x20 ///< Read-only - Value = 1 when proximity measurement data is available
#define _VCNL4010_AMBIENTREADY                                                  \
  0x40 ///< Read-only - Value = 1 when ambient light measurement data is
       ///< available

namespace esphome {
  namespace vcnl4010_freq {
    /** Proximity measurement rate */
    typedef enum {
      mps1_95 = 0,    // 1.95     measurements/sec (Default)
      mps3_90625 = 1, // 3.90625  measurements/sec
      mps7_8125 = 2,  // 7.8125   measurements/sec
      mps16_625 = 3,  // 16.625   measurements/sec
      mps31_25 = 4,   // 31.25    measurements/sec
      mps62_5 = 5,    // 62.5     measurements/sec
      mps125 = 6,     // 125      measurements/sec
      mps250 = 7,     // 250      measurements/sec
    } _vcnl4010_freq;
  }

  namespace vcnl4010_i2c_sensor {

    class Vcnl4010I2CSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
      public:
        void loop() override;

        void setup() override;
        void update() override;
        void dump_config() override;

        void set_proximity_sensor(sensor::Sensor *proximity_sensor) { proximity_sensor_ = proximity_sensor; }
        void set_ambient_sensor(sensor::Sensor *ambient_sensor) { ambient_sensor_ = ambient_sensor; }
      
        void set_led_target_current(uint8_t current_10mA);
        void set_frequency(vcnl4010_freq::_vcnl4010_freq freq);

      protected:
        sensor::Sensor *proximity_sensor_{nullptr};
        sensor::Sensor *ambient_sensor_{nullptr};

        uint8_t status;
        uint8_t ledTargetCurrent;

        void setLEDcurrent(uint8_t current_10mA);
        uint8_t getLEDcurrent(void);

        void requestProximity(void);
        void requestAmbient(void);
        bool tryGetProximity(uint16_t *val);
        bool tryGetAmbient(uint16_t *val);

        uint16_t read_register16(uint8_t address);

        i2c::I2CRegister regProductID = this->reg(_VCNL4010_PRODUCTID);
        i2c::I2CRegister regIRLED = this->reg(_VCNL4010_IRLED);
        i2c::I2CRegister regProxRate = this->reg(_VCNL4010_PROXRATE);
        i2c::I2CRegister regIntControl = this->reg(_VCNL4010_INTCONTROL);
        i2c::I2CRegister regIntStat = this->reg(_VCNL4010_INTSTAT);
        i2c::I2CRegister regCommand = this->reg(_VCNL4010_COMMAND);
    };

  }  // namespace Vcnl4010I2CSensor
}  // namespace esphome