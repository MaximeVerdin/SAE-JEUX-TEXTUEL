#include <stdio.h>
#include <string.h>
#include "upgrade.h"

/**
 * @brief Create upgrade with specified attributes
 */
Upgrade createUpgrade(const char *name, const char *description,
                      int cost, const char *upgradeType, int upgradeAmount)
{
    Upgrade upgrade;
    strncpy(upgrade.name, name, sizeof(upgrade.name) - 1);
    upgrade.name[sizeof(upgrade.name) - 1] = '\0';
    strncpy(upgrade.description, description, sizeof(upgrade.description) - 1);
    upgrade.description[sizeof(upgrade.description) - 1] = '\0';
    upgrade.cost = cost;
    strncpy(upgrade.upgradeType, upgradeType, sizeof(upgrade.upgradeType) - 1);
    upgrade.upgradeType[sizeof(upgrade.upgradeType) - 1] = '\0';
    upgrade.upgradeAmount = upgradeAmount;
    return upgrade;
}

/**
 * @brief Display upgrade information
 */
void displayUpgrade(const Upgrade *upgrade)
{
    printf("Upgrade Name: %s\n", upgrade->name);
    printf("Description: %s\n", upgrade->description);
    printf("Cost: %d\n", upgrade->cost);
    printf("Upgrade Type: %s\n", upgrade->upgradeType);
    printf("Upgrade Amount: %d\n", upgrade->upgradeAmount);
}
