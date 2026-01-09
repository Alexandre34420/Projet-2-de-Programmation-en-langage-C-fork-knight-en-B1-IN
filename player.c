#include "../headers/player.h"

Player* createPlayer(const char* username) {
    // Vérification que username n'est pas NULL
    if (username == NULL) {    // Évite le warning de variable non utilisée
        return NULL;
    }

    // Allocation dynamique de la mémoire pour le joueur
    Player* player = (Player*)malloc(sizeof(Player));

    // Vérification que l'allocation a réussi
    if (player == NULL) {
        return NULL;
    }

    // Copie sécurisée du username
    strncpy(player->username, username, 29);
    player->username[29] = '\0'; // Assure la terminaison

    // Initialisation des champs
    player->level = 1;
    player->spicyIndex = 1000;
    player->numGames = 0;
    player->numWins = 0;
    player->numLosses = 0;
    player->inQueue = 0;

    return player;
}

void destroyPlayer(Player* player) {
    if (player != NULL) {
        free(player);
    }
}

void displayPlayer(const Player* player) {
    if (player == NULL) {
        return;
    }

    printf("=========\n");
    printf("Player: %s\n", player->username);
    printf("Level: %d | Spicy Index: %d\n", player->level, player->spicyIndex);
    printf("Games: %d | Wins: %d | Losses: %d\n",
           player->numGames, player->numWins, player->numLosses);
    printf("In Queue: %s\n", player->inQueue ? "Yes" : "No");
    printf("\n");
}

void displayPlayerMini(const Player* player) {
    if (player == NULL) {
        return;
    }

    printf("[%s | Lvl: %d | SI: %d]\n",
           player->username, player->level, player->spicyIndex);
}
