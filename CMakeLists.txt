cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(ToDoDaSemana
    src/main.cpp
    src/model/Ship.cpp
)

target_include_directories(ToDoDaSemana PRIVATE include include/model)

target_link_libraries(ToDoDaSemana
    SFML::Graphics
    SFML::Window
    SFML::System
)

add_executable(test_ship_manual
    tests/test_ship_manual.cpp
    src/model/Ship.cpp
)
target_include_directories(test_ship_manual PRIVATE include include/model)