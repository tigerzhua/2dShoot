#include "includes/system.h"

#include "includes/levelReader.h"
#include "includes/player.h"
#include "includes/systemConsts.h"

namespace Shooter {
	System::System() {
		gameStatus_ = GameStatus::MainMenu;
		timeSinceMenuInput_ = sf::Time(sf::seconds(2*kMenuOperationCDSecond));
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

		bool menuCanUpdate = UpddateAndCheckMenuInputCD(delta);

		if (gameStatus_ == GameStatus::InGame) {
			world_.Update(delta, inputData);
			if (world_.IsGameEnded() && !world_.IsPlayerDead()) {
				curLevelIdx_ += 1;
				if (curLevelIdx_ < levelsData_.size()) {
					ToInbetweenLevel();
				} else {
					ToEndScreen();
				}
				world_.Clear();
			} else if (world_.IsGameEnded() && world_.IsPlayerDead()) {
				ToEndScreen();
				world_.Clear();
			}
		} else if (gameStatus_ == GameStatus::BetweenLevel) {
			if (!menuCanUpdate) {
				return;
			}

			if (inputData.startGame) {
				ResetMenuInputCD();
				if (curLevelIdx_ < levelsData_.size()) {
					ToGame();
				} else {
					// This should not happen
					ToEndScreen();
				}
			}
		} else if (gameStatus_ == GameStatus::EndScreen) {
			if (!menuCanUpdate) {
				return;
			}

			if (inputData.endGame) {
				ResetMenuInputCD();
				ToMainMenu();
			}
		} else if (gameStatus_ == GameStatus::MainMenu) {
			if (!menuCanUpdate) {
				return;
			}

			if (inputData.startGame) {
				ResetMenuInputCD();
				ToInbetweenLevel();
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
		SetupLevel(curLevelIdx_);
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
		curLevelIdx_ = 0;
	}
	
	// Includes rendering menu UI in different game status
	std::vector<RenderEntry> System::GenerateRenderEntries() {
		std::vector<RenderEntry> result = std::vector<RenderEntry>();

		// add UI
		if (gameStatus_ == GameStatus::InGame) {
			// First, put background at the bottom
			RenderEntry backgroundEntry = RenderEntry();
			backgroundEntry.texture = world_.GetBackgroundTexture();
			backgroundEntry.position = Vector2();
			result.push_back(backgroundEntry);
			
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
			titleTextEntry.text = "Level " + std::to_string(curLevelIdx_ + 1);
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
		// read level data from text
		LevelReader levelReader;
		levelReader.SetWorldBound(Vector2(kDefaultScreenWidth, kDefaultScreenHeight));
		levelReader.ReadFile("levels.txt");

		for (int i = 0; i < levelReader.GetData().size(); ++i) {
			levelsData_.push_back(levelReader.GetData()[i]);
		}

		// Add a random level
		levelReader.GenerateRandomLevel();
	}

	bool System::UpddateAndCheckMenuInputCD(sf::Time delta) {
		if (timeSinceMenuInput_.asSeconds() < kMenuOperationCDSecond) {
			timeSinceMenuInput_ += delta;
			return false;
		}

		return true;
	}

	void System::ResetMenuInputCD() {
		timeSinceMenuInput_ = sf::Time(sf::seconds(0.0f));
	}

}