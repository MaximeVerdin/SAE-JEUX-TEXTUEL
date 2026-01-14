/* Author names : VERDIN Maxime and FOURNIER Nathan */

#ifndef ITEMS_H
#define ITEMS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

#define MAX_WEAPONS 50
#define MAX_WEAPON_NAME 50

/**
 * @brief Weapon structure representing equippable items
 *
 * Contains weapon name and stat modifiers for attack, health, and luck.
 */
typedef struct
{
    char name[MAX_WEAPON_NAME]; /**< Weapon name */
    int attack;                 /**< Attack bonus when equipped */
    int health;                 /**< Health bonus when equipped */
    int luck;                   /**< Luck bonus when equipped */
} Weapon;

/**
 * @brief Load weapons from CSV file
 *
 * Reads weapon data from weapon.csv file and populates the weapons array.
 *
 * @param weapons Array to store loaded weapons
 * @param count Pointer to store the number of weapons loaded
 * @return 1 if successful, 0 if file not found
 */
int loadWeapons(Weapon weapons[], int *count);

/**
 * @brief Get a random weapon
 *
 * Returns a random weapon from the loaded weapons array.
 *
 * @param weapons Array of available weapons
 * @param count Number of weapons in the array
 * @return Random Weapon structure
 */
Weapon getRandomWeapon(Weapon weapons[], int count);

/**
 * @brief Display weapon information
 *
 * Prints weapon name and all stat modifiers.
 *
 * @param weapon Pointer to the weapon to display
 */
void displayWeapon(const Weapon *weapon);

/**
 * @brief Apply weapon stats to player
 *
 * Adds weapon stat bonuses to the player's attributes.
 *
 * @param player Pointer to the player
 * @param weapon Pointer to the weapon to apply
 */
void applyWeaponStats(Player *player, const Weapon *weapon);

/**
 * @brief Change player's weapon and apply stats
 *
 * Updates the player's equipped weapon and applies stat bonuses.
 *
 * @param player Pointer to the player
 * @param weapon Pointer to the new weapon
 */
void equipWeapon(Player *player, const Weapon *weapon);

#endif
