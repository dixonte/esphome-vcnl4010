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
    proximity:
      name: VCNL4010 proximity sensor
    ambient:
      name: VCNL4010 ambient sensor
```

Depending on your microcontroller of choice, you may be able to use different pins for the I2C bus. Also while in my particular case an `update_interval` of `1s` tested fine, I would not recommend pushing below this value.

# Special thanks

Thanks to [this project](https://github.com/jesserockz/esphome-external-component-examples/) without which I would have had a much harder time getting started.

Very special thanks to Adafruit, whose [VCNL4010 library](https://github.com/adafruit/Adafruit_VCNL4010) I referenced when creating this component.
