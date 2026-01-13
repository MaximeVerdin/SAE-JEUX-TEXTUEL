#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveManagement.h"
#include "player.h"

void createGame(char *name, int multiplayer, int difficulty, int skipTutorial)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", name);

    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Error creating game file.\n");
        return;
    }
    fprintf(file, "Name:%s\n", name);
    fprintf(file, "Difficulty:%d\n", difficulty);
    fprintf(file, "Progress:%d\n", 0);
    fprintf(file, "Multiplayer:%d\n", multiplayer);
    fprintf(file, "SkipTutorial:%d\n", skipTutorial);
    fprintf(file, "!\n");
    fprintf(file, "player;name;vie;attack;luck;weapon\n");
    fclose(file);
}

void addPlayersToSave(char *saveName, Player *players, int playerCount)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "a");
    if (!file)
    {
        printf("Error opening game file for adding players.\n");
        return;
    }

    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%d;%s;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].vie,
                players[i].attack,
                players[i].luck,
                players[i].weapon);
    }
    fprintf(file, "!\n");
    fclose(file);
}

int loadGameByName(char *saveName)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Error: Save file '%s' not found.\n", saveName);
        return -1;
    }

    char buffer[256];
    printf("Loading game '%s'...\n", saveName);

    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}

void saveGame(char *saveName, Player *players, int playerCount, int progress)
{
    char fileName[100];
    snprintf(fileName, sizeof(fileName), "saves/%s_save.csv", saveName);

    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Error saving game.\n");
        return;
    }

    fprintf(file, "Name:%s\n", saveName);
    fprintf(file, "Progress:%d\n", progress);
    fprintf(file, "!\n");
    fprintf(file, "player;name;vie;attack;luck;weapon\n");

    for (int i = 0; i < playerCount; i++)
    {
        fprintf(file, "%d;%s;%d;%d;%d;%s\n",
                i,
                players[i].name,
                players[i].vie,
                players[i].attack,
                players[i].luck,
                players[i].weapon);
    }

    fprintf(file, "!\n");
    fclose(file);
    printf("Game saved successfully!\n");
}
