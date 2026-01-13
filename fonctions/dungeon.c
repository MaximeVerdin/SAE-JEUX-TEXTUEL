#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dungeon.h"
#include "player.h"

void initDungeon(Dungeon *dungeon, int level)
{
    // Initialiser la grille avec des murs
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
    generateDungeon(dungeon);
}

void generateDungeon(Dungeon *dungeon)
{
    // Créer des salles plus grandes et mieux connectées
    int numRooms = 6 + rand() % 4; // 6-9 salles

    // Stocker les positions des salles pour les connecter
    int roomCenters[15][2];
    int roomCount = 0;

    for (int r = 0; r < numRooms; r++)
    {
        int roomW = 5 + rand() % 5; // 5-9 de large
        int roomH = 5 + rand() % 5; // 5-9 de haut
        int roomX = 1 + rand() % (DUNGEON_SIZE - roomW - 2);
        int roomY = 1 + rand() % (DUNGEON_SIZE - roomH - 2);

        for (int i = roomY; i < roomY + roomH && i < DUNGEON_SIZE - 1; i++)
        {
            for (int j = roomX; j < roomX + roomW && j < DUNGEON_SIZE - 1; j++)
            {
                dungeon->grid[i][j] = FLOOR;
            }
        }

        // Stocker le centre de la salle
        roomCenters[roomCount][0] = roomX + roomW / 2;
        roomCenters[roomCount][1] = roomY + roomH / 2;
        roomCount++;
    }

    // Connecter toutes les salles avec des couloirs
    for (int i = 1; i < roomCount; i++)
    {
        int x1 = roomCenters[i - 1][0];
        int y1 = roomCenters[i - 1][1];
        int x2 = roomCenters[i][0];
        int y2 = roomCenters[i][1];

        // Couloir horizontal puis vertical (ou l'inverse)
        if (rand() % 2 == 0)
        {
            // Horizontal d'abord
            int startX = (x1 < x2) ? x1 : x2;
            int endX = (x1 < x2) ? x2 : x1;
            for (int x = startX; x <= endX; x++)
            {
                if (x > 0 && x < DUNGEON_SIZE - 1)
                {
                    dungeon->grid[y1][x] = FLOOR;
                }
            }
            // Vertical ensuite
            int startY = (y1 < y2) ? y1 : y2;
            int endY = (y1 < y2) ? y2 : y1;
            for (int y = startY; y <= endY; y++)
            {
                if (y > 0 && y < DUNGEON_SIZE - 1)
                {
                    dungeon->grid[y][x2] = FLOOR;
                }
            }
        }
        else
        {
            // Vertical d'abord
            int startY = (y1 < y2) ? y1 : y2;
            int endY = (y1 < y2) ? y2 : y1;
            for (int y = startY; y <= endY; y++)
            {
                if (y > 0 && y < DUNGEON_SIZE - 1)
                {
                    dungeon->grid[y][x1] = FLOOR;
                }
            }
            // Horizontal ensuite
            int startX = (x1 < x2) ? x1 : x2;
            int endX = (x1 < x2) ? x2 : x1;
            for (int x = startX; x <= endX; x++)
            {
                if (x > 0 && x < DUNGEON_SIZE - 1)
                {
                    dungeon->grid[y2][x] = FLOOR;
                }
            }
        }
    }

    // Ajouter des murs à l'intérieur des salles (obstacles)
    for (int r = 0; r < numRooms; r++)
    {
        int roomW = 5 + rand() % 5;
        int roomH = 5 + rand() % 5;
        int roomX = 1 + rand() % (DUNGEON_SIZE - roomW - 2);
        int roomY = 1 + rand() % (DUNGEON_SIZE - roomH - 2);

        // Ajouter 1-3 obstacles par salle
        int numObstacles = 1 + rand() % 3;
        for (int o = 0; o < numObstacles; o++)
        {
            int obsX = roomX + 1 + rand() % (roomW - 2);
            int obsY = roomY + 1 + rand() % (roomH - 2);

            // Ne pas bloquer les entrées de salles (garder les bords)
            if (obsX > roomX && obsX < roomX + roomW - 1 &&
                obsY > roomY && obsY < roomY + roomH - 1)
            {
                dungeon->grid[obsY][obsX] = WALL;
            }
        }
    }

    // Ajouter quelques piliers dans les grands espaces ouverts
    for (int i = 3; i < DUNGEON_SIZE - 3; i++)
    {
        for (int j = 3; j < DUNGEON_SIZE - 3; j++)
        {
            if (dungeon->grid[i][j] == FLOOR && rand() % 15 == 0)
            {
                // Vérifier que ça ne bloque pas complètement
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
                {
                    dungeon->grid[i][j] = WALL;
                }
            }
        }
    }

    // Placer le joueur
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

    // Placer la sortie
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

    // Placer des ennemis (pas à côté du joueur)
    dungeon->enemyCount = 1 + rand() % 3;
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        int placed = 0;
        for (int i = 1; i < DUNGEON_SIZE - 1 && !placed; i++)
        {
            for (int j = 1; j < DUNGEON_SIZE - 1 && !placed; j++)
            {
                // Calculer la distance au joueur
                int distToPlayer = abs(i - dungeon->playerPos.y) + abs(j - dungeon->playerPos.x);

                if (dungeon->grid[i][j] == FLOOR &&
                    (i != dungeon->playerPos.y || j != dungeon->playerPos.x) &&
                    (i != dungeon->exitPos.y || j != dungeon->exitPos.x) &&
                    distToPlayer >= 5) // Minimum 5 cases de distance
                {
                    dungeon->enemies[e].x = j;
                    dungeon->enemies[e].y = i;
                    placed = 1;
                }
            }
        }
    }

    updateVision(dungeon);
}

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
        {
            return 0;
        }

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

void updateVision(Dungeon *dungeon)
{
    int visionRange = 3;

    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            dungeon->visible[i][j] = 0;
        }
    }

    // Raycasting pour la vision
    for (int dy = -visionRange; dy <= visionRange; dy++)
    {
        for (int dx = -visionRange; dx <= visionRange; dx++)
        {
            if (dx == 0 && dy == 0)
            {
                dungeon->visible[dungeon->playerPos.y][dungeon->playerPos.x] = 1;
                continue;
            }

            int targetX = dungeon->playerPos.x + dx;
            int targetY = dungeon->playerPos.y + dy;

            if (targetX >= 0 && targetX < DUNGEON_SIZE &&
                targetY >= 0 && targetY < DUNGEON_SIZE)
            {
                if (hasLineOfSight(dungeon, dungeon->playerPos.x,
                                   dungeon->playerPos.y, targetX, targetY))
                {
                    dungeon->visible[targetY][targetX] = 1;
                    dungeon->explored[targetY][targetX] = 1;
                }
            }
        }
    }
}

void displayDungeon(Dungeon *dungeon)
{
    printf("\n");
    printf("  ");
    for (int j = 0; j < DUNGEON_SIZE; j++)
    {
        printf("%d ", j % 10);
    }
    printf("\n");

    for (int i = 0; i < DUNGEON_SIZE; i++)
    {
        printf("%d ", i % 10);
        for (int j = 0; j < DUNGEON_SIZE; j++)
        {
            if (dungeon->visible[i][j])
            {
                // Vérifier si c'est le joueur
                if (i == dungeon->playerPos.y && j == dungeon->playerPos.x)
                {
                    printf("\033[1;32m%c\033[0m ", PLAYER);
                }
                // Vérifier si c'est un ennemi
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
                        printf("%c ", dungeon->grid[i][j]);
                    }
                }
            }
            else
            {
                // Case pas visible - cachée
                printf("  ");
            }
        }
        printf("\n");
    }

    // Légende
    printf("\n\033[1;32m@\033[0m Joueur  \033[1;31mE\033[0m Ennemi  \033[1;33mX\033[0m Sortie  # Mur\n");
}

int isWall(Dungeon *dungeon, int x, int y)
{
    if (x < 0 || x >= DUNGEON_SIZE || y < 0 || y >= DUNGEON_SIZE)
    {
        return 1;
    }
    return dungeon->grid[y][x] == WALL;
}

int movePlayer(Dungeon *dungeon, char direction)
{
    int newX = dungeon->playerPos.x;
    int newY = dungeon->playerPos.y;

    switch (direction)
    {
    case 'z':
        newY--;
        break; // Haut
    case 's':
        newY++;
        break; // Bas
    case 'q':
        newX--;
        break; // Gauche
    case 'd':
        newX++;
        break; // Droite
    default:
        return 0;
    }

    // Vérifier les limites et les murs
    if (isWall(dungeon, newX, newY))
    {
        printf("Mur! Vous ne pouvez pas passer.\n");
        return 0;
    }

    // Vérifier les ennemis - collision forcée!
    int enemyIdx = checkEnemyAt(dungeon, newX, newY);
    if (enemyIdx >= 0)
    {
        // Déplacer le joueur sur la case de l'ennemi
        dungeon->playerPos.x = newX;
        dungeon->playerPos.y = newY;
        updateVision(dungeon);
        // Retourner l'index de l'ennemi + 10 pour éviter confusion avec autres codes
        return 10 + enemyIdx;
    }

    // Vérifier la sortie
    if (dungeon->grid[newY][newX] == EXIT)
    {
        printf("Vous avez trouvé la sortie! Niveau complété!\n");
        return 2; // Indique la sortie
    }

    // Déplacer le joueur
    dungeon->playerPos.x = newX;
    dungeon->playerPos.y = newY;

    updateVision(dungeon);
    return 1;
}

int checkEnemyAt(Dungeon *dungeon, int x, int y)
{
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        if (dungeon->enemies[e].x == x && dungeon->enemies[e].y == y)
        {
            return e;
        }
    }
    return -1;
}

void removeEnemy(Dungeon *dungeon, int index)
{
    // Stocker la position de l'ennemi à supprimer
    int oldX = dungeon->enemies[index].x;
    int oldY = dungeon->enemies[index].y;

    // Déplacer les ennemis suivants
    for (int e = index; e < dungeon->enemyCount - 1; e++)
    {
        dungeon->enemies[e] = dungeon->enemies[e + 1];
    }
    dungeon->enemyCount--;

    // Effacer l'ennemi de la grille (remplacer par du sol)
    if (oldX >= 0 && oldX < DUNGEON_SIZE && oldY >= 0 && oldY < DUNGEON_SIZE)
    {
        dungeon->grid[oldY][oldX] = FLOOR;
    }
}

int canMoveTo(Dungeon *dungeon, int x, int y)
{
    if (isWall(dungeon, x, y))
        return 0;
    // Ne peut pas aller sur la sortie
    if (dungeon->grid[y][x] == EXIT)
        return 0;
    // Ne peut pas aller sur un autre ennemi (mais peut aller sur le joueur pour combat)
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        if (dungeon->enemies[e].x == x && dungeon->enemies[e].y == y)
            return 0;
    }
    return 1;
}

int isPlayerAdjacent(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x;
    int ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x;
    int py = dungeon->playerPos.y;

    return (abs(ex - px) <= 1 && abs(ey - py) <= 1);
}

int canSeePlayer(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x;
    int ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x;
    int py = dungeon->playerPos.y;

    // Vérifier si le joueur est visible par l'ennemi
    int dx = abs(ex - px);
    int dy = abs(ey - py);

    if (dx > 4 || dy > 4)
        return 0;

    return hasLineOfSight(dungeon, ex, ey, px, py);
}

// Retourne l'index de l'ennemi qui a attaqué, ou -1 si aucun
int moveEnemyTowardPlayer(Dungeon *dungeon, int enemyIdx)
{
    int ex = dungeon->enemies[enemyIdx].x;
    int ey = dungeon->enemies[enemyIdx].y;
    int px = dungeon->playerPos.x;
    int py = dungeon->playerPos.y;

    // Vérifier si l'ennemi est déjà sur le joueur (combat!)
    if (ex == px && ey == py)
    {
        return enemyIdx;
    }

    int moveX = 0, moveY = 0;

    if (px < ex)
        moveX = -1;
    else if (px > ex)
        moveX = 1;

    if (py < ey)
        moveY = -1;
    else if (py > ey)
        moveY = 1;

    // Essayer d'abord la direction directe vers le joueur
    int nx = ex + moveX;
    int ny = ey + moveY;

    // Si le joueur est là, combat!
    if (nx == px && ny == py)
    {
        // Déplacer l'ennemi sur le joueur
        dungeon->grid[ey][ex] = FLOOR;
        dungeon->enemies[enemyIdx].x = nx;
        dungeon->enemies[enemyIdx].y = ny;
        return enemyIdx; // Combat!
    }

    // Sinon essayer de se déplacer normalement
    if (canMoveTo(dungeon, nx, ny))
    {
        dungeon->grid[ey][ex] = FLOOR;
        dungeon->enemies[enemyIdx].x = nx;
        dungeon->enemies[enemyIdx].y = ny;
    }
    else
    {
        // Essayer les autres directions
        int directions[8][2] = {
            {moveX, moveY}, {moveX, 0}, {0, moveY}, {-moveX, moveY}, {moveX, -moveY}, {-moveX, 0}, {0, -moveY}, {0, 0}};

        for (int d = 0; d < 7; d++)
        {
            nx = ex + directions[d][0];
            ny = ey + directions[d][1];

            if (nx == px && ny == py)
            {
                // Attaquer le joueur!
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

void updateEnemies(Dungeon *dungeon)
{
    for (int e = 0; e < dungeon->enemyCount; e++)
    {
        if (canSeePlayer(dungeon, e))
        {
            int result = moveEnemyTowardPlayer(dungeon, e);
            if (result >= 0)
            {
                // Un ennemi a attaqué - le combat sera géré par le jeu
                printf("\n!!! UN ENNEMI VOUS ATTAQUE !!!\n");
            }
        }
    }
}
