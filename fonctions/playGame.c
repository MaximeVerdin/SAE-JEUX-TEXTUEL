#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "playGame.h"
#include "player.h"
#include "saveManagement.h"
#include "upgradeTree.h"
#include "dungeon.h"

typedef struct
{
    char name[50];
    int vie;
    int attack;
    int experience;
} Enemy;

typedef struct
{
    Player players[4];
    int playerCount;
    int progress;
    int difficulty;
    int currentLevel;
    Dungeon dungeon;
} GameState;

void displayGameMenu()
{
    printf("\n=== MENU DU JEU ===\n");
    printf("1. Explorer (z/haut, s/bas, q/gauche, d/droite)\n");
    printf("2. Combattre un ennemi proche\n");
    printf("3. Afficher les joueurs\n");
    printf("4. Améliorations\n");
    printf("5. Sauvegarder\n");
    printf("6. Quitter au menu principal\n");
    printf("Votre choix: ");
}

void displayDungeonMenu()
{
    printf("\n=== DONJON - Niveau %d ===\n", 1);
    printf("Utilisez z/s/q/d pour vous déplacer\n");
    printf("1. Retour au menu\n");
    printf("Votre choix: ");
}

Enemy createEnemy(int difficulty)
{
    Enemy enemy;
    const char *enemyNames[] = {"Gobelin", "Orc", "Squelette", "Troll", "Dragon"};

    strcpy(enemy.name, enemyNames[rand() % 5]);
    enemy.vie = 20 + (difficulty * 10) + (rand() % 20);
    enemy.attack = 5 + (difficulty * 3) + (rand() % 8);
    enemy.experience = 10 + (difficulty * 5);

    return enemy;
}

int combat(Player *player, Enemy *enemy)
{
    printf("\n=== COMBAT: %s vs %s ===\n", player->name, enemy->name);
    printf("%s - Vie: %d, Attaque: %d\n", enemy->name, enemy->vie, enemy->attack);

    while (enemy->vie > 0 && player->vie > 0)
    {
        printf("\n%s (Vie: %d) VS %s (Vie: %d)\n",
               player->name, player->vie, enemy->name, enemy->vie);

        // Tour du joueur
        int playerDamage = player->attack + (rand() % player->luck);
        enemy->vie -= playerDamage;
        printf("%s attaque pour %d dégâts!\n", player->name, playerDamage);

        if (enemy->vie <= 0)
        {
            printf("%s a été vaincu!\n", enemy->name);
            return 1;
        }

        // Tour de l'ennemi
        int enemyDamage = enemy->attack + (rand() % 5);
        damagePlayer(player, enemyDamage);
        printf("%s attaque pour %d dégâts!\n", enemy->name, enemyDamage);

        if (player->vie <= 0)
        {
            printf("%s a été vaincu...\n", player->name);
            return 0;
        }
    }

    return 0;
}

void displayUpgradesMenu()
{
    printf("\n=== AMÉLIORATIONS ===\n");
    printf("1. +5 Attaque (100 pièces)\n");
    printf("2. +10 Vie (100 pièces)\n");
    printf("3. +2 Chance (150 pièces)\n");
    printf("4. Retour\n");
    printf("Votre choix: ");
}

void showPlayers(GameState *game)
{
    printf("\n=== JOUEURS (%d/%d) ===\n", game->playerCount, game->playerCount);
    for (int i = 0; i < game->playerCount; i++)
    {
        printf("\nJoueur %d:\n", i + 1);
        displayPlayer(&game->players[i]);
    }
}

void exploreDungeon(GameState *game)
{
    displayDungeon(&game->dungeon);

    int exploring = 1;
    while (exploring)
    {
        char move;
        printf("\nDirection (z/haut, s/bas, q/gauche, d/droite) ou 1/retour: ");
        fflush(stdout);

        // Lire seulement le premier caractère
        move = getchar();

        // Vider le reste de la ligne
        while (getchar() != '\n')
            ;

        if (move == '1')
        {
            exploring = 0;
            continue;
        }

        if (move != 'z' && move != 's' && move != 'q' && move != 'd')
        {
            printf("Direction invalide! Utilisez z/s/q/d\n");
            continue;
        }

        int result = movePlayer(&game->dungeon, move);

        if (result == 2) // Sortie trouvée
        {
            printf("\n Félicitations! Vous avez complété le donjon!\n");
            game->currentLevel++;
            game->progress++;

            if (game->progress % 3 == 0)
            {
                game->difficulty++;
                printf("Difficulté augmentée! Niveau %d\n", game->difficulty);
            }

            // Générer un nouveau donjon
            initDungeon(&game->dungeon, game->currentLevel);
            printf("\nNouveau donjon généré - Niveau %d\n", game->currentLevel);
        }
        else if (result >= 10) // Ennemi rencontré - collision forcée!
        {
            int enemyIdx = result - 10;
            printf("\n!!! RENCONTRE AVEC UN ENNEMI !!!\n");
            Enemy enemy = createEnemy(game->difficulty);
            int victory = combat(&game->players[0], &enemy);

            if (victory)
            {
                printf("Victoire! +%d d'expérience\n", enemy.experience);
                game->players[0].attack += 2;
                removeEnemy(&game->dungeon, enemyIdx);
            }
            else
            {
                printf("Vous avez été vaincu! Récupération...\n");
                game->players[0].vie = 100;
            }
        }
        else if (result == 1)
        {
            // Les ennemis bougent après le joueur
            updateEnemies(&game->dungeon);
        }

        if (exploring)
        {
            displayDungeon(&game->dungeon);
        }
    }
}

void playGame(char *saveName)
{
    srand(time(NULL));

    GameState game;
    game.playerCount = 1;
    game.progress = 0;
    game.difficulty = 1;
    game.currentLevel = 1;

    // Créer un joueur par défaut
    Player defaultPlayer = createPlayer("Hero", 100, 10, 5, "Hands", NULL, 0);
    game.players[0] = defaultPlayer;

    // Initialiser le donjon
    initDungeon(&game.dungeon, 1);

    printf("\n=== BIENVENUE DANS LE JEU ===\n");
    printf("Partie chargée: %s\n", saveName);
    printf("Explorez le donjon pour trouver la sortie (X)\n");
    printf("Vous ne pouvez voir que ce qui est dans votre champ de vision!\n");

    int playing = 1;
    while (playing)
    {
        // Afficher le donjon
        displayDungeon(&game.dungeon);

        displayGameMenu();
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // Explorer
            exploreDungeon(&game);
            break;

        case 2: // Combattre un ennemi proche
        {
            // Chercher un ennemi adjacent
            int dx[4] = {0, 0, -1, 1};
            int dy[4] = {-1, 1, 0, 0};
            int found = 0;

            for (int i = 0; i < 4 && !found; i++)
            {
                int nx = game.dungeon.playerPos.x + dx[i];
                int ny = game.dungeon.playerPos.y + dy[i];
                int enemyIdx = checkEnemyAt(&game.dungeon, nx, ny);

                if (enemyIdx >= 0)
                {
                    found = 1;
                    Enemy enemy = createEnemy(game.difficulty);
                    int victory = combat(&game.players[0], &enemy);

                    if (victory)
                    {
                        printf("Victoire! +%d d'expérience\n", enemy.experience);
                        game.players[0].attack += 2;
                        removeEnemy(&game.dungeon, enemyIdx);
                    }
                    else
                    {
                        printf("Vous avez été vaincu! Récupération...\n");
                        game.players[0].vie = 100;
                    }
                }
            }

            if (!found)
            {
                printf("Aucun ennemi adjacent! Utilisez Explorer pour vous déplacer.\n");
            }
            break;
        }

        case 3: // Afficher les joueurs
            showPlayers(&game);
            break;

        case 4: // Améliorations
        {
            int upgradeChoice = 1;
            while (upgradeChoice != 4)
            {
                displayUpgradesMenu();
                scanf("%d", &upgradeChoice);

                switch (upgradeChoice)
                {
                case 1:
                    addAttack(&game.players[0], 5);
                    printf("Attaque augmentée!\n");
                    break;
                case 2:
                    healPlayer(&game.players[0], 10);
                    printf("Vie restaurée!\n");
                    break;
                case 3:
                    addLuck(&game.players[0], 2);
                    printf("Chance augmentée!\n");
                    break;
                }
            }
            break;
        }

        case 5: // Sauvegarder
            saveGame(saveName, game.players, game.playerCount, game.progress);
            break;

        case 6: // Quitter au menu
            playing = 0;
            printf("Retour au menu principal...\n");
            break;

        default:
            printf("Choix invalide!\n");
        }
    }

    // Libérer la mémoire
    for (int i = 0; i < game.playerCount; i++)
    {
        freePlayerAbilities(&game.players[i]);
    }
}
