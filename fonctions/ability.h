/* Author names : VERDIN Maxime and FOURNIER Nathan */

#ifndef ABILITY_H
#define ABILITY_H

#include <stdio.h>
#include <string.h>

/**
 * @brief Ability structure representing a player skill
 *
 * An ability is a special skill that a player can learn which
 * provides various bonuses during gameplay.
 */
typedef struct
{
    char name[50];         /**< Ability name */
    char boostType[50];    /**< Type of boost (e.g., "attack", "health", "luck") */
    int boostAmount;       /**< Amount of the boost */
    char description[200]; /**< Detailed description of the ability */
} Ability;

/**
 * @brief Class structure representing a character class
 *
 * A class defines the base attributes for characters of that type.
 * Classes provide starting stat bonuses and specializations.
 */
typedef struct
{
    char name[20];         /**< Class name */
    char description[100]; /**< Class description */
    int baseHealth;        /**< Base health points for this class */
    int baseAttack;        /**< Base attack power for this class */
    int baseLuck;          /**< Base luck factor for this class */
} Class;

/**
 * @brief Create a new ability with specified attributes
 *
 * Initializes an Ability structure with the given parameters.
 * All string fields are safely copied with bounds checking.
 *
 * @param name Ability name
 * @param boostType Type of bonus this ability provides
 * @param boostAmount Magnitude of the bonus
 * @param description Human-readable description
 * @return Initialized Ability structure
 */
Ability createAbility(const char *name, const char *boostType, int boostAmount, const char *description);

/**
 * @brief Display ability information in console
 *
 * Prints all ability attributes including name, boost type,
 * boost amount, and full description.
 *
 * @param ability Pointer to the ability to display
 */
void displayAbility(const Ability *ability);

/**
 * @brief Create a new class with specified attributes
 *
 * Initializes a Class structure with the given parameters.
 * All string fields are safely copied with bounds checking.
 *
 * @param name Class name
 * @param description Class description and lore
 * @param baseHealth Starting health points for this class
 * @param baseAttack Starting attack power for this class
 * @param baseLuck Starting luck factor for this class
 * @return Initialized Class structure
 */
Class createClass(const char *name, const char *description, int baseHealth, int baseAttack, int baseLuck);

/**
 * @brief Display class information in console
 *
 * Prints all class attributes including name, description,
 * and all base statistics.
 *
 * @param class Pointer to the class to display
 */
void displayClass(const Class *class);

#endif
