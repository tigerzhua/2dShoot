#pragma once

#include "includes/types.h"
#include "includes/assetUtils.h"

namespace Shooter {
	struct UnitStats {
		public:
			float hp;
			float damage;
			sf::Texture* textureRef = nullptr;

			UnitStats() {
				hp = 10.0f;
				damage = 0.0f;
				if (textureRef != nullptr) {
					delete(textureRef);
				}

				textureRef = nullptr;
			}
	};
	
	class Unit {
		public:
			Unit();
			Unit(std::string textureFilePath);
			UnitStats& GetStats() { return stats_; }
			void SetStats(UnitStats newStats) { stats_ = newStats; }

			static Unit* GetDefaultEnemyProjectile();
			static Unit* GetDefaultPlayerProjectile();
			static Unit* GetDefaultEnemyUnit();

		protected:
			Unit* projectile_;

		private:
			UnitStats stats_; // Its "data sheet"
			static Unit* defaultEnemyProjectileUnit_;
			static Unit* defaultPlayerProjectileUnit_;
			static Unit* defaultEnemyUnit_;
	};
}