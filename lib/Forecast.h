#pragma once
#include <chrono>
#include <string>
#include <vector>

#include "Weather.h"

namespace forecast {

static bool offline_mode = false;

static const std::string kWeatherParameters =
    "temperature_2m,apparent_temperature,precipitation,precipitation_"
    "probability,weather_code,wind_speed_10m,visibility";

class Day {
  std::string date;
  Weather daytimes[4];

 public:
  Day(const uint32_t number_of_day, const nlohmann::json &js);

  ftxui::Element ShowDayWeather() const;
};

class City {
  std::string city_name;
  float latitude;
  float longitude;
  uint8_t forecast_days;
  std::string start_date;
  std::string end_date;
  Weather current;
  std::vector<Day> days;

 public:
  City(const std::string city_name, const uint32_t forecast_days);

  void IncrementForecastDays();

  void DecrementForecastDays();

  void UpdateAllWeather();

  ftxui::Element ShowDaysWeather() const;

  ftxui::Element ShowCityWeather() const;
};

class Forecast {
  std::vector<City> cities;
  uint8_t default_forecast_days;
  std::chrono::seconds frequency_of_updating;

  void StartUpdating();

 public:
  Forecast(const std::string &name_of_config);

  void ShowForecast();
};

}  // namespace forecast