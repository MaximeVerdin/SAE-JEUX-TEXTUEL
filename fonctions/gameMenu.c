#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readParam(const char *filePath, const char *paramName)
{
    char buffer[256];
    FILE *file = fopen(filePath, "r");
    if (!file)
        return NULL;

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), file))
    {
        // parametre searching
        char *colon = strchr(buffer, ':');
        if (!colon)
            continue;

        *colon = '\0';
        char *key = buffer;
        char *value = colon + 1;

        // Trim leading spaces
        while (*value == ' ' || *value == '\t')
            value++;

        value[strcspn(value, "\r\n")] = '\0';

        if (strcmp(key, paramName) == 0)
        {
            char *result = malloc(strlen(value) + 1);
            if (result)
                strcpy(result, value);

            fclose(file);
            return result;
        }
    }

    fclose(file);
    return NULL;
}

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
}

int main()
{
    gameMenu();
    return 0;
}