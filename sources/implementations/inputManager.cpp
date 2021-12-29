#include "includes/inputManager.h"

#include <iostream>

#include "includes/vector2.h"

#include <SFML/Window/Keyboard.hpp>

namespace Shooter {
	InputManager::InputManager() {

	}

	void InputManager::Update(sf::Time delta) {
		inputData_ = InputData();

		// first, direction
		inputData_.playerDir = Vector2(0.0f, 0.0f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			inputData_.playerDir += Vector2(0.0f, 1.0f);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			inputData_.playerDir += Vector2(-1.0f, 0.0f);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			inputData_.playerDir += Vector2(0.0f, -1.0f);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			inputData_.playerDir += Vector2(1.0f, 0.0f);
		}

		// flip y
		inputData_.playerDir.y = -inputData_.playerDir.y;

		if (!inputData_.playerDir.IsZero()) {
			inputData_.playerDir.Normalize();
		}

		// second, fire
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) {
			inputData_.fire = true;
		}
		 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
			inputData_.startGame = true;
		}
		//Debug
		//std::cout << inputData_.playerDir.ToString() << std::endl;
	}
}