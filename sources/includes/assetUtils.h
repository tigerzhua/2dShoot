#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace Shooter {
	class AssetUtils {
		public:
			static const std::string assetFolder;

			static void LoadTexture(std::string filePath, sf::Texture& texture);
			static void LoadFont(std::string filePath, sf::Font& font);
			static std::string GetFullPath(std::string filePath);
	};
}