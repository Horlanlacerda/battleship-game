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

    # Tem um B.O. com esse Main
    # O componente sfml-main é uma biblioteca muito específica usada quase que exclusivamente no Windows
    # No Linux e no Mac, o C++ já usa o int main() nativamente de forma limpa, então o SFML geralmente não compila a biblioteca Main por padrão nesses sistemas
    # Então, eu (georis) vou remover, mas vou evitar ao máximo não comitar, qualquer coisa basta coloca a palavra 'Main' depois da palavra System
    find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)


    # <----------------------------------------------->
    # 1. Executáveis Principais
    # <----------------------------------------------->
    # Avisa o compilador onde procurar os arquivos .h do jogo principal
    # o compilador é extremamente literal e não faz buscas recursivas (ou seja, ele não entra nas subpastas por conta própria)
    add_executable(BatalhaNaval # Nome do arquivo compilado final
            src/main.cpp
            src/model/Board.cpp
            src/model/Ship.cpp
            src/model/GameState.cpp
            src/model/MapLayout.cpp
            src/model/ScoreCalculator.cpp
            src/model/AIPlayer.cpp
            src/controller/GameController.cpp
            src/controller/AutoPlacer.cpp
            src/view/BoardRenderer.cpp
            src/view/PlacementScreen.cpp
            src/view/MenuScreen.cpp
            src/view/GameScreen.cpp
            src/view/RankingScreen.cpp
            src/view/InstructionsScreen.cpp
            src/db/Database.cpp
            src/utils/UiTheme.cpp

            # adicionando os .cpp conforme criados
    )

    # Avisa o compilador onde procurar os arquivos .h do jogo principal
    target_include_directories(BatalhaNaval PRIVATE ${PASTAS_INCLUDE} include/db)

    # Conecta SFML e SQLite ao jogo principal
    target_link_libraries(BatalhaNaval
            SFML::Graphics
            SFML::Window
            SFML::System
            SQLite::SQLite3
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

    add_executable(test_gamestate
            tests/manual_gamestate_test.cpp
            src/model/Board.cpp
            src/model/Ship.cpp
            src/model/GameState.cpp
            src/model/MapLayout.cpp
    )
    target_include_directories(test_gamestate PRIVATE ${PASTAS_INCLUDE})
    # teste do GameController, que é o cérebro do jogo, e precisa de quase tudo
    add_executable(test_controller
            tests/main_test_controller.cpp
            src/controller/GameController.cpp
            src/model/Board.cpp
            src/view/BoardRenderer.cpp
            src/model/Ship.cpp
    )
    target_include_directories(test_controller PRIVATE ${PASTAS_INCLUDE})
    target_link_libraries(test_controller
            SFML::Graphics
            SFML::Window
            SFML::System
    )

    # <----------------------------------------------->
    # 3. Banco de Dados (SQLite)
    # <----------------------------------------------->
    find_package(SQLite3 REQUIRED)

    add_executable(TestDatabase
            tests/main_test_database.cpp
            src/db/Database.cpp
    )

    target_include_directories(TestDatabase PRIVATE ${PASTAS_INCLUDE} include/db)

    target_link_libraries(TestDatabase
            SQLite::SQLite3  # <-- O erro no meu linux foi porque estava SQLite3::SQLite3 sendo que o padrão é SQLite::SQLite3 (linux e mac)
    )