#pragma once

#include <SFML/System.hpp>

#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>

namespace Shooter {
	class Vector2 : public sf::Vector2f {
		public:
			Vector2() : sf::Vector2f(0.0f, 0.0f) {}
			Vector2(float x, float y) : sf::Vector2f(x, y) {}
			
			void Normalize() {
				float len = std::sqrt(x * x + y * y);
				x /= len;
				y /= len;
			}

			float SqrMagniture() {
				return x * x + y * y;
			}

			bool IsZero() {
				return x == 0.0f && y == 0.0f;
			}

			std::string ToString() {
				std::string result;
				result = "( " + std::to_string(x) + "," + std::to_string(y) + " )";
				return result;
			}
	};
}