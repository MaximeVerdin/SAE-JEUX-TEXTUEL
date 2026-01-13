#ifndef UPGRADETREE_H
#define UPGRADETREE_H

#include "upgrade.h"

// Fonctions pour l'arbre d'amélioration
UpgradeNode *createUpgradeNode(Upgrade upgrade);
void freeUpgradeTree(UpgradeNode *root);
void displayUpgradeTree(UpgradeNode *root);

#endif
