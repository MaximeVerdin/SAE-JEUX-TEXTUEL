#ifndef UPGRADE_H
#define UPGRADE_H

#include <stdio.h>
#include <string.h>

/**
 * @brief Upgrade structure for purchasable bonuses
 */
typedef struct
{
    char name[50];
    char description[200];
    int cost;
    char upgradeType[50];
    int upgradeAmount;
} Upgrade;

/**
 * @brief Upgrade tree node structure
 */
typedef struct UpgradeNode
{
    Upgrade upgrade;
    struct UpgradeNode *left;
    struct UpgradeNode *right;
} UpgradeNode;

Upgrade createUpgrade(const char *name, const char *description,
                      int cost, const char *upgradeType, int upgradeAmount);
void displayUpgrade(const Upgrade *upgrade);

#endif
