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
