#ifndef PLACEMENT_SCREEN_H // Proteção contra inclusão múltipla -> em C++ o mesmo header pode ser incluído diversas vezes.
#define PLACEMENT_SCREEN_H // Marca que esse arquivo já foi incluído

#include <SFML/Graphics.hpp>
#include <vector>

#include "Board.h"
#include "Ship.h"
#include "BoardRenderer.h"
#include "GameController.h"

/**
 * @brief Tela responsável pelo posicionamento manual dos navios.
 * 
 * @details A classe PlacementScreen representa a interface gráfica utilizada
 * urante a fase de preparação da partida. Ela permite que o jogador visualize
 * o tabuleiro, escolha a posição dos navios, altere sua orientação e confirme
 * o posicionamento
 * 
 * Responsabilidades:
 * - Exibir o tabuleiro.
 * - Exibir os navios posicionados.
 * - Exibir o preview do navio atual.
 * - Processar os eventos da janela.
 * - Solicitar ao Board a validação de posicionamento.
 * 
 * @author Horlan Lacerda
 * @since 1.0
 * @date 20/07/2026
 */

class PlacementScreen{

    public:

        /**
         * @brief Construtor da tela de posicionamento.
         * 
         * @param window Janela principal da aplicação.
         * @param board Tabuleiro utilizado durante o posicionamento.
         * @param ships Vetor contendo todos os navios que deverão ser posicionados.
         */
        PlacementScreen(sf::RenderWindow& window, // o "&" é semelhante a passar um objeto por referência em java.
                        Board& board,
                        std::vector<Ship>& ships);
        
        /**
         * @brief Executa o loop principal da tela.
         * 
         * @details Mantém a tela ativa enquanto a janela permanecer aberta.
         * Durante cada iteração são processados os eventos, atualizada a lógica
         * da interface e realizado o desenho de todos os elementos gráficos.
         */
        void run(); // similar a "while(window.isOpen())"
    
    private:
        
        /// Referência para a janela principal da aplicação.
        sf::RenderWindow& window;

        /// Referência para o tabuleiro que receberá os navios.
        Board& board;

        /// Vetor contendo todos os navios da partida.
        std::vector<Ship>& ships;

        /// Responsável por desenhar o tabuleiro.
        BoardRenderer boardRenderer;

        /// Responsável por converter coordenadas da tela para o tabuleiro.
        GameController gameController;

        /// Linha onde o mouse está apontando atualmente.
        int previewRow;

        /// Coluna onde o mouse está apontando atualmente.
        int previewCol;

        /// Indica se o preview está em uma posição válida.
        bool previewValid;

        /// Indica a orientação atual do navio (true = horizontal).
        bool horizontal;

        /// Índice do navio que está sendo posicionado (atual).
        int currentShip;

        /// Tamanho, em pixels, de cada célula do tabuleiro.
        const float cellSize = 40.f;

        /// Deslocamento horizontal, em pixels, onde o tabuleiro começa a ser desenhado.
        const float offsetX = 50.f;
        
        /// Deslocamento vertical, em pixels, onde o tabuleiro começa a ser desenhado.
        const float offsetY = 50.f;

        /**
         * @brief Processa todos os eventos da janela.
         * 
         * @details Trata eventos como fechamento da jenala, 
         * movimentação do mouse e cliques dos botões.
         */
        void processEvents();

        /**
         * @brief Atualiza a lógica da interface.
         * 
         * @details Atualiza informações necessárias para a renderização,
         * como posição do cursor e estado do preview do navio.
         */
        void update();

        /**
         * @brief Desenha todos os elementos da tela.
         *
         * @details Responsável por renderizar o tabuleiro, os navios
         * posicionados e demais componentes visuais.
         */
        void render();

        /**
         * @brief Desenha o preview do navio atual.
         *
         * @details O preview acompanha a posição do mouse e altera sua cor
         * de acordo com a validade do posicionamento:
         * - Verde: posição válida.
         * - Vermelho: posição inválida.
         */
        void drawPreview();
};

#endif