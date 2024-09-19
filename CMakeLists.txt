cmake_minimum_required(VERSION 3.12)

project(
    labwork7
    VERSION 1.0
    DESCRIPTION "C++ Year 2023 labwork7"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib)
add_subdirectory(bin)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG main 
)
FetchContent_MakeAvailable(ftxui)






