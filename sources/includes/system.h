#pragma once

#include <vector>

#include "includes/types.h"
#include "includes/world.h"
#include "includes/inputManager.h"

#include <SFML/System.hpp>

namespace Shooter {
	enum class GameStatus {
		MainMenu,
		InGame,
		EndScreen,
	};

	// the graphic "system"
	class System {
		public:
			System();
			~System();
			void Update(sf::Time delta);
			void UpdateUI(sf::Time delta);
			void ToGame();
			void ToEndScreen();
			void ToMainMenu();
			std::vector<RenderEntry> GenerateRenderEntries();

			void DebugSetupTestLevel();

		protected:
		private:
			World world_;
			InputManager inputManager_;
			sf::Texture hpBarTexture_;
			GameStatus gameStatus_;

	};
}