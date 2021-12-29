#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "includes/systemConsts.h"
#include "includes/system.h"
#include "includes/graphics.h"
#include "includes/debugManager.h"
#include "includes/unitTests.h"

#define RUN_UNIT_TEST false

using namespace Shooter;

int main() {
    // init systems
    Graphics graphicsSystem = Graphics(kDefaultScreenWidth, kDefaultScreenHeight);
    System gameSystem = System();

    sf::RenderWindow window(sf::VideoMode(kDefaultScreenWidth, kDefaultScreenHeight), "2D Shooter");

    // "main loop"
    sf::Clock clock;
    sf::Time frameTime = sf::Time(sf::seconds(0.0f));
    float kDelta = 1.0f / kDefaultFPS;

    if (RUN_UNIT_TEST) {
        UnitTest::RunUnitTests();
        return 0;
    }

    while (window.isOpen()) {
        frameTime = clock.getElapsedTime();
        if (frameTime.asSeconds() < kDelta) {
            continue;
        }

        //std::cout << frameTime.asSeconds() << std::endl;

        // time to update a frame
        clock.restart();
        
        sf::Event event;

        if (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        gameSystem.Update(frameTime);
        graphicsSystem.Update(gameSystem.GenerateRenderEntries());

        // draw stuff
        window.clear();

        // draw it to the window
        sf::Sprite frame(graphicsSystem.GetMainTexture());
        window.draw(frame);
        window.display();
    }

    return 0;
}