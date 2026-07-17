cmake_minimum_required(VERSION 3.20)

project(BattleShipGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # garante que o compilador rejeite código abaixo do C++17

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

# ----------------------------------------------->
# 1. Executáveis Principais
# ----------------------------------------------->
add_executable(BatalhaNaval # Nome do arquivo compilado final
        src/main.cpp
        src/model/Board.cpp
        src/model/Ship.cpp
        src/model/GameState.cpp
        src/model/MapLayout.cpp
        # conforme os .cpp usados pelo jogo forem criados, adicionar aqui:
)

# Avisa o compilador onde procurar os arquivos .h do jogo principal
target_include_directories(BatalhaNaval PRIVATE src include)

# Conecta SFML apenas ao jogo principal
target_link_libraries(BatalhaNaval
        SFML::Graphics
        SFML::Window
        SFML::System
)

# ----------------------------------------------->
# 2. Executáveis de Teste (rodam separadamente)
# ----------------------------------------------->
add_executable(test_board
        tests/manual_board_test.cpp
        src/model/Board.cpp       # o Link precisa do Board para poder testar
)

# Avisa o compilador onde procurar os arquivos .h para o teste
target_include_directories(test_board PRIVATE src include)