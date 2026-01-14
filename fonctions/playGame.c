#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playGame.h"
#include "player.h"
#include "saveManagement.h"
#include "upgradeTree.h"
#include "dungeon.h"
#include "utils.h"

/**
 * @brief Display the main game menu
 */
void displayGameMenu()
{
    printf("\n=== GAME MENU ===\n");
    printf("1. Explore (z/up, s/down, q/left, d/right)\n");
    printf("2. Fight a nearby enemy\n");
    printf("3. Display players\n");
    printf("4. Upgrades\n");
    printf("5. Save\n");
    printf("6. Return to main menu\n");
    printf("Your choice: ");
}

/**
 * @brief Display tutorial-specific game menu with limited options
 */
void displayTutorialGameMenu(GameState *game)
{
    printf("\n=== GAME MENU (TUTORIAL) ===\n");

    if (game->tutorialMode == 1 && game->tutorialStep == 0)
    {
        printf("1. Explore (discover the dungeon)\n");
        printf("Your choice: ");
    }
    else if (game->tutorialMode == 1 && game->tutorialStep <= 2)
    {
        printf("1. Explore (open the chest C)\n");
        printf("Your choice: ");
    }
    else if (game->tutorialMode == 1 && game->tutorialStep <= 4)
    {
        printf("1. Explore (face the enemy E)\n");
        printf("2. Combat (when adjacent to an enemy)\n");
        printf("Your choice: ");
    }
    else if (game->tutorialMode == 1 && game->tutorialStep <= 6)
    {
        printf("1. Explore (avoid the boss B and reach X)\n");
        printf("Your choice: ");
    }
    else
    {
        displayGameMenu();
    }
}

/**
 * @brief Display dungeon navigation menu
 */
void displayDungeonMenu()
{
    printf("\n=== DUNGEON - Level %d ===\n", 1);
    printf("Use z/s/q/d to move\n");
    printf("1. Return to menu\n");
    printf("Your choice: ");
}

/**
 * @brief Display combat action menu
 */
void displayCombatMenu()
{
    printf("\n=== COMBAT ACTIONS ===\n");
    printf("1. Fight - Attack the enemy\n");
    printf("2. Block - Reduce incoming damage by 50%% (parry chance based on luck)\n");
    printf("Your choice: ");
}

/**
 * @brief Run a combat encounter between player and enemy
 */
int combat(Player *player, Enemy *enemy)
{
    printf("\n=== COMBAT: %s vs %s ===\n", player->name, enemy->name);
    printf("%s - Health: %d, Attack: %d\n", enemy->name, enemy->health, enemy->attack);
    printf("Your Luck: %d (higher luck = better critical hits and parries)\n", player->luck);

    int blocking = 0; /* Track if player is blocking this turn */
    int parried = 0;  /* Track if player successfully parried */

    while (enemy->health > 0 && player->health > 0)
    {
        printf("\n%s (Health: %d) VS %s (Health: %d)\n",
               player->name, player->health, enemy->name, enemy->health);

        /* Reset blocking status at start of turn */
        blocking = 0;
        parried = 0;

        /* Display combat menu and get player choice */
        displayCombatMenu();
        int choice;
        scanf("%d", &choice);

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
                printf("\n*** CRITICAL HIT! ***\n");
                printf("%s strikes with deadly precision! ", player->name);
            }
            else
            {
                printf("\n%s attacks! ", player->name);
            }

            enemy->health -= playerDamage;
            printf("Deals %d damage!\n", playerDamage);

            if (enemy->health <= 0)
            {
                printf("%s has been defeated!\n", enemy->name);
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
                printf("\n*** PARRY! ***\n");
                printf("%s perfectly blocks the attack and counters!\n", player->name);
                printf("No damage taken and you strike back!\n");

                /* Counter-attack on parry */
                int counterDamage = player->attack + (rand() % player->luck);
                enemy->health -= counterDamage;
                printf("Counter-attack deals %d damage!\n", counterDamage);

                if (enemy->health <= 0)
                {
                    printf("%s has been defeated by the parry counter!\n", enemy->name);
                    return 1;
                }
            }
            else
            {
                printf("\n%s takes a defensive stance! Incoming damage will be reduced.\n", player->name);
            }
        }
        else
        {
            /* Invalid choice - treat as fight */
            printf("\nInvalid choice! %s attacks instead!\n", player->name);
            int playerDamage = player->attack + (rand() % player->luck);
            enemy->health -= playerDamage;
            printf("%s deals %d damage!\n", player->name, playerDamage);

            if (enemy->health <= 0)
            {
                printf("%s has been defeated!\n", enemy->name);
                return 1;
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
                printf("%s attacks! You blocked! Damage reduced from %d to %d.\n",
                       enemy->name, baseEnemyDamage, finalEnemyDamage);
            }
        }
        else
        {
            printf("%s attacks! Deals %d damage!\n", enemy->name, baseEnemyDamage);
        }

        damagePlayer(player, finalEnemyDamage);

        if (player->health <= 0)
        {
            printf("%s has been defeated...\n", player->name);
            return 0;
        }
    }

    return 0;
}

/**
 * @brief Display the upgrades menu
 */
void displayUpgradesMenu()
{
    printf("\n=== UPGRADES ===\n");
    printf("1. +5 Attack (100 coins)\n");
    printf("2. +10 Health (100 coins)\n");
    printf("3. +2 Luck (150 coins)\n");
    printf("4. Return\n");
    printf("Your choice: ");
}

/**
 * @brief Display all players in the game
 */
void showPlayers(GameState *game)
{
    printf("\n=== PLAYERS (%d/%d) ===\n", game->playerCount, game->playerCount);
    for (int i = 0; i < game->playerCount; i++)
    {
        printf("\nPlayer %d:\n", i + 1);
        displayPlayer(&game->players[i]);
    }
}

/**
 * @brief Handle chest interaction interface
 */
void handleChestInterface(GameState *game)
{
    printf("\n======================================\n");
    printf("  CHEST FOUND!\n");
    printf("======================================\n");
    printf("You found a mysterious chest.\n");
    printf("What would you like to do?\n\n");
    printf("1. Open the chest\n");
    printf("2. Leave it and move on\n");
    printf("\nYour choice: ");

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        if (openChest(&game->dungeon, game->dungeon.playerPos.x, game->dungeon.playerPos.y))
        {
            printf("\n*** YOU OPENED THE CHEST! ***\n");
            printf("You find: 50 gold coins!\n");
            game->players[0].attack += 5;
        }
        else
            printf("\nThe chest is already empty.\n");
        break;
    case 2:
        printf("\nYou decide to leave the chest for later.\n");
        game->dungeon.playerPos.y--;
        break;
    default:
        printf("\nInvalid choice. You leave the chest.\n");
        game->dungeon.playerPos.y--;
        break;
    }
}

/**
 * @brief Main dungeon exploration loop
 */
void exploreDungeon(GameState *game)
{
    displayDungeon(&game->dungeon);

    int exploring = 1;
    while (exploring)
    {
        char move;
        printf("\nDirection (z/up, s/down, q/left, d/right) or 1/return: ");
        fflush(stdout);

        /* Read the first character - if it's a newline, read the actual input */
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
            printf("Invalid direction! Use z/s/q/d\n");
            continue;
        }

        int result = movePlayer(&game->dungeon, move);

        /* Check if player is on a chest (return code 3) */
        if (result == 3)
        {
            handleChestInterface(game);

            if (game->tutorialMode == 1 && game->tutorialStep == 1)
                runTutorial("en", &game->tutorialStep, 2);
        }

        /* Check for chest interaction */
        int chestIdx = checkChestAt(&game->dungeon, game->dungeon.playerPos.x, game->dungeon.playerPos.y);
        if (chestIdx >= 0 && !game->dungeon.chestOpened)
        {
            if (game->tutorialMode == 1 && game->tutorialStep == 0)
                runTutorial("en", &game->tutorialStep, 1);
        }

        /* Boss dodge mechanic in tutorial */
        if (game->tutorialMode == 1 && game->tutorialStep == 5)
        {
            int bx = game->dungeon.bossPos.x;
            int by = game->dungeon.bossPos.y;
            int px = game->dungeon.playerPos.x;
            int py = game->dungeon.playerPos.y;

            int dist = abs(bx - px) + abs(by - py);
            if (dist == 1)
            {
                printf("\n!!! THE BOSS TRIES TO ATTACK YOU !!!\n");
                printf("You dodge the attack and flee!\n");
                runTutorial("en", &game->tutorialStep, 6);

                game->dungeon.grid[by][bx] = FLOOR;
                game->tutorialStep = 7;
            }
        }

        if (result == 2) /* Exit found */
        {
            printf("\n Congratulations! You completed the dungeon!\n");
            game->currentLevel++;
            game->progress++;

            if (game->progress % 3 == 0)
            {
                game->difficulty++;
                printf("Difficulty increased! Level %d\n", game->difficulty);
            }

            initDungeon(&game->dungeon, game->currentLevel);
            printf("\nNew dungeon generated - Level %d\n", game->currentLevel);
        }
        else if (result >= 10) /* Enemy encounter - forced collision! */
        {
            int enemyIdx = result - 10;
            printf("\n!!! ENCOUNTER WITH AN ENEMY !!!\n");
            Enemy enemy = createEnemy(game->difficulty);
            int victory = combat(&game->players[0], &enemy);

            if (victory)
            {
                printf("Victory! +%d experience\n", enemy.experience);
                game->players[0].attack += 2;
                removeEnemy(&game->dungeon, enemyIdx);

                if (game->tutorialMode == 1 && game->tutorialStep == 3)
                    runTutorial("en", &game->tutorialStep, 4);
            }
            else
            {
                printf("You have been defeated! Recovering...\n");
                game->players[0].health = 100;
            }
        }
        else if (result == 1 || result == 3)
        {
            if (result == 1)
                updateEnemies(&game->dungeon);
        }

        if (exploring)
            displayDungeon(&game->dungeon);
    }
}

/**
 * @brief Main game loop
 */
void playGame(GameState *game, const char *language)
{
    srand(time(NULL));

    if (game->tutorialMode == 1)
        placeTutorialElements(&game->dungeon);

    printf("\n=== WELCOME TO THE GAME ===\n");
    printf("Explore the dungeon to find the exit (X)\n");
    printf("You can only see what is in your field of vision!\n");
    printf("Walk on a chest (C) to open it!\n");

    if (game->tutorialMode == 1)
        runTutorial(language, &game->tutorialStep, 0);

    int playing = 1;
    while (playing)
    {
        displayDungeon(&game->dungeon);

        if (game->tutorialMode == 1 && game->tutorialStep <= 6)
            displayTutorialGameMenu(game);
        else
            displayGameMenu();

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: /* Explore */
            exploreDungeon(game);
            break;

        case 2: /* Fight nearby enemy */
        {
            if (game->tutorialMode == 1 && game->tutorialStep < 3)
            {
                printf("You cannot fight right now. Follow the tutorial!\n");
                break;
            }

            int dx[4] = {0, 0, -1, 1};
            int dy[4] = {-1, 1, 0, 0};
            int found = 0;

            for (int i = 0; i < 4 && !found; i++)
            {
                int nx = game->dungeon.playerPos.x + dx[i];
                int ny = game->dungeon.playerPos.y + dy[i];
                int enemyIdx = checkEnemyAt(&game->dungeon, nx, ny);

                if (enemyIdx >= 0)
                {
                    found = 1;
                    Enemy enemy = createEnemy(game->difficulty);
                    int victory = combat(&game->players[0], &enemy);

                    if (victory)
                    {
                        printf("Victory! +%d experience\n", enemy.experience);
                        game->players[0].attack += 2;
                        removeEnemy(&game->dungeon, enemyIdx);
                    }
                    else
                    {
                        printf("You have been defeated! Recovering...\n");
                        game->players[0].health = 100;
                    }
                }
            }

            if (!found)
                printf("No adjacent enemy! Use Explore to move.\n");
            break;
        }

        case 3: /* Display players */
            showPlayers(game);
            break;

        case 4: /* Upgrades */
        {
            int upgradeChoice = 1;
            while (upgradeChoice != 4)
            {
                displayUpgradesMenu();
                scanf("%d", &upgradeChoice);

                switch (upgradeChoice)
                {
                case 1:
                    addAttack(&game->players[0], 5);
                    printf("Attack increased!\n");
                    break;
                case 2:
                    healPlayer(&game->players[0], 10);
                    printf("Health restored!\n");
                    break;
                case 3:
                    addLuck(&game->players[0], 2);
                    printf("Luck increased!\n");
                    break;
                }
            }
            break;
        }

        case 5: /* Save */
            saveGame("current_save", game->players, game->playerCount, game->progress, &game->dungeon);
            break;

        case 6: /* Return to menu */
            playing = 0;
            printf("Returning to main menu...\n");
            break;

        default:
            printf("Invalid choice!\n");
        }
    }

    /* Free memory */
    for (int i = 0; i < game->playerCount; i++)
        freePlayerAbilities(&game->players[i]);
}
