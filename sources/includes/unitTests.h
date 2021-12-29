#pragma once

namespace Shooter {
	class UnitTest {
		public:
			// Unit
			static void UntStatsPlayerProjectile();
			static void UntStatsEnemy();
			static void UntStatsEnemyProjectile();

			// UnitEntry
			static void UnitEntryTakeDamage();
			static void UnitEntryMovement();
			static void UnitEntryMovement_2();
			static void UnitEntryMovement_3();
			static void UnitEntryFlagTest();

			// World 
			static void WorldAddUnit();
			static void WorldRemoveUnit();
			static void WorldFireTest();
			static void WorldGetPlayerHealthRatio();

			// Others
			static void GraphicsSetup();
			static void SystemSetup();

			// "Main" function
			static void RunUnitTests();
	};
}