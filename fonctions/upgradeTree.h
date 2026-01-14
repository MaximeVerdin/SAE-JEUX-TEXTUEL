#ifndef UPGRADETREE_H
#define UPGRADETREE_H

#include "upgrade.h"

UpgradeNode *createUpgradeNode(Upgrade upgrade);
void freeUpgradeTree(UpgradeNode *root);
void displayUpgradeTree(UpgradeNode *root);

#endif
