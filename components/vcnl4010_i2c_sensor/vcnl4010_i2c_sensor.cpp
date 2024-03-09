#include "esphome/core/log.h"
#include "vcnl4010_i2c_sensor.h"

namespace esphome {
    namespace vcnl4010_i2c_sensor {

        static const char *TAG = "vcnl4010_i2c_sensor.sensor";

        Adafruit_VCNL4010 vcnl;

        void Vcnl4010I2CSensor::setup() {
            ESP_LOGCONFIG(TAG, "Setting up VCNL4010...");
            vcnl.begin();
        }

        void Vcnl4010I2CSensor::update() {
            uint16_t prox = vcnl.readProximity();

            ESP_LOGD(TAG, "Got proximity=%d", prox);

            if (this->proximity_sensor_ != nullptr) {
                this->proximity_sensor_->publish_state(prox);
            }
        }

        void Vcnl4010I2CSensor::dump_config() {
            ESP_LOGD(TAG, "VCNL4010:");
            LOG_I2C_DEVICE(this);
            if (this->is_failed()) {
                ESP_LOGE(TAG, "Communication with VCNL4010 failed!");
            }
            LOG_SENSOR("  ", "Proximity", this->proximity_sensor_);
        }

    }  // namespace Vcnl4010I2CSensor
}  // namespace esphome