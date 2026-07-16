cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # garante que o compilador rejeite código abaixo do C++17

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(BatalhaNaval
    src/main.cpp
        src/model/Board.cpp
        # conforme os .cpp forem criados, adicionar aqui:
)

target_link_libraries(BatalhaNaval
    SFML::Graphics
    SFML::Window
    SFML::System
)