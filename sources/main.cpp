#include <SFML/Graphics.hpp>

#include "systemConsts.h"

using namespace Shooter;

int main() {
    sf::RenderWindow window(sf::VideoMode(kDefaultScreenWidth, kDefaultScreenHeight), "2D Shooter");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}