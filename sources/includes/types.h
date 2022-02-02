#pragma once

#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <includes/vector2.h>

namespace Shooter {
	struct Flag {
		public:
			bool alive;
			bool activated;

			Flag() {
				// fill with default values
				alive = false;
				activated = false;
			}
	};

	struct RenderEntry {
		public:
			sf::Texture* texture;
			sf::Sprite* sprite;
			Vector2 position;
			
			bool useTextureRect = false;
			sf::IntRect textureRect;
			
			bool isText = false;
			std::string text;
			int fontSize = 0;

			RenderEntry() {
				texture = nullptr;
				sprite = nullptr;
				position = Vector2();
				useTextureRect = false;
				textureRect = sf::IntRect();
				isText = false;
				text = "";
			}
	};

	struct LevelEntity {
		Vector2 position;
		float fireCD;
		int pattern;
	};

	struct LevelData {
		std::vector<LevelEntity> entities;

		LevelData() {
			entities = std::vector<LevelEntity>();
		}
	};
}