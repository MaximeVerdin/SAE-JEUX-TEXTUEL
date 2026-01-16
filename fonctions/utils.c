/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "saveManagement.h"

/* Static variable to track current language */
static char currentLanguage[16] = "en-gb";

/**
 * @brief Get the current language code
 */
const char *getLanguage()
{
    return currentLanguage;
}

/**
 * @brief Set the current language code
 */
void setLanguage(const char *lang)
{
    if (lang != NULL)
    {
        strncpy(currentLanguage, lang, sizeof(currentLanguage) - 1);
        currentLanguage[sizeof(currentLanguage) - 1] = '\0';
    }
}

/**
 * @brief Get translated text from trad.txt
 */
char *getTranslatedText(const char *textKey)
{
    if (textKey == NULL)
    {
        char *empty = malloc(1);
        if (empty)
            empty[0] = '\0';
        return empty;
    }

    /* Build path to trad.txt based on current language */
    char tradPath[128];
    snprintf(tradPath, sizeof(tradPath), "lang/%s/trad.txt", currentLanguage);

    /* Try to read the translation */
    char *translation = readParam(tradPath, textKey);

    /* If found, return it */
    if (translation != NULL)
    {
        return translation;
    }

    /* If not found, return a copy of the original key */
    char *fallback = malloc(strlen(textKey) + 1);
    if (fallback)
    {
        strcpy(fallback, textKey);
    }
    return fallback;
}

/**
 * @brief Get translated weapon name from trad.txt
 */
char *getWeaponTranslatedName(const char *weaponName)
{
    if (weaponName == NULL)
    {
        char *empty = malloc(1);
        if (empty)
            empty[0] = '\0';
        return empty;
    }

    /* Build path to trad.txt based on current language */
    char tradPath[128];
    snprintf(tradPath, sizeof(tradPath), "lang/%s/trad.txt", currentLanguage);

    /* Try to read the translation */
    char *translation = readParam(tradPath, weaponName);

    /* If found, return it */
    if (translation != NULL)
    {
        return translation;
    }

    /* If not found, return a copy of the original name */
    char *fallback = malloc(strlen(weaponName) + 1);
    if (fallback)
    {
        strcpy(fallback, weaponName);
    }
    return fallback;
}

/**
 * @brief Read a parameter value from a configuration file
 *
 * Searches for a parameter name in the specified file and returns
 * its associated value. The file format is "paramName:value".
 *
 * @param filePath Path to the configuration file
 * @param paramName Name of the parameter to find
 * @return Dynamically allocated string with the value, or NULL if not found
 */
char *readParam(const char *filePath, const char *paramName)
{
    char buffer[256];
    FILE *file = fopen(filePath, "r");
    if (!file)
        return NULL;

    /* Read file line by line */
    while (fgets(buffer, sizeof(buffer), file))
    {
        /* Parameter searching */
        char *colon = strchr(buffer, ':');
        if (!colon)
            continue;

        *colon = '\0';
        char *key = buffer;
        char *value = colon + 1;

        /* Trim leading spaces */
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

/**
 * @brief Free an array of parameter strings
 *
 * @param params Array of strings to free
 * @param count Number of strings in the array
 */
void freeParams(char **params, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(params[i]);
    }
}

/**
 * @brief Display the tutorial at game start
 *
 * Reads and prints the tutorial content from the appropriate
 * language file based on current language setting.
 */
void displayTutorial()
{
    char tutoPath[128];
    snprintf(tutoPath, sizeof(tutoPath), "lang/%s/tuto.txt", currentLanguage);

    FILE *file = fopen(tutoPath, "r");
    if (!file)
    {
        printf("Error: Could not open tutorial file.\n");
        return;
    }

    char line[512];
    printf("\n");
    printf("======================================\n");
    printf("           TUTORIAL                  \n");
    printf("======================================\n\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    printf("\n======================================\n\n");

    fclose(file);
}

/**
 * @brief Load all history lines from history file
 *
 * Parses the history file and extracts all non-empty lines,
 * including chapter titles (starting with '!') and narrative text.
 * The first chapter title is displayed immediately.
 *
 * @param game Pointer to the game state to populate with history lines
 * @param storyNumber Story file number to load (1 to TOTAL_STORIES)
 */
void loadGameHistory(GameState *game, int storyNumber)
{
    /* Initialize history fields */
    game->currentHistoryLine = 0;
    game->totalHistoryLines = 0;

    /* Store selected story in game state */
    game->selectedStory = storyNumber;

    /* Build path to history file based on current language and story number */
    char historyPath[128];
    snprintf(historyPath, sizeof(historyPath), "lang/%s/game_history/history_%02d.txt", currentLanguage, storyNumber);

    FILE *file = fopen(historyPath, "r");
    if (!file)
    {
        printf("Error: Could not open history file '%s'.\n", historyPath);
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), file) && game->totalHistoryLines < 200)
    {
        /* Remove newline and carriage return */
        line[strcspn(line, "\r\n")] = '\0';

        /* Skip empty lines */
        if (strlen(line) == 0)
        {
            continue;
        }

        /* Check if line starts with '!' (chapter title) */
        if (line[0] == '!')
        {
            /* Copy the line (without the leading '!') */
            strncpy(game->historyLines[game->totalHistoryLines], line + 1, sizeof(game->historyLines[0]) - 1);
            game->historyLines[game->totalHistoryLines][sizeof(game->historyLines[0]) - 1] = '\0';
            game->totalHistoryLines++;
        }
        else
        {
            /* Regular narrative line */
            strncpy(game->historyLines[game->totalHistoryLines], line, sizeof(game->historyLines[0]) - 1);
            game->historyLines[game->totalHistoryLines][sizeof(game->historyLines[0]) - 1] = '\0';
            game->totalHistoryLines++;
        }
    }

    fclose(file);

    /* Display the first chapter title immediately */
    if (game->totalHistoryLines > 0)
    {
        printf("\n");
        printf("======================================\n");
        printf("%s\n", game->historyLines[0]);
        printf("======================================\n");
        printf("\n");
        game->currentHistoryLine = 1; /* Start from the second line */
    }
}

/**
 * @brief Display the next history line based on player outcome
 *
 * When player exits successfully, displays the next history line.
 * When player dies, displays "???" to hide the line.
 * Shows both chapter titles and narrative lines as player progresses.
 */
void displayHistoryLine(GameState *game, int playerSurvived)
{
    if (game->currentHistoryLine >= game->totalHistoryLines)
    {
        /* No more history lines to display */
        return;
    }

    printf("\n");
    printf("--------------------------------------\n");

    if (playerSurvived)
    {
        /* Player survived - show the actual history line */
        printf("%s\n", game->historyLines[game->currentHistoryLine]);
    }
    else
    {
        /* Player died - show "???" instead */
        printf("???\n");
    }

    printf("--------------------------------------\n");
    printf("\n");

    /* Move to next history line */
    game->currentHistoryLine++;

    /* Check if we reached a new chapter title */
    if (game->currentHistoryLine < game->totalHistoryLines)
    {
        const char *line = game->historyLines[game->currentHistoryLine];
        /* Chapter titles start with "Chapter" or "Chapitre" */
        if (strncmp(line, "Chapter", 7) == 0 || strncmp(line, "Chapitre", 8) == 0)
        {
            printf("\n");
            printf("======================================\n");
            printf("%s\n", game->historyLines[game->currentHistoryLine]);
            printf("======================================\n");
            printf("\n");
            game->currentHistoryLine++; /* Skip past chapter title */
        }
    }
}

/* Author names : VERDIN Maxime and FOURNIER Nathan */
