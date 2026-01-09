#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player {
    char username[30];
    int level;
    int spicyIndex;
    int numGames;
    int numWins;
    int numLosses;
    int inQueue;
} Player;

// Fonctions de création/destruction
Player* createPlayer(const char* username);
void destroyPlayer(Player* player);
void displayPlayerMini(const Player* player);
int addPlayer(Player* players[], int* count, Player* player);
int searchPlayerByUsername(Player* players[], int count, const char* username);
int removePlayer(Player* players[], int* count, const char* username);
void sortPlayersBySpicyIndex(Player* players[], int count);
void displayAllPlayers(Player* players[], int count);

// Fonctions d'affichage
void displayPlayer(const Player* player);
void displayPlayerMini(const Player* player);

// Fonction utilitaire pour le tri
int comparePlayer(const Player* p1, const Player* p2);

#endif // PLAYER_H
