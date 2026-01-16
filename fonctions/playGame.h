/* Author names : VERDIN Maxime and FOURNIER Nathan */

#ifndef PLAYGAME_H
#define PLAYGAME_H

#include "saveManagement.h"
#include "items.h"

void playGame(GameState *game, const char *language);
void displayGameMenu();
void displayDungeonMenu();
void displayCombatMenu();
int combat(Player *player, Enemy *enemy);
void displayUpgradesMenu();
void showPlayers(GameState *game);
void handleChestInterface(GameState *game);
void exploreDungeon(GameState *game);

#endif
