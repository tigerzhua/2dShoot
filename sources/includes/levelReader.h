#pragma once

#include <fstream>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <string>       // std::string
#include <stdlib.h>     // std::rand
#include <vector>

#include "includes/types.h"
#include "includes/world.h"
#include "includes/vector2.h"

namespace Shooter {
	class LevelReader{
	public:
		LevelReader();
		void ReadFile(std::string fileName);
		std::vector<LevelData> GetData() { return storage_; }
		void SetWorldBound(Vector2 newBound) { worldBound_ = newBound; }
		void GenerateRandomLevel();
	private:
		std::vector<LevelData> storage_;
		Vector2 worldBound_;

		const std::string kColon = ":";
		const std::string kLevel = "level";
		const std::string kLevelEnd = "level_end";
		const std::string kEnemy = "Enemy";

		const int kMinEnemyCount = 2;
		const int kMaxEnemyCount = 4;
		const int kMinFireCD = 0.3f;
		const int kMaxFireCD = 1.5f;
	};
}