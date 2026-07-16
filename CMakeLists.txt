cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(BatalhaNaval
    src/main.cpp
    src/model/ScoreCalculator.cpp
    include/model/ScoreCalculator.h
)

target_include_directories(BatalhaNaval PRIVATE include)
target_link_libraries(BatalhaNaval PRIVATE SFML::Graphics SFML::Window SFML::System)
add_executable(test_menu
        tests/manual_menu_test.cpp
        src/view/MenuScreen.cpp
)
target_include_directories(test_menu PRIVATE include)

target_link_libraries(test_menu PRIVATE SFML::Graphics SFML::Window SFML::System)