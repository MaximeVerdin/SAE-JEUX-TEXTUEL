#ifndef UPGRADE_H
#define UPGRADE_H

#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[50];
    char description[200];
    int cost;
    char upgradeType[50];
    int upgradeAmount;
} Upgrade;

typedef struct UpgradeNode
{
    Upgrade upgrade;
    struct UpgradeNode *left;
    struct UpgradeNode *right;
} UpgradeNode;

// Fonctions pour Upgrade
Upgrade createUpgrade(const char *name, const char *description, int cost, const char *upgradeType, int upgradeAmount);
void displayUpgrade(const Upgrade *upgrade);

#endif
