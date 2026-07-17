#include <SFML/Graphics.hpp>
#include "model/Board.h"
#include "view/BoardRenderer.h"

int main() {
    // Cria um tabuleiro pequeno (5x5) so pra teste visual
    Board board(5, 5);

    // Posiciona alguns navios manualmente pra ver as cores na tela
    board.placeShip(0, 0, 3, true);   // navio horizontal de 3 celulas
    board.placeShip(2, 1, 2, false);  // navio vertical de 2 celulas

    // Simula alguns tiros pra ver HIT e MISS
    board.shoot(0, 0); // deve virar HIT (tinha navio)
    board.shoot(4, 4); // deve virar MISS (agua)

    // Cria a janela SFML
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Teste BoardRenderer");

    // Cria o renderer: celulas de 60px, comecando em (80, 80) na tela
    BoardRenderer renderer(window, 60.f, 80.f, 80.f);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(15, 30, 60));
        renderer.draw(board, false); // false = mostra os navios (tabuleiro do jogador)
        window.display();
    }

    return 0;
}