# Quick Reference - ESPHome Dew Point Component

## Basic Configuration

```yaml
sensor:
  - platform: dht22
    temperature:
      id: temp
    humidity:
      id: hum
  
  - platform: dew_point
    name: "Dew Point"
    temperature: temp
    humidity: hum
```

## Component Parameters

| Parameter | Required | Description | Default |
|-----------|----------|-------------|---------|
| `temperature` | ✅ | Temperature sensor ID | - |
| `humidity` | ✅ | Humidity sensor ID | - |
| `name` | ✅ | Sensor name | - |
| `id` | ❌ | Internal ID | Auto |
| `accuracy_decimals` | ❌ | Decimal places | 1 |
| `icon` | ❌ | Custom icon | `mdi:thermometer` |
| `filters` | ❌ | Sensor filters | None |

## Requirements

- **Temperature**: Must be in Celsius
- **Humidity**: Must be 0-100%
- **ESPHome**: Version 2023.1.0+
- **Platform**: ESP8266 or ESP32

## Dew Point Ranges

| Dew Point (°C) | Comfort Level | Condition |
|----------------|---------------|-----------|
| < 10 | Dry | Very comfortable |
| 10-13 | Comfortable | Pleasant |
| 13-16 | Slightly humid | Still comfortable |
| 16-18 | Somewhat uncomfortable | Getting humid |
| 18-21 | Uncomfortable | Humid |
| 21-24 | Very uncomfortable | Very humid |
| > 24 | Extremely uncomfortable | Oppressive |

## Common Use Cases

### Mold Prevention
```yaml
binary_sensor:
  - platform: template
    name: "Mold Risk"
    lambda: |-
      return id(dew_point).state > 15.0;
```

### Condensation Alert
```yaml
binary_sensor:
  - platform: template
    name: "Condensation Risk"
    lambda: |-
      float dew = id(dew_point).state;
      float surface = id(surface_temp).state;
      return (surface - dew) < 2.0;
```

### HVAC Control
```yaml
automation:
  - platform: numeric_state
    entity_id: sensor.dew_point
    above: 16.0
    then:
      - switch.turn_on: dehumidifier
```

## Troubleshooting Quick Fixes

| Problem | Quick Fix |
|---------|-----------|
| Shows NaN | Check sensor IDs match |
| Not updating | Verify source sensors update |
| Wrong values | Ensure temp in °C, humidity in % |
| Component not found | Check file locations |

## Formula

```
Dew Point = (b × α) / (a - α)

where:
α = ln(RH/100) + (a × T) / (b + T)
a = 17.27
b = 237.7
T = temperature (°C)
RH = relative humidity (%)
```

## Accuracy

- **Range**: -40°C to 50°C
- **Precision**: ±0.4°C
- **Update**: Instant (on sensor change)
- **Memory**: ~100 bytes

## File Locations

### Manual Install
```
/config/esphome/
└── custom_components/
    └── dew_point/
        ├── __init__.py
        └── dew_point.h
```

### GitHub Install
```yaml
external_components:
  - source: github://username/esphome-dew-point
    components: [ dew_point ]
```

## Logging

Enable debug output:
```yaml
logger:
  logs:
    dew_point: DEBUG
```

Output format:
```
[D][dew_point:123]: Calculated dew point: 12.3°C (T: 20.5°C, RH: 65.0%)
```

## Multiple Sensors

```yaml
sensor:
  # Room 1
  - platform: dew_point
    name: "Room 1 Dew Point"
    temperature: room1_temp
    humidity: room1_hum

  # Room 2
  - platform: dew_point
    name: "Room 2 Dew Point"
    temperature: room2_temp
    humidity: room2_hum
```

## Filters

```yaml
- platform: dew_point
  name: "Dew Point"
  temperature: temp
  humidity: hum
  filters:
    # Smooth readings
    - sliding_window_moving_average:
        window_size: 5
    # Only send changes > 0.5°C
    - delta: 0.5
    # Max 1 update per minute
    - throttle: 60s
```

## Integration with Home Assistant

Auto-discovered as:
```
sensor.device_name_dew_point
```

Device class: `temperature`
Unit: `°C`
State class: `measurement`

## Quick Commands

```bash
# Validate config
esphome config device.yaml

# Compile only
esphome compile device.yaml

# Upload OTA
esphome upload device.yaml

# View logs
esphome logs device.yaml

# Clean build
esphome clean device.yaml
```

## Resources

- GitHub: https://github.com/username/esphome-dew-point
- Docs: See README.md
- Examples: See EXAMPLES.md
- Install: See INSTALLATION.md

## Support

- ESPHome Discord: https://discord.gg/KhAMKrd
- GitHub Issues: Report bugs
- Community Forum: https://community.home-assistant.io/

---

**Version**: 1.0.0  
**License**: MIT  
**Author**: Your Name
