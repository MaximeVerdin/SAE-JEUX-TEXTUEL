/* Author names : VERDIN Maxime and FOURNIER Nathan */

/**
 * @brief Test program for the boss system
 *
 * This test program verifies the core functionality of the boss system:
 * 1. Boss creation with correct stats
 * 2. Boss detection at position
 * 3. Boss spawn logic (every 5 rooms)
 * 4. Boss blocking mechanism
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fonctions/dungeon.h"
#include "fonctions/player.h"

#define PASSED 0
#define FAILED 1

int testsPassed = 0;
int testsFailed = 0;

/**
 * @brief Test helper to compare expected vs actual values
 */
void testAssert(int condition, const char *testName, const char *message)
{
    if (condition)
    {
        printf("[PASS] %s\n", testName);
        testsPassed++;
    }
    else
    {
        printf("[FAIL] %s: %s\n", testName, message);
        testsFailed++;
    }
}

/**
 * @brief Test 1: Create boss with level scaling
 */
int testCreateBoss()
{
    printf("\n=== Test 1: createBoss() Level Scaling ===\n");

    srand(42); /* Fixed seed for reproducibility */

    /* Test boss at level 1 */
    Enemy boss1 = createBoss(1);
    printf("Level 1 Boss: %s (HP: %d, ATK: %d, EXP: %d)\n",
           boss1.name, boss1.health, boss1.attack, boss1.experience);

    testAssert(boss1.health >= 80 && boss1.health <= 139,
               "Level 1 Boss HP", "HP should be 80-109 (base + level*25 + rand)");
    testAssert(boss1.attack >= 12 && boss1.attack <= 31,
               "Level 1 Boss Attack", "Attack should be 12-21 (base + level*4 + rand)");
    testAssert(boss1.experience >= 50,
               "Level 1 Boss EXP", "EXP should be at least 50");

    /* Test boss at level 5 */
    srand(42); /* Reset seed */
    Enemy boss5 = createBoss(5);
    printf("Level 5 Boss: %s (HP: %d, ATK: %d, EXP: %d)\n",
           boss5.name, boss5.health, boss5.attack, boss5.experience);

    testAssert(boss5.health >= 180 && boss5.health <= 239,
               "Level 5 Boss HP", "HP should scale with level");
    testAssert(boss5.attack >= 28 && boss5.attack <= 47,
               "Level 5 Boss Attack", "Attack should scale with level");

    /* Test boss at level 10 */
    srand(42);
    Enemy boss10 = createBoss(10);
    printf("Level 10 Boss: %s (HP: %d, ATK: %d, EXP: %d)\n",
           boss10.name, boss10.health, boss10.attack, boss10.experience);

    testAssert(boss10.health >= 280 && boss10.health <= 339,
               "Level 10 Boss HP", "HP should scale with level");

    return PASSED;
}

/**
 * @brief Test 2: Boss detection at position
 */
int testCheckBossAt()
{
    printf("\n=== Test 2: checkBossAt() Position Detection ===\n");

    Dungeon dungeon;
    memset(&dungeon, 0, sizeof(Dungeon));

    /* Initially no boss */
    testAssert(checkBossAt(&dungeon, 5, 5) == 0,
               "No Boss Initially", "Should return 0 when no boss exists");

    /* Place boss at position (5, 5) */
    dungeon.bossFound = 1;
    dungeon.bossPos.x = 5;
    dungeon.bossPos.y = 5;

    testAssert(checkBossAt(&dungeon, 5, 5) == 1,
               "Boss Found at Position", "Should return 1 when boss is at position");
    testAssert(checkBossAt(&dungeon, 3, 3) == 0,
               "Boss Not Found Elsewhere", "Should return 0 when boss is elsewhere");
    testAssert(checkBossAt(&dungeon, 5, 6) == 0,
               "Boss Not Found Adjacent", "Should return 0 for adjacent but different position");

    /* Test with bossFound = 0 */
    dungeon.bossFound = 0;
    testAssert(checkBossAt(&dungeon, 5, 5) == 0,
               "Boss Found Returns 0 When bossFound=0", "Should return 0 when bossFound is false");

    return PASSED;
}

/**
 * @brief Test 3: Boss spawn logic (every 5 rooms)
 */
int testBossSpawnLogic()
{
    printf("\n=== Test 3: Boss Spawn Logic (Every 5 Rooms) ===\n");

    /* roomsSinceLastBoss values and expected boss room */
    struct
    {
        int roomsSinceLastBoss;
        int expectedIsBossRoom;
    } testCases[] = {
        {0, 0}, /* First room - no boss */
        {1, 0}, /* Second room - no boss */
        {2, 0}, /* Third room - no boss */
        {3, 0}, /* Fourth room - no boss */
        {4, 1}, /* Fifth room - BOSS! */
        {5, 0}, /* Sixth room - no boss */
        {6, 0}, /* Seventh room - no boss */
        {7, 0}, /* Eighth room - no boss */
        {8, 0}, /* Ninth room - no boss */
        {9, 1}, /* Tenth room - BOSS! */
    };

    int numTests = sizeof(testCases) / sizeof(testCases[0]);

    for (int i = 0; i < numTests; i++)
    {
        int rooms = testCases[i].roomsSinceLastBoss;
        int isBossRoom = (rooms % 5 == 4);
        int expected = testCases[i].expectedIsBossRoom;

        char testName[100];
        snprintf(testName, sizeof(testName), "Room %d is %s",
                 rooms + 1, expected ? "BOSS" : "regular");

        testAssert(isBossRoom == expected, testName,
                   expected ? "Should be boss room" : "Should be regular room");
    }

    return PASSED;
}

/**
 * @brief Test 4: Dungeon initialization with boss tracking
 */
int testDungeonInit()
{
    printf("\n=== Test 4: Dungeon Initialization ===\n");

    Dungeon dungeon;

    /* Initialize dungeon at level 1 */
    initDungeon(&dungeon, 1);

    /* Check boss tracking fields are initialized */
    testAssert(dungeon.bossFound == 0,
               "Boss Found Initialized", "bossFound should be 0 after init");
    testAssert(dungeon.bossPos.x == -1,
               "Boss Pos X Initialized", "bossPos.x should be -1 after init");
    testAssert(dungeon.bossPos.y == -1,
               "Boss Pos Y Initialized", "bossPos.y should be -1 after init");

    /* Verify dungeon was generated */
    int floorCount = 0;
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            if (dungeon.grid[i][j] == FLOOR)
                floorCount++;
        }
    }

    testAssert(floorCount > 0,
               "Dungeon Has Floors", "Dungeon should have floor tiles");
    testAssert(dungeon.exitPos.x >= 0 && dungeon.exitPos.x < DUNGEON_SIZE,
               "Exit Position X Valid", "Exit X should be within dungeon bounds");
    testAssert(dungeon.exitPos.y >= 0 && dungeon.exitPos.y < DUNGEON_SIZE,
               "Exit Position Y Valid", "Exit Y should be within dungeon bounds");

    return PASSED;
}

/**
 * @brief Test 5: Spawn boss at exit
 */
int testSpawnBossAtExit()
{
    printf("\n=== Test 5: spawnBossAtExit() ===\n");

    Dungeon dungeon;
    initDungeon(&dungeon, 1);

    /* Save original exit position */
    int exitX = dungeon.exitPos.x;
    int exitY = dungeon.exitPos.y;

    printf("Exit position before boss: (%d, %d)\n", exitX, exitY);

    /* Spawn boss at exit */
    spawnBossAtExit(&dungeon, 1);

    /* Verify boss was spawned */
    testAssert(dungeon.bossFound == 1,
               "Boss Found After Spawn", "bossFound should be 1");
    testAssert(dungeon.bossPos.x == exitX,
               "Boss X at Exit", "bossPos.x should match exit X");
    testAssert(dungeon.bossPos.y == exitY,
               "Boss Y at Exit", "bossPos.y should match exit Y");

    printf("Boss spawned at: (%d, %d)\n", dungeon.bossPos.x, dungeon.bossPos.y);

    /* Verify boss is at correct position using checkBossAt */
    testAssert(checkBossAt(&dungeon, exitX, exitY) == 1,
               "checkBossAt Returns 1 for Boss", "Should detect boss at exit");
    testAssert(checkBossAt(&dungeon, exitX + 1, exitY) == 0,
               "checkBossAt Returns 0 Elsewhere", "Should not detect boss elsewhere");

    return PASSED;
}

/**
 * @brief Test 6: Compare boss vs regular enemy stats
 */
int testBossVsEnemy()
{
    printf("\n=== Test 6: Boss vs Regular Enemy Stats ===\n");

    srand(42);
    Enemy enemy = createEnemy(5);
    srand(42);
    Enemy boss = createBoss(5);

    printf("Regular Enemy: %s (HP: %d, ATK: %d, EXP: %d)\n",
           enemy.name, enemy.health, enemy.attack, enemy.experience);
    printf("Boss Enemy: %s (HP: %d, ATK: %d, EXP: %d)\n",
           boss.name, boss.health, boss.attack, boss.experience);

    /* Boss should be significantly stronger */
    testAssert(boss.health > enemy.health,
               "Boss Has More HP", "Boss should have more health than regular enemy");
    testAssert(boss.attack > enemy.attack,
               "Boss Has More Attack", "Boss should have more attack than regular enemy");
    testAssert(boss.experience > enemy.experience,
               "Boss Gives More EXP", "Boss should give more experience");

    /* Verify scaling ratio */
    double healthRatio = (double)boss.health / enemy.health;
    double attackRatio = (double)boss.attack / enemy.attack;
    double expRatio = (double)boss.experience / enemy.experience;

    printf("Health Ratio: %.2fx, Attack Ratio: %.2fx, EXP Ratio: %.2fx\n",
           healthRatio, attackRatio, expRatio);

    testAssert(healthRatio >= 2.0,
               "Boss HP at Least 2x", "Boss should have at least 2x HP of regular enemy");

    return PASSED;
}

/**
 * @brief Main test runner
 */
int main()
{
    printf("==============================================\n");
    printf("       BOSS SYSTEM TEST SUITE\n");
    printf("==============================================\n");

    /* Run all tests */
    testCreateBoss();
    testCheckBossAt();
    testBossSpawnLogic();
    testDungeonInit();
    testSpawnBossAtExit();
    testBossVsEnemy();

    /* Print summary */
    printf("\n==============================================\n");
    printf("           TEST SUMMARY\n");
    printf("==============================================\n");
    printf("Tests Passed: %d\n", testsPassed);
    printf("Tests Failed: %d\n", testsFailed);
    printf("Total Tests:  %d\n", testsPassed + testsFailed);

    if (testsFailed == 0)
    {
        printf("\n*** ALL TESTS PASSED ***\n");
        return 0;
    }
    else
    {
        printf("\n*** SOME TESTS FAILED ***\n");
        return 1;
    }
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
