# ESPHome Dew Point Sensor Component

[![GitHub release](https://img.shields.io/github/release/iret33/esphome-dew-point.svg)](https://github.com/iret33/esphome-dew-point/releases)
[![GitHub downloads](https://img.shields.io/github/downloads/iret33/esphome-dew-point/total.svg)](https://github.com/iret33/esphome-dew-point/releases)
[![GitHub stars](https://img.shields.io/github/stars/iret33/esphome-dew-point.svg?style=social)](https://github.com/iret33/esphome-dew-point)
[![GitHub forks](https://img.shields.io/github/forks/iret33/esphome-dew-point.svg?style=social)](https://github.com/iret33/esphome-dew-point/fork)
[![License](https://img.shields.io/github/license/iret33/esphome-dew-point.svg)](LICENSE)

A custom ESPHome component that calculates dew point from existing temperature and humidity sensors using the Magnus formula.

## Features

- 🌡️ Accurate dew point calculation using the Magnus formula
- ⚡ Real-time updates when temperature or humidity changes
- 📊 Integrated as a native ESPHome sensor
- 🔧 Simple YAML configuration
- 🎯 Low memory footprint
- 📈 Proper logging and debugging support

## Installation

### Method 1: Manual Installation

1. Create a `dew_point` folder in your ESPHome `custom_components` directory
2. Copy `dew_point.h` and `__init__.py` to that folder

Your structure should look like:
```
config/
└── esphome/
    └── custom_components/
        └── dew_point/
            ├── __init__.py
            └── dew_point.h
```

### Method 2: External Components (Recommended)

Once published to GitHub, you can use:

```yaml
external_components:
  - source: github://iret33/esphome-dew-point
    components: [ dew_point ]
```

## Configuration

### Basic Configuration

```yaml
sensor:
  # Your existing temperature sensor
  - platform: dht
    pin: GPIO5
    temperature:
      id: room_temperature
      name: "Room Temperature"
    humidity:
      id: room_humidity
      name: "Room Humidity"
    update_interval: 60s

  # Dew point calculation
  - platform: dew_point
    name: "Room Dew Point"
    temperature: room_temperature
    humidity: room_humidity
```

### Advanced Configuration

```yaml
sensor:
  # BME280 sensor example
  - platform: bme280
    temperature:
      id: outdoor_temp
      name: "Outdoor Temperature"
    humidity:
      id: outdoor_humidity
      name: "Outdoor Humidity"
    pressure:
      name: "Outdoor Pressure"
    address: 0x76
    update_interval: 30s

  # Multiple dew point sensors
  - platform: dht
    pin: GPIO4
    temperature:
      id: indoor_temp
      name: "Indoor Temperature"
    humidity:
      id: indoor_humidity
      name: "Indoor Humidity"

  # Outdoor dew point
  - platform: dew_point
    name: "Outdoor Dew Point"
    id: outdoor_dew_point
    temperature: outdoor_temp
    humidity: outdoor_humidity
    accuracy_decimals: 1
    filters:
      - sliding_window_moving_average:
          window_size: 5
          send_every: 1

  # Indoor dew point
  - platform: dew_point
    name: "Indoor Dew Point"
    id: indoor_dew_point
    temperature: indoor_temp
    humidity: indoor_humidity
```

### Complete Device Example

```yaml
esphome:
  name: weather-station
  platform: ESP8266
  board: d1_mini

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:
  level: DEBUG

api:
  encryption:
    key: !secret api_encryption_key

ota:
  password: !secret ota_password

sensor:
  # Temperature and Humidity sensor
  - platform: dht22
    pin: GPIO5
    temperature:
      id: temp_sensor
      name: "DHT22 Temperature"
      filters:
        - offset: -0.5  # Calibration offset
    humidity:
      id: humidity_sensor
      name: "DHT22 Humidity"
    update_interval: 60s

  # Dew Point calculation
  - platform: dew_point
    name: "DHT22 Dew Point"
    temperature: temp_sensor
    humidity: humidity_sensor
    icon: "mdi:water-thermometer"

  # WiFi Signal Strength
  - platform: wifi_signal
    name: "Weather Station WiFi Signal"
    update_interval: 60s
```

## Configuration Variables

- **temperature** (**Required**, ID): The ID of the temperature sensor (must be in Celsius)
- **humidity** (**Required**, ID): The ID of the humidity sensor (must be in percentage 0-100)
- **name** (**Required**, string): The name of the dew point sensor
- **id** (*Optional*, ID): Manually specify the ID for code generation
- **accuracy_decimals** (*Optional*, int): Decimal places for the sensor value. Defaults to 1
- **filters** (*Optional*): Standard ESPHome sensor filters
- **icon** (*Optional*, string): Custom icon. Defaults to temperature icon
- **unit_of_measurement** (*Optional*, string): Unit symbol. Defaults to "°C"

## How It Works

The component uses the Magnus formula to calculate dew point:

```
Td = (b × α) / (a - α)

where:
α = ln(RH/100) + (a × T) / (b + T)
a = 17.27
b = 237.7 °C
T = temperature in °C
RH = relative humidity in %
```

This formula provides accuracy within ±0.4°C for:
- Temperature range: -40°C to 50°C
- Humidity range: 1% to 100%

## Use Cases

### HVAC Control
```yaml
# Prevent condensation
binary_sensor:
  - platform: template
    name: "Condensation Risk"
    lambda: |-
      float dew_point = id(outdoor_dew_point).state;
      float wall_temp = id(wall_temperature).state;
      return (wall_temp < dew_point + 2.0);
```

### Climate Comfort Monitoring
```yaml
# Comfort level indicator
text_sensor:
  - platform: template
    name: "Comfort Level"
    lambda: |-
      float dew_point = id(indoor_dew_point).state;
      if (dew_point < 10) return {"Dry"};
      else if (dew_point < 13) return {"Comfortable"};
      else if (dew_point < 16) return {"Slightly Humid"};
      else if (dew_point < 18) return {"Humid"};
      else if (dew_point < 21) return {"Very Humid"};
      else return {"Oppressive"};
```

### Mold Prevention
```yaml
# Mold risk detection
binary_sensor:
  - platform: template
    name: "Mold Risk"
    lambda: |-
      float dew_point = id(basement_dew_point).state;
      return (dew_point > 15.0);  # High mold risk above 15°C dew point
```

### Automation Example
```yaml
# Turn on dehumidifier when dew point is too high
switch:
  - platform: gpio
    pin: GPIO12
    id: dehumidifier
    name: "Dehumidifier"

automation:
  - platform: numeric_state
    entity_id: sensor.basement_dew_point
    above: 16.0
    then:
      - switch.turn_on: dehumidifier
  - platform: numeric_state
    entity_id: sensor.basement_dew_point
    below: 13.0
    then:
      - switch.turn_off: dehumidifier
```

## Troubleshooting

### Sensor shows NaN or no value

**Cause**: Temperature or humidity sensor not providing valid data

**Solution**: 
- Check that source sensors have valid IDs
- Verify sensors are updating and providing numeric values
- Check ESPHome logs for warnings

### Dew point seems incorrect

**Cause**: Temperature not in Celsius or humidity not in percentage

**Solution**:
- Ensure temperature sensor outputs Celsius (not Fahrenheit)
- Verify humidity is 0-100% (not 0.0-1.0)
- Check for sensor calibration issues

### Component not found

**Cause**: Component not in correct location

**Solution**:
- Verify `custom_components/dew_point/` exists
- Check both `__init__.py` and `dew_point.h` are present
- Restart ESPHome dashboard

### High CPU usage

**Cause**: Too frequent updates

**Solution**:
- Increase `update_interval` on source sensors
- Add filters like `throttle` or `delta`

## Debugging

Enable debug logging:

```yaml
logger:
  level: DEBUG
  logs:
    dew_point: DEBUG
```

This will show:
- Calculated dew point values
- Temperature and humidity inputs
- Any warnings or errors

## Performance

- **Memory**: ~100 bytes RAM
- **CPU**: Minimal (only calculates on sensor updates)
- **Accuracy**: ±0.4°C (within valid range)
- **Update latency**: Instant (triggered by source sensor)

## Comparison with Other Methods

### vs Template Sensor
- ✅ Faster (no YAML parsing)
- ✅ More efficient (compiled C++)
- ✅ Better error handling
- ✅ Native logging

### vs Home Assistant Template
- ✅ Calculated on ESP device
- ✅ No network dependency
- ✅ Works even if HA is down
- ✅ Reduces HA load

## Contributing

Contributions are welcome! Please:
- Follow ESPHome coding standards
- Test thoroughly before submitting
- Update documentation for new features
- Add examples for new use cases

## License

MIT License - See LICENSE file

## References

- [Magnus Formula](https://en.wikipedia.org/wiki/Dew_point#Calculating_the_dew_point)
- [ESPHome Custom Components](https://esphome.io/custom/custom_component.html)
- [ESPHome Sensor Documentation](https://esphome.io/components/sensor/index.html)

## Support

- GitHub Issues: [Report bugs or request features](https://github.com/iret33/esphome-dew-point/issues)
- ESPHome Discord: [Get help from the community](https://discord.gg/KhAMKrd)
- Home Assistant Forum: [Discuss use cases](https://community.home-assistant.io/)

---

Made with ❤️ for the ESPHome community
