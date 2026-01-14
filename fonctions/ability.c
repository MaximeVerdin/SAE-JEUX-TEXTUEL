#include <stdio.h>
#include <string.h>
#include "ability.h"

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
Ability createAbility(const char *name, const char *boostType, int boostAmount, const char *description)
{
    Ability ability;
    strncpy(ability.name, name, sizeof(ability.name) - 1);
    ability.name[sizeof(ability.name) - 1] = '\0';
    strncpy(ability.boostType, boostType, sizeof(ability.boostType) - 1);
    ability.boostType[sizeof(ability.boostType) - 1] = '\0';
    ability.boostAmount = boostAmount;
    strncpy(ability.description, description, sizeof(ability.description) - 1);
    ability.description[sizeof(ability.description) - 1] = '\0';
    return ability;
}

/**
 * @brief Display ability information in console
 *
 * Prints all ability attributes including name, boost type,
 * boost amount, and full description.
 *
 * @param ability Pointer to the ability to display
 */
void displayAbility(const Ability *ability)
{
    printf("Ability Name: %s\n", ability->name);
    printf("Boost Type: %s\n", ability->boostType);
    printf("Boost Amount: %d\n", ability->boostAmount);
    printf("Description: %s\n", ability->description);
}

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
Class createClass(const char *name, const char *description, int baseHealth, int baseAttack, int baseLuck)
{
    Class class;
    strncpy(class.name, name, sizeof(class.name) - 1);
    class.name[sizeof(class.name) - 1] = '\0';
    strncpy(class.description, description, sizeof(class.description) - 1);
    class.description[sizeof(class.description) - 1] = '\0';
    class.baseHealth = baseHealth;
    class.baseAttack = baseAttack;
    class.baseLuck = baseLuck;
    return class;
}

/**
 * @brief Display class information in console
 *
 * Prints all class attributes including name, description,
 * and all base statistics.
 *
 * @param class Pointer to the class to display
 */
void displayClass(const Class *class)
{
    printf("Class Name: %s\n", class->name);
    printf("Description: %s\n", class->description);
    printf("Base Health: %d\n", class->baseHealth);
    printf("Base Attack: %d\n", class->baseAttack);
    printf("Base Luck: %d\n", class->baseLuck);
}
