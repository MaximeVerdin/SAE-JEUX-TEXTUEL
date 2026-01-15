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
    char *param;

    /* Get translated strings using the translation system */
    char *gameName = getTranslatedText("gameName");
    char *gameMode = getTranslatedText("gameMode");
    char *newGame = getTranslatedText("newGame");
    char *loadGame = getTranslatedText("loadGame");
    char *selectLanguage = getTranslatedText("selectLanguage");

    do
    {
        printf("%s", selectLanguage);
        scanf("%s", language);
        if (strcmp(language, "fr") == 0)
        {
            strcpy(menuFilePath, "lang/fr-fr/menu.txt");
            setLanguage("fr-fr");
        }
        else
        {
            strcpy(menuFilePath, "lang/en-gb/menu.txt");
            setLanguage("en-gb");
        }
    } while (strcmp(language, "en") != 0 && strcmp(language, "fr") != 0);

    /* Re-get translated strings after language change */
    free(gameName);
    free(gameMode);
    free(newGame);
    free(loadGame);
    free(selectLanguage);

    gameName = getTranslatedText("gameName");
    gameMode = getTranslatedText("gameMode");
    newGame = getTranslatedText("newGame");
    loadGame = getTranslatedText("loadGame");

    /* Override with trad.txt values if they exist */
    param = readParam(menuFilePath, "gameName");
    if (param != NULL)
    {
        free(gameName);
        gameName = param;
    }

    param = readParam(menuFilePath, "gameMode");
    if (param != NULL)
    {
        free(gameMode);
        gameMode = param;
    }

    param = readParam(menuFilePath, "newGame");
    if (param != NULL)
    {
        free(newGame);
        newGame = param;
    }

    param = readParam(menuFilePath, "loadGame");
    if (param != NULL)
    {
        free(loadGame);
        loadGame = param;
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
        int difficultyChoice = 1; /* Normal mode */
        int playerCount = 1;

        char name[50];
        char *enterName = getTranslatedText("enterCharacterName");
        printf("%s", enterName);
        free(enterName);
        scanf("%s", name);
        Player player = createPlayer(name, 100, 10, 5, "Hands");

        char *enterSave = getTranslatedText("enterSaveName");
        printf("%s", enterSave);
        free(enterSave);
        scanf("%s", saveName);

        /* Tutorial is always skipped - pass 1 to skip tutorial */
        createGame(saveName, difficultyChoice, 1);
        addPlayersToSave(saveName, &player, playerCount);

        char *newGameCreated = getTranslatedText("newGameCreated");
        printf(newGameCreated, saveName);
        printf("\n");
        free(newGameCreated);

        /* Load the saved game state */
        GameState game = loadGameState(saveName);

        /* Tutorial is always disabled */
        game.tutorialMode = 0;

        /* Set the save name for future saves */
        strcpy(game.saveName, saveName);

        /* Run the game with the loaded state */
        playGame(&game, language);

        /* Free translated strings */
        free(gameName);
        free(gameMode);
        free(newGame);
        free(loadGame);
    }
    else
    {
        char *loadSave = getTranslatedText("loadSave");
        printf("%s\n", loadSave);
        free(loadSave);
        char saveName[50];
        scanf("%s", saveName);

        /* Load the saved game state */
        GameState game = loadGameState(saveName);

        /* Run the game without tutorial */
        game.tutorialMode = 0;

        /* Set the save name for future saves */
        strcpy(game.saveName, saveName);

        playGame(&game, language);

        /* Free translated strings */
        free(gameName);
        free(gameMode);
        free(newGame);
        free(loadGame);
    }
}
