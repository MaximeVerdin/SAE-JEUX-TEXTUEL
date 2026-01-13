#ifndef SAVEMANAGEMENT_H
#define SAVEMANAGEMENT_H

#include "player.h"

void createGame(char *name, int multiplayer, int difficulty, int skipTutorial);
void addPlayersToSave(char *fileName, Player *players, int playerCount);
int loadGameByName(char *saveName);
void saveGame(char *saveName, Player *players, int playerCount, int progress);

#endif
