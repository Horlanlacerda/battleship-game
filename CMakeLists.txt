cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Main)

add_executable(BatalhaNaval
    src/main.cpp
        # conforme os .cpp forem criados, adicionar aqui:
)

add_executable(TestRenderer
        tests/main_test_renderer.cpp
        src/model/Board.cpp
        src/view/BoardRenderer.cpp
)

add_executable(TestController
        tests/main_test_controller.cpp
        src/model/Board.cpp
        src/view/BoardRenderer.cpp
        src/controller/GameController.cpp
)

target_include_directories(TestRenderer PRIVATE include include/model include/view)
target_include_directories(TestController PRIVATE include include/model include/view include/controller)

target_link_libraries(BatalhaNaval
    SFML::Graphics
    SFML::Window
    SFML::System
)

target_link_libraries(TestRenderer
        SFML::Main
        SFML::Graphics
        SFML::Window
        SFML::System
)

target_link_libraries(TestController
        SFML::Main
        SFML::Graphics
        SFML::Window
        SFML::System
)