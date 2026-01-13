#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ability.h"

typedef struct
{
    char name[50];
    int vie;
    int attack;
    int luck;
    char weapon[50];
    Ability *skills;
    int skillCount;
} Player;

// Fonctions pour les joueurs
void damagePlayer(Player *player, int damage);
void healPlayer(Player *player, int heal);
void addAttack(Player *player, int attackBoost);
void addLuck(Player *player, int luckBoost);
void changeWeapon(Player *player, const char *newWeapon);
void displayPlayer(const Player *player);
void addAbility(Player *player, Ability newAbility);
void freePlayerAbilities(Player *player);
Player createPlayer(char name[50], int vie, int attack, int luck, char weapon[50], Ability *skills, int skillCount);

#endif
