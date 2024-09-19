#include <iostream>
#include <lib/Forecast.h>

std::string GetConfigPath(const int argc, char **argv) {
  std::string path;
  if (argc != 2) {
    std::cerr << "Invalid input format. Enter only the path to the "
                 "configuration file.";
    exit(EXIT_FAILURE);
  }
  path = argv[1];
  if (path.size() < 6 ||
      (path.substr(path.size() - 5, path.size()) != ".json")) {
    std::cerr << "Invalid configuration file type. Configuration file should "
                 "be json.";
    exit(EXIT_FAILURE);
  }
  return path;
}

int main(int argc, char **argv) {
  std::string path = GetConfigPath(argc, argv);
  forecast::Forecast f(path);
  f.ShowForecast();
  return 0;
}
