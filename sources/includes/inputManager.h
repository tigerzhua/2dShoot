#pragma once

#include "includes/player.h"
#include "includes/vector2.h"

#include <SFML/System.hpp>

namespace Shooter {
	struct InputData {
		public:
			Vector2 playerDir;
			bool fire = false;
			bool startGame = false;
			bool endGame = false;
			InputData() {
				fire = false;
				startGame = false;
				endGame = false;
			}
	};

	class InputManager {
		public:
			InputManager();
			void SetPlayer(Player* newPlayer) { player_ = newPlayer; }
			InputData GetInputData() { return inputData_; }
			void Update(sf::Time delta);

		protected:
		private:
			Player* player_;
			InputData inputData_;
	};
}