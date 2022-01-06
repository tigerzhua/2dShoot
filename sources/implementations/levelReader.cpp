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
                }
            }
            myfile.close();
        } else {
            std::cout << "[LevelReader]: Unable to open file " << fileName <<std::endl;
        }
	}
}