# Changelog

All notable changes to the ESPHome Dew Point Component will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-01-19

### Added
- Initial release of Dew Point Component
- Magnus formula implementation for dew point calculation
- Real-time updates based on temperature and humidity sensor changes
- Support for ESP8266 and ESP32 platforms
- Proper error handling for invalid sensor values
- Debug logging support
- Component configuration via YAML
- Auto-discovery in Home Assistant
- Comprehensive documentation and examples

### Features
- Accurate calculation within ±0.4°C for -40°C to 50°C range
- Low memory footprint (~100 bytes RAM)
- Instant updates on sensor state changes
- Support for multiple dew point sensors per device
- Native ESPHome sensor integration
- Proper state attributes and device class

### Documentation
- Complete README with use cases and troubleshooting
- Detailed installation guide
- Multiple example configurations
- API reference

## [Unreleased]

### Planned Features
- Fahrenheit input support with automatic conversion
- Absolute humidity calculation option
- Heat index calculation
- Wet bulb temperature calculation
- Enhanced comfort level indicators
- Multi-language support for documentation

### Under Consideration
- Configuration option for different dew point formulas
- Calibration offset parameters
- Alert thresholds with binary sensor output
- Statistical tracking (min/max/average)

---

## Version History

- **1.0.0** (2025-01-19) - Initial stable release

## Upgrade Guide

### From Pre-release to 1.0.0
No breaking changes. Simply update the component files.

## Support

For issues, questions, or feature requests, please visit:
- GitHub Issues: https://github.com/iret33/esphome-dew-point/issues
- ESPHome Discord: https://discord.gg/KhAMKrd
