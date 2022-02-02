#include "includes/unitTests.h"

#include <iostream>
#include <string>

#include <SFML/System.hpp>

#include "includes/systemConsts.h"
#include "includes/system.h"
#include "includes/graphics.h"
#include "includes/vector2.h";
#include "includes/player.h";
#include "includes/unit.h";
#include "includes/world.h"

#define TEST_START(x) std::cout<<"[UnitTest] "<< x << std::endl;
#define SINGLE_TEST_HEADER(x) std::cout << "Test " << x << std::endl;
#define CHECK_CONDITION(x,y) if (x == y) { std::cout << "[UnitTest] ";} else { std::cout << "[UnitTest][ERROR] ";}
#define CHECK_CONDITION_TWO(x,y, x2,y2) if (x == y && x2 == y2) { std::cout << "[UnitTest] ";} else { std::cout << "[UnitTest][ERROR] ";}
#define POST_CHECK_CONDITION(testName, actual, expected) std::cout << testName << ", expected: " << expected << ", getting: " << actual << std::endl;
#define TEST_END std::cout<<"------------"<< std::endl;

namespace Shooter {
	void UnitTest::UntStatsPlayerProjectile() {
		std::string testName = "Unit Stats Player Projectile";
		TEST_START(testName)
		SINGLE_TEST_HEADER(1)
		UnitStats testStats = Unit::GetDefaultPlayerProjectile()->GetStats();
		CHECK_CONDITION(testStats.hp, 10.0f)
		POST_CHECK_CONDITION(testName, testStats.hp, 10.0f)

		CHECK_CONDITION(testStats.damage, 3.0f)
		POST_CHECK_CONDITION(testName, testStats.damage, 3.0f)
		TEST_END
	}

	void UnitTest::UntStatsEnemy() {
		std::string testName = "Unit Stats Enemy";
		TEST_START(testName)
		SINGLE_TEST_HEADER(1)
		UnitStats testStats = Unit::GetDefaultEnemyUnit()->GetStats();
		CHECK_CONDITION(testStats.hp, 10.0f)
		POST_CHECK_CONDITION(testName, testStats.hp, 10.0f)

		TEST_END
	}

	void UnitTest::UntStatsEnemyProjectile() {
		std::string testName = "Unit Stats Enemy Projectile";
		TEST_START(testName)
		SINGLE_TEST_HEADER(1)
		UnitStats testStats = Unit::GetDefaultEnemyProjectile()->GetStats();
		CHECK_CONDITION(testStats.hp, 10.0f)
		POST_CHECK_CONDITION(testName, testStats.hp, 10.0f)

		CHECK_CONDITION(testStats.damage, 3.0f)
		POST_CHECK_CONDITION(testName, testStats.damage, 3.0f)

		TEST_END
	}

	void UnitTest::UnitEntryTakeDamage() {
		std::string testName = "UnitEntry Take Damage";
		TEST_START(testName)
		Unit unitRef = Unit();
		unitRef.GetStats().hp = 50.0f;
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		testEntry.TakeDamage(5.0f);
		float result = 45.0f;
		CHECK_CONDITION(testEntry.GetStats().hp, result)
		POST_CHECK_CONDITION(testName, testEntry.GetStats().hp, result)

		SINGLE_TEST_HEADER(2)
		testEntry.GetStats().hp = 50.0f;
		testEntry.GetFlags().alive = true;
		testEntry.TakeDamage(55.0f);
		result = 0.0f;
		CHECK_CONDITION(testEntry.GetStats().hp, result)
		std::cout << "UnitEntry Take Damage expected hp " << result << ", getting " << testEntry.GetStats().hp << std::endl;

		CHECK_CONDITION(testEntry.GetFlags().alive, false)
		std::cout << "UnitEntry Take Damage expected alive flag " << false << ", getting " << testEntry.GetFlags().alive << std::endl;
		TEST_END
	}

	void UnitTest::UnitEntryMovement() {
		TEST_START("UnitEntry Movement")
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		Vector2 dir = Vector2(0.0, 1.0);
		dir.Normalize();
		testEntry.direction = dir;
		testEntry.speed = 8.0f;
		testEntry.Update(sf::Time(sf::seconds(1.0f)));
		Vector2 result = Vector2(0.0f, 8.0f);
		CHECK_CONDITION_TWO(testEntry.position.x, 0.0f, testEntry.position.y, 8.0f)
		std::cout << "UnitEntry Movement, expect position " << result.ToString() << ", getting " << testEntry.position.ToString() << std::endl;
		TEST_END
	}

	void UnitTest::UnitEntryMovement_2() {
		TEST_START("UnitEntry Movement 2")
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		Vector2 dir = Vector2(1.0, 0.0);
		dir.Normalize();
		testEntry.direction = dir;
		testEntry.speed = 8.0f;
		testEntry.Update(sf::Time(sf::seconds(1.0f)));
		Vector2 result = Vector2(8.0f, 0.0f);
		CHECK_CONDITION_TWO(testEntry.position.x, 8.0f, testEntry.position.y, 0.0f)
		std::cout << "UnitEntry Movement, expect position " << result.ToString() << ", getting " << testEntry.position.ToString() << std::endl;
		TEST_END
	}

	void UnitTest::UnitEntryMovement_3() {
		TEST_START("UnitEntry Movement 3")
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		Vector2 dir = Vector2(-1.0, 0.0);
		dir.Normalize();
		testEntry.direction = dir;
		testEntry.speed = 8.0f;
		testEntry.Update(sf::Time(sf::seconds(1.0f)));
		Vector2 result = Vector2(-8.0f, 0.0f);
		CHECK_CONDITION_TWO(testEntry.position.x, -8.0f, testEntry.position.y, 0.0f)
		std::cout << "UnitEntry Movement, expect position " << result.ToString() << ", getting " << testEntry.position.ToString() << std::endl;
		TEST_END
	}

	void UnitTest::UnitEntryFlagTest() {
		TEST_START("UnitEntry Flag Test")
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		testEntry.SetActive(true);
		bool result = true;
		CHECK_CONDITION(testEntry.GetFlags().activated, result)
		std::cout << "UnitEntry Flag Test, expected alive flag " << result << ", getting " << testEntry.GetFlags().activated << std::endl;

		SINGLE_TEST_HEADER(2)
		testEntry.SetAlive(true);
		result = true;
		CHECK_CONDITION(testEntry.GetFlags().alive, result)

		std::cout << "UnitEntry Flag Test, expected alive flag " << result << ", getting " << testEntry.GetFlags().alive << std::endl;
		TEST_END
	}

	void UnitTest::WorldAddUnit() {
		TEST_START("World Add Unit")
		World testWorld = World();
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());
		
		SINGLE_TEST_HEADER(1)
		testWorld.AddUnitEntry(testEntry, false);
		int result = 1;
		CHECK_CONDITION(testWorld.GetUnitEntryCount(), result)
		std::cout << "World Add Unit, expect UnitEntry count " << result << ", getting " << testWorld.GetUnitEntryCount() << std::endl;
		TEST_END
	}

	void UnitTest::WorldRemoveUnit() {
		TEST_START("World Remove Unit")
		World testWorld = World();
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());

		SINGLE_TEST_HEADER(1)
		testEntry.GetFlags().alive = false;
		testWorld.AddUnitEntry(testEntry, false);
		std::vector<std::string> toRemove;
		toRemove.push_back("testId");
		std::vector<std::string> toRemoveProjectile;
		testWorld.RemoveDeadUnit(toRemove, toRemoveProjectile);
		int result = 0;
		CHECK_CONDITION(testWorld.GetUnitEntryCount(), result)
		std::cout << "World Remove Unit, expect UnitEntry count " << result << ", getting " << testWorld.GetUnitEntryCount() << std::endl;
		TEST_END
	}

	void UnitTest::WorldFireTest() {
		TEST_START("World Fire Test")
		World testWorld = World();
		Unit unitRef = Unit();
		UnitEntry testEntry = UnitEntry("testId", &unitRef, Vector2());
		testWorld.Fire(testEntry, Unit::GetDefaultEnemyProjectile(), true, Vector2(0.0f, 1.0f));

		SINGLE_TEST_HEADER(1)
		int result = 1;
		CHECK_CONDITION(testWorld.GetProjectileEntryCount(), result)
		std::cout << "World Fire Test, expect Projectile count " << result << ", getting " << testWorld.GetProjectileEntryCount() << std::endl;

		SINGLE_TEST_HEADER(2)
		float result2 = 3.0f;
		CHECK_CONDITION(Unit::GetDefaultEnemyProjectile()->GetStats().damage, result2)
		std::cout << "World Fire Test, expect Projectile damge " << result2 << ", getting " << Unit::GetDefaultEnemyProjectile()->GetStats().damage << std::endl;
		TEST_END
	}

	void UnitTest::WorldGetPlayerHealthRatio() {
		TEST_START("World Player Health Ratio")
		World testWorld = World();
		Player* player = new Player("player.png");
		UnitEntry* playerEntry = new UnitEntry("player", player, Vector2(400.0f, 920.0f));
		playerEntry->SetActive(true);
		playerEntry->SetAlive(true);
		testWorld.SetPlayer(playerEntry);

		SINGLE_TEST_HEADER(1)
		playerEntry->TakeDamage(5.0f);
		float result = playerEntry->GetStats().hp / player->GetStats().hp;
		CHECK_CONDITION(testWorld.GetPlayerHealthRatio(), result)
		std::cout << "World Player Health Ratio, expected " << result << ", getting " << testWorld.GetPlayerHealthRatio() << std::endl;
		TEST_END
	}

	void UnitTest::GraphicsSetup() {
		TEST_START("Graphics Setup")
		SINGLE_TEST_HEADER(1)
		Graphics graphicsSystem = Graphics(kDefaultScreenWidth, kDefaultScreenHeight);
		std::cout << "Graphics Setup: Please see logs from Graphic system "<< std::endl;
		TEST_END
	}

	void UnitTest::SystemSetup() {
		TEST_START("System Setup")
		SINGLE_TEST_HEADER(1)
		System gameSystem = System();
		std::cout << "System Setup: Please see logs from system " << std::endl;
		TEST_END
	}

	void UnitTest::RunUnitTests() {
		UntStatsPlayerProjectile();
		UntStatsEnemy();
		UntStatsEnemyProjectile();

		UnitEntryTakeDamage();
		UnitEntryMovement();
		UnitEntryMovement_2();
		UnitEntryMovement_3();
		UnitEntryFlagTest();

		WorldAddUnit();
		WorldRemoveUnit();
		WorldFireTest();
		WorldGetPlayerHealthRatio();

		GraphicsSetup();
		SystemSetup();
	}
}