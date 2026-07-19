cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # garante que o compilador rejeite código abaixo do C++17
set(PASTAS_INCLUDE # simplifica os includes do target
        src
        include
        include/model
        include/view
        include/controller
        include/utils
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# <----------------------------------------------->
# 1. Executáveis Principais
# <----------------------------------------------->
add_executable(BatalhaNaval # Nome do arquivo compilado final
        src/main.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/MapLayout.cpp
        src/model/ScoreCalculator.cpp
        include/model/ScoreCalculator.h
        # conforme os .cpp forem criados, adicionar aqui:
)

# Avisa o compilador onde procurar os arquivos .h do jogo principal
# o compilador é extremamente literal e não faz buscas recursivas (ou seja, ele não entra nas subpastas por conta própria)
target_include_directories(BatalhaNaval PRIVATE ${PASTAS_INCLUDE})

# Conecta SFML apenas ao jogo principal
target_link_libraries(BatalhaNaval
        SFML::Graphics
        SFML::Window
        SFML::System
)

# <----------------------------------------------->
# 2. Executáveis de Teste (rodam separadamente)
# <----------------------------------------------->
add_executable(test_board
        tests/manual_board_test.cpp
        src/model/Board.cpp       # o Linker precisa do Board para poder testar
)
# Avisa o compilador onde procurar os arquivos .h para o teste do board
target_include_directories(test_board PRIVATE ${PASTAS_INCLUDE})

add_executable(test_ship
        tests/manual_ship_test.cpp
        src/model/Ship.cpp
)
# Avisa o compilador onde procurar os arquivos .h para o teste do ship
target_include_directories(test_ship PRIVATE ${PASTAS_INCLUDE})