#include "includes/levelReader.h"

#include "includes/assetUtils.h"

namespace Shooter {
    LevelReader::LevelReader() {
        storage_ = std::vector<LevelData>();
        worldBound_ = Vector2(-1.0f, -1.0f);
    }

	void LevelReader::ReadFile(std::string fileName) {
        std::cout << "[LevelReader]: Reading file " << fileName << std::endl;

        std::string line;
        std::ifstream myfile(AssetUtils::GetFullPath(fileName));
        if (myfile.is_open()) {
            LevelData curLevelData;
            while (getline(myfile, line)) {
                if (line.find(kLevel) != std::string::npos && line.find(kLevelEnd) == std::string::npos) {
                    // level starts
                    std::cout << "[LevelReader] New Level Starts." << std::endl;
                    curLevelData = LevelData();
                } else if (line.find(kLevelEnd) != std::string::npos) {
                    // level ends
                    std::cout << "[LevelReader] New Level Ends." << std::endl << std::endl;
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

                    if (worldBound_.x > 0) {
                        if (World::IsOutOfWorld(worldBound_, levelEntity.position)) {
                            std::cout << "WARNING [LevelReader] Level entity at " << levelEntity.position.ToString() << ", out of world boundry." << std::endl;
                        }
                    }

                    std::string cdString = line.substr(line.find("),") + 2);
                    int endIdx = cdString.find(",");
                    levelEntity.fireCD = std::stof(cdString.substr(0, endIdx+1));

                    std::string patternString = cdString.substr(cdString.find(",") + 1);
                    levelEntity.pattern = std::stoi(patternString);

                    curLevelData.entities.push_back(levelEntity);
                    std::cout<< "[LevelReader] Added new Enemy: position " << 
                        levelEntity.position.ToString() << ", Firing CD: " << 
                        levelEntity.fireCD << ", Pattern: " << 
                        levelEntity.pattern << std::endl;
                }
            }
            myfile.close();
        } else {
            std::cout << "[LevelReader]: Unable to open file " << fileName <<std::endl;
        }
	}

    void LevelReader::GenerateRandomLevel() {
        LevelData curLevelData = LevelData();

        srand(time(NULL));
        int numEnemy = rand() % (kMaxEnemyCount - kMinEnemyCount) + kMinEnemyCount;
        for (int i = 0; i < numEnemy; ++i) {
            LevelEntity levelEntity;
            levelEntity.position.x = rand() % (int)(worldBound_.x - 2 * kMargin) + (int)kMargin;
            levelEntity.position.y = rand() % (int)(worldBound_.y - 2 * kMargin) + (int)kMargin;
            levelEntity.fireCD = (float)(rand() % (int)((kMaxFireCD - kMinFireCD) * 10) + (int)(kMinFireCD * 10))/10.0f;
            levelEntity.pattern = rand() % 1;
            curLevelData.entities.push_back(levelEntity);
        }

        storage_.push_back(curLevelData);
    }
}