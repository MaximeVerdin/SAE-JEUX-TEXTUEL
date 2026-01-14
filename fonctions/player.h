#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ability.h"

/**
 * @brief Player structure representing the game character
 *
 * This structure contains all information about a player including their
 * name, health, attack power, luck, equipped weapon, and learned skills.
 */
typedef struct
{
    char name[50];   /**< Player name */
    int health;      /**< Player health points */
    int attack;      /**< Player attack power */
    int luck;        /**< Player luck factor for critical hits */
    char weapon[50]; /**< Currently equipped weapon name */
    Ability *skills; /**< Array of learned abilities */
    int skillCount;  /**< Number of abilities learned */
} Player;

/**
 * @brief Apply damage to a player
 * @param player Pointer to the player to damage
 * @param damage Amount of damage to apply
 */
void damagePlayer(Player *player, int damage);

/**
 * @brief Heal a player by a specified amount
 * @param player Pointer to the player to heal
 * @param heal Amount of health to restore
 */
void healPlayer(Player *player, int heal);

/**
 * @brief Increase player's attack power
 * @param player Pointer to the player
 * @param attackBoost Amount to increase attack by
 */
void addAttack(Player *player, int attackBoost);

/**
 * @brief Increase player's luck factor
 * @param player Pointer to the player
 * @param luckBoost Amount to increase luck by
 */
void addLuck(Player *player, int luckBoost);

/**
 * @brief Change player's equipped weapon
 * @param player Pointer to the player
 * @param newWeapon Name of the new weapon
 */
void changeWeapon(Player *player, const char *newWeapon);

/**
 * @brief Display player information in console
 * @param player Pointer to the player to display
 */
void displayPlayer(const Player *player);

/**
 * @brief Add a new ability to player's skill set
 * @param player Pointer to the player
 * @param newAbility The ability to add
 */
void addAbility(Player *player, Ability newAbility);

/**
 * @brief Free all dynamically allocated abilities
 * @param player Pointer to the player
 */
void freePlayerAbilities(Player *player);

/**
 * @brief Create a new player with specified attributes
 * @param name Player name
 * @param health Initial health points
 * @param attack Initial attack power
 * @param luck Initial luck factor
 * @param weapon Initial weapon name
 * @param skills Array of starting abilities
 * @param skillCount Number of starting abilities
 * @return Initialized Player structure
 */
Player createPlayer(char name[50], int health, int attack, int luck, char weapon[50], Ability *skills, int skillCount);

#endif
