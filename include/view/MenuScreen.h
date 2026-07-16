#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SFML/Graphics.hpp>

enum class MenuOption { START, RANKING, INSTRUCTIONS, EXIT };

class MenuScreen {
public:
    explicit MenuScreen(sf::RenderWindow& window);
    MenuOption showMainMenu();

private:
    sf::RenderWindow& window;
    sf::Font font;
};

#endif // MENUSCREEN_H