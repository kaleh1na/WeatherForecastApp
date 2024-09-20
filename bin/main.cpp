#include <lib/Forecast.h>

#include <iostream>

int main(int argc, char **argv) {
  std::string path;
  std::string api_key;
  if (argc != 3) {
    std::cerr << "Invalid input format. Enter only the path to the "
                 "configuration file and API-key.";
    exit(EXIT_FAILURE);
  }
  path = argv[1];
  api_key = argv[2];
  if (path.size() < 6 ||
      (path.substr(path.size() - 5, path.size()) != ".json")) {
    std::cerr << "Invalid configuration file type. Configuration file should "
                 "be json.";
    exit(EXIT_FAILURE);
  }
  forecast::Forecast f(path, api_key);
  f.ShowForecast();
  return 0;
}
