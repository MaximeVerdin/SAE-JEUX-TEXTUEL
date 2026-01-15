#include <stdio.h>
#include <string.h>
#include "fonctions/dungeon.h"

int main() {
    printf("=== Test: Boss Display with Player on Same Tile ===\n\n");
    
    Dungeon dungeon;
    memset(&dungeon, 0, sizeof(Dungeon));
    
    /* Initialize dungeon */
    initDungeon(&dungeon, 1);
    
    /* Set player position */
    dungeon.playerPos.x = 10;
    dungeon.playerPos.y = 10;
    
    /* Set boss at same position as player */
    dungeon.bossFound = 1;
    dungeon.bossPos.x = 10;
    dungeon.bossPos.y = 10;
    
    /* Make tiles visible */
    for (int i = 0; i < DUNGEON_SIZE; i++) {
        for (int j = 0; j < DUNGEON_SIZE; j++) {
            dungeon.visible[i][j] = 1;
        }
    }
    
    printf("Player position: (%d, %d)\n", dungeon.playerPos.x, dungeon.playerPos.y);
    printf("Boss position: (%d, %d)\n", dungeon.bossPos.x, dungeon.bossPos.y);
    printf("\nExpected: Boss and player on same tile should show 'B@' (purple B, green @)\n\n");
    
    displayDungeon(&dungeon);
    
    printf("\n=== Test Complete ===\n");
    return 0;
}
