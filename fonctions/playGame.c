/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playGame.h"
#include "player.h"
#include "saveManagement.h"
#include "dungeon.h"
#include "utils.h"
#include "items.h"

/* Global weapons array loaded from CSV */
static Weapon g_weapons[MAX_WEAPONS];
static int g_weaponCount = 0;

/**
 * @brief Display the main game menu
 */
void displayGameMenu()
{
    char *gameMenuTitle = getTranslatedText("gameMenuTitle");
    char *exploreOption = getTranslatedText("exploreOption");
    char *displayPlayersOption = getTranslatedText("displayPlayersOption");
    char *upgradesOption = getTranslatedText("upgradesOption");
    char *saveOption = getTranslatedText("saveOption");
    char *returnMenuOption = getTranslatedText("returnMenuOption");
    char *yourChoice = getTranslatedText("yourChoice");

    printf("\n=== %s ===\n", gameMenuTitle);
    printf("1. %s\n", exploreOption);
    printf("2. %s\n", displayPlayersOption);
    printf("3. %s\n", upgradesOption);
    printf("4. %s\n", saveOption);
    printf("5. %s\n", returnMenuOption);
    printf("%s ", yourChoice);

    free(gameMenuTitle);
    free(exploreOption);
    free(displayPlayersOption);
    free(upgradesOption);
    free(saveOption);
    free(returnMenuOption);
    free(yourChoice);
}

/**
 * @brief Display dungeon navigation menu
 */
void displayDungeonMenu()
{
    char *dungeonTitle = getTranslatedText("dungeonTitle");
    char *dungeonDirections = getTranslatedText("dungeonDirections");
    char *returnToMenu = getTranslatedText("returnToMenu");
    char *yourChoice = getTranslatedText("yourChoice");

    printf("\n=== %s ===\n", dungeonTitle);
    printf("%s\n", dungeonDirections);
    printf("1. %s\n", returnToMenu);
    printf("%s ", yourChoice);

    free(dungeonTitle);
    free(dungeonDirections);
    free(returnToMenu);
    free(yourChoice);
}

/**
 * @brief Display combat action menu
 */
void displayCombatMenu()
{
    char *combatTitle = getTranslatedText("combatTitle");
    char *fightOption = getTranslatedText("fightOption");
    char *blockOption = getTranslatedText("blockOption");
    char *combatYourChoice = getTranslatedText("combatYourChoice");

    printf("\n=== %s ===\n", combatTitle);
    printf("1. %s\n", fightOption);
    printf("2. %s\n", blockOption);
    printf("%s ", combatYourChoice);

    free(combatTitle);
    free(fightOption);
    free(blockOption);
    free(combatYourChoice);
}

/**
 * @brief Run a combat encounter between player and enemy
 */
int combat(Player *player, Enemy *enemy)
{
    char *combatStart = getTranslatedText("combatStart");
    char *enemyInfo = getTranslatedText("enemyInfo");
    char *yourLuck = getTranslatedText("yourLuck");

    char *criticalHit = getTranslatedText("criticalHit");
    char *criticalHitText = getTranslatedText("criticalHitText");
    char *attackText = getTranslatedText("attackText");
    char *dealsDamage = getTranslatedText("dealsDamage");
    char *defeated = getTranslatedText("defeated");

    char *parryTitle = getTranslatedText("parryTitle");
    char *parryText = getTranslatedText("parryText");
    char *noDamageTaken = getTranslatedText("noDamageTaken");
    char *counterAttack = getTranslatedText("counterAttack");
    char *parryDefeated = getTranslatedText("parryDefeated");

    char *blockingStance = getTranslatedText("blockingStance");
    char *damageReduced = getTranslatedText("damageReduced");
    char *enemyAttacks = getTranslatedText("enemyAttacks");
    char *playerDefeated = getTranslatedText("playerDefeated");
    char *combatInvalidChoice = getTranslatedText("combatInvalidChoice");

    printf("\n");
    printf(combatStart, player->name, enemy->name);
    printf("\n");
    printf(enemyInfo, enemy->name, enemy->health, enemy->attack);
    printf("\n");
    printf(yourLuck, player->luck);
    printf("\n");

    int blocking = 0; /* Track if player is blocking this turn */
    int parried = 0;  /* Track if player successfully parried */

    while (enemy->health > 0 && player->health > 0)
    {
        printf("\n%s (Health: %d) VS %s (Health: %d)\n",
               player->name, player->health, enemy->name, enemy->health);

        /* Reset blocking status at start of turn */
        blocking = 0;
        parried = 0;

        /* Display combat menu and get player choice - loop until valid input */
        int choice = 0;
        while (choice != 1 && choice != 2)
        {
            displayCombatMenu();
            scanf("%d", &choice);

            if (choice != 1 && choice != 2)
            {
                printf("\n");
                printf("%s", combatInvalidChoice);
                printf("\n");
            }
        }

        if (choice == 1)
        {
            /* Player chooses to fight - attack the enemy */
            int playerDamage = player->attack + (rand() % player->luck);

            /* Critical hit chance based on luck (15% base + 2% per luck point) */
            int critChance = 15 + (player->luck * 2);
            int roll = rand() % 100;

            if (roll < critChance)
            {
                /* Critical hit! Double damage */
                playerDamage = playerDamage * 2;
                printf("\n%s\n", criticalHit);
                printf(criticalHitText, player->name);
            }
            else
            {
                printf("\n");
                printf(attackText, player->name);
            }
            printf(" ");
            printf(dealsDamage, playerDamage);
            printf("\n");

            enemy->health -= playerDamage;

            if (enemy->health <= 0)
            {
                printf(defeated, enemy->name);
                printf("\n");
                free(combatStart);
                free(enemyInfo);
                free(yourLuck);
                free(criticalHit);
                free(criticalHitText);
                free(attackText);
                free(dealsDamage);
                free(defeated);
                free(parryTitle);
                free(parryText);
                free(noDamageTaken);
                free(counterAttack);
                free(parryDefeated);
                free(blockingStance);
                free(damageReduced);
                free(enemyAttacks);
                free(playerDefeated);
                free(combatInvalidChoice);
                return 1;
            }
        }
        else if (choice == 2)
        {
            /* Player chooses to block - reduce incoming damage */
            blocking = 1;

            /* Parry chance based on luck (10% base + 3% per luck point) */
            int parryChance = 10 + (player->luck * 3);
            int parryRoll = rand() % 100;

            if (parryRoll < parryChance)
            {
                /* Successful parry! */
                parried = 1;
                printf("\n%s\n", parryTitle);
                printf("%s\n", noDamageTaken);

                /* Counter-attack on parry */
                int counterDamage = player->attack + (rand() % player->luck);
                printf(counterAttack, counterDamage);
                printf("\n");

                enemy->health -= counterDamage;

                if (enemy->health <= 0)
                {
                    printf(parryDefeated, enemy->name);
                    printf("\n");
                    free(combatStart);
                    free(enemyInfo);
                    free(yourLuck);
                    free(criticalHit);
                    free(criticalHitText);
                    free(attackText);
                    free(dealsDamage);
                    free(defeated);
                    free(parryTitle);
                    free(parryText);
                    free(noDamageTaken);
                    free(counterAttack);
                    free(parryDefeated);
                    free(blockingStance);
                    free(damageReduced);
                    free(enemyAttacks);
                    free(playerDefeated);
                    free(combatInvalidChoice);
                    return 1;
                }
            }
            else
            {
                printf("\n");
                printf(blockingStance, player->name);
                printf("\n");
            }
        }

        /* Enemy turn */
        int baseEnemyDamage = enemy->attack + (rand() % 5);
        int finalEnemyDamage = baseEnemyDamage;

        if (blocking)
        {
            if (parried)
            {
                /* Already parried - no additional damage */
                finalEnemyDamage = 0;
            }
            else
            {
                /* Standard block reduces damage by 50% */
                finalEnemyDamage = baseEnemyDamage / 2;
                printf(damageReduced, enemy->name, baseEnemyDamage, finalEnemyDamage);
                printf("\n");
            }
        }
        else
        {
            printf(enemyAttacks, enemy->name, baseEnemyDamage);
            printf("\n");
        }

        damagePlayer(player, finalEnemyDamage);

        if (player->health <= 0)
        {
            printf(playerDefeated, player->name);
            printf("\n");
            free(combatStart);
            free(enemyInfo);
            free(yourLuck);
            free(criticalHit);
            free(criticalHitText);
            free(attackText);
            free(dealsDamage);
            free(defeated);
            free(parryTitle);
            free(parryText);
            free(noDamageTaken);
            free(counterAttack);
            free(parryDefeated);
            free(blockingStance);
            free(damageReduced);
            free(enemyAttacks);
            free(playerDefeated);
            free(combatInvalidChoice);
            return 0;
        }
    }

    free(combatStart);
    free(enemyInfo);
    free(yourLuck);
    free(criticalHit);
    free(criticalHitText);
    free(attackText);
    free(dealsDamage);
    free(defeated);
    free(parryTitle);
    free(parryText);
    free(noDamageTaken);
    free(counterAttack);
    free(parryDefeated);
    free(blockingStance);
    free(damageReduced);
    free(enemyAttacks);
    free(playerDefeated);
    free(combatInvalidChoice);
    return 0;
}

/**
 * @brief Display the upgrades menu
 */
void displayUpgradesMenu()
{
    char *upgradesTitle = getTranslatedText("upgradesTitle");
    char *attackUpgrade = getTranslatedText("attackUpgrade");
    char *healthUpgrade = getTranslatedText("healthUpgrade");
    char *luckUpgrade = getTranslatedText("luckUpgrade");
    char *returnOption = getTranslatedText("returnOption");
    char *upgradeChoice = getTranslatedText("upgradeChoice");

    printf("\n=== %s ===\n", upgradesTitle);
    printf("1. %s\n", attackUpgrade);
    printf("2. %s\n", healthUpgrade);
    printf("3. %s\n", luckUpgrade);
    printf("4. %s\n", returnOption);
    printf("%s ", upgradeChoice);

    free(upgradesTitle);
    free(attackUpgrade);
    free(healthUpgrade);
    free(luckUpgrade);
    free(returnOption);
    free(upgradeChoice);
}

/**
 * @brief Display all players in the game
 */
void showPlayers(GameState *game)
{
    char *playersTitle = getTranslatedText("playersTitle");
    char *playerNumber = getTranslatedText("playerNumber");

    printf("\n");
    printf(playersTitle, game->playerCount, game->playerCount);
    printf("\n");
    for (int i = 0; i < game->playerCount; i++)
    {
        printf(playerNumber, i + 1);
        printf("\n");
        displayPlayer(&game->players[i]);
    }

    free(playersTitle);
    free(playerNumber);
}

void handleChestInterface(GameState *game)
{
    char *chestFound = getTranslatedText("chestFound");
    char *chestDescription = getTranslatedText("chestDescription");
    char *chestQuestion = getTranslatedText("chestQuestion");
    char *openChestText = getTranslatedText("openChest");
    char *leaveChest = getTranslatedText("leaveChest");
    char *yourChoice = getTranslatedText("yourChoice");
    char *chestOpened = getTranslatedText("chestOpened");
    char *chestReward = getTranslatedText("chestReward");
    char *chestEmpty = getTranslatedText("chestEmpty");
    char *chestLeave = getTranslatedText("chestLeave");
    char *chestInvalid = getTranslatedText("chestInvalid");

    printf("\n======================================\n");
    printf("  %s\n", chestFound);
    printf("======================================\n");
    printf("%s\n", chestDescription);
    printf("%s\n\n", chestQuestion);
    printf("1. %s\n", openChestText);
    printf("2. %s\n", leaveChest);
    printf("\n%s ", yourChoice);

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        if (openChest(&game->dungeon, game->dungeon.playerPos.x, game->dungeon.playerPos.y))
        {
            printf("\n*** %s ***\n", chestOpened);

            /* Get a random weapon from the loaded weapons */
            Weapon reward = getRandomWeapon(g_weapons, g_weaponCount);

            char *rewardName = getWeaponTranslatedName(reward.name);
            printf(chestReward, rewardName);
            printf("\n");
            free(rewardName);

            /* Show weapon comparison interface - allows player to equip or decline */
            if (weaponComparisonInterface(&game->players[0], &reward, g_weapons, g_weaponCount))
            {
                equipWeapon(&game->players[0], &reward, g_weapons, g_weaponCount);
            }
        }
        else
            printf("\n%s\n", chestEmpty);
        break;
    case 2:
        printf("\n%s\n", chestLeave);
        game->dungeon.chestOpened = 0; /* Reset so chest can be opened later */
        game->dungeon.playerPos.y--;
        break;
    default:
        printf("\n%s\n", chestInvalid);
        game->dungeon.chestOpened = 0; /* Reset so chest can be opened later */
        game->dungeon.playerPos.y--;
        break;
    }

    free(chestFound);
    free(chestDescription);
    free(chestQuestion);
    free(openChestText);
    free(leaveChest);
    free(yourChoice);
    free(chestOpened);
    free(chestReward);
    free(chestEmpty);
    free(chestLeave);
    free(chestInvalid);
}

void exploreDungeon(GameState *game)
{
    char *explorePrompt = getTranslatedText("explorePrompt");
    char *exploreInvalid = getTranslatedText("exploreInvalid");
    char *congratulations = getTranslatedText("congratulations");
    char *newDungeon = getTranslatedText("newDungeon");
    char *encounterEnemy = getTranslatedText("encounterEnemy");
    char *victoryExp = getTranslatedText("victoryExp");
    char *playerDefeatedRecover = getTranslatedText("playerDefeatedRecover");
    char *wallBlocked = getTranslatedText("wallBlocked");
    char *exitFound = getTranslatedText("exitFound");
    char *bossEncounter = getTranslatedText("bossEncounter");
    char *bossInfo = getTranslatedText("bossInfo");
    char *bossDefeated = getTranslatedText("bossDefeated");
    char *bossVictory = getTranslatedText("bossVictory");
    char *bossNotDefeated = getTranslatedText("bossNotDefeated");

    /* Check if this is a boss room (every 5 rooms: rooms 5, 10, 15, etc.) */
    int isBossRoom = (game->roomsSinceLastBoss % 5 == 4);

    /* Spawn boss at exit if this is a boss room */
    if (isBossRoom && !game->dungeon.bossFound)
    {
        spawnBossAtExit(&game->dungeon, game->currentLevel);
        game->bossActive = 1;
    }

    displayDungeon(&game->dungeon);

    /* Show boss warning if active */
    if (game->bossActive)
    {
        printf("\n!!! ATTENTION: UN BOSS GARDE LA SORTIE !!!\n");
        printf("Vous devez le vaincre pour completer ce donjon!\n\n");
    }

    int exploring = 1;
    while (exploring)
    {
        char move;
        printf("\n%s", explorePrompt);
        fflush(stdout);

        /* Read the first character - if it is a newline, read the actual input */
        move = getchar();
        if (move == '\n')
        {
            move = getchar();
        }

        if (move == '1')
        {
            exploring = 0;
            continue;
        }

        if (move != 'z' && move != 's' && move != 'q' && move != 'd')
        {
            printf("%s\n", exploreInvalid);
            continue;
        }

        int result = movePlayer(&game->dungeon, move);

        /* Check if player is on a chest (return code 3) */
        if (result == 3)
        {
            handleChestInterface(game);
        }

        /* Check if player encountered the boss (return code 4) */
        if (result == 4 && game->bossActive)
        {
            printf("\n%s\n", bossEncounter);

            /* Create and fight the boss */
            Enemy boss = createBoss(game->currentLevel);
            printf(bossInfo, boss.name, boss.health, boss.attack);
            printf("\n\n");

            int victory = combat(&game->players[0], &boss);

            if (victory)
            {
                printf(bossDefeated, boss.name);
                printf("\n");
                printf(bossVictory, boss.experience);
                printf("\n");
                game->players[0].attack += 5;
                game->bossActive = 0;
                game->dungeon.bossFound = 0;
                game->dungeon.bossPos.x = -1;
                game->dungeon.bossPos.y = -1;
            }
            else
            {
                printf("%s\n", playerDefeatedRecover);
                game->players[0].health = game->players[0].maxHealth;
                /* Push player back from boss */
                game->dungeon.playerPos.y--;
            }
        }
        else if (result == 2) /* Exit found */
        {
            /* Check if boss is blocking the exit */
            if (game->bossActive)
            {
                printf("\n%s\n", bossNotDefeated);
                /* Push player back */
                game->dungeon.playerPos.y--;
            }
            else
            {
                printf("\n %s\n", congratulations);
                /* Heal the player for 40% of maxHealth when completing a room */
                int healAmount = (game->players[0].maxHealth * 40) / 100;
                if (healAmount > 0)
                {
                    healPlayer(&game->players[0], healAmount);
                }

                /* Increment rooms counter for boss tracking */
                game->roomsSinceLastBoss++;

                game->currentLevel++;

                initDungeon(&game->dungeon, game->currentLevel);
                printf("\n");
                printf(newDungeon, game->currentLevel);
                printf("\n");
            }
        }
        else if (result >= 10) /* Enemy encounter - forced collision! */
        {
            int enemyIdx = result - 10;
            printf("\n%s\n", encounterEnemy);
            Enemy enemy = createEnemy(game->difficulty);
            int victory = combat(&game->players[0], &enemy);

            if (victory)
            {
                printf(victoryExp, enemy.experience);
                printf("\n");
                game->players[0].attack += 2;
                removeEnemy(&game->dungeon, enemyIdx);
            }
            else
            {
                printf("%s\n", playerDefeatedRecover);
                game->players[0].health = game->players[0].maxHealth;
            }
        }
        else if (result == 1 || result == 3)
        {
            if (result == 1)
            {
                int enemyAttack = updateEnemies(&game->dungeon);
                if (enemyAttack >= 0)
                {
                    /* Enemy attacked player - trigger combat */
                    printf("\n%s\n", encounterEnemy);
                    Enemy enemy = createEnemy(game->difficulty);
                    int victory = combat(&game->players[0], &enemy);

                    if (victory)
                    {
                        printf(victoryExp, enemy.experience);
                        printf("\n");
                        game->players[0].attack += 2;
                        removeEnemy(&game->dungeon, enemyAttack);
                    }
                    else
                    {
                        printf("%s\n", playerDefeatedRecover);
                        game->players[0].health = game->players[0].maxHealth;
                    }
                }
            }
        }

        if (exploring)
            displayDungeon(&game->dungeon);
    }

    free(explorePrompt);
    free(exploreInvalid);
    free(congratulations);
    free(newDungeon);
    free(encounterEnemy);
    free(victoryExp);
    free(playerDefeatedRecover);
    free(wallBlocked);
    free(exitFound);
    free(bossEncounter);
    free(bossInfo);
    free(bossDefeated);
    free(bossVictory);
    free(bossNotDefeated);
}

/**
 * @brief Main game loop
 */
void playGame(GameState *game, const char *language)
{
    srand(time(NULL));

    /* Load weapons from CSV file */
    char *weaponsLoaded = getTranslatedText("weaponsLoaded");
    char *weaponsWarning = getTranslatedText("weaponsWarning");

    if (!loadWeapons(g_weapons, &g_weaponCount))
    {
        printf("%s\n", weaponsWarning);
    }
    else
    {
        printf(weaponsLoaded, g_weaponCount);
        printf("\n");
    }

    free(weaponsLoaded);
    free(weaponsWarning);

    char *welcomeTitle = getTranslatedText("welcomeTitle");
    char *exploreInstructions = getTranslatedText("exploreInstructions");
    char *visionInstructions = getTranslatedText("visionInstructions");
    char *chestInstructions = getTranslatedText("chestInstructions");

    printf("\n=== %s ===\n", welcomeTitle);
    printf("%s\n", exploreInstructions);
    printf("%s\n", visionInstructions);
    printf("%s\n", chestInstructions);

    free(welcomeTitle);
    free(exploreInstructions);
    free(visionInstructions);
    free(chestInstructions);

    int playing = 1;
    while (playing)
    {
        displayDungeon(&game->dungeon);
        displayGameMenu();

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: /* Explore */
            exploreDungeon(game);
            break;

        case 2: /* Display players */
            showPlayers(game);
            break;

        case 3: /* Upgrades */
        {
            char *attackIncreased = getTranslatedText("attackIncreased");
            char *healthRestored = getTranslatedText("healthRestored");
            char *luckIncreased = getTranslatedText("luckIncreased");

            int upgradeChoice = 1;
            while (upgradeChoice != 4)
            {
                displayUpgradesMenu();
                scanf("%d", &upgradeChoice);

                switch (upgradeChoice)
                {
                case 1:
                    addAttack(&game->players[0], 5);
                    printf("%s\n", attackIncreased);
                    break;
                case 2:
                    healPlayer(&game->players[0], 10);
                    printf("%s\n", healthRestored);
                    break;
                case 3:
                    addLuck(&game->players[0], 2);
                    printf("%s\n", luckIncreased);
                    break;
                }
            }
            free(attackIncreased);
            free(healthRestored);
            free(luckIncreased);
            break;
        }

        case 4: /* Save */
            saveGame(game->saveName, game->players, game->playerCount, &game->dungeon,
                     game->currentLevel, game->bossActive, game->roomsSinceLastBoss, game->difficulty, game->multiplayer);
            break;

        case 5: /* Return to menu */
            playing = 0;
            char *returningMenu = getTranslatedText("returningMenu");
            printf("%s\n", returningMenu);
            free(returningMenu);
            break;

        default:
            char *invalidChoice = getTranslatedText("invalidChoice");
            printf("%s\n", invalidChoice);
            free(invalidChoice);
        }
    }
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
