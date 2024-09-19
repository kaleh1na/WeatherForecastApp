#include "Forecast.h"

#include <cpr/cpr.h>

#include <fstream>
#include <iostream>
#include <thread>

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace forecast;

Day::Day(const uint32_t number_of_day, const nlohmann::json &js)
    : date(GetDate(std::time({}) + number_of_day * 86400)) {
  daytimes[0] = Weather(number_of_day * 24, js, "Night");
  daytimes[1] = Weather(number_of_day * 24 + 6, js, "Morning");
  daytimes[2] = Weather(number_of_day * 24 + 12, js, "Afternoon");
  daytimes[3] = Weather(number_of_day * 24 + 18, js, "Evening");
}

ftxui::Element Day::ShowDayWeather() const {
  using namespace ftxui;
  return window(text(date) | hcenter | bold,
                hbox(daytimes[0].ShowWeather(), separator(),
                     daytimes[1].ShowWeather(), separator(),
                     daytimes[2].ShowWeather(), separator(),
                     daytimes[3].ShowWeather())) |
         yflex;
}

City::City(const std::string name, const uint32_t forecast_days)
    : forecast_days(forecast_days) {
  cpr::Response r = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                             cpr::Parameters{{"name", name}},
                             cpr::Header{{"X-Api-Key", kApiKey}});
  if (r.status_code == 200) {
    nlohmann::json js = nlohmann::json::parse(r.text);
    if (js.empty()) {
      std::cerr
          << "City named " + name +
                 " is not found. Please, check the accuracy of city's name.";
      exit(EXIT_FAILURE);
    }
    latitude = js[0]["latitude"];
    longitude = js[0]["longitude"];
    city_name = js[0]["name"];
    return;
  }
  if (r.status_code == 0) {
    std::cerr
        << "Unable to send a request. Please, check your internet connection.";
    exit(EXIT_FAILURE);
  }
  if (r.status_code >= 500) {
    std::cerr << "Unable to send a request. The api site is not responding.";
    exit(EXIT_FAILURE);
  }
  std::cerr << "Unable to send a request.";
  exit(EXIT_FAILURE);
}

void City::IncrementForecastDays() {
  if (forecast_days == 7) {
    return;
  }
  forecast_days++;
  UpdateAllWeather();
  if (offline_mode) {
    forecast_days--;
  }
}

void City::DecrementForecastDays() {
  if (forecast_days == 1) {
    return;
  }
  forecast_days--;
  days.pop_back();
  start_date = GetDate(std::time({}));
  end_date = GetDate(std::time({}) + forecast_days * 86400);
}

void City::UpdateAllWeather() {
  start_date = GetDate(std::time({}));
  end_date = GetDate(std::time({}) + forecast_days * 86400);
  cpr::Response r =
      cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
               cpr::Parameters{{"latitude", std::to_string(latitude)},
                               {"longitude", std::to_string(longitude)},
                               {"current", kWeatherParameters},
                               {"hourly", kWeatherParameters},
                               {"wind_speed_unit", "ms"},
                               {"timezone", "auto"},
                               {"start_date", start_date},
                               {"end_date", end_date}});
  if (r.status_code != 200) {
    offline_mode = true;
    return;
  }
  offline_mode = false;
  nlohmann::json js = nlohmann::json::parse(r.text);
  current = Weather(js);
  days.clear();
  for (int i = 0; i < forecast_days; ++i) {
    Day d(i, js);
    days.push_back(d);
  }
}

ftxui::Element City::ShowDaysWeather() const {
  using namespace ftxui;
  Elements el;
  for (int i = 0; i < forecast_days; ++i) {
    el.push_back(days[i].ShowDayWeather());
  }
  return vbox(el);
}

ftxui::Element City::ShowCityWeather() const {
  using namespace ftxui;
  if (offline_mode) {
    return vbox({text(city_name) | hcenter | bold,
                 text("offline mode  ") | align_right,
                 current.ShowCurrentWeather(), ShowDaysWeather()});
  }
  return vbox({text(city_name) | hcenter | bold, current.ShowCurrentWeather(),
               ShowDaysWeather()});
}

void Forecast::StartUpdating() {
  std::thread thread = std::thread([&] {
    while (true) {
      for (int i = 0; i < cities.size(); ++i) {
        cities[i].UpdateAllWeather();
      }
      std::this_thread::sleep_for(frequency_of_updating);
    }
  });
  thread.detach();
}

Forecast::Forecast(const std::string &name_of_config) {
  std::ifstream config_file(name_of_config);
  if (!config_file) {
    std::cerr << "Unable to find configuration file. Please check the path.";
    exit(EXIT_FAILURE);
  }
  nlohmann::json js = nlohmann::json::parse(config_file);
  config_file.close();
  std::vector<std::string> cities_names = js["cities_names"];
  if (cities_names.size() == 0) {
    std::cerr << "No city was selected";
    exit(EXIT_SUCCESS);
  }
  default_forecast_days = js["forecast_days"];
  if (default_forecast_days > 7) default_forecast_days = 7;
  if (default_forecast_days < 1) default_forecast_days = 1;
  if (js["frequency_of_updating"] > 0) {
    frequency_of_updating = std::chrono::seconds(js["frequency_of_updating"]);
  } else {
    frequency_of_updating = std::chrono::seconds(60);
  }
  for (int i = 0; i < cities_names.size(); ++i) {
    City c(cities_names[i], default_forecast_days);
    c.UpdateAllWeather();
    cities.push_back(c);
  }
  StartUpdating();
}

void Forecast::ShowForecast() {
  using namespace ftxui;
  auto scr = ScreenInteractive::FitComponent();
  size_t cur_city_number = 0;
  auto renderer =
      Renderer([&] { return cities[cur_city_number].ShowCityWeather(); });
  auto component = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('+')) {
      cities[cur_city_number].IncrementForecastDays();
      return true;
    }
    if (event == Event::Character('-')) {
      cities[cur_city_number].DecrementForecastDays();
      return true;
    }
    if (event == Event::Character('n')) {
      cur_city_number = (cur_city_number + 1) % cities.size();
      cities[cur_city_number].UpdateAllWeather();
      return true;
    }
    if (event == Event::Character('p')) {
      if (cur_city_number != 0) {
        cur_city_number--;
      } else {
        cur_city_number = cities.size() - 1;
      }
      cities[cur_city_number].UpdateAllWeather();
      return true;
    }
    if (event == Event::Special("\x1B")) {
      scr.Exit();
      return true;
    }
    return false;
  });
  scr.Loop(component);
}