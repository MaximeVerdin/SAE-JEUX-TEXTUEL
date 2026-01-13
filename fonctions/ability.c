#include <stdio.h>
#include <string.h>
#include "ability.h"

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

void displayAbility(const Ability *ability)
{
    printf("Ability Name: %s\n", ability->name);
    printf("Boost Type: %s\n", ability->boostType);
    printf("Boost Amount: %d\n", ability->boostAmount);
    printf("Description: %s\n", ability->description);
}

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

void displayClass(const Class *class)
{
    printf("Class Name: %s\n", class->name);
    printf("Description: %s\n", class->description);
    printf("Base Health: %d\n", class->baseHealth);
    printf("Base Attack: %d\n", class->baseAttack);
    printf("Base Luck: %d\n", class->baseLuck);
}
