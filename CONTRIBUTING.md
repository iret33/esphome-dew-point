# Contributing to ESPHome Dew Point Component

Thank you for your interest in contributing! This document provides guidelines for contributing to this project.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:

1. **Clear title** describing the problem
2. **ESPHome version** you're using
3. **Hardware** (ESP8266/ESP32, sensor type)
4. **Configuration** (sanitized YAML)
5. **Expected behavior** vs **actual behavior**
6. **Logs** showing the issue (with sensitive data removed)
7. **Steps to reproduce**

### Suggesting Enhancements

Enhancement suggestions are welcome! Please include:

1. **Clear description** of the enhancement
2. **Use case** - why it would be useful
3. **Expected behavior**
4. **Possible implementation** (if you have ideas)

### Pull Requests

We actively welcome pull requests!

#### Before You Start

1. Check existing issues and PRs
2. Discuss major changes in an issue first
3. Fork the repository
4. Create a feature branch

#### Development Setup

```bash
# Clone your fork
git clone https://github.com/iret33/esphome-dew-point.git
cd esphome-dew-point

# Create a branch
git checkout -b feature/my-feature
```

#### Making Changes

1. **Code Style**
   - Follow ESPHome coding standards
   - Use meaningful variable names
   - Add comments for complex logic
   - Keep functions focused and small

2. **C++ Guidelines** (dew_point.h)
   - Use `const` where appropriate
   - Initialize variables
   - Check for nullptr before use
   - Handle edge cases (NaN, infinity, out of range)
   - Use appropriate logging levels

3. **Python Guidelines** (__init__.py)
   - Follow PEP 8 style guide
   - Use type hints where possible
   - Validate configuration early
   - Provide helpful error messages

#### Testing

Before submitting, test your changes:

1. **Compile test**
   ```bash
   esphome config test-config.yaml
   ```

2. **Runtime test**
   - Flash to actual hardware
   - Verify sensor values
   - Check logs for errors
   - Test edge cases

3. **Test scenarios**
   - Normal operation
   - Sensor unavailable
   - Invalid values (NaN, negative, >100% humidity)
   - Rapid updates
   - Long-term stability

#### Documentation

Update documentation for any changes:

- README.md - Main documentation
- EXAMPLES.md - Add new examples
- INSTALLATION.md - Update if needed
- CHANGELOG.md - Document changes
- Code comments - Explain complex logic

#### Commit Messages

Use clear, descriptive commit messages:

```
Add heat index calculation feature

- Implement heat index formula
- Add configuration options
- Update documentation
- Add example configuration
```

Format:
- First line: Brief summary (50 chars max)
- Blank line
- Detailed description with bullet points

#### Submitting

1. **Push to your fork**
   ```bash
   git push origin feature/my-feature
   ```

2. **Create Pull Request**
   - Clear title describing the change
   - Reference any related issues
   - Describe what changed and why
   - Include testing results
   - Screenshots if relevant

3. **PR Checklist**
   - [ ] Code compiles without errors
   - [ ] Tested on actual hardware
   - [ ] Documentation updated
   - [ ] CHANGELOG.md updated
   - [ ] No unnecessary formatting changes
   - [ ] Commit messages are clear

## Code Review Process

1. Maintainers will review your PR
2. Address any feedback or requested changes
3. Once approved, it will be merged
4. Your contribution will be credited

## Development Guidelines

### Code Organization

```
esphome-dew-point/
├── dew_point.h           # C++ implementation
├── __init__.py           # Python config
├── README.md             # Main docs
├── EXAMPLES.md           # Example configs
└── INSTALLATION.md       # Setup guide
```

### Adding New Features

When adding features:

1. **Keep it simple** - Don't over-engineer
2. **Maintain compatibility** - Don't break existing configs
3. **Document thoroughly** - Update all relevant docs
4. **Add examples** - Show how to use the feature
5. **Test extensively** - Verify it works

### Common Contributions

#### Adding New Calculations

Example: Adding absolute humidity

1. Add calculation method to `dew_point.h`:
   ```cpp
   float calculate_absolute_humidity() {
       // Implementation
   }
   ```

2. Add configuration option in `__init__.py`:
   ```python
   CONF_CALCULATE_ABSOLUTE_HUMIDITY = "calculate_absolute_humidity"
   ```

3. Update documentation
4. Add example configuration

#### Improving Documentation

Documentation improvements are always welcome:
- Fix typos or unclear explanations
- Add more examples
- Improve troubleshooting guides
- Add translations

#### Bug Fixes

1. Create an issue first (if it doesn't exist)
2. Reference the issue in your PR
3. Include test showing the bug is fixed
4. Update documentation if needed

## Style Guidelines

### C++ Style

```cpp
// Good
float calculate_dew_point(float temperature, float humidity) {
    const float a = 17.27f;
    const float b = 237.7f;
    
    if (std::isnan(temperature) || std::isnan(humidity)) {
        return NAN;
    }
    
    float alpha = std::log(humidity / 100.0f) + 
                  (a * temperature) / (b + temperature);
    return (b * alpha) / (a - alpha);
}

// Bad
float calc(float t,float h){float a=17.27,b=237.7;return (b*(log(h/100)+(a*t)/(b+t)))/(a-(log(h/100)+(a*t)/(b+t)));}
```

### Python Style

```python
# Good
CONFIG_SCHEMA = (
    sensor.sensor_schema(
        DewPointComponent,
        unit_of_measurement=UNIT_CELSIUS,
        accuracy_decimals=1,
    )
    .extend({
        cv.Required(CONF_TEMPERATURE): cv.use_id(sensor.Sensor),
        cv.Required(CONF_HUMIDITY): cv.use_id(sensor.Sensor),
    })
)

# Bad
CONFIG_SCHEMA=sensor.sensor_schema(DewPointComponent,unit_of_measurement=UNIT_CELSIUS).extend({cv.Required(CONF_TEMPERATURE):cv.use_id(sensor.Sensor),cv.Required(CONF_HUMIDITY):cv.use_id(sensor.Sensor)})
```

### Documentation Style

- Use clear, simple language
- Include code examples
- Explain why, not just what
- Add troubleshooting tips
- Keep formatting consistent

## Communication

### Where to Ask Questions

- **General questions**: ESPHome Discord or Forums
- **Bug reports**: GitHub Issues
- **Feature requests**: GitHub Issues
- **Development discussion**: GitHub Discussions (if enabled)

### Code of Conduct

- Be respectful and professional
- Welcome newcomers
- Provide constructive feedback
- Stay on topic
- No harassment or discrimination

## Recognition

Contributors will be:
- Credited in CHANGELOG.md
- Mentioned in release notes
- Listed in GitHub contributors

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

If you have questions about contributing:
1. Check existing issues and documentation
2. Ask in ESPHome Discord
3. Create a GitHub issue

Thank you for contributing! 🎉
