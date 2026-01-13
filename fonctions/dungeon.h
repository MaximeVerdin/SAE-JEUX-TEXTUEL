#ifndef DUNGEON_H
#define DUNGEON_H

#define DUNGEON_SIZE 20
#define WALL '#'
#define FLOOR '.'
#define PLAYER '@'
#define ENEMY 'E'
#define EXIT 'X'
#define UNSEEN ' '

typedef struct
{
    int x, y;
} Position;

typedef struct
{
    char grid[DUNGEON_SIZE][DUNGEON_SIZE];
    int visible[DUNGEON_SIZE][DUNGEON_SIZE];
    int explored[DUNGEON_SIZE][DUNGEON_SIZE];
    Position playerPos;
    Position exitPos;
    int enemyCount;
    Position enemies[10];
} Dungeon;

// Fonctions du donjon
void initDungeon(Dungeon *dungeon, int level);
void generateDungeon(Dungeon *dungeon);
void displayDungeon(Dungeon *dungeon);
int movePlayer(Dungeon *dungeon, char direction);
int checkEnemyAt(Dungeon *dungeon, int x, int y);
void removeEnemy(Dungeon *dungeon, int index);
int isWall(Dungeon *dungeon, int x, int y);
void updateVision(Dungeon *dungeon);
int hasLineOfSight(Dungeon *dungeon, int x1, int y1, int x2, int y2);

// Fonctions d'IA des ennemis
void updateEnemies(Dungeon *dungeon);
int moveEnemyTowardPlayer(Dungeon *dungeon, int enemyIdx);
int canSeePlayer(Dungeon *dungeon, int enemyIdx);
int isPlayerAdjacent(Dungeon *dungeon, int enemyIdx);

#endif
