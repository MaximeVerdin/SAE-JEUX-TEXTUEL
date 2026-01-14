/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "player.h"

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
 * @brief Display weapon information
 */
void displayWeapon(const Weapon *weapon)
{
    printf("\n  === %s ===\n", weapon->name);
    printf("  Attack: %+d\n", weapon->attack);
    printf("  Health: %+d\n", weapon->health);
    printf("  Luck:   %+d\n", weapon->luck);
    printf("  =================\n");
}

/**
 * @brief Apply weapon stats to player
 */
void applyWeaponStats(Player *player, const Weapon *weapon)
{
    player->attack += weapon->attack;
    player->health += weapon->health;
    player->luck += weapon->luck;

    /* Ensure health doesn't go below 0 */
    if (player->health < 0)
    {
        player->health = 0;
    }
}

/**
 * @brief Change player's weapon and apply stats
 */
void equipWeapon(Player *player, const Weapon *weapon)
{
    strncpy(player->weapon, weapon->name, sizeof(player->weapon) - 1);
    player->weapon[sizeof(player->weapon) - 1] = '\0';
    applyWeaponStats(player, weapon);
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
