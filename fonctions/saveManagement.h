/* Author names : VERDIN Maxime and FOURNIER Nathan */

/* Author names : VERDIN Maxime and FOURNIER Nathan */

#ifndef SAVEMANAGEMENT_H
#define SAVEMANAGEMENT_H

#include "player.h"
#include "dungeon.h"

/**
 * @brief GameState structure containing all game data
 *
 * Stores the complete game state including all players,
 * dungeon, progress, and tutorial state.
 */
typedef struct
{
    Player players[4];      /**< Array of players in the game */
    int playerCount;        /**< Number of active players */
    int difficulty;         /**< Current difficulty level */
    int currentLevel;       /**< Current dungeon level */
    Dungeon dungeon;        /**< Current dungeon state */
    int tutorialMode;       /**< 1 if tutorial is active */
    int tutorialStep;       /**< Current tutorial progress */
    int multiplayer;        /**< 1 if multiplayer mode */
    int skipTutorial;       /**< 1 if tutorial should be skipped */
    char saveName[50];      /**< Name of the save file */
    int bossActive;         /**< 1 if boss is currently spawned */
    int roomsSinceLastBoss; /**< Counter for rooms completed since last boss */
} GameState;

/**
 * @brief Create a new game save file
 *
 * Initializes a new save file with the given game name,
 * difficulty setting, and tutorial preference.
 *
 * @param name Name of the save file
 * @param difficulty Difficulty level (0=easy, 1=medium, 2=hard)
 * @param skipTutorial 1 to skip tutorial, 0 to play
 */
void createGame(char *name, int difficulty, int skipTutorial);

/**
 * @brief Add players to an existing save file
 *
 * Appends player data to the specified save file.
 *
 * @param saveName Name of the save file
 * @param players Array of players to add
 * @param playerCount Number of players to add
 */
void addPlayersToSave(char *saveName, Player *players, int playerCount);

/**
 * @brief Load and display a save file by name
 *
 * Reads the save file and displays its contents.
 *
 * @param saveName Name of the save file to load
 * @return 0 on success, -1 if file not found
 */
int loadGameByName(char *saveName);

/**
 * @brief Save the current game state with level progress
 *
 * Writes the current game progress, level state, and player data
 * to the specified save file. This version saves the complete
 * game state including current level, boss status, and difficulty.
 *
 * @param saveName Name of the save file
 * @param players Array of players
 * @param playerCount Number of players
 * @param dungeon Pointer to the dungeon structure
 * @param currentLevel Current dungeon level
 * @param bossActive 1 if boss is currently active
 * @param roomsSinceLastBoss Rooms completed since last boss
 * @param difficulty Current difficulty level
 * @param multiplayer 1 if multiplayer mode
 */
void saveGame(char *saveName, Player *players, int playerCount, Dungeon *dungeon,
              int currentLevel, int bossActive, int roomsSinceLastBoss, int difficulty, int multiplayer);

/**
 * @brief Load a complete game state from save file
 *
 * Reads all game data from the save file and returns
 * a fully initialized GameState structure. This includes
 * level progress, boss status, and all player data.
 *
 * @param saveName Name of the save file to load
 * @return GameState structure with loaded data
 */
GameState loadGameState(const char *saveName);

#endif

/* Author names : VERDIN Maxime and FOURNIER Nathan */

