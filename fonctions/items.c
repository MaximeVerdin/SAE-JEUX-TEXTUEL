/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "player.h"
#include "utils.h"

/**
 * @brief Load weapons from CSV file
 */
int loadWeapons(Weapon weapons[], int *count)
{
    FILE *file = fopen("data/weapon.csv", "r");
    if (file == NULL)
    {
        printf("Warning: weapon.csv not found!\n");
        return 0;
    }

    char line[256];
    *count = 0;

    /* Skip header line */
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return 0;
    }

    /* Read weapons from file */
    while (fgets(line, sizeof(line), file) != NULL && *count < MAX_WEAPONS)
    {
        /* Remove trailing newline */
        line[strcspn(line, "\n")] = 0;

        /* Parse CSV format: name;attack;health;luck */
        char *token = strtok(line, ";");
        if (token == NULL)
            continue;

        strncpy(weapons[*count].name, token, MAX_WEAPON_NAME - 1);
        weapons[*count].name[MAX_WEAPON_NAME - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        weapons[*count].attack = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        weapons[*count].health = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;
        weapons[*count].luck = atoi(token);

        (*count)++;
    }

    fclose(file);
    return 1;
}

/**
 * @brief Get a random weapon
 */
Weapon getRandomWeapon(Weapon weapons[], int count)
{
    if (count <= 0)
    {
        /* Return a default weapon if none loaded */
        Weapon defaultWeapon;
        strcpy(defaultWeapon.name, "Fists");
        defaultWeapon.attack = 1;
        defaultWeapon.health = 0;
        defaultWeapon.luck = 0;
        return defaultWeapon;
    }

    int randomIndex = rand() % count;
    return weapons[randomIndex];
}

/**
 * @brief Display weapon information (with translated name)
 */
void displayWeapon(const Weapon *weapon)
{
    if (weapon == NULL)
        return;

    char *translatedName = getWeaponTranslatedName(weapon->name);
    char *statAttack = getTranslatedText("statAttack");
    char *statHealth = getTranslatedText("statHealth");
    char *statLuck = getTranslatedText("statLuck");

    printf("\n  === %s ===\n", translatedName ? translatedName : weapon->name);
    printf("  %s: %+d\n", statAttack, weapon->attack);
    printf("  %s: %+d\n", statHealth, weapon->health);
    printf("  %s:   %+d\n", statLuck, weapon->luck);
    printf("  =================\n");

    free(translatedName);
    free(statAttack);
    free(statHealth);
    free(statLuck);
}

/**
 * @brief Apply weapon stats to player
 */
void applyWeaponStats(Player *player, const Weapon *weapon)
{
    player->attack += weapon->attack;
    player->maxHealth += weapon->health;
    player->health += weapon->health;
    player->luck += weapon->luck;

    /* Ensure health doesn't go below 0 */
    if (player->health < 0)
    {
        player->health = 0;
    }
}

/**
 * @brief Remove weapon stats from player
 *
 * Subtracts the currently equipped weapon's stats from the player
 * and resets the weapon to "Fists" (default weapon with 0 stats).
 *
 * @param player Pointer to the player
 * @param weapons Array of available weapons to find the current weapon stats
 * @param weaponCount Number of weapons in the array
 */
void unequipWeapon(Player *player, const Weapon weapons[], int weaponCount)
{
    if (player == NULL)
        return;

    /* Fists is the default weapon with 0 stats */
    if (strcmp(player->weapon, "Fists") == 0)
    {
        return; /* Already unarmed, nothing to remove */
    }

    /* Find the current weapon in the array to get its stats */
    for (int i = 0; i < weaponCount; i++)
    {
        if (strcmp(weapons[i].name, player->weapon) == 0)
        {
            /* Subtract the weapon's stats */
            player->attack -= weapons[i].attack;
            player->maxHealth -= weapons[i].health;
            player->health -= weapons[i].health;
            player->luck -= weapons[i].luck;

            /* Ensure health doesn't go below 0 */
            if (player->health < 0)
            {
                player->health = 0;
            }
            break;
        }
    }

    /* Reset to "Fists" */
    strncpy(player->weapon, "Fists", sizeof(player->weapon) - 1);
    player->weapon[sizeof(player->weapon) - 1] = '\0';
}

/**
 * @brief Change player's weapon and apply stats
 *
 * First removes the current weapon's stats, then applies the new weapon's stats.
 *
 * @param player Pointer to the player
 * @param weapon Pointer to the new weapon
 * @param weapons Array of available weapons
 * @param weaponCount Number of weapons in the array
 */
void equipWeapon(Player *player, const Weapon *weapon, const Weapon weapons[], int weaponCount)
{
    /* First unequip current weapon to remove its stats */
    unequipWeapon(player, weapons, weaponCount);

    /* Then equip the new weapon and apply its stats */
    strncpy(player->weapon, weapon->name, sizeof(player->weapon) - 1);
    player->weapon[sizeof(player->weapon) - 1] = '\0';
    applyWeaponStats(player, weapon);
}

/**
 * @brief Display side-by-side comparison of current and new weapon
 */
void compareWeapons(const Weapon *currentWeapon, const Weapon *newWeapon)
{
    /* Get translated names for both weapons */
    char *currentTranslated = getWeaponTranslatedName(currentWeapon->name);
    char *newTranslated = getWeaponTranslatedName(newWeapon->name);

    const char *currentName = currentTranslated ? currentTranslated : currentWeapon->name;
    const char *newName = newTranslated ? newTranslated : newWeapon->name;

    char *weaponComparisonTitle = getTranslatedText("weaponComparisonTitle");
    char *currentWeaponLabel = getTranslatedText("currentWeapon");
    char *newWeaponLabel = getTranslatedText("newWeapon");
    char *statName = getTranslatedText("statName");
    char *statAttack = getTranslatedText("statAttack");
    char *statHealth = getTranslatedText("statHealth");
    char *statLuck = getTranslatedText("statLuck");
    char *summaryTitle = getTranslatedText("summaryTitle");
    char *detailedChanges = getTranslatedText("detailedChanges");

    printf("\n");
    printf("  ===============================================================================\n");
    printf("  |                        %-33s                    |\n", weaponComparisonTitle);
    printf("  ===============================================================================\n");
    printf("\n");

    /* Header row */
    printf("  +---------------------+------------------------+------------------------+\n");
    printf("  |       %-13s |     %-17s |      %-17s |\n", statName, currentWeaponLabel, newWeaponLabel);
    printf("  +---------------------+------------------------+------------------------+\n");

    /* Weapon name row */
    printf("  | %-19s | %-22s | %-22s |\n", "", currentName, newName);
    printf("  +---------------------+------------------------+------------------------+\n");

    /* Attack row */
    int attackDiff = newWeapon->attack - currentWeapon->attack;
    printf("  | %-19s | %+22d | %+22d |\n", statAttack, currentWeapon->attack, newWeapon->attack);
    printf("  +---------------------+------------------------+------------------------+\n");

    /* Health row */
    int healthDiff = newWeapon->health - currentWeapon->health;
    printf("  | %-19s | %+22d | %+22d |\n", statHealth, currentWeapon->health, newWeapon->health);
    printf("  +---------------------+------------------------+------------------------+\n");

    /* Luck row */
    int luckDiff = newWeapon->luck - currentWeapon->luck;
    printf("  | %-19s | %+22d | %+22d |\n", statLuck, currentWeapon->luck, newWeapon->luck);
    printf("  +---------------------+------------------------+------------------------+\n");

    /* Summary row - show net changes */
    printf("\n");
    printf("  ===============================================================================\n");
    printf("  |                              %-33s                    |\n", summaryTitle);
    printf("  ===============================================================================\n");

    int totalDiff = attackDiff + healthDiff + luckDiff;
    char *netChange = getTranslatedText("netChange");
    char *noChange = getTranslatedText("noChange");
    char *noStatChanges = getTranslatedText("noStatChanges");

    printf("  | %s ", netChange);

    if (totalDiff > 0)
        printf("+%-70d |\n", totalDiff);
    else if (totalDiff < 0)
        printf("%-71d |\n", totalDiff);
    else
        printf("%-71s |\n", noChange);

    printf("  +");

    for (int i = 0; i < 78; i++)
        printf("-");
    printf("+\n");

    /* Detailed breakdown */
    printf("\n  %s:\n", detailedChanges);

    if (attackDiff != 0)
    {
        printf("    %s: %+d -> %+d (%s%d)\n",
               statAttack, currentWeapon->attack, newWeapon->attack,
               attackDiff > 0 ? "+" : "", attackDiff);
    }

    if (healthDiff != 0)
    {
        printf("    %s: %+d -> %+d (%s%d)\n",
               statHealth, currentWeapon->health, newWeapon->health,
               healthDiff > 0 ? "+" : "", healthDiff);
    }

    if (luckDiff != 0)
    {
        printf("    %s:   %+d -> %+d (%s%d)\n",
               statLuck, currentWeapon->luck, newWeapon->luck,
               luckDiff > 0 ? "+" : "", luckDiff);
    }

    if (totalDiff == 0)
    {
        printf("    %s\n", noStatChanges);
    }

    printf("\n");

    /* Free allocated strings */
    free(currentTranslated);
    free(newTranslated);
    free(weaponComparisonTitle);
    free(currentWeaponLabel);
    free(newWeaponLabel);
    free(statName);
    free(statAttack);
    free(statHealth);
    free(statLuck);
    free(summaryTitle);
    free(detailedChanges);
    free(netChange);
    free(noChange);
    free(noStatChanges);
}

/**
 * @brief Interactive interface for weapon comparison and equip decision
 */
int weaponComparisonInterface(Player *player, const Weapon *newWeapon, Weapon weapons[], int weaponCount)
{
    /* Find the current weapon in the weapons array to get its stats */
    Weapon currentWeapon;
    int foundCurrent = 0;

    for (int i = 0; i < weaponCount; i++)
    {
        if (strcmp(weapons[i].name, player->weapon) == 0)
        {
            currentWeapon = weapons[i];
            foundCurrent = 1;
            break;
        }
    }

    /* If current weapon not found in CSV, create a default one */
    if (!foundCurrent)
    {
        strcpy(currentWeapon.name, player->weapon);
        currentWeapon.attack = 0;
        currentWeapon.health = 0;
        currentWeapon.luck = 0;
    }

    /* Get translated names for display */
    char *currentTranslated = getWeaponTranslatedName(currentWeapon.name);
    char *newTranslated = getWeaponTranslatedName(newWeapon->name);
    const char *currentDisplayName = currentTranslated ? currentTranslated : player->weapon;
    const char *newDisplayName = newTranslated ? newTranslated : newWeapon->name;

    char *foundNewWeapon = getTranslatedText("foundNewWeapon");
    char *currentWeaponLabel = getTranslatedText("currentWeaponLabel");
    char *newWeaponLabel = getTranslatedText("newWeaponLabel");
    char *equipChoice = getTranslatedText("equipChoice");
    char *equipNewWeapon = getTranslatedText("equipNewWeapon");
    char *keepCurrentWeapon = getTranslatedText("keepCurrentWeapon");
    char *yourChoice = getTranslatedText("yourChoice");
    char *equippedWeapon = getTranslatedText("equippedWeapon");
    char *newStats = getTranslatedText("newStats");
    char *statsAttack = getTranslatedText("statsAttack");
    char *statsHealth = getTranslatedText("statsHealth");
    char *statsLuck = getTranslatedText("statsLuck");
    char *keepWeapon = getTranslatedText("keepWeapon");

    /* Display the comparison */
    printf("\n");
    printf("  ################################################################################\n");
    printf("  #                                                                              #\n");
    printf("  #                      %s                                 #\n", foundNewWeapon);
    printf("  #                                                                              #\n");
    printf("  ################################################################################\n");
    printf("\n");
    printf(currentWeaponLabel, currentDisplayName);
    printf("\n");
    printf(newWeaponLabel, newDisplayName);
    printf("\n");

    /* Show side-by-side comparison */
    compareWeapons(&currentWeapon, newWeapon);

    /* Get player decision */
    printf("  %s\n", equipChoice);
    printf("\n");
    printf("  1. ");
    printf(equipNewWeapon, newDisplayName);
    printf("\n");
    printf("  2. ");
    printf(keepCurrentWeapon, currentDisplayName);
    printf("\n");
    printf("\n");
    printf("  %s ", yourChoice);

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\n");
        printf(equippedWeapon, newDisplayName);
        printf("\n");
        printf("%s\n", newStats);
        printf(statsAttack, player->attack);
        printf("\n");
        printf(statsHealth, player->health);
        printf("\n");
        printf(statsLuck, player->luck);
        printf("\n");
        free(currentTranslated);
        free(newTranslated);
        free(foundNewWeapon);
        free(currentWeaponLabel);
        free(newWeaponLabel);
        free(equipChoice);
        free(equipNewWeapon);
        free(keepCurrentWeapon);
        free(yourChoice);
        free(equippedWeapon);
        free(newStats);
        free(statsAttack);
        free(statsHealth);
        free(statsLuck);
        free(keepWeapon);
        return 1; /* Weapon equipped */
    }
    else
    {
        printf("\n");
        printf(keepWeapon, currentDisplayName);
        printf("\n");
        free(currentTranslated);
        free(newTranslated);
        free(foundNewWeapon);
        free(currentWeaponLabel);
        free(newWeaponLabel);
        free(equipChoice);
        free(equipNewWeapon);
        free(keepCurrentWeapon);
        free(yourChoice);
        free(equippedWeapon);
        free(newStats);
        free(statsAttack);
        free(statsHealth);
        free(statsLuck);
        free(keepWeapon);
        return 0; /* Weapon declined */
    }
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
