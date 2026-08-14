cmake_minimum_required(VERSION 3.20)
project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PASTAS_INCLUDE
        src
        include
        include/model
        include/view
        include/controller
        include/utils
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Main)

# <----------------------------------------------->
# Configuração do SQLite diretamente (teste)
# <----------------------------------------------->
add_library(sqlite3_lib STATIC "C:/sqlite3/sqlite3.c")
target_include_directories(sqlite3_lib PUBLIC "C:/sqlite3")

# <----------------------------------------------->
# 1. Executável Principal
# <----------------------------------------------->
add_executable(BatalhaNaval
        src/main.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/MapLayout.cpp
        src/model/ScoreCalculator.cpp
        src/controller/GameController.cpp
        src/view/BoardRenderer.cpp
        src/view/PlacementScreen.cpp
        src/view/RankingScreen.cpp
        src/utils/UiTheme.cpp
        src/controller/AutoPlacer.cpp
        src/db/Database.cpp
)
target_include_directories(BatalhaNaval PRIVATE ${PASTAS_INCLUDE} include/db)

# sqlite3_lib vincula o banco de dados e já passa a pasta include automaticamente
target_link_libraries(BatalhaNaval
        SFML::Graphics
        SFML::Window
        SFML::System
        sqlite3_lib
)

# <----------------------------------------------->
# 2. Executáveis de Teste
# <----------------------------------------------->
add_executable(test_board
        tests/manual_board_test.cpp
        src/model/Board.cpp
)
target_include_directories(test_board PRIVATE ${PASTAS_INCLUDE})

add_executable(test_ship
        tests/manual_ship_test.cpp
        src/model/Ship.cpp
)
target_include_directories(test_ship PRIVATE ${PASTAS_INCLUDE})

add_executable(test_gamestate
        tests/manual_gamestate_test.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/MapLayout.cpp
)
target_include_directories(test_gamestate PRIVATE ${PASTAS_INCLUDE})

add_executable(test_menu
        tests/manual_menu_test.cpp
        src/view/MenuScreen.cpp
        src/view/PlacementScreen.cpp
        src/view/GameScreen.cpp
        src/view/BoardRenderer.cpp
        src/view/RankingScreen.cpp
        src/utils/UiTheme.cpp
        src/db/Database.cpp
        src/model/MapLayout.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/AIPlayer.cpp
        src/model/ScoreCalculator.cpp
        src/controller/GameController.cpp
        src/controller/AutoPlacer.cpp
)
target_include_directories(test_menu PRIVATE ${PASTAS_INCLUDE} include/db)
target_link_libraries(test_menu
        SFML::Graphics
        SFML::Window
        SFML::System
        sqlite3_lib
)

# <----------------------------------------------->
# 3. Teste Banco de Dados
# <----------------------------------------------->
add_executable(TestDatabase
        tests/main_test_database.cpp
        src/db/Database.cpp
)
target_include_directories(TestDatabase PRIVATE ${PASTAS_INCLUDE} include/db)
target_link_libraries(TestDatabase sqlite3_lib)