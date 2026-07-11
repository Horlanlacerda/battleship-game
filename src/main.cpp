#include <SFML/Graphics.hpp>

int main(){

    sf::RenderWindow window(
        sf::VideoMode({800, 600}),
        "BattleShip Game"
    );

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if(event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(50, 80, 150));
        window.display();   
    }
    
    return 0;
}

