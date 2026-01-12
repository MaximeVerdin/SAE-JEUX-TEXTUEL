#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "saveManagement.h"
#include "player.h"

void gameMenu()
{

    int choice = 0;
    int multiplayer = 0;
    char *language = "en";
    char *menuFilePath = "menu.txt";
    printf("Select a language:\n");
    scanf("%s", language);
    if (strcmp(language, "fr") == 0)
    {
        menuFilePath = strcat("../lang/fr-fr", menuFilePath);
    }
    else
    {
        menuFilePath = strcat("../lang/en-gb", menuFilePath);
    }

    char *gameName = readParam(menuFilePath, "gameName");

    char *gameMode = readParam(menuFilePath, "gameMode");

    char *newGame = readParam(menuFilePath, "newGame");
    char *loadGame = readParam(menuFilePath, "loadGame");

    char *skipTutorial = readParam(menuFilePath, "skipTutorial");

    char *difficulty[3];
    difficulty[0] = readParam(menuFilePath, "easyDifficulty");
    difficulty[1] = readParam(menuFilePath, "mediumDifficulty");
    difficulty[2] = readParam(menuFilePath, "hardDifficulty");

    printf("=== %s ===\n", gameName ? gameName : "Game");
    printf("mode: %s\n", gameMode ? gameMode : "SOLO MODE / COOP MODE");
    do
    {
        printf("0. Solo Mode\n");
        printf("1. Coop Mode\n");
        scanf("%d", &multiplayer);
    } while (multiplayer < 0 || multiplayer > 1);
    do
    {
        printf("0. %s\n", newGame ? newGame : "New Game");
        printf("1. %s\n", loadGame ? loadGame : "Load Game");
        scanf("%d", &choice);
    } while (choice < 0 || choice > 1);
    if (choice == 0)
    {
        char name[50];
        int difficultyChoice = 0;
        int skipTutorialChoice = 0;
        int playerCount = 1;

        if (multiplayer == 1)
        {
            printf("Enter the number of player: ");
            scanf("%d", &playerCount);
        }

        Player *players = NULL;
        players = malloc(playerCount * sizeof(Player));

        for (int i = 0; i < playerCount; i++)
        {
            printf("Enter your character's name: ");
            scanf("%s", players[i].name);
            // Initialize default stats
            players[i].vie = 100;
            players[i].attack = 10;
            players[i].luck = 5;
            strcpy(players[i].weapon, "Hands");
        }

        do
        {
            printf("Select difficulty:\n");
            for (int i = 0; i < 3; i++)
            {
                printf("%d. %s\n", i, difficulty[i] ? difficulty[i] : "Difficulty");
            }
            scanf("%d", &difficultyChoice);
        } while (difficultyChoice < 0 || difficultyChoice > 2);

        do
        {
            printf("0. No %s\n", skipTutorial ? skipTutorial : "Skip Tutorial");
            printf("1. Yes %s\n", skipTutorial ? skipTutorial : "Skip Tutorial");
            scanf("%d", &skipTutorialChoice);
        } while (skipTutorialChoice < 0 || skipTutorialChoice > 1);
        createGame(name, multiplayer, difficultyChoice, skipTutorialChoice);
        printf("New game created for %s!\n", name);
    }
    else
    {
        printf("Load game feature is not implemented yet.\n");
    }
}

int main()
{
    gameMenu();
    return 0;
}