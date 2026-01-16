/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dungeon.h"
#include "player.h"

/**
 * @brief Initialize a new dungeon for the given level
 *
 * @param dungeon Pointer to the dungeon structure to initialize
 * @param level Current difficulty level
 * @param generateGrid If 1, generates a new procedural dungeon; if 0, only resets state
 */
void initDungeon(Dungeon *dungeon, int level, int generateGrid)
{
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            dungeon->grid[i][j] = WALL;
            dungeon->visible[i][j] = 0;
            dungeon->explored[i][j] = 0;
        }
    }
    dungeon->enemyCount = 0;
    dungeon->hasChest = 0;
    dungeon->chestOpened = 0;
    dungeon->enemyFound = 0;
    dungeon->bossFound = 0;
    dungeon->bossPos.x = -1;
    dungeon->bossPos.y = -1;

    if (generateGrid)
    {
        generateDungeon(dungeon);
    }
}

/**
 * @brief Generate a procedurally created dungeon
 */
void generateDungeon(Dungeon *dungeon)
{
    int numRooms = 6 + rand() % 4;

    int roomCenters[15][2];
    int roomCount = 0;

    for (int r = 0; r < numRooms; r++)
    {
        int roomW = 5 + rand() % 5;
        int roomH = 5 + rand() % 5;
        int roomX = 1 + rand() % (DUNGEON_SIZE - roomW - 2);
        int roomY = 1 + rand() % (DUNGEON_SIZE - roomH - 2);

        for (int i = roomY; i < roomY + roomH && i < DUNGEON_SIZE - 1; i++)
        {
            for (int j = roomX; j < roomX + roomW && j < DUNGEON_SIZE - 1; j++)
            {
                dungeon->grid[i][j] = FLOOR;
            }
        }

        roomCenters[roomCount][0] = roomX + roomW / 2;
        roomCenters[roomCount][1] = roomY + roomH / 2;
        roomCount++;
    }

    for (int i = 1; i < roomCount; i++)
    {
        int x1 = roomCenters[i - 1][0];
        int y1 = roomCenters[i - 1][1];
        int x2 = roomCenters[i][0];
        int y2 = roomCenters[i][1];

        if (rand() % 2 == 0)
        {
            int startX = (x1 < x2) ? x1 : x2;
            int endX = (x1 < x2) ? x2 : x1;
            for (int x = startX; x <= endX; x++)
            {
                if (x > 0 && x < DUNGEON_SIZE - 1)
                    dungeon->grid[y1][x] = FLOOR;
            }
            int startY = (y1 < y2) ? y1 : y2;
            int endY = (y1 < y2) ? y2 : y1;
            for (int y = startY; y <= endY; y++)
            {
                if (y > 0 && y < DUNGEON_SIZE - 1)
                    dungeon->grid[y][x2] = FLOOR;
            }
        }
        else
        {
            int startY = (y1 < y2) ? y1 : y2;
            int endY = (y1 < y2) ? y2 : y1;
            for (int y = startY; y <= endY; y++)
            {
                if (y > 0 && y < DUNGEON_SIZE - 1)
                    dungeon->grid[y][x1] = FLOOR;
            }
            int startX = (x1 < x2) ? x1 : x2;
            int endX = (x1 < x2) ? x2 : x1;
            for (int x = startX; x <= endX; x++)
            {
                if (x > 0 && x < DUNGEON_SIZE - 1)
                    dungeon->grid[y2][x] = FLOOR;
            }
        }
    }

    for (int r = 0; r < numRooms; r++)
    {
        int roomW = 5 + rand() % 5;
        int roomH = 5 + rand() % 5;
        int roomX = 1 + rand() % (DUNGEON_SIZE - roomW - 2);
        int roomY = 1 + rand() % (DUNGEON_SIZE - roomH - 2);

        int numObstacles = 1 + rand() % 3;
        for (int o = 0; o < numObstacles; o++)
        {
            int obsX = roomX + 1 + rand() % (roomW - 2);
            int obsY = roomY + 1 + rand() % (roomH - 2);

            if (obsX > roomX && obsX < roomX + roomW - 1 &&
                obsY > roomY && obsY < roomY + roomH - 1)
            {
                dungeon->grid[obsY][obsX] = WALL;
            }
        }
    }

    for (int i = 3; i < DUNGEON_SIZE - 3; i++)
    {
        for (int j = 3; j < DUNGEON_SIZE - 3; j++)
        {
            if (dungeon->grid[i][j] == FLOOR && rand() % 15 == 0)
            {
                int floorNeighbors = 0;
                if (i > 0 && dungeon->grid[i - 1][j] == FLOOR)
                    floorNeighbors++;
                if (i < DUNGEON_SIZE - 1 && dungeon->grid[i + 1][j] == FLOOR)
                    floorNeighbors++;
                if (j > 0 && dungeon->grid[i][j - 1] == FLOOR)
                    floorNeighbors++;
                if (j < DUNGEON_SIZE - 1 && dungeon->grid[i][j + 1] == FLOOR)
                    floorNeighbors++;

                if (floorNeighbors >= 3)
                    dungeon->grid[i][j] = WALL;
            }
        }
    }

    int placed = 0;
    for (int i = 1; i < DUNGEON_SIZE - 1 && !placed; i++)
    {
        for (int j = 1; j < DUNGEON_SIZE - 1 && !placed; j++)
        {
            if (dungeon->grid[i][j] == FLOOR)
            {
                dungeon->playerPos.x = j;
                dungeon->playerPos.y = i;
                placed = 1;
            }
        }
    }

    int exitPlaced = 0;
    for (int i = DUNGEON_SIZE - 2; i > 0 && !exitPlaced; i--)
    {
        for (int j = DUNGEON_SIZE - 2; j > 0 && !exitPlaced; j--)
        {
            if (dungeon->grid[i][j] == FLOOR &&
                (i != dungeon->playerPos.y || j != dungeon->playerPos.x))
            {
                dungeon->exitPos.x = j;
                dungeon->exitPos.y = i;
                dungeon->grid[i][j] = EXIT;
                exitPlaced = 1;
            }
        }
    }

    /* Ensure exit is accessible by making at least one adjacent tile a floor */
    ensureExitAccess(dungeon);

    /* Ensure player is connected to the rest of the dungeon */
    ensurePlayerAccess(dungeon);

    dungeon->enemyCount = 1 + rand() % 3;
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        int p = 0;
        for (int i = 1; i < DUNGEON_SIZE - 1 && !p; i++)
        {
            for (int j = 1; j < DUNGEON_SIZE - 1 && !p; j++)
            {
                int dist = abs(i - dungeon->playerPos.y) + abs(j - dungeon->playerPos.x);
                if (dungeon->grid[i][j] == FLOOR &&
                    (i != dungeon->playerPos.y || j != dungeon->playerPos.x) &&
                    (i != dungeon->exitPos.y || j != dungeon->exitPos.x) &&
                    dist >= 5)
                {
                    dungeon->enemies[e].x = j;
                    dungeon->enemies[e].y = i;
                    p = 1;
                }
            }
        }
    }

    placeRandomChests(dungeon);

    /* Boss position is initialized in initDungeon() */
    /* Boss will be spawned by spawnBossAtExit() when appropriate */

    updateVision(dungeon);
}

/**
 * @brief Spawn a boss at the exit position
 *
 * Places a boss at the exit of the dungeon to block progress.
 * The boss replaces the exit marker until defeated.
 *
 * @param dungeon Pointer to the current dungeon
 * @param level Current dungeon level for difficulty scaling
 */
void spawnBossAtExit(Dungeon *dungeon, int level)
{
    /* Place boss at exit position */
    dungeon->bossPos.x = dungeon->exitPos.x;
    dungeon->bossPos.y = dungeon->exitPos.y;
    dungeon->bossFound = 1;

    printf("\n");
    printf("!!! UN BOSS GARDE LA SORTIE! !!!\n");
    printf("Vous devez vaincre %s pour progresser!\n", createBoss(level).name);
    printf("\n");
}

/**
 * @brief Check line-of-sight using Bresenham's algorithm
 */
int hasLineOfSight(Dungeon *dungeon, int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1, y = y1;

    while (1)
    {
        if (x == x2 && y == y2)
            return 1;

        if (dungeon->grid[y][x] == WALL && !(x == x1 && y == y1))
            return 0;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }
    }
}

/**
 * @brief Update fog-of-war visibility
 */
void updateVision(Dungeon *dungeon)
{
    int visionRange = 3;

    for (int i = 0; i < DUNGEON_SIZE; i++)
        for (int j = 0; j < DUNGEON_SIZE; j++)
            dungeon->visible[i][j] = 0;

    for (int dy = -visionRange; dy <= visionRange; dy++)
    {
        for (int dx = -visionRange; dx <= visionRange; dx++)
        {
            if (dx == 0 && dy == 0)
            {
                dungeon->visible[dungeon->playerPos.y][dungeon->playerPos.x] = 1;
                continue;
            }

            int tx = dungeon->playerPos.x + dx;
            int ty = dungeon->playerPos.y + dy;

            if (tx >= 0 && tx < DUNGEON_SIZE && ty >= 0 && ty < DUNGEON_SIZE)
            {
                if (hasLineOfSight(dungeon, dungeon->playerPos.x,
                                   dungeon->playerPos.y, tx, ty))
                {
                    dungeon->visible[ty][tx] = 1;
                    dungeon->explored[ty][tx] = 1;
                }
            }
        }
    }
}

/**
 * @brief Display the dungeon grid
 */
void displayDungeon(Dungeon *dungeon)
{
    printf("\n  ");
    for (int j = 0; j < DUNGEON_SIZE; j++)
        printf("%d ", j % 10);
    printf("\n");

    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        printf("%d ", i % 10);
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            if (dungeon->visible[i][j])
            {
                /* Check if player and boss are on the same tile - show B@ */
                if (i == dungeon->playerPos.y && j == dungeon->playerPos.x &&
                    dungeon->bossFound && dungeon->bossPos.x == j && dungeon->bossPos.y == i)
                {
                    printf("\033[1;35mB\033[0m\033[1;32m@\033[0m ");
                }
                /* Check if player is on this tile */
                else if (i == dungeon->playerPos.y && j == dungeon->playerPos.x)
                    printf("\033[1;32m%c\033[0m ", PLAYER);
                /* Check if boss is on this tile */
                else if (dungeon->bossFound &&
                         dungeon->bossPos.x == j && dungeon->bossPos.y == i)
                {
                    printf("\033[1;35m%c\033[0m ", 'B');
                }
                /* Check for regular enemies */
                else
                {
                    int isEnemy = 0;
                    for (int e = 0; e < dungeon->enemyCount; e++)
                    {
                        if (dungeon->enemies[e].x == j && dungeon->enemies[e].y == i)
                        {
                            printf("\033[1;31m%c\033[0m ", ENEMY);
                            isEnemy = 1;
                            break;
                        }
                    }
                    if (!isEnemy)
                    {
                        /* Apply colors to chests, exit, and floor */
                        if (dungeon->grid[i][j] == CHEST)
                            printf("\033[1;33m%c\033[0m ", CHEST); /* Gold for chest */
                        else if (dungeon->grid[i][j] == EXIT)
                            printf("\033[1;36m%c\033[0m ", EXIT); /* Cyan for exit */
                        else if (dungeon->grid[i][j] == FLOOR)
                            printf("\033[0;90m%c\033[0m ", FLOOR); /* Dark gray for floor */
                        else
                            printf("%c ", dungeon->grid[i][j]); /* Walls and others */
                    }
                }
            }
            else
                printf("  ");
        }
        printf("\n");
    }

    printf("\n\033[1;32m@\033[0m Player  \033[1;35mB\033[0m Boss  \033[1;31mE\033[0m Enemy  \033[1;36mX\033[0m Exit  # Wall  \033[1;33mC\033[0m Chest\n");
}

/**
 * @brief Check if position contains a wall
 */
int isWall(Dungeon *dungeon, int x, int y)
{
    if (x < 0 || x >= DUNGEON_SIZE || y < 0 || y >= DUNGEON_SIZE)
        return 1;
    return dungeon->grid[y][x] == WALL;
}

/**
 * @brief Move player in specified direction
 */
int movePlayer(Dungeon *dungeon, char direction)
{
    int newX = dungeon->playerPos.x;
    int newY = dungeon->playerPos.y;

    switch (direction)
    {
    case 'z':
        newY--;
        break;
    case 's':
        newY++;
        break;
    case 'q':
        newX--;
        break;
    case 'd':
        newX++;
        break;
    default:
        return 0;
    }

    if (isWall(dungeon, newX, newY))
    {
        printf("Wall! You cannot pass.\n");
        return 0;
    }

    /* Check if moving onto boss position - return 4 for boss encounter */
    if (dungeon->bossFound && dungeon->bossPos.x == newX && dungeon->bossPos.y == newY)
    {
        dungeon->playerPos.x = newX;
        dungeon->playerPos.y = newY;
        updateVision(dungeon);
        return 4; /* Boss encounter code */
    }

    int enemyIdx = checkEnemyAt(dungeon, newX, newY);
    if (enemyIdx >= 0)
    {
        dungeon->playerPos.x = newX;
        dungeon->playerPos.y = newY;
        updateVision(dungeon);
        return 10 + enemyIdx;
    }

    /* Check if player is trying to access exit - if boss is there, trigger boss encounter */
    if (dungeon->grid[newY][newX] == EXIT)
    {
        if (dungeon->bossFound)
        {
            /* Boss is at the exit - move player to boss position to trigger encounter */
            dungeon->playerPos.x = newX;
            dungeon->playerPos.y = newY;
            updateVision(dungeon);
            return 4; /* Boss encounter code */
        }
        printf("You found the exit! Level completed!\n");
        return 2;
    }

    int chestFound = checkChestAt(dungeon, newX, newY);
    if (chestFound && !dungeon->chestOpened)
    {
        dungeon->playerPos.x = newX;
        dungeon->playerPos.y = newY;
        updateVision(dungeon);
        return 3;
    }

    /* Reset chestOpened flag if player is not on a chest */
    if (!checkChestAt(dungeon, dungeon->playerPos.x, dungeon->playerPos.y))
    {
        dungeon->chestOpened = 0;
    }

    dungeon->playerPos.x = newX;
    dungeon->playerPos.y = newY;
    updateVision(dungeon);
    return 1;
}

/**
 * @brief Check if enemy exists at position
 */
int checkEnemyAt(Dungeon *dungeon, int x, int y)
{
    for (int e = 0; e < dungeon->enemyCount; e++)
        if (dungeon->enemies[e].x == x && dungeon->enemies[e].y == y)
            return e;
    return -1;
}

/**
 * @brief Remove enemy from dungeon
 */
void removeEnemy(Dungeon *dungeon, int index)
{
    int oldX = dungeon->enemies[index].x;
    int oldY = dungeon->enemies[index].y;

    for (int e = index; e < dungeon->enemyCount - 1; e++)
        dungeon->enemies[e] = dungeon->enemies[e + 1];
    dungeon->enemyCount--;

    if (oldX >= 0 && oldX < DUNGEON_SIZE && oldY >= 0 && oldY < DUNGEON_SIZE)
        dungeon->grid[oldY][oldX] = FLOOR;
}

/**
 * @brief Check if position is valid for movement
 */
int canMoveTo(Dungeon *dungeon, int x, int y)
{
    if (isWall(dungeon, x, y))
        return 0;
    if (dungeon->grid[y][x] == EXIT)
        return 0;
    for (int e = 0; e < dungeon->enemyCount; e++)
        if (dungeon->enemies[e].x == x && dungeon->enemies[e].y == y)
            return 0;
    return 1;
}

/**
 * @brief Check if player is adjacent to enemy
 */
int isPlayerAdjacent(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x, ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x, py = dungeon->playerPos.y;
    return (abs(ex - px) <= 1 && abs(ey - py) <= 1);
}

/**
 * @brief Check if enemy can see player
 */
int canSeePlayer(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x, ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x, py = dungeon->playerPos.y;
    int dx = abs(ex - px), dy = abs(ey - py);
    if (dx > 4 || dy > 4)
        return 0;
    return hasLineOfSight(dungeon, ex, ey, px, py);
}

/**
 * @brief Move enemy toward player with "cut off" behavior
 * @note Enemies normally move horizontally OR vertically, never diagonally
 * @note BUT: Enemies CAN move diagonally when 1 tile away from player (adjacent)
 * @note Enemies try to cut off the player by aligning their row or column
 */
int moveEnemyTowardPlayer(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x, ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x, py = dungeon->playerPos.y;

    if (ex == px && ey == py)
        return enemyIdx;

    int dx = px - ex;
    int dy = py - ey;
    int absDx = abs(dx);
    int absDy = abs(dy);

    /* Check if enemy is adjacent to player (1 tile away in any direction: cardinal or diagonal) */
    int isCardinallyAdjacent = ((absDx == 1 && absDy == 0) || (absDx == 0 && absDy == 1));
    int isDiagonallyAdjacent = (absDx == 1 && absDy == 1);
    int isAdjacent = isCardinallyAdjacent || isDiagonallyAdjacent;

    /* If adjacent (cardinal or diagonal), move directly to player position to trigger combat */
    if (isAdjacent && canMoveTo(dungeon, px, py))
    {
        int nx = px;
        int ny = py;
        dungeon->grid[ey][ex] = FLOOR;
        dungeon->enemies[enemyIdx].x = nx;
        dungeon->enemies[enemyIdx].y = ny;
        return enemyIdx; /* Return enemyIdx to trigger combat! */
    }

    /* Try to "cut off" the player - align with player's row or column first */
    /* Priority: try to close the larger gap first (more effective cutting off) */
    int triedHorizontalFirst = 0;

    /* Strategy: If we're not aligned with player, try to align first */
    if (ex != px && ey != py)
    {
        /* Not aligned - try to cut off by moving in the direction of larger gap */
        if (absDx >= absDy)
        {
            /* Try horizontal first to align X with player */
            int nx = ex + (dx > 0 ? 1 : -1);
            int ny = ey;
            if (canMoveTo(dungeon, nx, ny))
            {
                dungeon->grid[ey][ex] = FLOOR;
                dungeon->enemies[enemyIdx].x = nx;
                dungeon->enemies[enemyIdx].y = ny;
                return -1;
            }
            triedHorizontalFirst = 1;
        }
        else
        {
            /* Try vertical first to align Y with player */
            int nx = ex;
            int ny = ey + (dy > 0 ? 1 : -1);
            if (canMoveTo(dungeon, nx, ny))
            {
                dungeon->grid[ey][ex] = FLOOR;
                dungeon->enemies[enemyIdx].x = nx;
                dungeon->enemies[enemyIdx].y = ny;
                return -1;
            }
        }
    }

    /* If aligned on one axis, move along that axis toward player */
    if (ex == px)
    {
        /* Aligned horizontally - move vertically toward player */
        int ny = ey + (dy > 0 ? 1 : -1);
        if (canMoveTo(dungeon, ex, ny))
        {
            dungeon->grid[ey][ex] = FLOOR;
            dungeon->enemies[enemyIdx].y = ny;
            return -1;
        }
    }
    else if (ey == py)
    {
        /* Aligned vertically - move horizontally toward player */
        int nx = ex + (dx > 0 ? 1 : -1);
        if (canMoveTo(dungeon, nx, ey))
        {
            dungeon->grid[ey][ex] = FLOOR;
            dungeon->enemies[enemyIdx].x = nx;
            return -1;
        }
    }

    /* Try alternative directions - only cardinal (no diagonals) */
    int dirs[4][2];

    if (triedHorizontalFirst)
    {
        /* We tried horizontal, now try vertical directions */
        dirs[0][0] = 0;
        dirs[0][1] = (dy > 0 ? 1 : -1); /* vertical toward player */
        dirs[1][0] = 0;
        dirs[1][1] = (dy > 0 ? -1 : 1); /* vertical away */
        dirs[2][0] = (dx > 0 ? 1 : -1);
        dirs[2][1] = 0; /* horizontal toward player */
        dirs[3][0] = (dx > 0 ? -1 : 1);
        dirs[3][1] = 0; /* horizontal away */
    }
    else
    {
        /* We tried vertical, now try horizontal directions */
        dirs[0][0] = (dx > 0 ? 1 : -1);
        dirs[0][1] = 0; /* horizontal toward player */
        dirs[1][0] = (dx > 0 ? -1 : 1);
        dirs[1][1] = 0; /* horizontal away */
        dirs[2][0] = 0;
        dirs[2][1] = (dy > 0 ? 1 : -1); /* vertical toward player */
        dirs[3][0] = 0;
        dirs[3][1] = (dy > 0 ? -1 : 1); /* vertical away */
    }

    for (int d = 0; d < 4; d++)
    {
        int nx = ex + dirs[d][0];
        int ny = ey + dirs[d][1];
        if (canMoveTo(dungeon, nx, ny))
        {
            dungeon->grid[ey][ex] = FLOOR;
            dungeon->enemies[enemyIdx].x = nx;
            dungeon->enemies[enemyIdx].y = ny;
            return -1;
        }
    }

    return -1;
}

/**
 * @brief Update all enemy positions
 */
int updateEnemies(Dungeon *dungeon)
{
    int attackResult = -1;
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        if (canSeePlayer(dungeon, e))
        {
            int result = moveEnemyTowardPlayer(dungeon, e);
            if (result >= 0)
            {
                printf("\n!!! AN ENEMY IS ATTACKING YOU !!!\n");
                attackResult = result;
            }
        }
    }
    return attackResult;
}

/**
 * @brief Check if chest exists at position
 */
int checkChestAt(Dungeon *dungeon, int x, int y)
{
    if (dungeon->hasChest && dungeon->chest.x == x && dungeon->chest.y == y)
        return 1;
    return 0;
}

/**
 * @brief Open chest at position
 */
int openChest(Dungeon *dungeon, int x, int y)
{
    if (checkChestAt(dungeon, x, y))
    {
        dungeon->grid[y][x] = FLOOR;
        dungeon->hasChest = 0;
        dungeon->chestOpened = 1;
        return 1;
    }
    return 0;
}

/**
 * @brief Ensure the exit is accessible by clearing at least one adjacent floor
 *
 * This function checks all 4 adjacent tiles to the exit and converts
 * one of them to a floor if they are all walls, ensuring the exit
 * is reachable by the player.
 */
void ensureExitAccess(Dungeon *dungeon)
{
    int ex = dungeon->exitPos.x;
    int ey = dungeon->exitPos.y;

    /* Check all 4 directions */
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

    for (int d = 0; d < 4; d++)
    {
        int nx = ex + directions[d][0];
        int ny = ey + directions[d][1];

        /* If valid position and not the player's position */
        if (nx >= 0 && nx < DUNGEON_SIZE && ny >= 0 && ny < DUNGEON_SIZE)
        {
            if (!(nx == dungeon->playerPos.x && ny == dungeon->playerPos.y))
            {
                /* Convert this tile to floor to ensure access */
                dungeon->grid[ny][nx] = FLOOR;
                break;
            }
        }
    }
}

/**
 * @brief Place a random treasure chest
 */
void placeRandomChests(Dungeon *dungeon)
{
    dungeon->hasChest = 0;
    dungeon->chestOpened = 0;

    /* 20% chance no chest in dungeon */
    if (rand() % 5 == 0)
        return;

    /* Place at most one chest */
    int p = 0, attempts = 0;
    while (!p && attempts < 100)
    {
        int x = 1 + rand() % (DUNGEON_SIZE - 2);
        int y = 1 + rand() % (DUNGEON_SIZE - 2);

        if (dungeon->grid[y][x] == FLOOR &&
            (x != dungeon->playerPos.x || y != dungeon->playerPos.y) &&
            (x != dungeon->exitPos.x || y != dungeon->exitPos.y) &&
            checkEnemyAt(dungeon, x, y) < 0)
        {
            dungeon->grid[y][x] = CHEST;
            dungeon->chest.x = x;
            dungeon->chest.y = y;
            dungeon->hasChest = 1;
            p = 1;
        }
        attempts++;
    }
}

/**
 * @brief Create enemy with difficulty scaling
 */
Enemy createEnemy(int difficulty)
{
    Enemy enemy;
    const char *names[] = {"Goblin", "Orc", "Skeleton", "Troll", "Dragon"};
    strcpy(enemy.name, names[rand() % 5]);
    enemy.health = 20 + (difficulty * 10) + (rand() % 20);
    enemy.attack = 5 + (difficulty * 3) + (rand() % 8);
    enemy.experience = 10 + (difficulty * 5);
    return enemy;
}

/**
 * @brief Create a powerful boss enemy with enhanced stats
 *
 * Boss enemies have significantly higher stats than regular enemies
 * and scale with the current dungeon level. Boss names are more
 * menacing and include titles.
 *
 * @param level Current dungeon level
 * @return Initialized Enemy structure with boss stats
 */
Enemy createBoss(int level)
{
    Enemy enemy;
    const char *bossNames[] = {
        "Goblin King", "Orc Warlord", "Lich Lord",
        "Troll Chieftain", "Ancient Dragon", "Demon Lord",
        "Dark Knight", "Shadow Mage", "Beast Master", "Warrior Prime"};
    strcpy(enemy.name, bossNames[rand() % 10]);

    /* Boss stats scale with level - adjusted for better balance
     * Level 1: HP 50-64, ATK 6-14 (was 80-109, 12-21) */
    enemy.health = 50 + (level * 15) + (rand() % 15);
    enemy.attack = 6 + (level * 3) + (rand() % 8);
    enemy.experience = 30 + (level * 10);

    return enemy;
}

/**
 * @brief Check if a boss exists at the specified position
 *
 * @param dungeon Pointer to the current dungeon
 * @param x X coordinate to check
 * @param y Y coordinate to check
 * @return 1 if boss found at position, 0 otherwise
 */
int checkBossAt(Dungeon *dungeon, int x, int y)
{
    if (dungeon->bossFound &&
        dungeon->bossPos.x == x && dungeon->bossPos.y == y)
        return 1;
    return 0;
}

/**
 * @brief Check if the dungeon is fully connected using flood-fill
 *
 * This function verifies that all floor tiles in the dungeon are reachable
 * from each other using flood-fill (BFS) starting from the player's position.
 * If the player or exit is isolated, the dungeon generation should be redone.
 *
 * @param dungeon Pointer to the current dungeon
 * @return 1 if dungeon is fully connected, 0 if there are isolated areas
 */
int isDungeonConnected(Dungeon *dungeon)
{
    /* Create visited array */
    int visited[DUNGEON_SIZE][DUNGEON_SIZE];
    for (int i = 0; i < DUNGEON_SIZE; i++)
        for (int j = 0; j < DUNGEON_SIZE; j++)
            visited[i][j] = 0;

    /* Queue for BFS */
    Position queue[DUNGEON_SIZE * DUNGEON_SIZE];
    int front = 0, back = 0;

    /* Start flood-fill from player's position */
    Position start = dungeon->playerPos;
    queue[back++] = start;
    visited[start.y][start.x] = 1;

    int floorCount = 0;
    int visitedFloorCount = 0;

    /* Count total floor tiles */
    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            if (dungeon->grid[i][j] == FLOOR || dungeon->grid[i][j] == EXIT ||
                dungeon->grid[i][j] == CHEST)
            {
                floorCount++;
            }
        }
    }

    /* BFS traversal */
    while (front < back)
    {
        Position current = queue[front++];
        visitedFloorCount++;

        /* Check all 4 neighbors */
        int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        for (int d = 0; d < 4; d++)
        {
            int nx = current.x + directions[d][0];
            int ny = current.y + directions[d][1];

            if (nx >= 0 && nx < DUNGEON_SIZE && ny >= 0 && ny < DUNGEON_SIZE)
            {
                char tile = dungeon->grid[ny][nx];
                int isPassable = (tile == FLOOR || tile == EXIT || tile == CHEST);

                if (isPassable && !visited[ny][nx])
                {
                    visited[ny][nx] = 1;
                    queue[back].x = nx;
                    queue[back].y = ny;
                    back++;
                }
            }
        }
    }

    /* If we visited all floor tiles, the dungeon is connected */
    return (visitedFloorCount >= floorCount);
}

/**
 * @brief Ensure player position is connected to the rest of the dungeon
 *
 * If the player's starting position is isolated from the main dungeon,
 * this function carves a path from the player to the nearest accessible floor.
 *
 * @param dungeon Pointer to the current dungeon
 */
void ensurePlayerAccess(Dungeon *dungeon)
{
    /* Create visited array */
    int visited[DUNGEON_SIZE][DUNGEON_SIZE];
    for (int i = 0; i < DUNGEON_SIZE; i++)
        for (int j = 0; j < DUNGEON_SIZE; j++)
            visited[i][j] = 0;

    /* Queue for BFS */
    Position queue[DUNGEON_SIZE * DUNGEON_SIZE];
    int front = 0, back = 0;

    /* Start BFS from player position */
    Position start = dungeon->playerPos;
    queue[back++] = start;
    visited[start.y][start.x] = 1;

    Position parent[DUNGEON_SIZE][DUNGEON_SIZE];
    for (int i = 0; i < DUNGEON_SIZE; i++)
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            parent[i][j].x = -1;
            parent[i][j].y = -1;
        }

    Position foundFloor = {-1, -1};
    int found = 0;

    /* BFS to find nearest floor tile that's not the player's position */
    while (front < back && !found)
    {
        Position current = queue[front++];

        int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        for (int d = 0; d < 4 && !found; d++)
        {
            int nx = current.x + directions[d][0];
            int ny = current.y + directions[d][1];

            if (nx >= 0 && nx < DUNGEON_SIZE && ny >= 0 && ny < DUNGEON_SIZE)
            {
                char tile = dungeon->grid[ny][nx];
                int isPassable = (tile == FLOOR || tile == EXIT || tile == CHEST);

                if (isPassable && !visited[ny][nx])
                {
                    visited[ny][nx] = 1;
                    parent[ny][nx] = current;
                    queue[back].x = nx;
                    queue[back].y = ny;
                    back++;

                    /* If this is a floor tile that's not the player's start, we found a path */
                    if (tile == FLOOR && (nx != start.x || ny != start.y))
                    {
                        foundFloor.x = nx;
                        foundFloor.y = ny;
                        found = 1;
                    }
                }
            }
        }
    }

    /* If we found a floor tile to connect to, carve the path */
    if (found)
    {
        /* Backtrack from the found floor to the player's position */
        Position current = foundFloor;
        while (current.x != start.x || current.y != start.y)
        {
            Position prev = parent[current.y][current.x];

            /* If this is not the player's start position, convert to floor */
            if ((current.x != start.x || current.y != start.y) &&
                !(current.x == dungeon->exitPos.x && current.y == dungeon->exitPos.y) &&
                !(current.x == dungeon->chest.x && current.y == dungeon->chest.y))
            {
                dungeon->grid[current.y][current.x] = FLOOR;
            }

            current = prev;
        }
    }
}
