#include "view/PlacementScreen.h"
#include <iostream>

/**
 * @brief Implementação da tela de posicionamento manual dos navios.
 * 
 * @details Contém a implemetação dos métodos responsáveis por:
 * - Processar eventos da janela;
 * - Atualizar o estado do posicionamento;
 * - Renderizar o tabuleiro;
 * - Exibir o preview do navio que está sendo posicionado.
 * 
 * @author Horlan Lacerda
 * @version 1.0
 * @date 22/07/2026
 */

/**
 * @brief Construtor da tela de posicionamento manual.
 * 
 * @details Inicializa a tela de posicionamento associando-a à janela principal,
 * ao tabuleiro e à frota de navios. Também inicializa os componentes
 * responsáveis pela renderização do tabuleiro e pela conversão das
 * coordenadas do mouse para coordenadas da matriz.
 * 
 * O posicionamento inicial do cursor é definido como inválido e o primeiro
 * navio da frota é selecionado para posicionamento na orientação horizontal.
 * 
 * @param window Referência para a janela principal da aplicação.
 * @param board Referência para o tabuleiro utilizado durante o posicionamento.
 * @param ships Referência para o vetor contendo os navios da partida.
 */
PlacementScreen::PlacementScreen(sf::RenderWindow& window, Board& board, std::vector<Ship>& ships) // O "&" significa passar o objeto original sem criar uma cópia.
                                                                                                   // Além disso, em C++, atributos que são referências (&) ou objetos 
                                                                                                   // complexos devem ser inicializados aqui antes do corpo {} do 
                                                                                                   // construtor executar.
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

/**
 * @brief Executa o loop principal da tela de posicionamento.
 * 
 * @details Mantém a tela ativa enquanto a janela estiver aberta e ainda
 * existirem navios pendentes de posicionamento.
 * 
 * A cada iteração são executadas três etapas:
 * - Processamento dos eventos de entrada do usuário.
 * - Atualização do estado do preview do navio. 
 * - Renderização do estado atual da tela.
 * 
 * O loop é encerrado quando a janela é fechada ou quando todos os navios
 * da frota foram posicionados no tabuleiro.
 */
void PlacementScreen::run(){
    while(window.isOpen() && currentShip < static_cast<int>(ships.size())){
        processEvents();
        update();
        render();
    }
}

/**
 * @brief Processa os eventos de entrada do usuário.
 * 
 * @details Trata os eventos relacionados à janela e à interação do jogador
 * durante o posicionamento dos navios.
 * 
 * Os eventos processados são:
 * - Fechamento da janela.
 * - Movimentação do mouse sobre o tabuleiro.
 * - Clique com o botão direito para alternar a orientação do navio.
 * - Clique com o botão esquerdo para tentar posicionar o navio.
 * 
 * A posição do mouse é convertida de coordenadas da tela para coordenadas
 * do tabuleiro por meio do GameController.
 * 
 * O posicionamento somente é realizado quando a posição escolhida é válida
 * segundo as regras definidas pelo Board.
 */
void PlacementScreen::processEvents(){
    while(const std::optional event = window.pollEvent()){

        // Verifica se o evento atual é do tipo "fechar janela"
        if(event->is<sf::Event::Closed>()){
            window.close();
            return;
        }

        // Processa a movimentação do mmouse.
        if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
            sf::Vector2i mousePos(mouseMoved->position.x, mouseMoved->position.y);
            gameController.screenToBoardCoord(mousePos, previewRow, previewCol);
        }

        // Processa os cliques dos botões do mouse.
        if(const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){

            // Botão direito: alterna entre orientação horizontal e vertical.
            if(mousePressed->button == sf::Mouse::Button::Right){
                horizontal = !horizontal;
            }

            // Botão esquerdo: tenta posicionar o navio.
            if(mousePressed->button == sf::Mouse::Button::Left){
                int row, col;
                sf::Vector2i mousePos(mousePressed->position.x, mousePressed->position.y);

                // Verifica se o clique corresponde a uma célula válida.
                if (gameController.screenToBoardCoord(mousePos, row, col)) {

                    // Solicita ao Board a validação e o posicionamento.
                    if (board.placeShip(row, col, ships[currentShip].getSize(), horizontal)) {
                        
                        ships[currentShip].setPosition(row, col, horizontal);

                        // Avança para o próximo navio da frota.
                        currentShip++;
                    }
                }
            }
        }
    }
}

/**
 * @brief Atualiza o estado lógico do preview do navio atual.
 * 
 * @details Verifica se o navio atualmente selecionado pode ser colocado
 * na posição indicada pelo cursor do mouse, considerando seu tamanho,
 * orientação, limites do tabuleiro e possíveis conflitos com outros navios
 * ou terrenos.
 * 
 * A validação é realizada pelo método Board::canPlace().
 * 
 * Quando o cursor não está sobre uma posição válida do tabuleiro,
 * o preview é considerado inválido.
 */
void PlacementScreen::update(){

    // Verifica se todos os navios já foram posicionados. 
    if (currentShip >= static_cast<int>(ships.size())) {
        return;
    }

    // Verifica se o cursor está sobre uma célula válida.
    if (previewRow != -1 && previewCol != -1) {
        int shipSize = ships[currentShip].getSize();
        previewValid = board.canPlace(previewRow, previewCol, shipSize, horizontal);
    } else {
        previewValid = false;
    }
}

/**
 * @brief Renderiza os elementos gráficos da tela.
 * 
 * @details Limpa a janela, desenha o tabuleiro com o estado atual dos
 * navios já posicionados e, enquanto houver navios pendentes, desenha
 * o preview do navio atualmente selecionado.
 * 
 * O preview é desenhado sobre o tabuleiro para fornecer ao jogador
 * uma representação visual da posição em que o navio será colocado.
 */
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

/**
 * @brief Desenha o preview visual do navio atual.
 *
 * @details O preview acompanha a posição do cursor do mouse e representa
 * visualmente todas as células que seriam ocupadas pelo navio.
 *
 * A cor do preview indica a validade da posição:
 * - Verde: o navio pode ser colocado na posição.
 * - Vermelho: o navio não pode ser colocado na posição.
 *
 * A orientação do preview depende do estado atual da variável horizontal.
 * Quando horizontal é true, as células ocupadas avançam pelas colunas.
 * Quando horizontal é false, as células ocupadas avançam pelas linhas.
 *
 * @note O preview possui finalidade exclusivamente visual e não modifica
 * o estado do Board ou dos objetos Ship.
 */
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