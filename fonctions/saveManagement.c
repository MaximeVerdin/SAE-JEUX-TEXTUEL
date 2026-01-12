#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void createGame(char *name, int multiplayer, int difficulty, int skipTutorial)
{
    // Function to create a new game
    char *fileName = strcat(name, "_save.csv");
    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Error creating game file.\n");
        return;
    }
    fprintf(file, "Name:%s\n", name);
    fprintf(file, "Difficulty:%d\n", difficulty);
    fprintf(file, "Progress:%d\n", 0);

    fprintf(file, "player;vie;attack;luck;weapon;skills\n");

    fprintf(file, "upgrade;boost;unlocked\n");

    fclose(file);
}

void addPlayersToSave(char *fileName, Player *players, int playerCount)
{
    char buffer[256];
    FILE *file = fopen(fileName, "r");
    FILE *tempFile = fopen("temp_save.csv", "w");
    if (!file && !tempFile)
    {
        printf("Error opening game file for adding players.\n");
        return;
    }

    int inPlayersSection = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        fputs(buffer, tempFile);
        if (strncmp(buffer, "player;", 7) == 0)
        {
            inPlayersSection = 1;
            // Skip the header line
            fgets(buffer, sizeof(buffer), file);
            fputs(buffer, tempFile);
            // Write player data
            for (int i = 0; i < playerCount; i++)
            {
                fprintf(tempFile, "%s;%d;%d;%d;%s;%s\n",
                        players[i].name,
                        players[i].vie,
                        players[i].attack,
                        players[i].luck,
                        players[i].weapon,
                        players[i].skills);
            }
        }
        else if (inPlayersSection && strncmp(buffer, "!", 1) == 0)
        {
            inPlayersSection = 0;
        }
    }
}
