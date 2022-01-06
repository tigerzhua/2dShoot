#pragma once

#include <fstream>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <string>       // std::string
#include <vector>

#include "includes/types.h"

namespace Shooter {
	class LevelReader{
	public:
		LevelReader();
		void ReadFile(std::string fileName);
		std::vector<LevelData> GetData() { return storage_; }
	private:
		std::vector<LevelData> storage_;

		const std::string kColon = ":";
		const std::string kLevel = "level";
		const std::string kLevelEnd = "level_end";
		const std::string kEnemy = "Enemy";
	};
}