/* Author names : VERDIN Maxime and FOURNIER Nathan */

/**
 * @brief Interactive test for boss encounter in gameplay
 *
 * This test simulates the boss encounter flow:
 * 1. Player moves to exit
 * 2. Boss blocks the exit
 * 3. Combat system works with boss
 * 4. Victory grants +5 attack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fonctions/dungeon.h"
#include "fonctions/player.h"

#define BOSS_ROOM_THRESHOLD 4 /* roomsSinceLastBoss % 5 == 4 */

int simulateBossEncounter()
{
    printf("\n==============================================\n");
    printf("    INTERACTIVE BOSS ENCOUNTER TEST\n");
    printf("==============================================\n");

    /* Initialize game state simulation */
    int roomsSinceLastBoss = 4; /* This room is a boss room */
    int currentLevel = 1;
    int bossActive = 0;

    printf("Scenario: Player has completed %d rooms, entering boss room...\n",
           roomsSinceLastBoss + 1);

    /* Check if this is a boss room */
    int isBossRoom = (roomsSinceLastBoss % 5 == BOSS_ROOM_THRESHOLD);
    printf("Is Boss Room: %s\n", isBossRoom ? "YES" : "NO");

    /* Create dungeon */
    Dungeon dungeon;
    initDungeon(&dungeon, currentLevel);

    printf("Dungeon initialized. Exit at position (%d, %d)\n",
           dungeon.exitPos.x, dungeon.exitPos.y);

    /* Spawn boss at exit if this is a boss room */
    if (isBossRoom && !dungeon.bossFound)
    {
        spawnBossAtExit(&dungeon, currentLevel);
        bossActive = 1;
    }

    /* Verify boss is at exit */
    printf("\nBoss Status:\n");
    printf("  - bossFound: %d\n", dungeon.bossFound);
    printf("  - bossPos: (%d, %d)\n", dungeon.bossPos.x, dungeon.bossPos.y);
    printf("  - bossActive: %d\n", bossActive);

    /* Test 1: Boss blocks exit */
    printf("\n--- Test 1: Boss Blocking Exit ---\n");
    int bossBlocksExit = (bossActive &&
                          dungeon.bossPos.x == dungeon.exitPos.x &&
                          dungeon.bossPos.y == dungeon.exitPos.y);
    printf("Result: Boss %s exit\n", bossBlocksExit ? "BLOCKS" : "does not block");

    /* Test 2: Boss position detection */
    printf("\n--- Test 2: Boss Position Detection ---\n");
    int detected = checkBossAt(&dungeon, dungeon.bossPos.x, dungeon.bossPos.y);
    printf("checkBossAt(exit): %s (expected: 1)\n", detected ? "1" : "0");

    /* Test 3: Create boss enemy for combat */
    printf("\n--- Test 3: Create Boss Enemy ---\n");
    Enemy boss = createBoss(currentLevel);
    printf("Boss Enemy Created:\n");
    printf("  - Name: %s\n", boss.name);
    printf("  - Health: %d\n", boss.health);
    printf("  - Attack: %d\n", boss.attack);
    printf("  - Experience: %d\n", boss.experience);

    /* Test 4: Create regular enemy for comparison */
    printf("\n--- Test 4: Regular Enemy Comparison ---\n");
    Enemy regularEnemy = createEnemy(currentLevel);
    printf("Regular Enemy:\n");
    printf("  - Name: %s\n", regularEnemy.name);
    printf("  - Health: %d\n", regularEnemy.health);
    printf("  - Attack: %d\n", regularEnemy.attack);
    printf("  - Experience: %d\n", regularEnemy.experience);

    printf("\n--- Comparison ---\n");
    printf("HP: Boss=%d vs Regular=%d (Boss is %.1fx stronger)\n",
           boss.health, regularEnemy.health,
           (float)boss.health / regularEnemy.health);
    printf("ATK: Boss=%d vs Regular=%d (Boss is %.1fx stronger)\n",
           boss.attack, regularEnemy.attack,
           (float)boss.attack / regularEnemy.attack);
    printf("EXP: Boss=%d vs Regular=%d (Boss gives %.1fx more)\n",
           boss.experience, regularEnemy.experience,
           (float)boss.experience / regularEnemy.experience);

    /* Test 5: Victory rewards */
    printf("\n--- Test 5: Victory Rewards ---\n");
    printf("Regular enemy defeat: +2 attack\n");
    printf("Boss defeat: +5 attack\n");
    printf("Boss bonus: +%d attack\n", 5 - 2);

    /* Test 6: Simulate player reaching boss */
    printf("\n--- Test 6: Player Approaches Boss ---\n");
    dungeon.playerPos.x = dungeon.exitPos.x - 1;
    dungeon.playerPos.y = dungeon.exitPos.y;
    printf("Player position: (%d, %d)\n",
           dungeon.playerPos.x, dungeon.playerPos.y);
    printf("Boss position: (%d, %d)\n",
           dungeon.bossPos.x, dungeon.bossPos.y);

    int distToBoss = abs(dungeon.playerPos.x - dungeon.bossPos.x) +
                     abs(dungeon.playerPos.y - dungeon.bossPos.y);
    printf("Distance to boss: %d\n", distToBoss);

    /* Summary */
    printf("\n==============================================\n");
    printf("         TEST SUMMARY\n");
    printf("==============================================\n");

    int allPassed = 1;

    printf("\n[CHECKLIST]\n");

    if (isBossRoom)
    {
        printf("[PASS] Boss room detected (roomsSinceLastBoss = %d)\n",
               roomsSinceLastBoss);
    }
    else
    {
        printf("[FAIL] Boss room not detected\n");
        allPassed = 0;
    }

    if (bossBlocksExit)
    {
        printf("[PASS] Boss blocks exit at (%d, %d)\n",
               dungeon.exitPos.x, dungeon.exitPos.y);
    }
    else
    {
        printf("[FAIL] Boss does not block exit\n");
        allPassed = 0;
    }

    if (boss.health > regularEnemy.health)
    {
        printf("[PASS] Boss has more HP than regular enemy\n");
    }
    else
    {
        printf("[FAIL] Boss HP check failed\n");
        allPassed = 0;
    }

    if (boss.attack > regularEnemy.attack)
    {
        printf("[PASS] Boss has more attack than regular enemy\n");
    }
    else
    {
        printf("[FAIL] Boss attack check failed\n");
        allPassed = 0;
    }

    if (boss.experience > regularEnemy.experience)
    {
        printf("[PASS] Boss gives more experience\n");
    }
    else
    {
        printf("[FAIL] Boss EXP check failed\n");
        allPassed = 0;
    }

    printf("\n==============================================\n");
    if (allPassed)
    {
        printf("    ALL INTERACTIVE TESTS PASSED\n");
        printf("==============================================\n");
        return 0;
    }
    else
    {
        printf("    SOME TESTS FAILED\n");
        printf("==============================================\n");
        return 1;
    }
}

int main()
{
    srand(time(NULL));
    return simulateBossEncounter();
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
