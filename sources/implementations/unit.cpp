#include "includes/unit.h"

namespace Shooter {
	Unit* Unit::defaultEnemyProjectileUnit_ = nullptr;
	Unit* Unit::defaultPlayerProjectileUnit_ = nullptr;
	Unit* Unit::defaultEnemyUnit_ = nullptr;

	Unit::Unit() {
		projectile_ = nullptr;
	}

	Unit::Unit(std::string textureFilePath) {
		projectile_ = nullptr;
		stats_.textureRef = new sf::Texture();
		AssetUtils::LoadTexture(textureFilePath, *stats_.textureRef);
	}

	Unit* Unit::GetDefaultEnemyProjectile() {
		if (defaultEnemyProjectileUnit_ == nullptr) {
			defaultEnemyProjectileUnit_ = new Unit("bullet.png");
			defaultEnemyProjectileUnit_->GetStats().damage = 3.0f;
		}

		return defaultEnemyProjectileUnit_;
	}

	Unit* Unit::GetDefaultPlayerProjectile() {
		if (defaultPlayerProjectileUnit_ == nullptr) {
			defaultPlayerProjectileUnit_ = new Unit("bullet_player.png");
			defaultPlayerProjectileUnit_->GetStats().damage = 3.0f;
		}

		return defaultPlayerProjectileUnit_;
	}

	Unit* Unit::GetDefaultEnemyUnit() {
		if (defaultEnemyUnit_ == nullptr) {
			defaultEnemyUnit_ = new Unit("enemy.png");
		}
		return defaultEnemyUnit_;
	}
}