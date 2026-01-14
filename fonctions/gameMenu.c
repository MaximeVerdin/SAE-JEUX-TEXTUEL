#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameMenu.h"
#include "utils.h"
#include "saveManagement.h"
#include "player.h"
#include "playGame.h"

/**
 * @brief Main game menu that handles game initialization
 *
 * Displays the main menu, handles new game/load game selection,
 * difficulty selection, and launches the game loop.
 */
void gameMenu()
{
    int choice = 0;
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

        char name[50];
        printf("Enter your character's name: ");
        scanf("%s", name);
        Player player = createPlayer(name, 100, 10, 5, "Hands", NULL, 0);

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

        createGame(saveName, difficultyChoice, skipTutorialChoice);
        addPlayersToSave(saveName, &player, playerCount);

        printf("New game created for %s!\n", saveName);

        /* Load the saved game state */
        GameState game = loadGameState(saveName);

        /* Set tutorial mode based on choice */
        game.tutorialMode = (skipTutorialChoice == 0) ? 1 : 0;

        /* Run the game with the loaded state */
        playGame(&game, language);

        freePlayerAbilities(&player);
    }
    else
    {
        printf("Enter the name of your saved game: ");
        char saveName[50];
        scanf("%s", saveName);

        /* Load the saved game state */
        GameState game = loadGameState(saveName);

        /* Run the game without tutorial */
        game.tutorialMode = 0;

        playGame(&game, language);
    }

    /* Free memory */
    for (int i = 0; i < 3; i++)
    {
        free(difficulty[i]);
    }
}
