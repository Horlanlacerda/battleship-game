cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Main)

add_executable(BatalhaNaval
    src/main.cpp
        # conforme os .cpp forem criados, adicionar aqui:
)
<<<<<<< Updated upstream

target_link_libraries(BatalhaNaval
    SFML::Graphics
    SFML::Window
    SFML::System
=======
add_executable(test_board
        tests/manual_board_test.cpp
)

add_executable(TestRenderer
        tests/main_test_renderer.cpp
        src/model/Board.cpp
        src/view/BoardRenderer.cpp
)

target_include_directories(BatalhaNaval PRIVATE src)
target_include_directories(test_board PRIVATE include)
target_include_directories(TestRenderer PRIVATE include include/model include/view)

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
>>>>>>> Stashed changes
)