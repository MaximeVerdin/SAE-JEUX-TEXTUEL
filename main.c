#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions/utils.h"
#include "fonctions/saveManagement.h"
#include "fonctions/player.h"
#include "fonctions/gameMenu.h"

int main()
{
    printf("======================================\n");
    printf("     JEUX TEXTUEL - ADVENTURE        \n");
    printf("======================================\n\n");

    gameMenu();

    printf("\nMerci d'avoir joué!\n");
    return 0;
}
