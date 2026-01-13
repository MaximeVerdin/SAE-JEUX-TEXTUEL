#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameMenu.h"
#include "utils.h"
#include "saveManagement.h"
#include "player.h"
#include "playGame.h"

void gameMenu()
{
    int choice = 0;
    int multiplayer = 0;
    char language[10] = "en";
    char menuFilePath[100];
    char gameName[50] = "Game";
    char gameMode[50] = "Adventure";
    char newGame[50] = "New Game";
    char loadGame[50] = "Load Game";
    char skipTutorial[50] = "Skip Tutorial";
    char *difficulty[3] = {NULL, NULL, NULL};
    char *param;

    printf("Select a language (en/fr): ");
    scanf("%s", language);

    if (strcmp(language, "fr") == 0)
    {
        strcpy(menuFilePath, "lang/fr-fr/menu.txt");
    }
    else
    {
        strcpy(menuFilePath, "lang/en-gb/menu.txt");
    }

    param = readParam(menuFilePath, "gameName");
    if (param != NULL)
    {
        strcpy(gameName, param);
        free(param);
    }

    param = readParam(menuFilePath, "gameMode");
    if (param != NULL)
    {
        strcpy(gameMode, param);
        free(param);
    }

    param = readParam(menuFilePath, "newGame");
    if (param != NULL)
    {
        strcpy(newGame, param);
        free(param);
    }

    param = readParam(menuFilePath, "loadGame");
    if (param != NULL)
    {
        strcpy(loadGame, param);
        free(param);
    }

    param = readParam(menuFilePath, "skipTutorial");
    if (param != NULL)
    {
        strcpy(skipTutorial, param);
        free(param);
    }

    difficulty[0] = readParam(menuFilePath, "easyDifficulty");
    difficulty[1] = readParam(menuFilePath, "mediumDifficulty");
    difficulty[2] = readParam(menuFilePath, "hardDifficulty");

    printf("=== %s ===\n", gameName);
    printf("Mode: %s\n", gameMode);

    do
    {
        printf("0. Solo Mode\n");
        printf("1. Coop Mode\n");
        scanf("%d", &multiplayer);
    } while (multiplayer < 0 || multiplayer > 1);

    do
    {
        printf("0. %s\n", newGame);
        printf("1. %s\n", loadGame);
        scanf("%d", &choice);
    } while (choice < 0 || choice > 1);

    if (choice == 0)
    {
        char saveName[50];
        int difficultyChoice = 0;
        int skipTutorialChoice = 0;
        int playerCount = 1;

        if (multiplayer == 1)
        {
            printf("Enter the number of player (max 4): ");
            scanf("%d", &playerCount);
            if (playerCount > 4)
                playerCount = 4;
            if (playerCount < 1)
                playerCount = 1;
        }

        Player *players = malloc(playerCount * sizeof(Player));
        if (players == NULL)
        {
            printf("Memory allocation error!\n");
            return;
        }

        for (int i = 0; i < playerCount; i++)
        {
            char name[50];
            printf("Enter your character's name: ");
            scanf("%s", name);
            players[i] = createPlayer(name, 100, 10, 5, "Hands", NULL, 0);
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
            printf("0. No %s\n", skipTutorial);
            printf("1. Yes %s\n", skipTutorial);
            scanf("%d", &skipTutorialChoice);
        } while (skipTutorialChoice < 0 || skipTutorialChoice > 1);

        printf("Enter a name for your save: ");
        scanf("%s", saveName);

        createGame(saveName, multiplayer, difficultyChoice, skipTutorialChoice);
        addPlayersToSave(saveName, players, playerCount);

        printf("New game created for %s!\n", saveName);

        if (loadGameByName(saveName) != 0)
        {
            printf("Loading error!\n");
        }
        else
        {
            playGame(saveName);
        }

        for (int i = 0; i < playerCount; i++)
        {
            freePlayerAbilities(&players[i]);
        }
        free(players);
    }
    else
    {
        printf("Enter the name of your saved game: ");
        char saveName[50];
        scanf("%s", saveName);

        if (loadGameByName(saveName) != 0)
        {
            printf("Loading error!\n");
        }
        else
        {
            playGame(saveName);
        }
    }

    // Libérer la mémoire
    for (int i = 0; i < 3; i++)
    {
        free(difficulty[i]);
    }
}
