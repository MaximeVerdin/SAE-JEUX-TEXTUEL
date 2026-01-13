#ifndef ABILITY_H
#define ABILITY_H

#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[50];
    char boostType[50];
    int boostAmount;
    char description[200];
} Ability;

typedef struct
{
    char name[20];
    char description[100];
    int baseHealth;
    int baseAttack;
    int baseLuck;
} Class;

// Fonctions pour Ability
Ability createAbility(const char *name, const char *boostType, int boostAmount, const char *description);
void displayAbility(const Ability *ability);

// Fonctions pour Class
Class createClass(const char *name, const char *description, int baseHealth, int baseAttack, int baseLuck);
void displayClass(const Class *class);

#endif
