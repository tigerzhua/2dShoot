#include "includes/levelReader.h"

#include "includes/assetUtils.h"

namespace Shooter {
    LevelReader::LevelReader() {
        storage_ = std::vector<LevelData>();
    }

	void LevelReader::ReadFile(std::string fileName) {
        std::string line;
        std::ifstream myfile(AssetUtils::GetFullPath(fileName));
        if (myfile.is_open()) {
            LevelData curLevelData;
            while (getline(myfile, line)) {
                if (line.find(kLevel) != std::string::npos && line.find(kLevelEnd) == std::string::npos) {
                    // level starts
                    curLevelData = LevelData();
                } else if (line.find(kLevelEnd) != std::string::npos) {
                    // level ends
                    storage_.push_back(curLevelData);
                } else if (line.find(kEnemy) != std::string::npos){
                    // An enemy
                    LevelEntity levelEntity;
                    size_t posStartIdx = line.find("(");
                    size_t posEndIdx = line.find(")");
                    if (posStartIdx == std::string::npos || posEndIdx == std::string::npos) {
                        std::cout << "[LevelReader]: Error when reading file " << fileName << ", line: " << line << std::endl;
                    }
                    std::string posString = line.substr(posStartIdx + 1, posEndIdx - posStartIdx);
                    std::stringstream ss(posString);

                    std::string substr;
                    getline(ss, substr, ',');
                    levelEntity.position.x = std::stof(substr);

                    getline(ss, substr, ',');
                    levelEntity.position.y = std::stof(substr);

                    std::string cdString = line.substr(line.find("),") + 2);
                    levelEntity.fireCD = std::stof(cdString);

                    curLevelData.entities.push_back(levelEntity);
                }
            }
            myfile.close();
        } else {
            std::cout << "[LevelReader]: Unable to open file " << fileName <<std::endl;
        }
	}
}