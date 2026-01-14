/* Author names : VERDIN Maxime and FOURNIER Nathan */

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
    char *param;

    do
    {
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
    } while (strcmp(language, "en") != 0 && strcmp(language, "fr") != 0);

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
        int difficultyChoice = 1; // Normal mode
        int playerCount = 1;

        char name[50];
        printf("Enter your character's name: ");
        scanf("%s", name);
        Player player = createPlayer(name, 100, 10, 5, "Hands");

        printf("Enter a name for your save: ");
        scanf("%s", saveName);

        /* Tutorial is always skipped - pass 1 to skip tutorial */
        createGame(saveName, difficultyChoice, 1);
        addPlayersToSave(saveName, &player, playerCount);

        printf("New game created for %s!\n", saveName);

        /* Load the saved game state */
        GameState game = loadGameState(saveName);

        /* Tutorial is always disabled */
        game.tutorialMode = 0;

        /* Set the save name for future saves */
        strcpy(game.saveName, saveName);

        /* Run the game with the loaded state */
        playGame(&game, language);
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

        /* Set the save name for future saves */
        strcpy(game.saveName, saveName);

        playGame(&game, language);
    }
}
