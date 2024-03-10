#include "esphome/core/log.h"
#include "vcnl4010_i2c_sensor.h"

namespace esphome {
    namespace vcnl4010_i2c_sensor {

        static const char *TAG = "vcnl4010_i2c_sensor.sensor";

        void Vcnl4010I2CSensor::setup() {
            ESP_LOGCONFIG(TAG, "Setting up VCNL4010...");

            uint8_t rev = this->regProductID.get();
            ESP_LOGD(TAG, "Rev: %x", rev);
            if ((rev & 0xF0) != 0x20) {
                ESP_LOGE(TAG, "Could not set up VCNL4010, revision mismatch!");
            }

            this->setLEDcurrent(20); // 200 mA
            this->setFrequency(_VCNL4010_16_625); // 16.625 readings/second

            this->regIntControl = 0x08;
        }

        void Vcnl4010I2CSensor::update() {
            if (this->proximity_sensor_ != nullptr) {
                uint16_t prox = this->readProximity();
                ESP_LOGD(TAG, "Got proximity=%d", prox);
                this->proximity_sensor_->publish_state(prox);
            }
            if (this->ambient_sensor_ != nullptr) {
                uint16_t amb = this->readAmbient();
                ESP_LOGD(TAG, "Got ambient=%d", amb);
                this->ambient_sensor_->publish_state(amb);
            }
        }

        void Vcnl4010I2CSensor::dump_config() {
            ESP_LOGD(TAG, "VCNL4010:");
            LOG_I2C_DEVICE(this);
            if (this->is_failed()) {
                ESP_LOGE(TAG, "Communication with VCNL4010 failed!");
            }
            LOG_SENSOR("  ", "Proximity", this->proximity_sensor_);
            LOG_SENSOR("  ", "Ambient", this->ambient_sensor_);
        }



        void Vcnl4010I2CSensor::setLEDcurrent(uint8_t current_10mA) {
            if (current_10mA > 20)
                current_10mA = 20;
            this->regIRLED = current_10mA;
        }

        uint8_t Vcnl4010I2CSensor::getLEDcurrent(void) { 
            return this->regIRLED.get();
        }

        void Vcnl4010I2CSensor::setFrequency(_vcnl4010_freq freq) {
            this->regProductID = freq;
        }

        uint16_t Vcnl4010I2CSensor::readProximity(void) {
            uint8_t i = this->regIntStat.get();
            i &= ~0x80;
            this->regIntStat = i;

            this->regCommand = _VCNL4010_MEASUREPROXIMITY;
            while (1) {
                uint8_t result = this->regCommand.get();

                if (result & _VCNL4010_PROXIMITYREADY) {
                    return this->read_register16(_VCNL4010_PROXIMITYDATA);
                }
                delay(1);
            }

            return 0;
        }

        uint16_t Vcnl4010I2CSensor::readAmbient(void) {
            uint8_t i = this->regIntStat.get();
            i &= ~0x80;
            this->regIntStat = i;

            this->regCommand = _VCNL4010_MEASUREAMBIENT;
            while (1) {
                uint8_t result = this->regCommand.get();
                if (result & _VCNL4010_AMBIENTREADY) {
                    return this->read_register16(_VCNL4010_AMBIENTDATA);
                }
                delay(1);
            }
        }

        // Read 2 bytes from VCNL4010 at address
        uint16_t Vcnl4010I2CSensor::read_register16(uint8_t address) {
            uint8_t buffer[2] = {0,0};
            this->read_register(address, buffer, 2, true);
            return (uint16_t(buffer[0]) << 8) | uint16_t(buffer[1]);
        }
    }  // namespace Vcnl4010I2CSensor
}  // namespace esphome