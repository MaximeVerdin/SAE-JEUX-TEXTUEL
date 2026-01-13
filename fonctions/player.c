#include <stdio.h>
#include <string.h>
#include "player.h"

void damagePlayer(Player *player, int damage)
{
    player->vie -= damage;
    if (player->vie < 0)
    {
        player->vie = 0;
    }
}

void healPlayer(Player *player, int heal)
{
    player->vie += heal;
}

void addAttack(Player *player, int attackBoost)
{
    player->attack += attackBoost;
}

void addLuck(Player *player, int luckBoost)
{
    player->luck += luckBoost;
}

void changeWeapon(Player *player, const char *newWeapon)
{
    strncpy(player->weapon, newWeapon, sizeof(player->weapon) - 1);
    player->weapon[sizeof(player->weapon) - 1] = '\0';
}

void displayPlayer(const Player *player)
{
    printf("Player Name: %s\n", player->name);
    printf("Vie: %d\n", player->vie);
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

void addAbility(Player *player, Ability newAbility)
{
    player->skills = realloc(player->skills, (player->skillCount + 1) * sizeof(Ability));
    if (player->skills != NULL)
    {
        player->skills[player->skillCount] = newAbility;
        player->skillCount++;
    }
}

void freePlayerAbilities(Player *player)
{
    free(player->skills);
    player->skills = NULL;
    player->skillCount = 0;
}

Player createPlayer(char name[50], int vie, int attack, int luck, char weapon[50], Ability *skills, int skillCount)
{
    Player player;
    strncpy(player.name, name, sizeof(player.name) - 1);
    player.name[sizeof(player.name) - 1] = '\0';
    player.vie = vie;
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
