#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveManagement.h"
#include "player.h"
#include "dungeon.h"

/**
 * @brief Create a new game save file
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
    fprintf(file, "Name:%s\n", name);
    fprintf(file, "Difficulty:%d\n", difficulty);
    fprintf(file, "Progress:%d\n", 0);
    fprintf(file, "SkipTutorial:%d\n", skipTutorial);
    fprintf(file, "!\n");
    fprintf(file, "player;name;health;attack;luck;weapon\n");
    fclose(file);
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
        fprintf(file, "%d;%s;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].health,
                players[i].attack,
                players[i].luck,
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
 * @brief Save the current game state
 */
void saveGame(char *saveName, Player *players, int playerCount, int progress, Dungeon *dungeon)
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
    fprintf(file, "Progress:%d\n", progress);

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

    /* Save chest positions and state */
    fprintf(file, "ChestCount:%d\n", dungeon->chestCount);
    fprintf(file, "ChestOpened:%d\n", dungeon->chestOpened);
    for (int c = 0; c < dungeon->chestCount; c++)
    {
        fprintf(file, "Chest:%d;%d;%d\n", c, dungeon->chests[c].x, dungeon->chests[c].y);
    }

    fprintf(file, "!\n");
    fprintf(file, "player;name;health;attack;luck;weapon\n");

    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%d;%s;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].health,
                players[i].attack,
                players[i].luck,
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
    game.progress = 0;
    game.difficulty = 1;
    game.currentLevel = 1;
    game.tutorialMode = 0;
    game.tutorialStep = 0;

    /* Create default player */
    Player defaultPlayer = createPlayer("Hero", 100, 10, 5, "Hands");
    game.players[0] = defaultPlayer;

    /* Initialize dungeon */
    initDungeon(&game.dungeon, 1);

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

            /* Check if this is a regular line (not a key:value or section marker) */
            if (strchr(buffer, ':') == NULL &&
                strchr(buffer, ';') == NULL &&
                gridRow < DUNGEON_SIZE &&
                strlen(buffer) >= DUNGEON_SIZE)
            {
                for (int j = 0; j < DUNGEON_SIZE && j < (int)strlen(buffer); j++)
                {
                    game.dungeon.grid[gridRow][j] = buffer[j];
                }
                gridRow++;
                continue;
            }
        }

        /* Parse game info lines */
        if (strncmp(buffer, "Name:", 5) == 0)
        {
            printf("Save name: %s\n", buffer + 5);
        }
        else if (strncmp(buffer, "Progress:", 9) == 0)
        {
            game.progress = atoi(buffer + 9);
            printf("Progress: %d\n", game.progress);
        }
        else if (strncmp(buffer, "Difficulty:", 11) == 0)
        {
            game.difficulty = atoi(buffer + 11);
            printf("Difficulty: %d\n", game.difficulty);
        }
        else if (strncmp(buffer, "SkipTutorial:", 13) == 0)
        {
            game.skipTutorial = atoi(buffer + 13);
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
        else if (strncmp(buffer, "ChestCount:", 11) == 0)
        {
            game.dungeon.chestCount = atoi(buffer + 11);
        }
        else if (strncmp(buffer, "ChestOpened:", 12) == 0)
        {
            game.dungeon.chestOpened = atoi(buffer + 12);
        }
        else if (strncmp(buffer, "Chest:", 6) == 0)
        {
            /* Format: Chest:index;x;y */
            char *token = strtok(buffer + 6, ";");
            if (token)
            {
                int idx = atoi(token);
                token = strtok(NULL, ";");
                if (token)
                {
                    game.dungeon.chests[idx].x = atoi(token);
                    token = strtok(NULL, ";");
                    if (token && idx >= 0 && idx < 5)
                        game.dungeon.chests[idx].y = atoi(token);
                }
            }
        }
        else if (strcmp(buffer, "!") == 0 && !inGridSection)
        {
            inPlayerSection = 1;
            continue;
        }
        /* Parse player data lines (format: index;name;health;attack;luck;weapon) */
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
            game.players[playerIdx].attack = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL)
                continue;
            game.players[playerIdx].luck = atoi(token);

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

            printf("Loaded player %d: %s (Health:%d, Attack:%d, Luck:%d, Weapon:%s)\n",
                   playerIdx + 1,
                   game.players[playerIdx].name,
                   game.players[playerIdx].health,
                   game.players[playerIdx].attack,
                   game.players[playerIdx].luck,
                   game.players[playerIdx].weapon);
        }
    }

    fclose(file);
    printf("Game loaded successfully!\n");

    return game;
}
