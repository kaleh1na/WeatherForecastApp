#pragma once

#include <cpr/cpr.h>

#include <ftxui/dom/elements.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "ftxui/component/component.hpp"

namespace forecast {

static const std::string kApiKey = "I4kjQyIZRd3wXAyO7TKX7w==YpCHIAcMQPJrQFpa";

std::string GetDate(std::time_t time);

std::string FloatToString(const float value);

class Weather {
  std::string daytime;
  float temperature;
  float apparent_temperature;
  float precipitation;
  uint8_t precipitation_probability;
  uint8_t weather_code;
  float wind_speed;
  uint16_t visibility;

  ftxui::Color GetTemperatureColor(const float num) const;

  ftxui::Color GetWindSpeedColor(const float num) const;

  ftxui::Element GetTemperature() const;

  ftxui::Element GetWindSpeed() const;

  ftxui::Element GetPrecipitation() const;

  ftxui::Element GetVisibility() const;

  std::string GetWeatherDescription() const;

  ftxui::Element GetWeatherImage() const;

 public:
  Weather(uint32_t ind, const nlohmann::json &js, const std::string &daytime);

  Weather(const nlohmann::json &js);

  Weather() = default;

  ftxui::Element ShowWeather() const;

  ftxui::Element ShowCurrentWeather() const;
};

}  // namespace forecast