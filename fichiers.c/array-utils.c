#include "../headers/array-utils.h"

int addPlayer(Player* players[], int* count, Player* player) {
    if (players == NULL || count == NULL || player == NULL) {
        return 0;
    }

    if (*count >= MAX_PLAYERS) {
        return 0;
    }

    players[*count] = player;
    (*count)++;

    return 1;
}

int searchPlayerByUsername(Player* players[], int count, const char* username) {
    if (players == NULL || username == NULL) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (players[i] != NULL && strcmp(players[i]->username, username) == 0) {
            return i;
        }
    }

    return -1;
}

int removePlayer(Player* players[], int* count, const char* username) {
    if (players == NULL || count == NULL || username == NULL) {
        return 0;
    }

    int index = searchPlayerByUsername(players, *count, username);

    if (index == -1) {
        return 0;
}

    // Libération de la mémoire du joueur
    destroyPlayer(players[index]);

    // Décalage des éléments vers la gauche
    for (int i = index; i < *count - 1; i++) {
        players[i] = players[i + 1];
    }

    // Mise à NULL du dernier élément
    players[*count - 1] = NULL;

    (*count)--;

    return 1;
}

int comparePlayer(const Player* p1, const Player* p2) {
    if (p1 == NULL || p2 == NULL) {
        return 0;
    }

    return p1->spicyIndex - p2->spicyIndex;
}

void sortPlayersBySpicyIndex(Player* players[], int count) {
    if (players == NULL || count <= 1) {
        return;
    }

    // Tri à bulles (décroissant)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j] != NULL && players[j + 1] != NULL) {
                if (comparePlayer(players[j], players[j + 1]) < 0) {
                    // Échange
                    Player* temp = players[j];
                    players[j] = players[j + 1];
                    players[j + 1] = temp;
                }
            }
        }
    }

}

void displayAllPlayers(Player* players[], int count) {
    printf("=== Liste des joueurs ===\n");

    if (count == 0) {
        printf("Aucun joueur.\n");
    } else {
        for (int i = 0; i < count; i++) {
            if (players[i] != NULL) {
                displayPlayerMini(players[i]);
            }
        }
    }

    printf("Total: %d joueur%s\n", count, count > 1 ? "s" : "");
}

// ============================================================================
// Part 2: Dynamic array management
// ============================================================================

Player** createPlayerArray(int size) {
    if (size <= 0) {
        return NULL;
    }

    // Allocation du tableau de pointeurs
    Player** array = (Player**)malloc(size * sizeof(Player*));

    if (array == NULL) {
        return NULL;
    }

    // Initialisation à NULL
    for (int i = 0; i < size; i++) {
        array[i] = NULL;
    }

    return array;
}

void destroyPlayerArray(Player** array, int size) {
    if (array == NULL) {
        return;
    }

    // Libération de chaque joueur
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            destroyPlayer(array[i]);
            array[i] = NULL;
        }
    }

    // Libération du tableau lui-même
    free(array);
}

// ============================================================================
// Part 2: File I/O
// ============================================================================

Player** loadPlayers(const char* filename, int* numPlayers) {
    if (filename == NULL || numPlayers == NULL) {
        return NULL;
    }

    // Ouverture du fichier en lecture
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier '%s'\n", filename);
        return NULL;
    }

    // Lecture du nombre de joueurs
    int count;
    if (fscanf(file, "%d", &count) != 1 || count <= 0) {
        printf("Erreur : nombre de joueurs invalide dans le fichier\n");
        fclose(file);
        return NULL;
    }

    // Création du tableau dynamique
    Player** array = createPlayerArray(count);
    if (array == NULL) {
        printf("Erreur : échec d'allocation du tableau\n");
        fclose(file);
        return NULL;
    }

    // Lecture de chaque joueur
    for (int i = 0; i < count; i++) {
        char username[30];
        int level, spicyIndex, numGames, numWins, numLosses, inQueue;

        if (fscanf(file, "%29s %d %d %d %d %d %d",
                   username, &level, &spicyIndex, &numGames,
                   &numWins, &numLosses, &inQueue) != 7) {
            printf("Erreur : données invalides pour le joueur %d\n", i + 1);
            destroyPlayerArray(array, count);
            fclose(file);
            return NULL;
        }

        // Création du joueur
        Player* player = createPlayer(username);
        if (player == NULL) {
            printf("Erreur : échec de création du joueur '%s'\n", username);
            destroyPlayerArray(array, count);
            fclose(file);
            return NULL;
        }

        // Mise à jour des champs
        player->level = level;
        player->spicyIndex = spicyIndex;
        player->numGames = numGames;
        player->numWins = numWins;
        player->numLosses = numLosses;
        player->inQueue = inQueue;

        array[i] = player;
    }

    fclose(file);
    *numPlayers = count;

    printf("✓ %d joueur%s chargé%s depuis '%s'\n",
           count, count > 1 ? "s" : "", count > 1 ? "s" : "", filename);

    return array;
}

void savePlayers(const char* filename, Player** array, int size) {
    if (filename == NULL || array == NULL) {
        printf("Erreur : paramètres invalides pour la sauvegarde\n");
        return;
    }

    // Ouverture du fichier en écriture
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur : impossible de créer le fichier '%s'\n", filename);
        return;
    }

    // Écriture du nombre de joueurs
    fprintf(file, "%d\n", size);

    // Écriture de chaque joueur
    for (int i = 0; i < size; i++) {
        if (array[i] != NULL) {
            fprintf(file, "%s %d %d %d %d %d %d\n",
                    array[i]->username,
                    array[i]->level,
                    array[i]->spicyIndex,
                    array[i]->numGames,
                    array[i]->numWins,
                    array[i]->numLosses,
                    array[i]->inQueue);
        }
    }

    fclose(file);
    printf("✓ %d joueur%s sauvegardé%s dans '%s'\n",
           size, size > 1 ? "s" : "", size > 1 ? "s" : "", filename);
}
