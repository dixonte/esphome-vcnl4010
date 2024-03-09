import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    UNIT_EMPTY
)

CONF_PROXIMITY = 'proximity'

DEPENDENCIES = ['i2c']

CONF_I2C_ADDR = 0x13

vcnl4010_i2c_sensor_ns = cg.esphome_ns.namespace('vcnl4010_i2c_sensor')
Vcnl4010I2CSensor = vcnl4010_i2c_sensor_ns.class_('Vcnl4010I2CSensor', cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Vcnl4010I2CSensor),
            cv.Optional("proximity"): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=1
            ),
        }
    )
    .extend(cv.polling_component_schema('60s'))
    .extend(i2c.i2c_device_schema(CONF_I2C_ADDR))
)

async def to_code(config):
    # cg.add_library(
    #     name="Adafruit BusIO",
    #     repository="https://github.com/adafruit/Adafruit_BusIO",
    #     version="1.15.0"
    # )
    # cg.add_library(
    #     name="Adafruit VCNL",
    #     repository="https://github.com/adafruit/Adafruit_VCNL4010",
    #     version="1.1.2"
    # )

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_PROXIMITY in config:
        sens = await sensor.new_sensor(config[CONF_PROXIMITY])
        cg.add(var.set_proximity_sensor(sens))
