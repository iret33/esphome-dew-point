# ESPHome Dew Point Sensor - Example Configurations

## Example 1: Basic DHT22 Setup

```yaml
esphome:
  name: basic-dew-point
  platform: ESP8266
  board: nodemcuv2

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

sensor:
  - platform: dht
    pin: GPIO5
    temperature:
      id: my_temperature
      name: "Temperature"
    humidity:
      id: my_humidity
      name: "Humidity"
    update_interval: 60s

  - platform: dew_point
    name: "Dew Point"
    temperature: my_temperature
    humidity: my_humidity
```

## Example 2: BME280 with Multiple Rooms

```yaml
esphome:
  name: multi-room-monitor
  platform: ESP32
  board: esp32dev

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

sensor:
  # Living Room - BME280 on 0x76
  - platform: bme280
    temperature:
      id: living_temp
      name: "Living Room Temperature"
    humidity:
      id: living_humidity
      name: "Living Room Humidity"
    pressure:
      name: "Living Room Pressure"
    address: 0x76
    update_interval: 30s

  - platform: dew_point
    name: "Living Room Dew Point"
    temperature: living_temp
    humidity: living_humidity

  # Bedroom - BME280 on 0x77
  - platform: bme280
    temperature:
      id: bedroom_temp
      name: "Bedroom Temperature"
    humidity:
      id: bedroom_humidity
      name: "Bedroom Humidity"
    pressure:
      name: "Bedroom Pressure"
    address: 0x77
    update_interval: 30s

  - platform: dew_point
    name: "Bedroom Dew Point"
    temperature: bedroom_temp
    humidity: bedroom_humidity
```

## Example 3: Greenhouse Monitoring with Alerts

```yaml
esphome:
  name: greenhouse-monitor
  platform: ESP8266
  board: d1_mini

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

sensor:
  - platform: dht22
    pin: GPIO5
    temperature:
      id: greenhouse_temp
      name: "Greenhouse Temperature"
      filters:
        - sliding_window_moving_average:
            window_size: 5
            send_every: 1
    humidity:
      id: greenhouse_humidity
      name: "Greenhouse Humidity"
      filters:
        - sliding_window_moving_average:
            window_size: 5
            send_every: 1
    update_interval: 30s

  - platform: dew_point
    name: "Greenhouse Dew Point"
    id: greenhouse_dew_point
    temperature: greenhouse_temp
    humidity: greenhouse_humidity

binary_sensor:
  # High humidity/condensation risk
  - platform: template
    name: "Greenhouse Condensation Risk"
    lambda: |-
      float dew_point = id(greenhouse_dew_point).state;
      float temperature = id(greenhouse_temp).state;
      // Risk if temperature is within 2°C of dew point
      return (temperature - dew_point) < 2.0;

switch:
  - platform: gpio
    pin: GPIO12
    name: "Greenhouse Fan"
    id: greenhouse_fan

automation:
  # Turn on fan when condensation risk is high
  - platform: template
    lambda: |-
      float dew_point = id(greenhouse_dew_point).state;
      float temperature = id(greenhouse_temp).state;
      return (temperature - dew_point) < 2.0;
    then:
      - switch.turn_on: greenhouse_fan
  
  # Turn off fan when safe
  - platform: template
    lambda: |-
      float dew_point = id(greenhouse_dew_point).state;
      float temperature = id(greenhouse_temp).state;
      return (temperature - dew_point) > 5.0;
    then:
      - switch.turn_off: greenhouse_fan
```

## Example 4: Basement Mold Prevention System

```yaml
esphome:
  name: basement-monitor
  platform: ESP32
  board: esp32dev

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

sensor:
  - platform: dht22
    pin: GPIO5
    temperature:
      id: basement_temp
      name: "Basement Temperature"
    humidity:
      id: basement_humidity
      name: "Basement Humidity"
    update_interval: 60s

  - platform: dew_point
    name: "Basement Dew Point"
    id: basement_dew_point
    temperature: basement_temp
    humidity: basement_humidity

text_sensor:
  # Mold risk level
  - platform: template
    name: "Basement Mold Risk"
    update_interval: 60s
    lambda: |-
      float dew_point = id(basement_dew_point).state;
      float humidity = id(basement_humidity).state;
      
      if (dew_point > 20 || humidity > 70) {
        return {"High Risk"};
      } else if (dew_point > 15 || humidity > 60) {
        return {"Moderate Risk"};
      } else if (dew_point > 10 || humidity > 50) {
        return {"Low Risk"};
      } else {
        return {"Minimal Risk"};
      }

switch:
  - platform: gpio
    pin: GPIO12
    name: "Basement Dehumidifier"
    id: dehumidifier

automation:
  # High risk - turn on dehumidifier
  - platform: numeric_state
    entity_id: sensor.basement_dew_point
    above: 16.0
    then:
      - switch.turn_on: dehumidifier
  
  # Safe level - turn off dehumidifier
  - platform: numeric_state
    entity_id: sensor.basement_dew_point
    below: 12.0
    then:
      - switch.turn_off: dehumidifier
```

## Example 5: Weather Station with Comprehensive Data

```yaml
esphome:
  name: weather-station
  platform: ESP32
  board: esp32dev

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

i2c:
  sda: GPIO21
  scl: GPIO22

sensor:
  # BME280 for temperature, humidity, and pressure
  - platform: bme280
    temperature:
      id: outdoor_temp
      name: "Outdoor Temperature"
      accuracy_decimals: 1
    humidity:
      id: outdoor_humidity
      name: "Outdoor Humidity"
      accuracy_decimals: 0
    pressure:
      name: "Atmospheric Pressure"
      accuracy_decimals: 1
    address: 0x76
    update_interval: 60s

  # Dew point calculation
  - platform: dew_point
    name: "Outdoor Dew Point"
    id: outdoor_dew_point
    temperature: outdoor_temp
    humidity: outdoor_humidity
    icon: "mdi:water-thermometer"

  # Heat index (feels like temperature)
  - platform: template
    name: "Heat Index"
    lambda: |-
      float T = id(outdoor_temp).state;
      float RH = id(outdoor_humidity).state;
      
      // Simple heat index formula (Fahrenheit base)
      float T_f = T * 9.0/5.0 + 32.0;
      float HI = -42.379 + 2.04901523*T_f + 10.14333127*RH 
                 - 0.22475541*T_f*RH - 0.00683783*T_f*T_f 
                 - 0.05481717*RH*RH + 0.00122874*T_f*T_f*RH 
                 + 0.00085282*T_f*RH*RH - 0.00000199*T_f*T_f*RH*RH;
      
      // Convert back to Celsius
      return (HI - 32.0) * 5.0/9.0;
    unit_of_measurement: "°C"
    accuracy_decimals: 1
    update_interval: 60s

  # Absolute humidity (g/m³)
  - platform: template
    name: "Absolute Humidity"
    lambda: |-
      float T = id(outdoor_temp).state;
      float RH = id(outdoor_humidity).state;
      
      // Calculate saturation vapor pressure
      float es = 6.112 * exp((17.67 * T) / (T + 243.5));
      // Calculate actual vapor pressure
      float e = es * (RH / 100.0);
      // Calculate absolute humidity
      float ah = (216.7 * e) / (T + 273.15);
      
      return ah;
    unit_of_measurement: "g/m³"
    accuracy_decimals: 1
    update_interval: 60s

  # WiFi Signal
  - platform: wifi_signal
    name: "Weather Station WiFi Signal"
    update_interval: 60s

text_sensor:
  # Comfort assessment
  - platform: template
    name: "Comfort Level"
    update_interval: 60s
    lambda: |-
      float dew_point = id(outdoor_dew_point).state;
      
      if (dew_point < 10) {
        return {"Dry - Low humidity"};
      } else if (dew_point < 13) {
        return {"Comfortable - Pleasant"};
      } else if (dew_point < 16) {
        return {"Comfortable - Slightly humid"};
      } else if (dew_point < 18) {
        return {"Somewhat uncomfortable"};
      } else if (dew_point < 21) {
        return {"Uncomfortable - Humid"};
      } else if (dew_point < 24) {
        return {"Very uncomfortable - Very humid"};
      } else {
        return {"Extremely uncomfortable - Oppressive"};
      }
```

## Example 6: HVAC Control with Condensation Prevention

```yaml
esphome:
  name: hvac-controller
  platform: ESP8266
  board: nodemcuv2

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

dallas:
  - pin: GPIO4  # OneWire for wall temperature

sensor:
  # Indoor conditions
  - platform: dht22
    pin: GPIO5
    temperature:
      id: indoor_temp
      name: "Indoor Temperature"
    humidity:
      id: indoor_humidity
      name: "Indoor Humidity"
    update_interval: 30s

  - platform: dew_point
    name: "Indoor Dew Point"
    id: indoor_dew_point
    temperature: indoor_temp
    humidity: indoor_humidity

  # Wall temperature (to detect condensation risk)
  - platform: dallas
    address: 0x1234567890abcdef  # Your DS18B20 address
    id: wall_temp
    name: "Wall Temperature"

  # Temperature difference
  - platform: template
    name: "Wall Temperature Margin"
    lambda: |-
      float wall = id(wall_temp).state;
      float dew_point = id(indoor_dew_point).state;
      return wall - dew_point;
    unit_of_measurement: "°C"
    accuracy_decimals: 1

binary_sensor:
  # Condensation risk indicator
  - platform: template
    name: "Condensation Risk"
    lambda: |-
      float wall = id(wall_temp).state;
      float dew_point = id(indoor_dew_point).state;
      // Risk if wall is within 2°C of dew point
      return (wall - dew_point) < 2.0;

climate:
  - platform: bang_bang
    name: "HVAC Controller"
    sensor: indoor_temp
    default_target_temperature_low: 20
    default_target_temperature_high: 24
    heat_action:
      - switch.turn_on: heater
    cool_action:
      - switch.turn_on: cooler
    idle_action:
      - switch.turn_off: heater
      - switch.turn_off: cooler

switch:
  - platform: gpio
    pin: GPIO12
    name: "Heater"
    id: heater

  - platform: gpio
    pin: GPIO13
    name: "Cooler"
    id: cooler

  - platform: gpio
    pin: GPIO14
    name: "Ventilation Fan"
    id: vent_fan

automation:
  # Emergency ventilation if condensation risk is high
  - platform: template
    lambda: |-
      float wall = id(wall_temp).state;
      float dew_point = id(indoor_dew_point).state;
      return (wall - dew_point) < 1.0;
    then:
      - switch.turn_on: vent_fan
      - logger.log: "HIGH CONDENSATION RISK - Ventilation activated"
  
  # Turn off ventilation when safe
  - platform: template
    lambda: |-
      float wall = id(wall_temp).state;
      float dew_point = id(indoor_dew_point).state;
      return (wall - dew_point) > 5.0;
    then:
      - switch.turn_off: vent_fan
```

## Example 7: External Components (GitHub Integration)

```yaml
esphome:
  name: dew-point-device
  platform: ESP8266
  board: d1_mini

# Use component from GitHub
external_components:
  - source: github://iret33/esphome-dew-point
    components: [ dew_point ]
    refresh: 1h

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

logger:

api:

ota:

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

---

**Note**: All examples assume you've properly installed the dew_point component. Replace `iret33` with your actual GitHub username for external components.
