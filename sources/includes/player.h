#pragma once

#include "includes/unit.h"

namespace Shooter {
	class Player : public Unit {
		public:
			Player(std::string textureFileName) : Unit(textureFileName) {}
	};
}