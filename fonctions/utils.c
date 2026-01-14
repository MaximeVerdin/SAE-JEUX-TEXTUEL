/* Author names : VERDIN Maxime and FOURNIER Nathan */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* ===== Tutorial System Implementation ===== */

/**
 * @brief Read the entire tutorial file into memory
 *
 * @param filePath Path to the tutorial file
 * @return Dynamically allocated string with file contents, or NULL on error
 */
char *readTutorialFile(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        printf("Cannot open tutorial file: %s\n", filePath);
        return NULL;
    }

    /* Get file size */
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(fileSize + 1);
    if (!content)
    {
        fclose(file);
        return NULL;
    }

    fread(content, 1, fileSize, file);
    content[fileSize] = '\0';
    fclose(file);

    return content;
}

/**
 * @brief Extract a tutorial segment marked by the given marker
 *
 * Tutorial segments are delimited by $marker$ tags in the file.
 *
 * @param tutorialContent Full tutorial file content
 * @param marker Name of the segment to extract
 * @return Dynamically allocated segment string, or NULL if not found
 */
char *getTutorialSegment(const char *tutorialContent, const char *marker)
{
    if (!tutorialContent || !marker)
        return NULL;

    char startMarker[100];

    /* Create start marker */
    sprintf(startMarker, "$%s$", marker);

    /* Find start marker */
    char *start = strstr(tutorialContent, startMarker);
    if (!start)
        return NULL;

    /* Move past the start marker */
    start += strlen(startMarker);

    /* Find end marker (next $ marker) */
    char *end = strstr(start, "$");
    if (!end)
        return NULL;

    /* Calculate length and extract */
    int length = end - start;
    char *segment = malloc(length + 1);
    if (segment)
    {
        strncpy(segment, start, length);
        segment[length] = '\0';
    }

    return segment;
}

/**
 * @brief Display a tutorial segment
 *
 * @param tutorialContent Full tutorial file content
 * @param marker Name of the segment to display
 */
void displayTutorialSegment(const char *tutorialContent, const char *marker)
{
    char *segment = getTutorialSegment(tutorialContent, marker);
    if (segment)
    {
        printf("\n");
        printf("======================================\n");
        printf("%s", segment);
        printf("======================================\n\n");
        free(segment);
    }
    else
    {
        printf("\n[Tutorial: %s not found]\n", marker);
    }
}

/**
 * @brief Check if a tutorial step should be shown
 *
 * @param progress Current tutorial progress pointer
 * @param step Step to check
 * @return 1 if step should be shown, 0 otherwise
 */
int shouldShowTutorialStep(int *progress, int step)
{
    return (*progress < step);
}

/**
 * @brief Advance to a specific tutorial step
 *
 * @param progress Current tutorial progress pointer
 * @param step Step to advance to
 */
void advanceTutorialStep(int *progress, int step)
{
    if (*progress < step)
    {
        *progress = step;
    }
}

/**
 * @brief Run a tutorial step
 *
 * Displays the specified tutorial step and waits for user input.
 *
 * @param language Language code ("en" or "fr")
 * @param tutorialProgress Current tutorial progress pointer
 * @param step Step number to run (0-8)
 */
void runTutorial(const char *language, int *tutorialProgress, int step)
{
    /* Select tutorial file based on language */
    char tutoFilePath[100];
    if (strcmp(language, "fr") == 0)
    {
        strcpy(tutoFilePath, "lang/fr-fr/tuto.txt");
    }
    else
    {
        strcpy(tutoFilePath, "lang/en-gb/tuto.txt");
    }

    /* Read tutorial content */
    char *tutorialContent = readTutorialFile(tutoFilePath);
    if (!tutorialContent)
    {
        printf("Error: Cannot load tutorial\n");
        return;
    }

    /* Tutorial steps and their markers */
    const char *steps[][2] = {
        {"0", "intro"},                     /* Welcome/introduction */
        {"1", "find_the_chest"},            /* Find the chest */
        {"2", "open_the_chest"},            /* Open the chest */
        {"3", "spot_enemy_tuto"},           /* Enemy appears */
        {"4", "start_fight_tutorial"},      /* Start fight tutorial */
        {"5", "end_fight_tutorial"},        /* End fight tutorial */
        {"6", "spot_boss_tutorial"},        /* Boss appears */
        {"7", "start_tutorial_boss_fight"}, /* Boss fight */
        {"8", "end_tutorial_boss_fight"},   /* End boss fight */
    };

    /* Display current step */
    if (step >= 0 && step < 9)
    {
        displayTutorialSegment(tutorialContent, steps[step][1]);
        advanceTutorialStep(tutorialProgress, step);

        /* Pause for user to read */
        printf("Press ENTER to continue...");
        getchar();
        getchar();
    }

    free(tutorialContent);
}
