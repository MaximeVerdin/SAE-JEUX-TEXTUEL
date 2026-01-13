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

void freeParams(char **params, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(params[i]);
    }
}