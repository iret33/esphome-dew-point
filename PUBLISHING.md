# Publishing Guide - ESPHome Dew Point Component

This guide will help you publish your ESPHome Dew Point Component to GitHub and make it available for others to use.

## Prerequisites

- GitHub account
- Git installed locally
- ESPHome for testing
- Basic understanding of Git and GitHub

## Step 1: Prepare Your Repository

### Update Personal Information

Replace placeholders in the following files:

1. **LICENSE**
   - Replace `[Your Name]` with your actual name

2. **README.md**
   - Replace `iret33` with your GitHub username
   - Update any contact information

3. **QUICK_REFERENCE.md**
   - Update the author section at the bottom

4. **All example files**
   - Replace `iret33` in external_components URLs

## Step 2: Test Locally

Before publishing, thoroughly test the component:

### 1. Manual Installation Test

```bash
# Copy to your ESPHome config
cp -r . /config/esphome/custom_components/dew_point/

# Test with a real device
esphome run test-config.yaml
```

### 2. Verify Functionality

- Check that dew point calculates correctly
- Test with different sensors (DHT22, BME280, etc.)
- Verify real-time updates work
- Check logs for any warnings or errors
- Test edge cases (disconnected sensors, invalid values)

### 3. Validate Documentation

- Read through all documentation
- Try following installation instructions
- Test example configurations
- Verify links work

## Step 3: Create GitHub Repository

### Via GitHub Web Interface

1. Go to https://github.com
2. Click **+** → **New repository**
3. Repository name: `esphome-dew-point`
4. Description: "ESPHome custom component for calculating dew point from temperature and humidity sensors"
5. Choose **Public**
6. **Do NOT** initialize with README (we have one)
7. **Do NOT** add .gitignore or license (we have those too)
8. Click **Create repository**

### Via GitHub CLI (Alternative)

```bash
gh repo create esphome-dew-point --public --description "ESPHome dew point calculator component"
```

## Step 4: Initialize and Push Repository

```bash
# Navigate to your component directory
cd /path/to/esphome_dew_point

# Initialize Git repository
git init

# Add all files
git add .

# Create initial commit
git commit -m "Initial commit: ESPHome Dew Point Component v1.0.0

- Magnus formula implementation for dew point calculation
- Real-time updates based on sensor changes
- Support for ESP8266 and ESP32
- Comprehensive documentation and examples
- Error handling and validation
- MIT licensed"

# Add remote
git remote add origin https://github.com/iret33/esphome-dew-point.git

# Rename branch to main (if needed)
git branch -M main

# Push to GitHub
git push -u origin main
```

## Step 5: Create Release

### Why Create a Release?

Releases provide:
- Version tracking
- Download links
- Change history
- Stable reference points

### Creating v1.0.0 Release

#### Via GitHub Web Interface

1. Go to your repository
2. Click **Releases** → **Create a new release**
3. Click **Choose a tag**
4. Type: `v1.0.0`
5. Click **Create new tag: v1.0.0 on publish**
6. Release title: `v1.0.0 - Initial Release`
7. Description:

```markdown
# ESPHome Dew Point Component v1.0.0

First stable release of the ESPHome Dew Point Component!

## Features

- ✅ Accurate dew point calculation using Magnus formula
- ✅ Real-time updates on temperature/humidity changes
- ✅ Support for ESP8266 and ESP32 platforms
- ✅ Low memory footprint (~100 bytes)
- ✅ Proper error handling and validation
- ✅ Auto-discovery in Home Assistant
- ✅ Comprehensive documentation

## Installation

### External Components (Recommended)

```yaml
external_components:
  - source: github://iret33/esphome-dew-point@v1.0.0
    components: [ dew_point ]
```

### Manual Installation

Download the component files and place them in your `custom_components/dew_point/` directory.

## Quick Start

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

## Documentation

- [Installation Guide](INSTALLATION.md)
- [Example Configurations](EXAMPLES.md)
- [Quick Reference](QUICK_REFERENCE.md)

## Requirements

- ESPHome 2023.1.0 or higher
- Temperature sensor (output in Celsius)
- Humidity sensor (output in 0-100%)

## Accuracy

±0.4°C for temperature range -40°C to 50°C

## License

MIT License - See LICENSE file

## Support

- [GitHub Issues](https://github.com/iret33/esphome-dew-point/issues)
- [ESPHome Discord](https://discord.gg/KhAMKrd)
```

8. Click **Publish release**

#### Via GitHub CLI (Alternative)

```bash
gh release create v1.0.0 \
  --title "v1.0.0 - Initial Release" \
  --notes-file RELEASE_NOTES.md
```

## Step 6: Add Topics/Tags

Help people find your component:

1. Go to repository main page
2. Click the **⚙️** icon next to "About"
3. Add topics:
   - `esphome`
   - `home-assistant`
   - `custom-component`
   - `dew-point`
   - `temperature`
   - `humidity`
   - `sensor`
   - `esp8266`
   - `esp32`
4. Save changes

## Step 7: Update Repository Settings

### Enable Features

1. Go to **Settings** → **General**
2. Features section:
   - ✅ Issues
   - ✅ Discussions (optional)
   - ✅ Projects (optional)
3. Save

### Branch Protection (Optional)

For main branch:
1. Go to **Settings** → **Branches**
2. Add rule for `main`
3. Enable:
   - Require pull request before merging
   - Require status checks to pass

## Step 8: Announce Your Component

### ESPHome Discord

Post in #community-projects:

```
Hey everyone! 👋

I've created a new ESPHome component for calculating dew point:
https://github.com/iret33/esphome-dew-point

Features:
- Real-time dew point calculation
- Works with any temp/humidity sensor
- Low memory usage
- Easy to configure

Example:
```yaml
sensor:
  - platform: dew_point
    name: "Dew Point"
    temperature: my_temp
    humidity: my_humidity
```

Feedback welcome!
```

### Home Assistant Community Forum

Create a topic in **Share Your Projects**:

Title: **[ESPHome Component] Dew Point Calculator**

```markdown
I've created an ESPHome component for calculating dew point from temperature and humidity sensors.

**GitHub**: https://github.com/iret33/esphome-dew-point

**Features:**
- Accurate Magnus formula calculation
- Real-time updates
- Works with ESP8266/ESP32
- Simple YAML configuration

**Installation:**
[Installation instructions...]

**Example Use Cases:**
- HVAC condensation prevention
- Mold risk monitoring
- Comfort level tracking

Let me know if you have questions or suggestions!
```

### Reddit

Post in r/homeassistant or r/homeautomation

## Step 9: Usage Documentation

### README Badges

Add status badges to README.md:

```markdown
![GitHub release](https://img.shields.io/github/v/release/iret33/esphome-dew-point)
![GitHub stars](https://img.shields.io/github/stars/iret33/esphome-dew-point)
![GitHub issues](https://img.shields.io/github/issues/iret33/esphome-dew-point)
![License](https://img.shields.io/github/license/iret33/esphome-dew-point)
```

### Usage Example in Your Config

Add to README showing GitHub usage:

```yaml
external_components:
  - source: github://iret33/esphome-dew-point@v1.0.0
    components: [ dew_point ]
    refresh: 1h
```

## Step 10: Maintenance

### Responding to Issues

- Check issues regularly
- Be helpful and patient
- Ask for logs and configuration
- Reproduce issues when possible
- Thank contributors

### Accepting Pull Requests

1. Review code changes
2. Test locally if possible
3. Provide constructive feedback
4. Merge when ready
5. Thank the contributor

### Creating Updates

When releasing updates:

1. Update CHANGELOG.md
2. Update version in manifest (if applicable)
3. Commit changes
4. Create new release with tag (e.g., v1.1.0)
5. Document changes clearly

## Troubleshooting Publishing

### Git Push Fails

```bash
# Check remote
git remote -v

# Re-add remote if needed
git remote remove origin
git remote add origin https://github.com/iret33/esphome-dew-point.git

# Force push if necessary (only for initial push)
git push -u origin main --force
```

### Release Creation Fails

- Ensure you're on the main branch
- Check you have permissions
- Verify tag doesn't already exist

### Component Not Found When Using

Users should use:
```yaml
external_components:
  - source: github://iret33/esphome-dew-point
    # NOT .git at the end
```

## Post-Publishing Checklist

- [ ] Repository is public
- [ ] All files pushed successfully
- [ ] Release v1.0.0 created
- [ ] README has correct GitHub URLs
- [ ] License has your name
- [ ] Topics/tags added to repository
- [ ] Issues enabled
- [ ] Component tested with external_components
- [ ] Documentation reviewed for errors
- [ ] Announced in ESPHome Discord
- [ ] Posted in Home Assistant forums

## Future Plans

### Version 1.1.0 Ideas

- Fahrenheit input support
- Absolute humidity calculation
- Heat index calculation
- Multiple formula options

### Community Feedback

- Monitor issues for feature requests
- Engage with users
- Consider implementing popular requests
- Keep documentation updated

## Resources

- [GitHub Docs](https://docs.github.com)
- [ESPHome External Components](https://esphome.io/components/external_components.html)
- [Semantic Versioning](https://semver.org)

## Getting Help

If you have questions about publishing:
- GitHub Docs
- ESPHome Discord
- Open an issue in this repository

---

**Congratulations on publishing your component!** 🎉

Remember: The community will appreciate your contribution. Be responsive, be patient, and enjoy seeing others use your work!
