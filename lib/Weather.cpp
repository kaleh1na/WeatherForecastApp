#include "Weather.h"

#include <ctime>

using namespace forecast;

std::string forecast::GetDate(std::time_t time) {
  std::string date;
  date.resize(11);
  strftime(&date[0], 11, "%F", std::localtime(&time));
  date.erase(date.find_first_of('\0'));
  return date;
}

std::string forecast::FloatToString(const float value) {
  std::ostringstream ss;
  ss << value;
  std::string s(ss.str());
  return s;
}

ftxui::Color Weather::GetTemperatureColor(const float num) const {
  using namespace ftxui;
  if (num >= 35) return Color::Red1;
  if (num >= 30) return Color::OrangeRed1;
  if (num >= 25) return Color::Orange1;
  if (num >= 20) return Color::Yellow;
  if (num >= 15) return Color::GreenYellow;
  if (num >= 10) return Color::Green;
  if (num >= 5) return Color::GreenLight;
  if (num >= 0) return Color::LightCyan3;
  if (num >= -10) return Color::SkyBlue1;
  if (num >= -20) return Color::BlueLight;
  if (num >= -30) return Color::Blue3;
  return Color::Magenta1;
}

ftxui::Color Weather::GetWindSpeedColor(const float num) const {
  using namespace ftxui;
  if (num >= 20) return Color::Red1;
  if (num >= 15) return Color::Red;
  if (num >= 11) return Color::OrangeRed1;
  if (num >= 8) return Color::Orange1;
  if (num >= 5.5) return Color::Yellow;
  if (num >= 3.5) return Color::YellowLight;
  if (num >= 1.5) return Color::GreenYellow;
  return Color::White;
}

ftxui::Element Weather::GetTemperature() const {
  using namespace ftxui;
  return hbox({text(FloatToString(temperature)) |
                   color(GetTemperatureColor(temperature)),
               text("("),
               text(FloatToString(apparent_temperature)) |
                   color(GetTemperatureColor(apparent_temperature)),
               text(")°C")});
}

ftxui::Element Weather::GetWindSpeed() const {
  using namespace ftxui;
  return hbox(
      {text(FloatToString(wind_speed)) | color(GetWindSpeedColor(wind_speed)),
       text(" m/s")});
}

ftxui::Element Weather::GetPrecipitation() const {
  using namespace ftxui;
  return text(std::to_string(precipitation_probability) + "% " +
              FloatToString(precipitation) + " mm");
}

ftxui::Element Weather::GetVisibility() const {
  using namespace ftxui;
  if (visibility > 10) {
    return text(">10 km");
  }
  return text(std::to_string(visibility) + " km");
}

std::string Weather::GetWeatherDescription() const {
  switch (weather_code) {
    case 0:
      return "Clear";
    case 1:
      return "Mainly Clear";
    case 2:
      return "Partly Cloudy";
    case 3:
      return "Cloudy";
    case 45:
      return "Foggy";
    case 48:
      return "Rime Fog";
    case 51:
      return "Light Drizzle";
    case 53:
      return "Drizzle";
    case 55:
      return "Heavy Drizzle";
    case 56:
      return "Light Freezing Drizzle";
    case 57:
      return "Freezing Drizzle";
    case 61:
      return "Light Rain";
    case 63:
      return "Rain";
    case 65:
      return "Heavy Rain";
    case 66:
      return "Light Freezing Rain";
    case 67:
      return "Freezing Rain";
    case 71:
      return "Light Snow";
    case 73:
      return "Snow";
    case 75:
      return "Heavy Snow";
    case 77:
      return "Snow Grains";
    case 80:
      return "Light Showers";
    case 81:
      return "Showers";
    case 82:
      return "Heavy Showers";
    case 85:
      return "Light Snow Showers";
    case 86:
      return "Snow Showers";
    case 95:
      return "Thunderstorm";
    case 96:
      return "Light Thunderstorms With Hail";
    case 99:
      return "Thunderstorm With Hail";
    default:
      return "";
  }
}

ftxui::Element Weather::GetWeatherImage() const {
  using namespace ftxui;
  switch (weather_code) {
    case 0:
      return vbox({text("\\   / ") | hcenter | color(Color::Yellow1),
                   text(".-. ") | hcenter | color(Color::Yellow1),
                   text("  ― (   ) ―   ") | hcenter | color(Color::Yellow1),
                   text("`-’ ") | hcenter | color(Color::Yellow1),
                   text("/   \\ ") | hcenter | color(Color::Yellow1)}) |
             size(WIDTH, EQUAL, 15);
    case 1:
      return vbox(
                 {hbox({text("   \\    ") | color(Color::Yellow1), text("..")}),
                  hbox({text("    .-.") | color(Color::Yellow1), text("(  )")}),
                  hbox(
                      {text(" ― (  ") | color(Color::Yellow1), text("(__(_)")}),
                  text("    `-’") | color(Color::Yellow1),
                  text("   /   \\") | color(Color::Yellow1)}) |
             size(WIDTH, EQUAL, 15);
    case 2:
      return vbox({text("    \\  /") | color(Color::Yellow1),
                   hbox({text("  _ /'"
                              "'") |
                             color(Color::Yellow1),
                         text(".-.")}),
                   hbox({text("    \\_") | color(Color::Yellow1),
                         text("(   ).   ")}),
                   hbox({text("    /") | color(Color::Yellow1),
                         text("(___(__) ")})}) |
             size(WIDTH, EQUAL, 15);
    case 3:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)   ")}) |
             size(WIDTH, EQUAL, 15);
    case 45:
      return vbox({text(""), text("  _ - _ - _ -") | color(Color::GrayLight),
                   text("   _ - _ - _") | color(Color::GrayLight),
                   text("  _ - _ - _ -") | color(Color::GrayLight)}) |
             size(WIDTH, EQUAL, 15);
    case 48:
      return vbox({text(""), text("  _ - _ - _ -") | color(Color::GrayDark),
                   text("   _ - _ - _") | color(Color::GrayDark),
                   text("  _ - _ - _ -") | color(Color::GrayDark)}) |
             size(WIDTH, EQUAL, 15);
    case 51:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("  ‘  ‘  ‘  ‘   ") | color(Color::SkyBlue1)}) |
             size(WIDTH, EQUAL, 15);
    case 53:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("   ‚ ‘ ‚ ‘ ‚   ") | color(Color::SkyBlue1)}) |
             size(WIDTH, EQUAL, 15);
    case 55:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("  ‚‘‚‘‚‘‚‘‚‘   ") | color(Color::SkyBlue1)}) |
             size(WIDTH, EQUAL, 15);
    case 56:
      return vbox({
                 text("      .--.     "),
                 text("   .-(    ).   "),
                 text("  (___.__)__)  "),
                 hbox({text("*"), text(" ‚") | color(Color::SkyBlue1),
                       text(" *"), text(" ‚") | color(Color::SkyBlue1),
                       text(" *")}) |
                     hcenter,
             }) |
             size(WIDTH, EQUAL, 15);
    case 57:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text(" ‚") | color(Color::SkyBlue1), text(" *"),
                         text(" ‚") | color(Color::SkyBlue1), text(" *"),
                         text(" ‚") | color(Color::SkyBlue1)}) |
                       hcenter,
                   hbox({text(" ‚") | color(Color::SkyBlue1), text(" *"),
                         text(" ‚") | color(Color::SkyBlue1), text(" *"),
                         text(" ‚    ") | color(Color::SkyBlue1)}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 61:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("   ‚‘ ‚‘ ‚‘    ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 63:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("   ‚‘ ‚‘ ‚‘    ") | color(Color::Blue),
                   text(" ‚‘ ‚‘ ‚‘       ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 65:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("   ‚‘‚‘‚‘‚‘   ") | color(Color::Blue),
                   text(" ‚‘‚‘‚‘‚‘     ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 66:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("*"), text("‚‘") | color(Color::Blue), text("*"),
                         text(" ‚‘") | color(Color::Blue), text(" * ")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 67:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("‚‘") | color(Color::Blue), text(" *"),
                         text(" ‚‘") | color(Color::Blue), text(" *")}) |
                       hcenter,
                   hbox({text("‚‘") | color(Color::Blue), text(" * "),
                         text("‚‘") | color(Color::Blue), text(" *   ")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 71:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "), text("  *   *   *   ")}) |
             size(WIDTH, EQUAL, 15);
    case 73:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "), text("   *   *   *   "),
                   text(" *   *   *   ")}) |
             size(WIDTH, EQUAL, 15);
    case 75:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "), text("  *  *  *  *  "),
                   text(" *  *  *  *  ")}) |
             size(WIDTH, EQUAL, 15);
    case 77:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "), text("   * * * * * "),
                   text("  * * * * * ")}) |
             size(WIDTH, EQUAL, 15);
    case 80:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("    /  /  /    ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 81:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("    /  /  /    ") | color(Color::Blue),
                   text("  /  /  /      ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 82:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   text("   / / / / /   ") | color(Color::Blue),
                   text(" / / / / /     ") | color(Color::Blue)}) |
             size(WIDTH, EQUAL, 15);
    case 85:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("*"), text(" /") | color(Color::Blue), text("*"),
                         text(" /") | color(Color::Blue), text(" *")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 86:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text(" /") | color(Color::Blue), text(" *"),
                         text(" /") | color(Color::Blue), text(" *")}) |
                       hcenter,
                   hbox({text("/ ") | color(Color::Blue), text("* "),
                         text("/ ") | color(Color::Blue), text("*   ")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 95:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue)}) |
                       hcenter,
                   hbox({text("/ /") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/ /") | color(Color::Blue)}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 96:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1), text("·"),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue)}) |
                       hcenter,
                   hbox({text("· "), text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue), text(" ·")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    case 99:
      return vbox({text("      .--.     "), text("   .-(    ).   "),
                   text("  (___.__)__)  "),
                   hbox({text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1), text("*"),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue)}) |
                       hcenter,
                   hbox({text("* "), text("/") | color(Color::Blue),
                         text(" ⚡") | color(Color::Yellow1),
                         text("/") | color(Color::Blue), text(" *")}) |
                       hcenter}) |
             size(WIDTH, EQUAL, 15);
    default:
      return text("");
  }
}

Weather::Weather(uint32_t ind, const nlohmann::json &js,
                 const std::string &daytime)
    : daytime(daytime),
      temperature(js["hourly"]["temperature_2m"][ind]),
      apparent_temperature(js["hourly"]["apparent_temperature"][ind]),
      precipitation(js["hourly"]["precipitation"][ind]),
      precipitation_probability(js["hourly"]["precipitation_probability"][ind]),
      weather_code(js["hourly"]["weather_code"][ind]),
      wind_speed(js["hourly"]["wind_speed_10m"][ind]),
      visibility(js["hourly"]["visibility"][ind]) {}

Weather::Weather(const nlohmann::json &js)
    : daytime("Current"),
      temperature(js["current"]["temperature_2m"]),
      apparent_temperature(js["current"]["apparent_temperature"]),
      precipitation(js["current"]["precipitation"]),
      precipitation_probability(js["current"]["precipitation_probability"]),
      weather_code(js["current"]["weather_code"]),
      wind_speed(js["current"]["wind_speed_10m"]),
      visibility(js["current"]["visibility"]) {}

ftxui::Element Weather::ShowWeather() const {
  using namespace ftxui;
  return vbox({text(daytime) | hcenter, separator(),
               hbox({GetWeatherImage(),
                     vbox({text(GetWeatherDescription()), GetTemperature(),
                           GetWindSpeed(), GetVisibility(),
                           GetPrecipitation()})})}) |
         size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 7);
  ;
}

ftxui::Element Weather::ShowCurrentWeather() const {
  using namespace ftxui;
  return hbox({GetWeatherImage(),
               vbox({text(GetWeatherDescription()), GetTemperature(),
                     GetWindSpeed(), GetVisibility(), GetPrecipitation()})}) |
         size(WIDTH, EQUAL, 30) | size(HEIGHT, EQUAL, 6);
}