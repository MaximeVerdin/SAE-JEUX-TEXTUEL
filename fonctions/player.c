/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <string.h>
#include "player.h"

/**
 * @brief Apply damage to a player
 *
 * Reduces the player's health by the specified damage amount.
 * Health is clamped to 0 minimum to prevent negative values.
 *
 * @param player Pointer to the player to damage
 * @param damage Amount of damage to apply
 */
void damagePlayer(Player *player, int damage)
{
    player->health -= damage;
    if (player->health < 0)
    {
        player->health = 0;
    }
}

/**
 * @brief Heal a player by a specified amount
 *
 * Increases the player's health by the healing amount.
 *
 * @param player Pointer to the player to heal
 * @param heal Amount of health to restore
 */
void healPlayer(Player *player, int heal)
{
    player->health += heal;
}

/**
 * @brief Increase player's attack power
 *
 * Adds the specified boost to the player's attack attribute.
 *
 * @param player Pointer to the player
 * @param attackBoost Amount to increase attack by
 */
void addAttack(Player *player, int attackBoost)
{
    player->attack += attackBoost;
}

/**
 * @brief Increase player's luck factor
 *
 * Adds the specified boost to the player's luck attribute.
 * Luck affects critical hit chances and other random events.
 *
 * @param player Pointer to the player
 * @param luckBoost Amount to increase luck by
 */
void addLuck(Player *player, int luckBoost)
{
    player->luck += luckBoost;
}

/**
 * @brief Change player's equipped weapon
 *
 * Replaces the player's current weapon with a new one.
 * The weapon name is safely copied with bounds checking.
 *
 * @param player Pointer to the player
 * @param newWeapon Name of the new weapon
 */
void changeWeapon(Player *player, const char *newWeapon)
{
    strncpy(player->weapon, newWeapon, sizeof(player->weapon) - 1);
    player->weapon[sizeof(player->weapon) - 1] = '\0';
}

/**
 * @brief Display player information in console
 *
 * Prints all player attributes including name, health, attack,
 * luck, weapon, and all learned abilities.
 *
 * @param player Pointer to the player to display
 */
void displayPlayer(const Player *player)
{
    printf("Player Name: %s\n", player->name);
    printf("Health: %d\n", player->health);
    printf("Attack: %d\n", player->attack);
    printf("Luck: %d\n", player->luck);
    printf("Weapon: %s\n", player->weapon);
    printf("Skills: %d\n", player->skillCount);
    for (int i = 0; i < player->skillCount; i++)
    {
        displayAbility(&player->skills[i]);
        printf("\n");
    }
}

/**
 * @brief Add a new ability to player's skill set
 *
 * Dynamically reallocates the skills array to accommodate
 * the new ability and increments the skill count.
 *
 * @param player Pointer to the player
 * @param newAbility The ability to add
 */
void addAbility(Player *player, Ability newAbility)
{
    player->skills = realloc(player->skills, (player->skillCount + 1) * sizeof(Ability));
    if (player->skills != NULL)
    {
        player->skills[player->skillCount] = newAbility;
        player->skillCount++;
    }
}

/**
 * @brief Free all dynamically allocated abilities
 *
 * Releases memory used by the skills array and resets
 * the skill count to zero.
 *
 * @param player Pointer to the player
 */
void freePlayerAbilities(Player *player)
{
    free(player->skills);
    player->skills = NULL;
    player->skillCount = 0;
}

/**
 * @brief Create a new player with specified attributes
 *
 * Initializes a Player structure with the given parameters.
 * The skills array is initialized as NULL and will be populated
 * by subsequent addAbility calls.
 *
 * @param name Player name (max 49 characters)
 * @param health Initial health points
 * @param attack Initial attack power
 * @param luck Initial luck factor
 * @param weapon Initial weapon name (max 49 characters)
 * @param skills Array of starting abilities (can be NULL)
 * @param skillCount Number of starting abilities
 * @return Initialized Player structure
 */
Player createPlayer(char name[50], int health, int attack, int luck, char weapon[50], Ability *skills, int skillCount)
{
    Player player;
    strncpy(player.name, name, sizeof(player.name) - 1);
    player.name[sizeof(player.name) - 1] = '\0';
    player.health = health;
    player.attack = attack;
    player.luck = luck;
    strncpy(player.weapon, weapon, sizeof(player.weapon) - 1);
    player.weapon[sizeof(player.weapon) - 1] = '\0';

    player.skillCount = 0;
    player.skills = NULL;

    for (int i = 0; i < skillCount; i++)
    {
        addAbility(&player, skills[i]);
    }

    return player;
}
