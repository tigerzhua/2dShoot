#include "includes/system.h"

#include "includes/player.h"
#include "includes/systemConsts.h"

namespace Shooter {
	System::System() {
		gameStatus_ = GameStatus::MainMenu;
		// init level data
		LoadLevelData();

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
				curLevelIdx += 1;
				if (curLevelIdx < levelsData_.size()) {
					ToInbetweenLevel();
				} else {
					ToEndScreen();
				}
				world_.Clear();
			}
		} else if (gameStatus_ == GameStatus::BetweenLevel) {
			if (inputData.startGame) {
				if (curLevelIdx < levelsData_.size()) {
					ToGame();
				} else {
					// This should not happen
					ToEndScreen();
				}
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
		} else if (gameStatus_ == GameStatus::BetweenLevel) {
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
		SetupLevel(curLevelIdx);
		inputManager_.SetPlayer(player);
	}

	void System::ToInbetweenLevel() {
		gameStatus_ = GameStatus::BetweenLevel;
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
		} else if (gameStatus_ == GameStatus::BetweenLevel) {
			RenderEntry titleTextEntry = RenderEntry();
			titleTextEntry.isText = true;
			titleTextEntry.position = Vector2(200.0f, 500.0f);
			titleTextEntry.text = "Level " + std::to_string(curLevelIdx + 1);
			titleTextEntry.fontSize = 40;
			result.push_back(titleTextEntry);

			RenderEntry instructionEntry = RenderEntry();
			instructionEntry.isText = true;
			instructionEntry.position = Vector2(200.0f, 600.0f);
			instructionEntry.text = "Press Enter to start";
			instructionEntry.fontSize = 20;
			result.push_back(instructionEntry);
		}

		return result;
	}

	void System::SetupLevel(int idx) {
		if (idx < 0 || idx >= levelsData_.size()) {
			std::cout << "System: No level at idx " << idx << std::endl;
			return;
		}

		for (int i = 0; i < levelsData_[idx].entities.size(); ++i) {
			Unit* enemyUnitRef = Unit::GetDefaultEnemyUnit();
			// Must contain the string "Enemy"
			std::string enemyId = "Enemy" + std::to_string(i);
			UnitEntry enemy = UnitEntry(enemyId, enemyUnitRef, levelsData_[idx].entities[i].position);
			enemy.fireCD = levelsData_[idx].entities[i].fireCD;
			world_.AddUnitEntry(enemy, false);
		}
	}

	void System::DebugSetupTestLevel() {
		world_.DebugSetupTestLevel();
	}

	void System::LoadLevelData() {
		// Test level at idx 0
		LevelData singleLevelData = LevelData();
		LevelEntity entity;
		entity.position = Vector2(400.0f, 400.0f);
		entity.fireCD = 2.0f;
		singleLevelData.entities.push_back(entity);

		LevelEntity entity2;
		entity2.position = Vector2(300.0f, 500.0f);
		entity2.fireCD = 1.5f;
		singleLevelData.entities.push_back(entity2);

		levelsData_.push_back(singleLevelData);

		// TODO: read level data from text
		LevelData singleLevelData2 = LevelData();
		LevelEntity entity3;
		entity3.position = Vector2(400.0f, 600.0f);
		entity3.fireCD = 2.0f;
		singleLevelData2.entities.push_back(entity3);

		LevelEntity entity4;
		entity4.position = Vector2(300.0f, 500.0f);
		entity4.fireCD = 1.5f;
		singleLevelData2.entities.push_back(entity4);

		levelsData_.push_back(singleLevelData2);
	}

}