cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # garante que o compilador rejeite código abaixo do C++17

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(BatalhaNaval
        src/main.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/MapLayout.cpp
        # conforme os .cpp forem criados, adicionar aqui:
)

# Diz ao CMake para considerar a pasta "src" como base para procurar os Headers (.h)
target_include_directories(BatalhaNaval PRIVATE src)

target_link_libraries(BatalhaNaval
        SFML::Graphics
        SFML::Window
        SFML::System
)