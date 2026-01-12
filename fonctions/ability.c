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
