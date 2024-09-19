add_library(weather Weather.cpp Forecast.cpp)

target_link_libraries(weather 
  PUBLIC cpr::cpr
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC ftxui::screen
  PUBLIC ftxui::dom
  PUBLIC ftxui::component
)