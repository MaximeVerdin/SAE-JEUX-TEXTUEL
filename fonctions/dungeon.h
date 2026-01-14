#ifndef DUNGEON_H
#define DUNGEON_H

#include "player.h"

#define DUNGEON_SIZE 20 /**< Size of the dungeon grid (20x20) */
#define WALL '#'        /**< Character representing walls */
#define FLOOR '.'       /**< Character representing floor tiles */
#define PLAYER '@'      /**< Character representing the player */
#define ENEMY 'E'       /**< Character representing enemies */
#define EXIT 'X'        /**< Character representing exit */
#define UNSEEN ' '      /**< Character representing unseen tiles */
#define CHEST 'C'       /**< Character representing treasure chests */

/**
 * @brief Position structure for 2D coordinates
 *
 * Represents a position on the dungeon grid using x and y coordinates.
 */
typedef struct
{
    int x, y; /**< X and Y coordinates */
} Position;

// Forward declaration for Enemy struct
typedef struct Enemy Enemy;

/**
 * @brief Enemy structure representing dungeon enemies
 *
 * Contains all information about an enemy including name, health,
 * attack power, and experience value when defeated.
 */
struct Enemy
{
    char name[50];  /**< Enemy name */
    int health;     /**< Enemy health points */
    int attack;     /**< Enemy attack power */
    int experience; /**< Experience gained when defeated */
};

/**
 * @brief Dungeon structure representing the game level
 *
 * Contains the complete dungeon state including the grid layout,
 * player position, enemy positions, chest locations, and visibility data.
 */
typedef struct
{
    char grid[DUNGEON_SIZE][DUNGEON_SIZE];    /**< 2D grid of tile types */
    int visible[DUNGEON_SIZE][DUNGEON_SIZE];  /**< Visibility fog-of-war data */
    int explored[DUNGEON_SIZE][DUNGEON_SIZE]; /**< Explored tile tracking */
    Position playerPos;                       /**< Player's current position */
    Position exitPos;                         /**< Dungeon exit position */
    int enemyCount;                           /**< Number of active enemies */
    Position enemies[10];                     /**< Array of enemy positions */
    int chestCount;                           /**< Number of chests in dungeon */
    Position chests[5];                       /**< Array of chest positions */
    int chestOpened;                          /**< Flag if current chest opened */
    int enemyFound;                           /**< Tutorial: enemy discovered */
    int bossFound;                            /**< Tutorial: boss discovered */
    Position bossPos;                         /**< Tutorial boss position */
} Dungeon;

/**
 * @brief Initialize a new dungeon for the given level
 *
 * Creates a fresh dungeon layout with walls, floors, enemies,
 * and exit positioned appropriately for the difficulty level.
 *
 * @param dungeon Pointer to the dungeon structure to initialize
 * @param level Current difficulty level
 */
void initDungeon(Dungeon *dungeon, int level);

/**
 * @brief Generate a procedurally created dungeon
 *
 * Creates a random dungeon layout with connected rooms,
 * corridors, obstacles, enemies, and treasure chests.
 *
 * @param dungeon Pointer to the dungeon structure to populate
 */
void generateDungeon(Dungeon *dungeon);

/**
 * @brief Generate the tutorial dungeon layout
 *
 * Creates a fixed tutorial map designed to teach players
 * basic mechanics including movement, combat, and objectives.
 *
 * @param dungeon Pointer to the dungeon structure to initialize
 */
void generateTutorialDungeon(Dungeon *dungeon);

/**
 * @brief Display the dungeon grid in console
 *
 * Renders the current dungeon state showing the player,
 * visible enemies, chests, exit, and terrain features.
 * Uses colored output for different entity types.
 *
 * @param dungeon Pointer to the dungeon to display
 */
void displayDungeon(Dungeon *dungeon);

/**
 * @brief Move the player in the specified direction
 *
 * Updates the player's position based on input direction.
 * Handles wall collisions, enemy encounters, chest interactions,
 * and exit discovery. Returns codes indicating the result.
 *
 * @param dungeon Pointer to the current dungeon
 * @param direction Movement direction (z/s/q/d)
 * @return Result code: 0=failed, 1=moved, 2=exit found, 3=on chest, 10+=enemy
 */
int movePlayer(Dungeon *dungeon, char direction);

/**
 * @brief Check if an enemy exists at the specified position
 *
 * Searches the enemy array for an enemy at the given coordinates.
 *
 * @param dungeon Pointer to the current dungeon
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @return Enemy index if found, -1 otherwise
 */
int checkEnemyAt(Dungeon *dungeon, int x, int y);

/**
 * @brief Remove an enemy from the dungeon
 *
 * Removes the specified enemy from the active enemy list
 * and clears their position on the grid.
 *
 * @param dungeon Pointer to the current dungeon
 * @param index Index of the enemy to remove
 */
void removeEnemy(Dungeon *dungeon, int index);

/**
 * @brief Check if a position contains a wall
 *
 * Returns true if the specified coordinates contain a wall
 * or are outside the dungeon boundaries.
 *
 * @param dungeon Pointer to the current dungeon
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @return 1 if wall, 0 if passable
 */
int isWall(Dungeon *dungeon, int x, int y);

/**
 * @brief Update the visibility fog-of-war
 *
 * Recalculates which tiles are visible from the player's
 * current position using raycasting for line-of-sight.
 *
 * @param dungeon Pointer to the dungeon to update
 */
void updateVision(Dungeon *dungeon);

/**
 * @brief Check line-of-sight between two points
 *
 * Uses Bresenham's line algorithm to determine if
 * there is an unobstructed path between two positions.
 *
 * @param dungeon Pointer to the current dungeon
 * @param x1 Starting X coordinate
 * @param y1 Starting Y coordinate
 * @param x2 Target X coordinate
 * @param y2 Target Y coordinate
 * @return 1 if line-of-sight exists, 0 if blocked
 */
int hasLineOfSight(Dungeon *dungeon, int x1, int y1, int x2, int y2);

/* ===== Enemy AI Functions ===== */

/**
 * @brief Update all enemy positions and behaviors
 *
 * For each enemy that can see the player, attempts to move
 * them toward the player's position with basic pathfinding.
 *
 * @param dungeon Pointer to the current dungeon
 */
void updateEnemies(Dungeon *dungeon);

/**
 * @brief Move a specific enemy toward the player
 *
 * Uses simple pathfinding to move the enemy toward the player.
 * If the enemy reaches the player, combat is initiated.
 *
 * @param dungeon Pointer to the current dungeon
 * @param enemyIndex Index of the enemy to move
 * @return Enemy index if attacking player, -1 otherwise
 */
int moveEnemyTowardPlayer(Dungeon *dungeon, int enemyIdx);

/**
 * @brief Check if an enemy can see the player
 *
 * Determines if line-of-sight exists and if the player
 * is within detection range of the specified enemy.
 *
 * @param dungeon Pointer to the current dungeon
 * @param enemyIndex Index of the enemy
 * @return 1 if player is visible, 0 otherwise
 */
int canSeePlayer(Dungeon *dungeon, int enemyIdx);

/**
 * @brief Check if player is adjacent to an enemy
 *
 * Returns true if the specified enemy is in a neighboring
 * tile (including diagonals) to the player's position.
 *
 * @param dungeon Pointer to the current dungeon
 * @param enemyIndex Index of the enemy
 * @return 1 if adjacent, 0 otherwise
 */
int isPlayerAdjacent(Dungeon *dungeon, int enemyIdx);

/* ===== Chest Functions ===== */

/**
 * @brief Check if a chest exists at the specified position
 *
 * Searches the chest array for a chest at the given coordinates.
 *
 * @param dungeon Pointer to the current dungeon
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @return Chest index if found, -1 otherwise
 */
int checkChestAt(Dungeon *dungeon, int x, int y);

/**
 * @brief Open a chest at the specified position
 *
 * Opens the chest if one exists at the coordinates,
 * marking it as opened and clearing it from the grid.
 *
 * @param dungeon Pointer to the current dungeon
 * @param x X coordinate of the chest
 * @param y Y coordinate of the chest
 * @return 1 if chest opened, 0 if no chest found
 */
int openChest(Dungeon *dungeon, int x, int y);

/**
 * @brief Place random treasure chests in the dungeon
 *
 * Randomly places 0-3 treasure chests in valid locations
 * (floor tiles not occupied by player, exit, or enemies).
 *
 * @param dungeon Pointer to the dungeon to populate
 */
void placeRandomChests(Dungeon *dungeon);

/**
 * @brief Place tutorial-specific elements
 *
 * Places a chest at a specific tutorial-appropriate location
 * for the tutorial gameplay sequence.
 *
 * @param dungeon Pointer to the tutorial dungeon
 */
void placeTutorialElements(Dungeon *dungeon);

/**
 * @brief Create an enemy with difficulty-based stats
 *
 * Generates a new enemy with health, attack, and experience
 * scaled according to the specified difficulty level.
 *
 * @param difficulty Current game difficulty level
 * @return Initialized Enemy structure
 */
Enemy createEnemy(int difficulty);

/**
 * @brief Run a combat encounter between player and enemy
 *
 * Executes a turn-based combat loop where the player and
 * enemy exchange attacks until one is defeated.
 *
 * @param player Pointer to the player
 * @param enemy Pointer to the enemy
 * @return 1 if player wins, 0 if enemy wins
 */
int combat(Player *player, Enemy *enemy);

/**
 * @brief Run the tutorial combat encounter
 *
 * Special combat sequence for the tutorial that guides
 * the player through basic combat mechanics.
 *
 * @param player Pointer to the player
 * @return 1 if tutorial completed, 0 if failed
 */
int fightTutorialEnemy(Player *player);

/**
 * @brief Run a boss fight encounter
 *
 * Special boss combat with enhanced enemy stats and
 * unique boss name generation.
 *
 * @param player Pointer to the player
 * @param bossName Name of the boss type
 * @param race Boss race/classification
 * @return 1 if boss defeated, 0 if player fails
 */
int fightBoss(Player *player, const char *bossName, const char *race);

#endif
