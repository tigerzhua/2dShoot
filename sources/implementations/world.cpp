#include <iostream>

#include "includes/world.h"

#include "includes/system.h"

namespace Shooter {
	void UnitEntry::Update(sf::Time delta) {
		timer_ += delta.asSeconds();
		position += (direction * speed * delta.asSeconds());
		//std::cout << id << " " << (direction * speed * delta.asSeconds()).y << std::endl;
	}

	void UnitEntry::SetActive(bool active) {
		flags_.activated = active;
	}

	void UnitEntry::SetAlive(bool alive) {
		flags_.alive = alive;
	}

	void UnitEntry::TakeDamage(float damage) {
		runtimeStats_.hp -= damage;
		if (runtimeStats_.hp <= 0.0f) {
			runtimeStats_.hp = 0.0f;
			OnDeath();
		}
	}

	void UnitEntry::OnDeath() {
		flags_.alive = false;
		std::cout << "Unit: " << id << " dead" << std::endl;
	}

	World::World() {
		unitEntries_ = std::map<std::string, UnitEntry>();
		projectiles_ = std::map<std::string, UnitEntry>();
		elapsedTime_ = sf::Time(sf::seconds(0.0f));

		AssetUtils::LoadTexture("background.png", backgroundTexture_);
	}

	void World::Clear() {
		unitEntries_ = std::map<std::string, UnitEntry>();
		projectiles_ = std::map<std::string, UnitEntry>();
		elapsedTime_ = sf::Time(sf::seconds(0.0f));
		if (playerEntry_ != nullptr) {
			delete(playerEntry_);
			playerEntry_ = nullptr;
		}
	}

	void World::SetSize(Vector2 newSize) {
		worldSize_ = Vector2(newSize.x - 35.0f, newSize.y - 50.0f); 
	}

	void World::OnStart() {
		elapsedTime_ = sf::Time(sf::seconds(0.0f));
	}

	void World::AddUnitEntry(UnitEntry unitEntry, bool projectile) {
		std::map<std::string, UnitEntry>* mapToAdd = &unitEntries_;
		if (projectile) {
			mapToAdd = &projectiles_;
		}

		if (mapToAdd->find(unitEntry.id) != mapToAdd->end()) {
			std::cout << "[Error]World::AddUnitEntry trying to add an unit that already existed, id: " << unitEntry.id << std::endl;
		} else {
			mapToAdd->insert( std::pair<std::string, UnitEntry>(unitEntry.id, unitEntry));
		}
	}

	void World::Update(sf::Time delta, InputData& inputData) {
		elapsedTime_ += delta;

		// Update Units
		std::vector<std::string> toRemove;
		for (std::map<std::string, UnitEntry>::iterator it = unitEntries_.begin(); it != unitEntries_.end(); ++it) {
			UnitEntry& entry = it->second;
			const Flag flags = entry.GetFlags();
			// if it's time for a unit to be activated, do so
			if (!flags.activated && entry.entryTime <= elapsedTime_.asSeconds()) {
				IntroduceUnit(entry);
			} else { // this makes each new unit having one frame delay
				if (!flags.alive || IsOutOfWorld(entry)) {
					toRemove.push_back(it->first);
				} else {
					entry.Update(delta);
					if (entry.CanFire()) {
						Fire(entry, Unit::GetDefaultEnemyProjectile());
					}
				}
			}
		}

		if (playerEntry_ != nullptr) {
			if (IsOutOfWorld(*playerEntry_)) {
				if (playerEntry_->position.x > worldSize_.x) {
					playerEntry_->position.x = worldSize_.x;
				} else if (playerEntry_->position.x < 0.0f) {
					playerEntry_->position.x = 0.0f;
				} else if (playerEntry_->position.y > worldSize_.y) {
					playerEntry_->position.y = worldSize_.y;
				} else if (playerEntry_->position.y < 0.0f) {
					playerEntry_->position.y = 0.0f;
				}
			}
		}

		std::vector<std::string> toRemoveProjectile;
		for (std::map<std::string, UnitEntry>::iterator it = projectiles_.begin(); it != projectiles_.end(); ++it) {
			UnitEntry& entry = it->second;
			const Flag flags = entry.GetFlags();
			// if it's time for a unit to be activated, do so
			if (!flags.activated && entry.entryTime <= elapsedTime_.asSeconds()) {
				IntroduceUnit(entry);
			}
			else { // this makes each new unit having one frame delay
				if (!flags.alive || IsOutOfWorld(entry)) {
					toRemoveProjectile.push_back(it->first);
				} else {
					entry.Update(delta);
				}
			}
		}

		SetPlayerDirection(inputData.playerDir);
		if (playerEntry_ != nullptr) {
			playerEntry_->Update(delta);

			if (inputData.fire && playerEntry_->CanFire()) {
				Fire(*playerEntry_, Unit::GetDefaultPlayerProjectile(), true, Vector2(0.0f, -1.0f));
			}
		}

		// finally, remove dead units
		for (unsigned int i = 0; i < toRemove.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = unitEntries_.find(toRemove[i]);
			DeleteUnit(it->second);
			unitEntries_.erase(it);
		}

		for (unsigned int i = 0; i < toRemoveProjectile.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = projectiles_.find(toRemoveProjectile[i]);
			DeleteUnit(it->second);
			projectiles_.erase(it);
		}

		toRemove.clear();
		toRemoveProjectile.clear();
		UpdateDamagePhase(toRemove, toRemoveProjectile);

		for (unsigned int i = 0; i < toRemove.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = unitEntries_.find(toRemove[i]);
			DeleteUnit(it->second);
			unitEntries_.erase(it);
		}

		for (unsigned int i = 0; i < toRemoveProjectile.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = projectiles_.find(toRemoveProjectile[i]);
			DeleteUnit(it->second);
			projectiles_.erase(it);
		}
	}

	void World::RemoveDeadUnit(std::vector<std::string>& toRemove, std::vector<std::string>& toRemoveProjectile) {
		for (unsigned int i = 0; i < toRemove.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = unitEntries_.find(toRemove[i]);
			DeleteUnit(it->second);
			unitEntries_.erase(it);
		}

		for (unsigned int i = 0; i < toRemoveProjectile.size(); ++i) {
			std::map<std::string, UnitEntry>::iterator it = projectiles_.find(toRemoveProjectile[i]);
			DeleteUnit(it->second);
			projectiles_.erase(it);
		}
	}

	std::vector<UnitEntry> World::GetActiveUnits() {
		std::vector<UnitEntry> result;

		for (std::map<std::string, UnitEntry>::iterator it = unitEntries_.begin(); it != unitEntries_.end(); ++it) {
			UnitEntry entry = it->second;
			const Flag flags = entry.GetFlags();
			// if it's time for a unit to be activated, do so
			if (flags.activated) {
				result.push_back(entry);
			}
		}

		for (std::map<std::string, UnitEntry>::iterator it = projectiles_.begin(); it != projectiles_.end(); ++it) {
			UnitEntry entry = it->second;
			const Flag flags = entry.GetFlags();
			// if it's time for a unit to be activated, do so
			if (flags.activated) {
				result.push_back(entry);
			}
		}

		result.push_back(*playerEntry_);

		return result;
	}

	void World::DebugSetupTestLevel() {
		Unit* enemyUnitRef = Unit::GetDefaultEnemyUnit();
		UnitEntry enemy = UnitEntry("testEnemy1", enemyUnitRef, Vector2(400.0f, 400.0f));
		enemy.fireCD = 2.0f;
		AddUnitEntry(enemy, false);

		UnitEntry enemy2 = UnitEntry("testEnemy2", enemyUnitRef, Vector2(300.0f, 500.0f));
		enemy2.fireCD = 1.5f;
		AddUnitEntry(enemy2, false);
	}

	// Private function(s)
	void World::IntroduceUnit(UnitEntry& entry) {
		std::cout << "Introduce Unit Entry: " << entry.id << std::endl;
		entry.SetActive(true);
		entry.SetAlive(true);
	}

	void World::DeleteUnit(UnitEntry& entry) {
		std::cout << "Delete Unit Entry: " << entry.id << std::endl;
		entry.SetActive(false);
		entry.SetAlive(false);
	}

	void World::Fire(UnitEntry& host, Unit* projectile, bool useDirectionOverride, Vector2 projectileDirection) {
		host.ResetTimer();
		std::string projectileId = host.id + "_projectile_" + std::to_string(elapsedTime_.asMicroseconds());
		UnitEntry projectileEntry = UnitEntry(projectileId, projectile, host.position);
		projectileEntry.speed = 200.0f;

		if (useDirectionOverride) {
			projectileDirection.Normalize();
			projectileEntry.direction = projectileDirection;
			AddUnitEntry(projectileEntry, true);
		} else {
			int pattern = host.GetStats().attackPattern;
			if (pattern == 1) {
				projectileDirection = Vector2(0.0, 1.0);
				projectileDirection.Normalize();
				projectileEntry.direction = projectileDirection;
				AddUnitEntry(projectileEntry, true);

				// 2 more shots
				std::string projectileIdL = host.id + "_projectile_" + std::to_string(elapsedTime_.asMicroseconds()) + "_L";
				UnitEntry projectileEntryL = UnitEntry(projectileIdL, projectile, host.position);
				projectileEntryL.speed = 200.0f;
				Vector2 projectileDirectionL = Vector2(-3.0, 4.0);
				projectileDirectionL.Normalize();
				projectileEntryL.direction = projectileDirectionL;
				AddUnitEntry(projectileEntryL, true);

				std::string projectileIdR = host.id + "_projectile_" + std::to_string(elapsedTime_.asMicroseconds()) + "_R";
				UnitEntry projectileEntryR = UnitEntry(projectileIdR, projectile, host.position);
				projectileEntryR.speed = 200.0f;
				Vector2 projectileDirectionR = Vector2(3.0, 4.0);
				projectileDirectionR.Normalize();
				projectileEntryR.direction = projectileDirectionR;
				AddUnitEntry(projectileEntryR, true);
			} else { 
				// pattern == 0 and default
				projectileDirection = Vector2(0.0, 1.0);
				projectileDirection.Normalize();
				projectileEntry.direction = projectileDirection;
				AddUnitEntry(projectileEntry, true);
			}
		}
	}

	bool World::IsOutOfWorld(UnitEntry& entry) {
		return entry.position.x > worldSize_.x || entry.position.x < 0.0f || entry.position.y > worldSize_.y || entry.position.y < 0.0f;
	}

	bool  World::IsOutOfWorld(Vector2 worldBound, Vector2 position) {
		return position.x > worldBound.x || position.x < 0.0f || position.y > worldBound.y || position.y < 0.0f;
	}

	bool World::IsGameEnded() {
		return unitEntries_.size() == 0 || IsPlayerDead();
	}

	bool World::IsPlayerDead() {
		if (playerEntry_ == nullptr) {
			return true;
		}

		if (playerEntry_->GetStats().hp <= 0) {
			return true;
		}

		return false;
	}

	float World::GetPlayerHealthRatio() {
		if (playerEntry_ == nullptr) {
			return -1.0f;
		}

		return (playerEntry_->GetStats().hp) / (playerEntry_->unitRef->GetStats().hp);
	}

	void World::UpdateDamagePhase(std::vector<std::string>& toRemove, std::vector<std::string>& toRemoveProjectile) {
		unitEntries_.insert(std::pair<std::string, UnitEntry>(playerEntry_->id, *playerEntry_));

		for (std::map<std::string, UnitEntry>::iterator pIt = projectiles_.begin(); pIt != projectiles_.end(); ++pIt) {
			for (std::map<std::string, UnitEntry>::iterator it = unitEntries_.begin(); it != unitEntries_.end(); ++it) {
				UnitEntry& unit = it->second;
				UnitEntry& projectile = pIt->second;

				if (unit.id.find("player") != std::string::npos && projectile.id.find("player") != std::string::npos) {
					continue;
				}

				if (unit.id.find("Enemy") != std::string::npos && projectile.id.find("Enemy") != std::string::npos) {
					continue;
				}

				Vector2 positionDiff = Vector2((unit.position - projectile.position).x, (unit.position - projectile.position).y);
				if (positionDiff.SqrMagniture() <= 100.0f) {
					toRemoveProjectile.push_back(projectile.id);
					// take damage
					if (unit.id.find("player") != std::string::npos) {
						playerEntry_->TakeDamage(projectile.GetStats().damage);
					} else {
						unit.TakeDamage(projectile.GetStats().damage);
					}
				}
			}
		}

		unitEntries_.erase(unitEntries_.find(playerEntry_->id));
	}
}