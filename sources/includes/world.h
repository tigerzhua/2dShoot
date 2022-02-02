#pragma once

#include <string>
#include <vector>
#include <map>

#include <SFML/System.hpp>

#include "includes/unit.h"
#include "includes/player.h"
#include "includes/bullet.h"
#include "includes/vector2.h"
#include "includes/inputManager.h"

namespace Shooter {
	class UnitEntry {
		public:
			std::string id;
			Unit* unitRef;
			float entryTime;
			Vector2 position;
			Vector2 direction;
			float speed;
			float fireCD;
			int attackPattern;  // 0: normal, 1: triple shot, 2: double shot

			UnitEntry() = delete;

			UnitEntry(std::string newId, Unit* newUnitRef, Vector2 newPosition) {
				id = newId;
				unitRef = newUnitRef;
				entryTime = 0.0f;
				position = newPosition;
				direction = Vector2(0.0f, 1.0f);
				speed = 0.0f;
				timer_ = 0.0f;
				fireCD = 1.0f;
				attackPattern = 0;
				runtimeStats_ = unitRef->GetStats();
			}

			bool CanFire() { return fireCD <= timer_; }
			void ResetTimer() { timer_ = 0.0f; }
			Flag& GetFlags() { return flags_; }
			UnitStats& GetStats() { return runtimeStats_; }
			void OnDeath();
			void TakeDamage(float damage);
			void SetActive(bool active);
			void SetAlive(bool alive);

			void Update(sf::Time delta);

		private:
			float timer_;
			Flag flags_;
			UnitStats runtimeStats_;
	};

	// The class for a game world
	class World {
		public:
			World();
			void SetSize(Vector2 newSize);
			void Clear();
			void OnStart();
			void Update(sf::Time delta, InputData& inputData);
			void SetPlayer(UnitEntry* newPlayer) { playerEntry_ = newPlayer; }
			void SetPlayerSpeed(float newSpeed) { playerEntry_->speed = newSpeed; }
			void SetPlayerDirection(Vector2 newDir) { playerEntry_->direction = newDir; }
			void AddUnitEntry(UnitEntry unitEntry, bool projectile);
			void RemoveDeadUnit(std::vector<std::string>& toRemove, std::vector<std::string>& toRemoveProjectile);
			void Fire(UnitEntry& host, Unit* projectile, bool useDirectionOverride = false, Vector2 projectileDirection = Vector2());

			int GetUnitEntryCount() { return unitEntries_.size(); }
			int GetProjectileEntryCount() { return projectiles_.size(); }
			std::vector<UnitEntry> GetActiveUnits();
			float GetPlayerHealthRatio();
			sf::Texture* GetBackgroundTexture() { return &backgroundTexture_; }
			bool IsOutOfWorld(UnitEntry& entry);
			static bool IsOutOfWorld(Vector2 worldBound, Vector2 position);
			bool IsGameEnded();
			bool IsPlayerDead();

			void DebugSetupTestLevel();
			std::map<std::string, UnitEntry> unitEntries_;
		protected:
		private:
			Vector2 worldSize_;
			//std::map<std::string, UnitEntry> unitEntries_;
			std::map<std::string, UnitEntry> projectiles_;
			UnitEntry* playerEntry_ = nullptr;
			sf::Time elapsedTime_;
			sf::Texture backgroundTexture_;

			void IntroduceUnit(UnitEntry& entry);
			void DeleteUnit(UnitEntry& entry);
			void UpdateDamagePhase(std::vector<std::string>& toRemove, std::vector<std::string>& toRemoveProjectile);
	};
}
