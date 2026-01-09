#include "../headers/match.h"

Match * createMatch() {
    // Allocation de la structure Match
    Match* match = (Match*)malloc(sizeof(Match));
    if (match == NULL) {
        return NULL;
    }

    // Allocation du tableau des joueurs
    match->players = (Player**)malloc(MATCH_MAX_PLAYERS * sizeof(Player*));
    if (match->players == NULL) {
        free(match);
        return NULL;
    }

    // Initialisation des champs
    match->results = NULL;
    match->numPlayers = 0;

    // Initialisation du tableau des joueurs à NULL
    for (int i = 0; i < MATCH_MAX_PLAYERS; i++) {
        match->players[i] = NULL;
    }

    return match;
}

void destroyMatch(Match * match) {
    if (match == NULL) {
        return;
    }

    // Libération du tableau des résultats s'il y en a
    if (match->results != NULL) {
        free(match->results);
    }

    // Libération du tableau de joueurs (mais pas les joueurs eux-mêmes)
    if (match->players != NULL) {
        free(match->players);
    }

    // Libération de la structure Match
    free(match);
}

void simulateMatch(Match * match) {
    if (match == NULL || match->numPlayers == 0) {
        return;
    }

    // Libération de l'ancien tableau de résultats s'il existe
    if (match->results != NULL) {
        free(match->results);
    }

    // Allocation du nouveau tableau de résultats
    match->results = (int*)malloc(match->numPlayers * sizeof(int));
    if (match->results == NULL) {
        printf("Erreur d'allocation pour les résultats du match.\n");
        return;
    }

    // Initialisation avec les indices des joueurs (0, 1, 2, ..., n-1)
    for (int i = 0; i < match->numPlayers; i++) {
        match->results[i] = i;
    }
}

void displayMatchResult(const Match * match) {
    if (match == NULL) {
        printf("Match invalide.\n");
        return;
    }

    if (match->results == NULL) {
        printf("Aucun résultat disponible. Le match n'a pas été simulé.\n");
        return;
    }

    printf("==MATCH RESULTS\n");


    int n = match->numPlayers;

    // Affichage du classement
    for (int r = 0; r < n; r++) {
        int playerIndex = match->results[r];
        Player* player = match->players[playerIndex];

        if (player == NULL) {
            continue;
        }

        // Calcul du gain pour affichage
        int gain = 0;
        if (n > 1) {
            gain = (n - r - 1) * (100 / (n - 1));
        }

        // Affichage d'une victoire pour le vainqueur
        if (r == 0) {
            printf("victoire%d. ", r + 1);
        } else {
            printf(" %d. ", r + 1);
        }

        printf("[%s | Lvl: %d | Spicy: %d]  (+%d)\n",
               player->username,
               player->level,
               player->spicyIndex - gain,  // Spicy avant le gain
               gain);
    }

    printf("=========\n\n");
}

void addMatchPlayers(t_queue * queue, Match * match) {
    if (queue == NULL || match == NULL) {
        return;
    }

    // Retirer des joueurs de la file jusqu'à atteindre la limite
    while (match->numPlayers < MATCH_MAX_PLAYERS && !isQueueEmpty(queue)) {
        Player* player = dequeue(queue);
        if (player != NULL) {
            match->players[match->numPlayers] = player;
            match->numPlayers++;
            // Note: dequeue() met déjà inQueue à 0
        }
    }
}

void displayMatchInfo(const Match * match) {
    if (match == NULL) {
        printf("Match invalide.\n");
        return;
    }

    printf("===MATCH INFORMATION\n");
   ;
    printf("Participants: %d\n", match->numPlayers);
    printf("---------\n");

    for (int i = 0; i < match->numPlayers; i++) {
        if (match->players[i] != NULL) {
            printf("%d. ", i + 1);
            displayPlayerMini(match->players[i]);
        }
    }

    printf("=========\n\n");
}

void updatePlayerStats(Match * match) {
    if (match == NULL || match->results == NULL || match->numPlayers == 0) {
        return;
    }

    int n = match->numPlayers;

    // Pour chaque position dans le classement
    for (int r = 0; r < n; r++) {
        int playerIndex = match->results[r];
        Player* player = match->players[playerIndex];

        if (player == NULL) {
            continue;
        }

        // Incrémenter le nombre de parties
        player->numGames++;

        // Mise à jour des victoires et défaites
        if (r == 0) {
            player->numWins++;
        } else {
            player->numLosses++;
        }

        // Calcul du gain de spicyIndex
        int gain = 0;
        if (n > 1) {
            gain = (n - r - 1) * (100 / (n - 1));
        }

        player->spicyIndex += gain;
    }
}

void launchMatch(t_queue * queue) {
    if (queue == NULL) {
        printf("File invalide.\n");
        return;
    }

    printf("LANCEMENT D'UN MATCH FIFO\n");

    // Vérification qu'il y a assez de joueurs
    if (isQueueEmpty(queue)) {
        printf("La file est vide. Impossible de créer un match.\n\n");
        return;
    }

    // Création du match
    Match* match = createMatch();
    if (match == NULL) {
        printf("Erreur lors de la création du match.\n\n");
        return;
    }

    // Ajout des joueurs
    addMatchPlayers(queue, match);

    if (match->numPlayers < 2) {
        printf("Pas assez de joueurs pour un match (minimum 2).\n");
        printf("Joueurs disponibles : %d\n\n", match->numPlayers);
        destroyMatch(match);
        return;
    }

    // Affichage des informations
    displayMatchInfo(match);

    // Initialisation du générateur aléatoire
    srand(time(NULL));

    // Simulation du match
    printf("Simulation du match en cours...\n\n");
    simulateMatch(match);

    // Mise à jour des statistiques
    updatePlayerStats(match);

    // Affichage des résultats
    displayMatchResult(match);

    printf("Statistiques des joueurs mises à jour.\n\n");

    // Libération de la mémoire
    destroyMatch(match);
}

void launchBalancedMatch(t_priority_queue * p_queue) {
    if (p_queue == NULL) {
        printf("File de priorité invalide.\n");
        return;
    }

    printf("LANCEMENT D'UN MATCH ÉQUILIBRÉ\n");


    // Vérification qu'il y a assez de joueurs
    if (isQueueEmpty(p_queue)) {
        printf("La file de priorité est vide. Impossible de créer un match.\n\n");
        return;
    }

    // Création du match
    Match* match = createMatch();
    if (match == NULL) {
        printf("Erreur lors de la création du match.\n\n");
        return;
    }

    // Extraction des joueurs de la file de priorité
    while (match->numPlayers < MATCH_MAX_PLAYERS && !isQueueEmpty(p_queue)) {
        Player* player = removeHighestPriority(p_queue);
        if (player != NULL) {
            match->players[match->numPlayers] = player;
            match->numPlayers++;
        }
    }

    if (match->numPlayers < 2) {
        printf("Pas assez de joueurs pour un match (minimum 2).\n");
        printf("   Joueurs disponibles : %d\n\n", match->numPlayers);
        destroyMatch(match);
        return;
    }

    // Calcul de l'écart de niveau
    int minSpicy = match->players[match->numPlayers - 1]->spicyIndex;
    int maxSpicy = match->players[0]->spicyIndex;
    int ecart = maxSpicy - minSpicy;

    printf("Équilibrage du match :\n");
    printf("   • Spicy Index min : %d\n", minSpicy);
    printf("   • Spicy Index max : %d\n", maxSpicy);
    printf("   • Écart de niveau : %d points", ecart);

    if (ecart <= 200) {
        printf("(Excellent)\n\n");
    } else if (ecart <= 400) {
        printf("(Acceptable)\n\n");
    } else {
        printf("(Mauvais)\n\n");
    }

    // Affichage des informations
    displayMatchInfo(match);

    // Initialisation du générateur aléatoire
    srand(time(NULL));

    // Simulation du match
    printf("Simulation du match en cours...\n\n");
    simulateMatch(match);

    // Mise à jour des statistiques
    updatePlayerStats(match);

    // Affichage des résultats
    displayMatchResult(match);

    printf("Statistiques des joueurs mises à jour.\n\n");

    // Libération de la mémoire
    destroyMatch(match);
}
void fisherYatesShuffle(int * array, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1); // indice aléatoire entre 0 et i
        // Échange array[i] et array[j]
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

