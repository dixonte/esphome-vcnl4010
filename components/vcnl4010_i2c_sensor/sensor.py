import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    UNIT_EMPTY,
    DEVICE_CLASS_DISTANCE,
    DEVICE_CLASS_ILLUMINANCE,
    STATE_CLASS_MEASUREMENT
)

CONF_PROXIMITY = 'proximity'
CONF_AMBIENT = 'ambient'
CONF_LED_TARGET_CURRENT = 'led_target_current'
CONF_FREQ = 'freq'

DEPENDENCIES = ['i2c']

CONF_I2C_ADDR = 0x13

vcnl4010_i2c_sensor_ns = cg.esphome_ns.namespace('vcnl4010_i2c_sensor')
Vcnl4010I2CSensor = vcnl4010_i2c_sensor_ns.class_('Vcnl4010I2CSensor', cg.PollingComponent, i2c.I2CDevice)

freq_ns = cg.esphome_ns.namespace('vcnl4010_freq')
FreqMode = freq_ns.enum("_vcnl4010_freq", is_class=False)
FREQ_MODES = {
    "mps1_95": FreqMode.mps1_95,
    "mps3_90625": FreqMode.mps3_90625,
    "mps7_8125": FreqMode.mps7_8125,
    "mps16_625": FreqMode.mps16_625,
    "mps31_25": FreqMode.mps31_25,
    "mps62_5": FreqMode.mps62_5,
    "mps125": FreqMode.mps125,
    "mps250": FreqMode.mps250,
}

def validate_led_target_current(value):
    if (not isinstance(value, int)) or value < 0 or value > 20:
        raise cv.Invalid(f"{CONF_LED_TARGET_CURRENT}: must be an integer between 0 and 20.")
    return value

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Vcnl4010I2CSensor),
            # default 200mA
            cv.Optional(CONF_LED_TARGET_CURRENT, default=20): validate_led_target_current,
            # default 1.95 measurements/sec
            cv.Optional(CONF_FREQ, default="mps1_95"): cv.enum(
                FREQ_MODES, lower=True, space="_"
            ),
            cv.Optional(CONF_PROXIMITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT
            ),
            cv.Optional(CONF_AMBIENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_ILLUMINANCE,
                state_class=STATE_CLASS_MEASUREMENT
            ),
        }
    )
    .extend(cv.polling_component_schema('60s'))
    .extend(i2c.i2c_device_schema(CONF_I2C_ADDR))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_LED_TARGET_CURRENT in config:
        cg.add(var.set_led_target_current(config[CONF_LED_TARGET_CURRENT]))

    if CONF_FREQ in config:
        cg.add(var.set_frequency(config[CONF_FREQ]))

    if CONF_PROXIMITY in config:
        sens = await sensor.new_sensor(config[CONF_PROXIMITY])
        cg.add(var.set_proximity_sensor(sens))

    if CONF_AMBIENT in config:
        sens = await sensor.new_sensor(config[CONF_AMBIENT])
        cg.add(var.set_ambient_sensor(sens))
