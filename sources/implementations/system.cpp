#include "includes/system.h"

#include "includes/player.h"
#include "includes/systemConsts.h"

namespace Shooter {
	System::System() {
		gameStatus_ = GameStatus::MainMenu;

		AssetUtils::LoadTexture("bar.png", hpBarTexture_);
		std::cout << "System Init complete" << std::endl;
	}

	System::~System() {

	}

	void System::Update(sf::Time delta) {
		inputManager_.Update(delta);
		InputData inputData = inputManager_.GetInputData();

		if (gameStatus_ == GameStatus::InGame) {
			world_.Update(delta, inputData);
			if (world_.IsGameEnded()) {
				ToEndScreen();
				world_.Clear();
			}
		} else if (gameStatus_ == GameStatus::EndScreen) {
			if (inputData.endGame) {
				ToMainMenu();
			}
		} else if (gameStatus_ == GameStatus::MainMenu) {
			if (inputData.startGame) {
				ToGame();
			}
		}

		UpdateUI(delta);
	}

	void System::UpdateUI(sf::Time delta) {
		if (gameStatus_ == GameStatus::InGame) {
			// TBD
		} else if (gameStatus_ == GameStatus::MainMenu) {
			// TBD
		} else if (gameStatus_ == GameStatus::EndScreen) {
			// TBD
		}
	}

	void System::ToGame() {
		if (gameStatus_ == GameStatus::InGame) {
			return;
		}

		gameStatus_ = GameStatus::InGame;

		Player* player = new Player("player.png");
		UnitEntry* playerEntry = new UnitEntry("player", player, Vector2(400.0f, 920.0f));
		playerEntry->speed = 80.0f;
		playerEntry->fireCD = 0.333f;
		playerEntry->SetActive(true);
		playerEntry->SetAlive(true);

		world_.SetPlayer(playerEntry);
		world_.SetSize(Vector2(kDefaultScreenWidth, kDefaultScreenHeight));
		DebugSetupTestLevel();
		inputManager_.SetPlayer(player);
	}

	void System::ToEndScreen() {
		if (gameStatus_ != GameStatus::InGame) {
			return;
		}

		gameStatus_ = GameStatus::EndScreen;
	}

	void System::ToMainMenu() {
		if (gameStatus_ == GameStatus::MainMenu) {
			return;
		}

		gameStatus_ = GameStatus::MainMenu;
	}
	
	// Includes rendering menu UI in different game status
	std::vector<RenderEntry> System::GenerateRenderEntries() {
		std::vector<RenderEntry> result = std::vector<RenderEntry>();

		// add UI
		if (gameStatus_ == GameStatus::InGame) {
			std::vector<UnitEntry> activeUnits = world_.GetActiveUnits();

			for (unsigned int i = 0; i < activeUnits.size(); ++i) {
				RenderEntry renderEntry = RenderEntry();
				renderEntry.texture = activeUnits[i].unitRef->GetStats().textureRef;
				renderEntry.position = activeUnits[i].position;
				result.push_back(renderEntry);
			}

			RenderEntry uiEntry = RenderEntry();
			uiEntry.texture = &hpBarTexture_;
			uiEntry.position = Vector2(30.0f, 30.0f);
			uiEntry.useTextureRect = true;
			float ratio = world_.GetPlayerHealthRatio();
			if (ratio < 0) {
				ratio = 0.0f;
			}

			uiEntry.textureRect = sf::IntRect(0,0, uiEntry.texture->getSize().x * ratio, uiEntry.texture->getSize().y);

			result.push_back(uiEntry);
		} else if (gameStatus_ == GameStatus::MainMenu) {
			RenderEntry titleTextEntry = RenderEntry();
			titleTextEntry.isText = true;
			titleTextEntry.position = Vector2(200.0f, 500.0f);
			titleTextEntry.text = "Space Shooter";
			titleTextEntry.fontSize = 40;
			result.push_back(titleTextEntry);

			RenderEntry instructionEntry = RenderEntry();
			instructionEntry.isText = true;
			instructionEntry.position = Vector2(200.0f, 600.0f);
			instructionEntry.text = "Press Enter to start";
			instructionEntry.fontSize = 20;
			result.push_back(instructionEntry);
		} else if (gameStatus_ == GameStatus::EndScreen) {
			RenderEntry gameOverTextEntry = RenderEntry();
			gameOverTextEntry.isText = true;
			gameOverTextEntry.position = Vector2(200.0f, 500.0f);
			gameOverTextEntry.text = "Game Over";
			gameOverTextEntry.fontSize = 40;
			result.push_back(gameOverTextEntry);

			RenderEntry instructionEntry = RenderEntry();
			instructionEntry.isText = true;
			instructionEntry.position = Vector2(200.0f, 600.0f);
			instructionEntry.text = "Press Space to continue";
			instructionEntry.fontSize = 20;
			result.push_back(instructionEntry);
		}

		return result;
	}

	void System::DebugSetupTestLevel() {
		world_.DebugSetupTestLevel();
	}
}