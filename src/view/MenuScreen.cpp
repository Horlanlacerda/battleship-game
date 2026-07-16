#include "view/MenuScreen.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <optional>

MenuScreen::MenuScreen(sf::RenderWindow& window) : window(window) {
    if (!font.openFromFile("../assets/fonts/arial.ttf"))
        throw std::runtime_error("Fonte nao encontrada: assets/fonts/arial.ttf");
}
MapType MenuScreen::showMapSelection() {
    sf::RectangleShape btnPoca({200.f, 60.f});
    sf::RectangleShape btnLago({200.f, 60.f});
    sf::RectangleShape btnOceano({200.f, 60.f});

    btnPoca.setPosition({300.f, 150.f});
    btnLago.setPosition({300.f, 250.f});
    btnOceano.setPosition({300.f, 350.f});

    btnPoca.setFillColor(sf::Color(40, 80, 140));
    btnLago.setFillColor(sf::Color(40, 80, 140));
    btnOceano.setFillColor(sf::Color(40, 80, 140));

    sf::Text txtPoca(font, "Poca (5x5)", 22);
    sf::Text txtLago(font, "Lago (8x8)", 22);
    sf::Text txtOceano(font, "Oceano (10x10)", 22);
    txtPoca.setPosition({310.f, 165.f});
    txtLago.setPosition({310.f, 265.f});
    txtOceano.setPosition({310.f, 365.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f click = {(float)mousePressed->position.x,
                                          (float)mousePressed->position.y};

                    if (btnPoca.getGlobalBounds().contains(click))   return MapType::POCA;
                    if (btnLago.getGlobalBounds().contains(click))   return MapType::LAGO;
                    if (btnOceano.getGlobalBounds().contains(click)) return MapType::OCEANO;
                }
            }
        }

        window.clear(sf::Color(15, 30, 60));
        window.draw(btnPoca);   window.draw(txtPoca);
        window.draw(btnLago);   window.draw(txtLago);
        window.draw(btnOceano); window.draw(txtOceano);
        window.display();
    }
    return MapType::OCEANO; // fallback se a janela fechar sem seleção
}

MenuOption MenuScreen::showMainMenu() {
    sf::Text title(font, "BATALHA NAVAL", 48);
    title.setFillColor(sf::Color::White);
    title.setPosition({220.f, 60.f});

    std::vector<sf::RectangleShape> buttons(4, sf::RectangleShape({260.f, 55.f}));
    std::vector<sf::Text> labels;
    std::vector<std::string> names = {"Iniciar Jogo", "Ranking", "Instrucoes", "Sair"};

    for (int i = 0; i < 4; i++) {
        buttons[i].setPosition({270.f, 180.f + i * 80.f});
        buttons[i].setFillColor(sf::Color(40, 80, 140));
        labels.emplace_back(font, names[i], 24);
        labels[i].setPosition({285.f, 195.f + i * 80.f});
    }

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                return MenuOption::EXIT;

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2f click = {(float)mousePressed->position.x,
                                          (float)mousePressed->position.y};

                    if (buttons[0].getGlobalBounds().contains(click)) return MenuOption::START;
                    if (buttons[1].getGlobalBounds().contains(click)) return MenuOption::RANKING;
                    if (buttons[2].getGlobalBounds().contains(click)) return MenuOption::INSTRUCTIONS;
                    if (buttons[3].getGlobalBounds().contains(click)) return MenuOption::EXIT;
                }
            }
        }

        window.clear(sf::Color(15, 30, 60));
        window.draw(title);
        for (int i = 0; i < 4; i++) {
            window.draw(buttons[i]);
            window.draw(labels[i]);
        }
        window.display();
    }
    return MenuOption::EXIT;
}