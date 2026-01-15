/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dungeon.h"
#include "player.h"

/**
 * @brief Initialize a new dungeon for the given level
 */
void initDungeon(Dungeon *dungeon, int level)
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
    generateDungeon(dungeon);
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
    updateVision(dungeon);
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
                if (i == dungeon->playerPos.y && j == dungeon->playerPos.x)
                    printf("\033[1;32m%c\033[0m ", PLAYER);
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
                    if (!isEnemy && dungeon->bossFound &&
                        dungeon->bossPos.x == j && dungeon->bossPos.y == i)
                    {
                        printf("\033[1;35m%c\033[0m ", 'B');
                        isEnemy = 1;
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

    printf("\n\033[1;32m@\033[0m Player  \033[1;31mE\033[0m Enemy  \033[1;36mX\033[0m Exit  # Wall  \033[1;33mC\033[0m Chest\n");
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

    int enemyIdx = checkEnemyAt(dungeon, newX, newY);
    if (enemyIdx >= 0)
    {
        dungeon->playerPos.x = newX;
        dungeon->playerPos.y = newY;
        updateVision(dungeon);
        return 10 + enemyIdx;
    }

    if (dungeon->grid[newY][newX] == EXIT)
    {
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
 * @brief Move enemy toward player
 */
int moveEnemyTowardPlayer(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x, ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x, py = dungeon->playerPos.y;

    if (ex == px && ey == py)
        return enemyIdx;

    int moveX = 0, moveY = 0;
    if (px < ex)
        moveX = -1;
    else if (px > ex)
        moveX = 1;
    if (py < ey)
        moveY = -1;
    else if (py > ey)
        moveY = 1;

    int nx = ex + moveX, ny = ey + moveY;

    if (nx == px && ny == py)
    {
        dungeon->grid[ey][ex] = FLOOR;
        dungeon->enemies[enemyIdx].x = nx;
        dungeon->enemies[enemyIdx].y = ny;
        return enemyIdx;
    }

    if (canMoveTo(dungeon, nx, ny))
    {
        dungeon->grid[ey][ex] = FLOOR;
        dungeon->enemies[enemyIdx].x = nx;
        dungeon->enemies[enemyIdx].y = ny;
    }
    else
    {
        int dirs[8][2] = {{moveX, moveY}, {moveX, 0}, {0, moveY}, {-moveX, moveY}, {moveX, -moveY}, {-moveX, 0}, {0, -moveY}, {0, 0}};
        for (int d = 0; d < 7; d++)
        {
            nx = ex + dirs[d][0];
            ny = ey + dirs[d][1];
            if (nx == px && ny == py)
            {
                dungeon->grid[ey][ex] = FLOOR;
                dungeon->enemies[enemyIdx].x = nx;
                dungeon->enemies[enemyIdx].y = ny;
                return enemyIdx;
            }
            if (canMoveTo(dungeon, nx, ny))
            {
                dungeon->grid[ey][ex] = FLOOR;
                dungeon->enemies[enemyIdx].x = nx;
                dungeon->enemies[enemyIdx].y = ny;
                return -1;
            }
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
