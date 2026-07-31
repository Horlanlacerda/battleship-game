#pragma once

#include <SFML/Graphics.hpp>

enum class MenuOption { START, RANKING, INSTRUCTIONS, EXIT };

class MenuScreen {
public:
    explicit MenuScreen(sf::RenderWindow& window);
    MenuOption showMainMenu();
    MapType showMapSelection();

private:
    sf::RenderWindow& window;
    sf::Font font;
};