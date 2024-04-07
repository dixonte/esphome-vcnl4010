# ![ESPHome logo](esphome-logo.png) VCNL4010 external component

A good-enough-for-me external component to read the VCNL4010 proximity sensor and present its values to ESPHome.

# Usage
You can find a full example in this repository, [example.yaml](example.yaml). However, you will need to change the `source:` as I use this file for local development.

See below for a yaml fragment containing only the relevant details.

```yaml
i2c:
  sda: 21
  scl: 22

external_components:
  source: github://dixonte/esphome-vcnl4010
  components: [vcnl4010_i2c_sensor]

sensor:
  - platform: vcnl4010_i2c_sensor
    update_interval: 1s
    led_target_current: 10
    freq: mps1_95
    proximity:
      name: VCNL4010 proximity sensor
    ambient:
      name: VCNL4010 ambient sensor
```

Depending on your microcontroller of choice, you may be able to use different pins for the I2C bus. Also while in my particular case an `update_interval` of `1s` tested fine, I would not recommend pushing below this value.

`led_target_current` sets the current drawn by the LED of the sensor in 10s of mA, and can vary between 0 (off) and 20 (200 mA). Default value is 20 (200 mA).

`freq` is the frequency of readings, and can be any of the following values:
* "mps1_95" (1.95 measurements/sec, the default)
* "mps3_90625" (3.90625 measurements/sec)
* "mps7_8125" (7.8125 measurements/sec)
* "mps16_625" (16.625 measurements/sec)
* "mps31_25" (32.25 measurements/sec)
* "mps62_5" (62.5 measurements/sec)
* "mps125" (125 measurements/sec)
* "mps250" (250 measurements/sec)

# Special thanks

Thanks to [this project](https://github.com/jesserockz/esphome-external-component-examples/) without which I would have had a much harder time getting started.

Very special thanks to Adafruit, whose [VCNL4010 library](https://github.com/adafruit/Adafruit_VCNL4010) I referenced when creating this component.
