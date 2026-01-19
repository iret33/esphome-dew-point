# Installation Guide - ESPHome Dew Point Component

## Prerequisites

- ESPHome installed (version 2023.1.0 or higher)
- Existing temperature and humidity sensors configured
- Basic understanding of ESPHome YAML configuration

## Installation Methods

### Method 1: Manual Installation (Recommended for Testing)

This method is best for local development and testing.

#### Step 1: Create Directory Structure

In your ESPHome configuration directory, create the custom components folder:

```bash
# Navigate to your ESPHome config directory
cd /config/esphome/

# Create the custom components directory structure
mkdir -p custom_components/dew_point
```

Your directory structure should look like:
```
/config/esphome/
├── custom_components/
│   └── dew_point/
│       ├── __init__.py
│       └── dew_point.h
├── secrets.yaml
└── your-device.yaml
```

#### Step 2: Copy Component Files

Copy the two required files to the `custom_components/dew_point/` directory:
- `__init__.py` - Python configuration file
- `dew_point.h` - C++ header with calculation logic

#### Step 3: Configure Your Device

Edit your device YAML file (e.g., `weather-station.yaml`):

```yaml
esphome:
  name: weather-station
  platform: ESP8266
  board: d1_mini

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:
  level: DEBUG  # Enable debug for testing

api:

ota:

sensor:
  # Your existing temperature sensor
  - platform: dht22
    pin: GPIO5
    temperature:
      id: my_temperature
      name: "Temperature"
    humidity:
      id: my_humidity
      name: "Humidity"
    update_interval: 60s

  # Add dew point sensor
  - platform: dew_point
    name: "Dew Point"
    temperature: my_temperature
    humidity: my_humidity
```

#### Step 4: Compile and Upload

```bash
# Validate configuration
esphome config weather-station.yaml

# Compile and upload
esphome run weather-station.yaml
```

### Method 2: External Components (GitHub)

Once the component is published to GitHub, this is the easiest method.

#### Step 1: Configure External Component

Add to your device YAML:

```yaml
external_components:
  - source: github://iret33/esphome-dew-point
    components: [ dew_point ]
    refresh: 1h  # Check for updates every hour
```

#### Step 2: Use the Component

```yaml
sensor:
  - platform: dht22
    pin: GPIO5
    temperature:
      id: temp
      name: "Temperature"
    humidity:
      id: hum
      name: "Humidity"

  - platform: dew_point
    name: "Dew Point"
    temperature: temp
    humidity: hum
```

#### Step 3: Compile and Upload

ESPHome will automatically download the component from GitHub:

```bash
esphome run weather-station.yaml
```

### Method 3: Local External Components

For development or when you want version control without GitHub.

#### Step 1: Create a Git Repository

```bash
# Create a local repository
mkdir ~/esphome-dew-point
cd ~/esphome-dew-point
git init

# Create component structure
mkdir -p components/dew_point
cp __init__.py components/dew_point/
cp dew_point.h components/dew_point/

# Commit
git add .
git commit -m "Initial commit"
```

#### Step 2: Reference Local Repository

```yaml
external_components:
  - source:
      type: local
      path: /path/to/esphome-dew-point
    components: [ dew_point ]
```

## Verification

### Check Logs

After uploading, monitor the logs:

```bash
esphome logs weather-station.yaml
```

You should see:
```
[C][dew_point:XXX]: Dew Point Sensor:
[C][dew_point:XXX]:   Dew Point 'Dew Point'
[C][dew_point:XXX]:   Temperature Source: 'Temperature'
[C][dew_point:XXX]:   Humidity Source: 'Humidity'
```

### Check Home Assistant

If connected to Home Assistant, check:
1. Go to Developer Tools → States
2. Look for `sensor.weather_station_dew_point`
3. Verify it shows a numeric value

### Test Updates

Change temperature or humidity and watch the dew point update in real-time.

## Troubleshooting

### Component Not Found

**Error**: `Component dew_point not found`

**Solutions**:
1. Verify directory structure:
   ```bash
   ls -la custom_components/dew_point/
   # Should show: __init__.py and dew_point.h
   ```

2. Check file permissions:
   ```bash
   chmod 644 custom_components/dew_point/*
   ```

3. Restart ESPHome dashboard

### Compilation Errors

**Error**: `'log' is not a member of 'std'`

**Solution**: The component uses `<cmath>`. This is included in `dew_point.h`. If you still get errors, check your ESPHome version:

```bash
esphome version
```

Update if needed:
```bash
pip install -U esphome
```

### Sensor Shows NaN

**Issue**: Dew point sensor shows `NaN` or no value

**Causes and Solutions**:

1. **Source sensors not ready**
   - Wait 1-2 minutes after boot
   - Check source sensors are updating

2. **Invalid sensor IDs**
   ```yaml
   # Incorrect
   - platform: dew_point
     temperature: temperature  # Wrong!
     
   # Correct
   - platform: dew_point
     temperature: my_temperature  # Use the sensor ID
   ```

3. **Temperature not in Celsius**
   - Component expects Celsius
   - Add conversion filter if needed:
   ```yaml
   temperature:
     id: my_temp
     filters:
       - multiply: 0.5556  # If Fahrenheit, convert
       - offset: -32
   ```

### Updates Not Working

**Issue**: Dew point doesn't update when temperature/humidity changes

**Solutions**:

1. **Check update intervals match**
   ```yaml
   sensor:
     - platform: dht22
       update_interval: 60s  # Same as dew_point
   ```

2. **Enable debug logging**
   ```yaml
   logger:
     level: DEBUG
     logs:
       dew_point: VERBOSE
   ```

3. **Verify sensor state changes**
   ```
   [D][dew_point:XXX]: Calculated dew point: 12.3°C (T: 20.5°C, RH: 65.0%)
   ```

### High Memory Usage

**Issue**: ESP8266 running out of memory

**Solutions**:

1. **Reduce logging**
   ```yaml
   logger:
     level: WARN
   ```

2. **Disable unnecessary components**

3. **Use ESP32 instead**
   - More RAM available
   - Better for complex setups

### Incorrect Values

**Issue**: Dew point seems wrong

**Verification**:
1. Check temperature is in Celsius
2. Verify humidity is 0-100%
3. Compare with online calculator
4. Check sensor calibration

**Common Issues**:
- Humidity sensor returning 0.65 instead of 65% → multiply by 100
- Temperature in Fahrenheit → convert to Celsius
- Sensor placement → ensure sensors are in same location

## Advanced Configuration

### Multiple Dew Point Sensors

```yaml
sensor:
  # Room 1
  - platform: dht22
    pin: GPIO5
    temperature:
      id: room1_temp
      name: "Room 1 Temp"
    humidity:
      id: room1_hum
      name: "Room 1 Humidity"

  - platform: dew_point
    name: "Room 1 Dew Point"
    temperature: room1_temp
    humidity: room1_hum

  # Room 2
  - platform: dht22
    pin: GPIO4
    temperature:
      id: room2_temp
      name: "Room 2 Temp"
    humidity:
      id: room2_hum
      name: "Room 2 Humidity"

  - platform: dew_point
    name: "Room 2 Dew Point"
    temperature: room2_temp
    humidity: room2_hum
```

### With Filters

```yaml
sensor:
  - platform: dew_point
    name: "Dew Point"
    temperature: my_temp
    humidity: my_hum
    filters:
      # Smooth out readings
      - sliding_window_moving_average:
          window_size: 5
          send_every: 1
      # Only send significant changes
      - delta: 0.5
      # Limit update rate
      - throttle: 30s
```

### Custom Icons and Units

```yaml
sensor:
  - platform: dew_point
    name: "Dew Point"
    temperature: my_temp
    humidity: my_hum
    icon: "mdi:water-thermometer"
    accuracy_decimals: 2
    internal: false  # Make visible to Home Assistant
```

## Integration with Home Assistant

### Auto-Discovery

If using the ESPHome API, sensors are automatically discovered in Home Assistant.

### Manual Configuration

If needed, add to Home Assistant's `configuration.yaml`:

```yaml
sensor:
  - platform: esphome
    host: weather-station.local
    sensors:
      - name: "Weather Station Dew Point"
        id: dew_point
```

### Lovelace Card Example

```yaml
type: entities
title: Weather Data
entities:
  - entity: sensor.weather_station_temperature
  - entity: sensor.weather_station_humidity
  - entity: sensor.weather_station_dew_point
    icon: mdi:water-thermometer
```

## Updating the Component

### Manual Installation

1. Download new files
2. Replace in `custom_components/dew_point/`
3. Recompile and upload

### External Components

Automatic updates based on `refresh` setting:

```yaml
external_components:
  - source: github://iret33/esphome-dew-point@main
    components: [ dew_point ]
    refresh: 1h
```

To force update:
```bash
esphome clean weather-station.yaml
esphome run weather-station.yaml
```

## Uninstallation

### Remove from Device

1. Remove dew_point sensor from YAML
2. Recompile and upload
3. Component code remains but unused

### Remove from File System

```bash
rm -rf custom_components/dew_point/
```

### Remove from Home Assistant

The sensor entity will be marked unavailable and can be removed from the UI.

## Next Steps

- See [EXAMPLES.md](EXAMPLES.md) for complete configuration examples
- Check [README.md](README.md) for usage guides
- Review logs for any warnings or errors
- Calibrate your sensors for best accuracy

## Getting Help

If you encounter issues:

1. Check logs with `esphome logs device.yaml`
2. Enable debug logging
3. Verify sensor IDs and configuration
4. Check GitHub issues
5. Ask on ESPHome Discord or forums

## Common Questions

**Q: Can I use Fahrenheit?**
A: Component expects Celsius. Add a conversion filter if your sensor outputs Fahrenheit.

**Q: How accurate is the calculation?**
A: ±0.4°C within -40°C to 50°C temperature range.

**Q: Does it work with ESP32?**
A: Yes, works with both ESP8266 and ESP32.

**Q: Can I use multiple instances?**
A: Yes, create multiple sensors with different IDs.

**Q: Will this work offline?**
A: Yes, calculation happens on the ESP device, no network needed.

Good luck with your installation! 🎉
