#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include <cmath>

namespace esphome {
namespace dew_point {

class DewPointComponent : public Component, public sensor::Sensor {
 public:
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) {
    temperature_sensor_ = temperature_sensor;
  }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) {
    humidity_sensor_ = humidity_sensor;
  }

  void setup() override {
    // Register callbacks for sensor updates
    if (this->temperature_sensor_ != nullptr) {
      this->temperature_sensor_->add_on_state_callback([this](float state) {
        this->temperature_value_ = state;
        this->update_dew_point_();
      });
      // Get initial value
      if (this->temperature_sensor_->has_state()) {
        this->temperature_value_ = this->temperature_sensor_->state;
      }
    }

    if (this->humidity_sensor_ != nullptr) {
      this->humidity_sensor_->add_on_state_callback([this](float state) {
        this->humidity_value_ = state;
        this->update_dew_point_();
      });
      // Get initial value
      if (this->humidity_sensor_->has_state()) {
        this->humidity_value_ = this->humidity_sensor_->state;
      }
    }

    // Calculate initial dew point
    this->update_dew_point_();
  }

  void dump_config() override {
    ESP_LOGCONFIG("dew_point", "Dew Point Sensor:");
    LOG_SENSOR("  ", "Dew Point", this);
    LOG_SENSOR("  ", "Temperature Source", this->temperature_sensor_);
    LOG_SENSOR("  ", "Humidity Source", this->humidity_sensor_);
  }

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void update_dew_point_() {
    // Check if we have valid values for both sensors
    if (std::isnan(this->temperature_value_) || std::isnan(this->humidity_value_)) {
      ESP_LOGW("dew_point", "Temperature or humidity value is NaN, skipping calculation");
      return;
    }

    // Check for valid humidity range
    if (this->humidity_value_ <= 0.0f || this->humidity_value_ > 100.0f) {
      ESP_LOGW("dew_point", "Humidity value out of range (0-100): %.2f", this->humidity_value_);
      return;
    }

    // Magnus formula constants
    const float a = 17.27f;
    const float b = 237.7f;

    // Calculate dew point using Magnus formula
    // Td = (b * α) / (a - α)
    // where α = ln(RH/100) + (a * T) / (b + T)
    
    float alpha = std::log(this->humidity_value_ / 100.0f) + 
                  (a * this->temperature_value_) / (b + this->temperature_value_);
    
    float dew_point = (b * alpha) / (a - alpha);

    // Publish the calculated dew point
    this->publish_state(dew_point);
    
    ESP_LOGD("dew_point", "Calculated dew point: %.1f°C (T: %.1f°C, RH: %.1f%%)", 
             dew_point, this->temperature_value_, this->humidity_value_);
  }

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  float temperature_value_{NAN};
  float humidity_value_{NAN};
};

}  // namespace dew_point
}  // namespace esphome
