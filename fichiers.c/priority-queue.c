#include "../headers/priority-queue.h"
#include <stdio.h>
#include <stdlib.h>

t_priority_queue * createPriorityQueue() {
    return createQueue();
}

void insertWithPriority(t_priority_queue * queue, Player * player) {
    if (queue == NULL || player == NULL) {
        return;
    }

    // Vérification que le joueur n'est pas déjà en file
    if (player->inQueue == 1) {
        printf("Le joueur '%s' est déjà dans la file.\n", player->username);
        return;
    }

    // Création de la nouvelle cellule
    t_cell* newCell = createCell(player);
    if (newCell == NULL) {
        printf("Erreur d'allocation pour la cellule.\n");
        return;
    }

    // Marquer le joueur comme étant en file
    player->inQueue = 1;

    // Cas 1 : File vide
    if (isQueueEmpty(queue)) {
        queue->head = newCell;
        queue->tail = newCell;
        return;
    }

    // Cas 2 : Insertion en tête (spicyIndex plus grand que le premier)
    if (player->spicyIndex > queue->head->value->spicyIndex) {
        newCell->next = queue->head;
        queue->head = newCell;
        return;
    }

    // Cas 3 : Insertion au milieu ou en fin
    t_cell* current = queue->head;
    t_cell* previous = NULL;

    // Parcourir jusqu'à trouver la position d'insertion
    // On cherche le premier joueur avec spicyIndex STRICTEMENT inférieur
    while (current != NULL && current->value->spicyIndex >= player->spicyIndex) {
        previous = current;
        current = current->next;
    }

    // Insertion entre previous et current
    newCell->next = current;
    previous->next = newCell;

    // Si on insère en fin de file, mettre à jour tail
    if (current == NULL) {
        queue->tail = newCell;
    }
}

Player * removeHighestPriority(t_priority_queue * queue) {
    if (queue == NULL || isQueueEmpty(queue)) {
        return NULL;
    }
    return dequeue(queue);
}

void displayPriorityQueue(t_priority_queue * queue) {
    printf("File de Priorité\n");

    if (queue == NULL || isQueueEmpty(queue)) {
        printf("File vide.\n");
        printf("Total: 0 joueurs (ordre décroissant)\n");
        printf("===\n");
        return;
    }

    // Parcours de la file
    t_cell* current = queue->head;
    int count = 0;

    while (current != NULL) {
        count++;
        printf("%d. [%s | Lvl: %d | Spicy: %d]",
               count,
               current->value->username,
               current->value->level,
               current->value->spicyIndex);

        // Marquer le premier comme max
        if (count == 1) {
            printf(" (max)");
        }

        printf("\n");
        current = current->next;
    }

    printf("Total: %d joueur%s (ordre décroissant)\n", count, count > 1 ? "s" : "");
    printf("====\n");
}

void destroyPriorityQueue(t_priority_queue * queue) {
    (void)queue;
}

void formBalancedMatch(t_priority_queue* queue) {
    if (queue == NULL) {
        printf("File de priorité invalide.\n");
        return;
    }

    printf("FORMATION D'UN MATCH ÉQUILIBRÉ\n");

    // Tableau pour stocker les joueurs du match
    Player* match[12];
    int matchSize = 0;

    // Retirer jusqu'à 12 joueurs de la file
    while (matchSize < 12 && !isQueueEmpty(queue)) {
        Player* player = removeHighestPriority(queue);
        if (player != NULL) {
            match[matchSize] = player;
            matchSize++;
        }
    }

    // Vérification du nombre de joueurs
    if (matchSize < 2) {
        printf("Pas assez de joueurs pour former un match (minimum 2).\n");
        printf("Joueurs disponibles : %d\n\n", matchSize);

        // Remettre les joueurs dans la file
        for (int i = 0; i < matchSize; i++) {
            insertWithPriority(queue, match[i]);
        }
        return;
    }

    // Calcul des statistiques
    int minSpicy = match[matchSize - 1]->spicyIndex;
    int maxSpicy = match[0]->spicyIndex;
    int ecart = maxSpicy - minSpicy;
    int totalSpicy = 0;

    for (int i = 0; i < matchSize; i++) {
        totalSpicy += match[i]->spicyIndex;
    }

    int avgSpicy = totalSpicy / matchSize;

    // Affichage des participants
    printf("Participants au match (%d joueurs) :\n", matchSize);
    printf("───────────\n");
    for (int i = 0; i < matchSize; i++) {
        printf("  %2d. [%s | Lvl: %d | SI: %d]",
               i + 1,
               match[i]->username,
               match[i]->level,
               match[i]->spicyIndex);

        if (i == 0) {
            printf("(Meilleur)");
        } else if (i == matchSize - 1) {
            printf("(Plus faible)");
        }

        printf("\n");
    }

    // Affichage des statistiques
    printf("\n Statistiques du match :\n");
    printf("────────────────────────────────────────\n");
    printf("  • Spicy Index min    : %d\n", minSpicy);
    printf("  • Spicy Index max    : %d\n", maxSpicy);
    printf("  • Spicy Index moyen  : %d\n", avgSpicy);
    printf("  • Écart de niveau    : %d points", ecart);

    // Évaluation de l'équilibrage
    if (ecart <= 200) {
        printf("(Excellent)\n");
    } else if (ecart <= 400) {
        printf("(Acceptable)\n");
    } else {
        printf("(Déséquilibré)\n");
    }

    // Division en équipes (facultatif)
    printf("\n Répartition en équipes :\n");
    printf("─────────\n");

    int team1Size = matchSize / 2;
    int team2Size = matchSize - team1Size;

    printf("Équipe 1 (%d joueurs) :\n", team1Size);
    for (int i = 0; i < team1Size; i++) {
        printf("   • %s (SI: %d)\n", match[i]->username, match[i]->spicyIndex);
    }

    printf("\n Équipe 2 (%d joueurs) :\n", team2Size);
    for (int i = team1Size; i < matchSize; i++) {
        printf("   • %s (SI: %d)\n", match[i]->username, match[i]->spicyIndex);
    }

    // Mise à jour des statistiques des joueurs
    printf("\n Mise à jour des statistiques...\n");
    for (int i = 0; i < matchSize; i++) {
        match[i]->numGames++;
    }
    printf("✓ Nombre de parties incrémenté pour tous les joueurs.\n");

    printf(" LE MATCH EST PRÊT À COMMENCER !\n");
}
