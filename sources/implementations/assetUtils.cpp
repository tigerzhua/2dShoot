#include "includes/assetUtils.h"

#include <iostream>

namespace Shooter {
	const std::string AssetUtils::assetFolder = "assets";

	void AssetUtils::LoadTexture(std::string filePath, sf::Texture& texture) {
		if (!texture.loadFromFile(assetFolder + "\\" + filePath)) {
			std::cout << "[Error]AssetUtils::LoadTexture unable to load texture: " << filePath << std::endl;
		}
	}

	void AssetUtils::LoadFont(std::string filePath, sf::Font& font) {
		if (!font.loadFromFile(assetFolder + "\\" + filePath)) {
			std::cout << "[Error]AssetUtils::LoadFont unable to load font: " << filePath << std::endl;
		}
	}

	std::string AssetUtils::GetFullPath(std::string filePath) {
		return assetFolder + "\\" + filePath;
	}
}