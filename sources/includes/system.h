#pragma once

#include <string>
#include <vector>

#include "includes/types.h"
#include "includes/world.h"
#include "includes/inputManager.h"
#include "includes/vector2.h"

#include <SFML/System.hpp>

namespace Shooter {
	enum class GameStatus {
		MainMenu,
		InGame,
		BetweenLevel,
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
			void ToInbetweenLevel();
			void ToEndScreen();
			void ToMainMenu();
			std::vector<RenderEntry> GenerateRenderEntries();
			void SetupLevel(int idx);

			void DebugSetupTestLevel();

		protected:
		private:
			void LoadLevelData();

			World world_;
			InputManager inputManager_;
			sf::Texture hpBarTexture_;
			GameStatus gameStatus_;
			std::vector<LevelData> levelsData_;
			int curLevelIdx = 0;

	};
}