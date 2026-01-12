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