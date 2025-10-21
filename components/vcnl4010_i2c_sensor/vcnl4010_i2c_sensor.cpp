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

            this->setLEDcurrent(0);

            this->regIntControl = 0x08;

            this->status = 0;
        }

        void Vcnl4010I2CSensor::loop() {
            if (this->status == _VCNL4010_MEASUREPROXIMITY) {
                uint16_t prox;
                if (this->tryGetProximity(&prox)) {
                    this->proximity_sensor_->publish_state(prox);

                    this->status = 0;
                    if (this->ambient_sensor_ != nullptr) {
                        this->requestAmbient();
                    }
                }
            }

            if (this->status == _VCNL4010_MEASUREAMBIENT) {
                uint16_t amb;
                if (this->tryGetAmbient(&amb)) {
                    this->ambient_sensor_->publish_state(amb);

                    this->status = 0;
                }
            }
        }

        void Vcnl4010I2CSensor::update() {
            if (this->proximity_sensor_ != nullptr) {
                this->requestProximity();
            }
            else if (this->ambient_sensor_ != nullptr) {
                this->requestAmbient();
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

        void Vcnl4010I2CSensor::set_led_target_current(uint8_t current_10mA) {
            this->ledTargetCurrent = current_10mA;
            ESP_LOGD(TAG, "LED target current set to %d mA", current_10mA * 10);
        }

        void Vcnl4010I2CSensor::set_frequency(vcnl4010_freq::_vcnl4010_freq freq) {
            this->regProxRate = freq;
            ESP_LOGD(TAG, "Frequency set to %d", freq);
        }



        void Vcnl4010I2CSensor::setLEDcurrent(uint8_t current_10mA) {
            if (current_10mA > 20)
                current_10mA = 20;
            this->regIRLED = current_10mA;
            ESP_LOGD(TAG, "LED current set to %d mA", current_10mA * 10);
        }

        uint8_t Vcnl4010I2CSensor::getLEDcurrent(void) { 
            return this->regIRLED.get();
        }

        void Vcnl4010I2CSensor::requestProximity(void) {
            if (this->status == 0) {
                this->setLEDcurrent(this->ledTargetCurrent);

                uint8_t i = this->regIntStat.get();
                i &= ~0x80;
                this->regIntStat = i;

                this->regCommand = _VCNL4010_MEASUREPROXIMITY;
                this->status = _VCNL4010_MEASUREPROXIMITY;
            }
        }

        void Vcnl4010I2CSensor::requestAmbient(void) {
            if (this->status == 0) {
                uint8_t i = this->regIntStat.get();
                i &= ~0x80;
                this->regIntStat = i;

                this->regCommand = _VCNL4010_MEASUREAMBIENT;
                this->status = _VCNL4010_MEASUREAMBIENT;
            }
        }

        bool Vcnl4010I2CSensor::tryGetProximity(uint16_t *val) {
            uint8_t result = this->regCommand.get();
            if (result & _VCNL4010_PROXIMITYREADY) {
                uint16_t prox = this->read_register16(_VCNL4010_PROXIMITYDATA);
                ESP_LOGD(TAG, "Got proximity=%d", prox);
                *val = prox;
                this->setLEDcurrent(0);
                return true;
            }
            return false;
        }

        bool Vcnl4010I2CSensor::tryGetAmbient(uint16_t *val) {
            uint8_t result = this->regCommand.get();
            if (result & _VCNL4010_AMBIENTREADY) {
                uint16_t amb = this->read_register16(_VCNL4010_AMBIENTDATA);
                ESP_LOGD(TAG, "Got ambient=%d", amb);
                *val = amb;
                return true;
            }
            return false;
        }

        // Read 2 bytes from VCNL4010 at address
        uint16_t Vcnl4010I2CSensor::read_register16(uint8_t address) {
            uint8_t buffer[2] = {0,0};
            this->read_register(address, buffer, 2);
            return (uint16_t(buffer[0]) << 8) | uint16_t(buffer[1]);
        }
    }  // namespace Vcnl4010I2CSensor
}  // namespace esphome
