#include "view/PlacementScreen.h"
#include <iostream>

// O "&" significa passar o objeto original sem criar uma cópia.
//Em C++, atributos que são referências (&) ou objetos complexos devem ser inicializados aqui antes do corpo {} do construtor executar.
PlacementScreen::PlacementScreen(sf::RenderWindow& window, Board& board, std::vector<Ship>& ships)
    : window(window),
      board(board),
      ships(ships),
      boardRenderer(window, cellSize, offsetX, offsetY),
      gameController(cellSize, offsetX, offsetY, board.getRows(), board.getCols()),
      previewRow(-1),
      previewCol(-1),
      previewValid(false),
      horizontal(true),
      currentShip(0)
    {}

void PlacementScreen::run(){
    while(window.isOpen() && currentShip < static_cast<int>(ships.size())){
        processEvents();
        update();
        render();
    }
}

void PlacementScreen::processEvents(){
    while(const std::optional event = window.pollEvent()){

        // Verifica se o evento atual é do tipo "fechar janela"
        if(event->is<sf::Event::Closed>()){
            window.close();
            return;
        }

        if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
            sf::Vector2i mousePos(mouseMoved->position.x, mouseMoved->position.y);
            gameController.screenToBoardCoord(mousePos, previewRow, previewCol);
        }

        if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
            if(mousePressed->button == sf::Mouse::Button::Right){
                horizontal = !horizontal;
            }

            if(mousePressed->button == sf::Mouse::Button::Left){
                int row, col;
                sf::Vector2i mousePos(mousePressed->position.x, mousePressed->position.y);

                if (gameController.screenToBoardCoord(mousePos, row, col)) {
                    if (board.placeShip(row, col, ships[currentShip].getSize(), horizontal)) {
                        // TODO: Atualizar a posição do navio quando Ship::setPosition() estiver implementado.

                        currentShip++;
                    }
                }
            }
        }
    }
}

void PlacementScreen::update(){
    if (currentShip >= static_cast<int>(ships.size())) {
        return; // Todos os navios já foram posicionados
    }

    if (previewRow != -1 && previewCol != -1) {
        int shipSize = ships[currentShip].getSize();
        previewValid = board.canPlace(previewRow, previewCol, shipSize, horizontal);
    } else {
        previewValid = false;
    }
}

void PlacementScreen::render(){
    window.clear(sf::Color(15, 30, 60));

    // Desenha o tabuleiro com os navios já posicionados
    boardRenderer.draw(board, false);

    // Desenha o preview visual do navio sob o cursor antes de colocar
    if (currentShip < static_cast<int>(ships.size())) {
        drawPreview(); 
    }

    window.display();
}

void PlacementScreen::drawPreview(){
    if(previewRow < 0 || previewCol < 0) return;

    int shipSize = ships[currentShip].getSize();

    for(int i = 0; i < shipSize; i++){

        int row = horizontal ? previewRow : previewRow + i;
        int col = horizontal ? previewCol + i : previewCol;

        // Se estiver dentro das dimensões válidas do tabuleiro
        if(row >= 0 && row < board.getRows() && col >= 0 && col < board.getCols()){
            sf::RectangleShape previewCell({cellSize - 2.f, cellSize - 2.f});
            previewCell.setPosition({offsetX + col * cellSize, offsetY + row * cellSize});

            if(previewValid){
                previewCell.setFillColor(sf::Color(0, 255, 0, 120));
            } else{
                previewCell.setFillColor(sf::Color(255, 0, 0, 120));
            }

            previewCell.setOutlineColor(sf::Color::White);
            previewCell.setOutlineThickness(1.f);

            window.draw(previewCell);
        }
    }
}