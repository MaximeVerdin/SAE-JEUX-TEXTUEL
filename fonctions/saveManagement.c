/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "saveManagement.h"
#include "player.h"
#include "dungeon.h"

/**
 * @brief Create a new game save file
 *
 * Initializes a new save file with the given game name,
 * difficulty setting, and tutorial preference.
 * Randomly selects a story file for this game.
 */
void createGame(char *name, int difficulty, int skipTutorial)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", name);

    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Error creating game file.\n");
        return;
    }

    /* Randomly select a story (1 to TOTAL_STORIES) */
    srand((unsigned int)time(NULL));
    int selectedStory = (rand() % TOTAL_STORIES) + 1;

    fprintf(file, "Name:%s\n", name);
    fprintf(file, "Difficulty:%d\n", difficulty);
    fprintf(file, "SkipTutorial:%d\n", skipTutorial);
    /* Initialize boss tracking for new game - boss spawns after 5 rooms */
    fprintf(file, "BossActive:0\n");
    fprintf(file, "RoomsSinceLastBoss:0\n");
    fprintf(file, "CurrentLevel:1\n");
    /* Save the randomly selected story number */
    fprintf(file, "SelectedStory:%d\n", selectedStory);
    fprintf(file, "!\n");
    fprintf(file, "player;name;health;maxHealth;attack;luck;coins;weapon\n");
    fclose(file);

    printf("New game created with story %d\n", selectedStory);
}

/**
 * @brief Add players to an existing save file
 */
void addPlayersToSave(char *saveName, Player *players, int playerCount)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "a");
    if (!file)
    {
        printf("Error opening game file for adding players.\n");
        return;
    }

    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%d;%s;%d;%d;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].health,
                players[i].maxHealth,
                players[i].attack,
                players[i].luck,
                players[i].coins,
                players[i].weapon);
    }
    fprintf(file, "!\n");
    fclose(file);
}

/**
 * @brief Load and display a save file by name
 */
int loadGameByName(char *saveName)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Error: Save file '%s' not found.\n", saveName);
        return -1;
    }

    char buffer[256];
    printf("Loading game '%s'...\n", saveName);

    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}

/**
 * @brief Save the current game state with level progress
 */
void saveGame(char *saveName, Player *players, int playerCount, Dungeon *dungeon,
              int currentLevel, int bossActive, int roomsSinceLastBoss, int difficulty, int multiplayer,
              int selectedStory)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Error saving game.\n");
        return;
    }

    fprintf(file, "Name:%s\n", saveName);

    /* Save game state - level progress and difficulty */
    fprintf(file, "CurrentLevel:%d\n", currentLevel);
    fprintf(file, "BossActive:%d\n", bossActive);
    fprintf(file, "RoomsSinceLastBoss:%d\n", roomsSinceLastBoss);
    fprintf(file, "Difficulty:%d\n", difficulty);
    fprintf(file, "Multiplayer:%d\n", multiplayer);
    fprintf(file, "SkipTutorial:1\n");

    /* Save selected story */
    fprintf(file, "SelectedStory:%d\n", selectedStory);

    /* Save dungeon state - player position (room number) */
    fprintf(file, "RoomX:%d\n", dungeon->playerPos.x);
    fprintf(file, "RoomY:%d\n", dungeon->playerPos.y);

    /* Save dungeon grid */
    fprintf(file, "Grid:\n");
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            fprintf(file, "%c", dungeon->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    /* Save enemy positions */
    fprintf(file, "EnemyCount:%d\n", dungeon->enemyCount);
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        fprintf(file, "Enemy:%d;%d;%d\n", e, dungeon->enemies[e].x, dungeon->enemies[e].y);
    }

    /* Save chest state */
    fprintf(file, "HasChest:%d\n", dungeon->hasChest);
    fprintf(file, "ChestOpened:%d\n", dungeon->chestOpened);
    if (dungeon->hasChest)
    {
        fprintf(file, "Chest:%d;%d\n", dungeon->chest.x, dungeon->chest.y);
    }

    fprintf(file, "!\n");
    fprintf(file, "player;name;health;maxHealth;attack;luck;coins;weapon\n");

    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%d;%s;%d;%d;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].health,
                players[i].maxHealth,
                players[i].attack,
                players[i].luck,
                players[i].coins,
                players[i].weapon);
    }

    fprintf(file, "!\n");
    fclose(file);
    printf("Game saved successfully!\n");
}

/**
 * @brief Load a complete game state from save file
 */
GameState loadGameState(const char *saveName)
{
    GameState game;

    /* Initialize with defaults */
    memset(&game, 0, sizeof(GameState));
    game.playerCount = 1;
    game.difficulty = 1;
    game.currentLevel = 1;
    game.tutorialStep = 0;
    game.bossActive = 0;
    game.roomsSinceLastBoss = 0;
    game.multiplayer = 0;
    game.skipTutorial = 1;
    game.selectedStory = 1; /* Default to story 1 */
    /* tutorialMode is set based on skipTutorial after loading */

    /* Create default player */
    Player defaultPlayer = createPlayer("Hero", 100, 10, 5, "Hands");
    game.players[0] = defaultPlayer;

    /* Initialize dungeon - will be overwritten if save has dungeon data */
    initDungeon(&game.dungeon, 1, 1);

    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Error: Save file '%s' not found.\n", saveName);
        return game;
    }

    char buffer[256];
    int inPlayerSection = 0;
    int inGridSection = 0;
    int gridRow = 0;
    int hasGridData = 0;

    printf("Loading game '%s'...\n", saveName);

    while (fgets(buffer, sizeof(buffer), file))
    {
        /* Remove newline */
        buffer[strcspn(buffer, "\n")] = 0;

        /* Check for grid section */
        if (strcmp(buffer, "Grid:") == 0)
        {
            inGridSection = 1;
            gridRow = 0;
            continue;
        }

        /* Parse dungeon grid */
        if (inGridSection && !inPlayerSection)
        {
            if (strcmp(buffer, "!") == 0)
            {
                inGridSection = 0;
                inPlayerSection = 1;
                continue;
            }

            /* Check if this is a regular grid row (length >= DUNGEON_SIZE) */
            if (gridRow < DUNGEON_SIZE && strlen(buffer) >= DUNGEON_SIZE)
            {
                /* Only process if it doesn't contain key:value patterns or section markers */
                if (strchr(buffer, ':') == NULL && strchr(buffer, ';') == NULL)
                {
                    hasGridData = 1;
                    for (int j = 0; j < DUNGEON_SIZE && j < (int)strlen(buffer); j++)
                    {
                        game.dungeon.grid[gridRow][j] = buffer[j];
                    }
                    gridRow++;
                    continue;
                }
            }
        }

        /* Parse game info lines */
        if (strncmp(buffer, "Name:", 5) == 0)
        {
            printf("Save name: %s\n", buffer + 5);
        }
        else if (strncmp(buffer, "CurrentLevel:", 13) == 0)
        {
            game.currentLevel = atoi(buffer + 13);
            printf("Current Level: %d\n", game.currentLevel);
        }
        else if (strncmp(buffer, "BossActive:", 11) == 0)
        {
            game.bossActive = atoi(buffer + 11);
            printf("Boss Active: %d\n", game.bossActive);
        }
        else if (strncmp(buffer, "RoomsSinceLastBoss:", 19) == 0)
        {
            game.roomsSinceLastBoss = atoi(buffer + 19);
            printf("Rooms Since Last Boss: %d\n", game.roomsSinceLastBoss);
        }
        else if (strncmp(buffer, "Difficulty:", 11) == 0)
        {
            game.difficulty = atoi(buffer + 11);
            printf("Difficulty: %d\n", game.difficulty);
        }
        else if (strncmp(buffer, "Multiplayer:", 12) == 0)
        {
            game.multiplayer = atoi(buffer + 12);
            printf("Multiplayer: %d\n", game.multiplayer);
        }
        else if (strncmp(buffer, "SkipTutorial:", 13) == 0)
        {
            game.skipTutorial = atoi(buffer + 13);
        }
        /* Parse SelectedStory for story selection */
        else if (strncmp(buffer, "SelectedStory:", 14) == 0)
        {
            game.selectedStory = atoi(buffer + 14);
            printf("Selected Story: %d\n", game.selectedStory);
        }
        /* Parse RoomX and RoomY for player position */
        else if (strncmp(buffer, "RoomX:", 6) == 0)
        {
            game.dungeon.playerPos.x = atoi(buffer + 6);
            printf("Player X position: %d\n", game.dungeon.playerPos.x);
        }
        else if (strncmp(buffer, "RoomY:", 6) == 0)
        {
            game.dungeon.playerPos.y = atoi(buffer + 6);
            printf("Player Y position: %d\n", game.dungeon.playerPos.y);
        }
        else if (strncmp(buffer, "EnemyCount:", 11) == 0)
        {
            game.dungeon.enemyCount = atoi(buffer + 11);
        }
        else if (strncmp(buffer, "Enemy:", 6) == 0)
        {
            /* Format: Enemy:index;x;y */
            char *token = strtok(buffer + 6, ";");
            if (token)
            {
                int idx = atoi(token);
                token = strtok(NULL, ";");
                if (token && idx >= 0 && idx < 10)
                {
                    game.dungeon.enemies[idx].x = atoi(token);
                    token = strtok(NULL, ";");
                    if (token)
                        game.dungeon.enemies[idx].y = atoi(token);
                }
            }
        }
        else if (strncmp(buffer, "HasChest:", 9) == 0)
        {
            game.dungeon.hasChest = atoi(buffer + 9);
        }
        else if (strncmp(buffer, "ChestOpened:", 12) == 0)
        {
            game.dungeon.chestOpened = atoi(buffer + 12);
        }
        else if (strncmp(buffer, "Chest:", 6) == 0)
        {
            /* Format: Chest:x;y */
            char *token = strtok(buffer + 6, ";");
            if (token)
            {
                game.dungeon.chest.x = atoi(token);
                token = strtok(NULL, ";");
                if (token)
                    game.dungeon.chest.y = atoi(token);
            }
        }
        else if (strcmp(buffer, "!") == 0 && !inGridSection)
        {
            inPlayerSection = 1;
            continue;
        }
        /* Parse player data lines (format: index;name;health;maxHealth;attack;luck;coins;weapon) */
        else if (inPlayerSection && strstr(buffer, ";") != NULL)
        {
            if (strncmp(buffer, "player;", 7) == 0)
            {
                continue; /* Skip header line */
            }

            char *token = strtok(buffer, ";");
            if (token == NULL)
                continue;

            int playerIdx = atoi(token);
            if (playerIdx < 0 || playerIdx >= 4)
                continue;

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;

            /* Store player name */
            strncpy(game.players[playerIdx].name, token, sizeof(game.players[playerIdx].name) - 1);
            game.players[playerIdx].name[sizeof(game.players[playerIdx].name) - 1] = '\0';

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].health = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].maxHealth = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].attack = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].luck = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].coins = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            strncpy(game.players[playerIdx].weapon, token, sizeof(game.players[playerIdx].weapon) - 1);
            game.players[playerIdx].weapon[sizeof(game.players[playerIdx].weapon) - 1] = '\0';

            /* Update player count */
            if (playerIdx + 1 > game.playerCount)
            {
                game.playerCount = playerIdx + 1;
            }

            printf("Loaded player %d: %s (Health:%d/%d, Attack:%d, Luck:%d, Coins:%d, Weapon:%s)\n",
                   playerIdx + 1,
                   game.players[playerIdx].name,
                   game.players[playerIdx].health,
                   game.players[playerIdx].maxHealth,
                   game.players[playerIdx].attack,
                   game.players[playerIdx].luck,
                   game.players[playerIdx].coins,
                   game.players[playerIdx].weapon);
        }
    }

    fclose(file);

    /* If we have dungeon grid data, we need to reinitialize with the saved level */
    if (hasGridData)
    {
        /* Reinitialize dungeon with the saved current level but don't regenerate grid */
        initDungeon(&game.dungeon, game.currentLevel, 0);

        /* Re-apply the saved grid data */
        file = fopen(fileName, "r");
        if (file)
        {
            inGridSection = 0;
            gridRow = 0;

            while (fgets(buffer, sizeof(buffer), file))
            {
                buffer[strcspn(buffer, "\n")] = 0;

                if (strcmp(buffer, "Grid:") == 0)
                {
                    inGridSection = 1;
                    gridRow = 0;
                    continue;
                }

                if (inGridSection)
                {
                    if (strcmp(buffer, "!") == 0)
                    {
                        inGridSection = 0;
                        continue;
                    }

                    if (gridRow < DUNGEON_SIZE && strlen(buffer) >= DUNGEON_SIZE)
                    {
                        if (strchr(buffer, ':') == NULL && strchr(buffer, ';') == NULL)
                        {
                            for (int j = 0; j < DUNGEON_SIZE && j < (int)strlen(buffer); j++)
                            {
                                game.dungeon.grid[gridRow][j] = buffer[j];
                            }
                            gridRow++;
                        }
                    }
                }
            }
            fclose(file);
        }
    }

    printf("Game loaded successfully!\n");

    /* Update vision based on loaded player position */
    updateVision(&game.dungeon);

    /* Set tutorialMode based on skipTutorial (tutorial is shown if skipTutorial is 0) */
    game.tutorialMode = !game.skipTutorial;

    return game;
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
