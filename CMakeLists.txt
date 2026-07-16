cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(ToDoDaSemana
    src/main.cpp
    src/model/ScoreCalculator.cpp
    include/model/ScoreCalculator.h
)

target_link_libraries(ToDoDaSemana
    SFML::Graphics
    SFML::Window
    SFML::System
)