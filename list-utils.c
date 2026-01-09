#include "../headers/list-utils.h"

t_cell * createCell(Player * player) {
    if (player == NULL) {
        return NULL;
    }

    // Allocation de la cellule
    t_cell* cell = (t_cell*)malloc(sizeof(t_cell));
    if (cell == NULL) {
        return NULL;
    }

    // Initialisation
    cell->value = player;
    cell->next = NULL;

    return cell;

}


void destroyCell(t_cell * cell) {
    if (cell != NULL) {
        free(cell);
    }
}

t_queue * createQueue() {
    t_queue* queue = (t_queue*)malloc(sizeof(t_queue));
    if (queue == NULL) {
        return NULL;
    }

    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

int isQueueEmpty(t_queue * queue) {
    if (queue == NULL) {
        return 1;
    }

    return (queue->head == NULL);
}
void displayQueue(t_queue * queue) {
    printf("File d'attente\n");

    if (queue == NULL || isQueueEmpty(queue)) {
        printf("File vide.\n");
        printf("Total: 0 joueurs en attente\n");
        printf("==\n");
        return;
    }

    // Parcours de la file
    t_cell* current = queue->head;
    int count = 0;

    while (current != NULL) {
        count++;
        printf("%d. ", count);
        displayPlayerMini(current->value);
        current = current->next;
    }

    printf("Total: %d joueur%s en attente\n", count, count > 1 ? "s" : "");
    printf("==\n");
}
void enqueue(t_queue * queue, Player * player) {
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

    // Cas 1 : file vide
    if (isQueueEmpty(queue)) {
        queue->head = newCell;
        queue->tail = newCell;
    }
    // Cas 2 : file non vide
    else {
        queue->tail->next = newCell;
        queue->tail = newCell;
    }
}

// Retire et retourne le premier joueur de la file
Player* dequeue(t_queue* queue) {
    if (queue == NULL || isQueueEmpty(queue)) {
        return NULL;
    }

    // Sauvegarde du pointeur vers le joueur
    Player* player = queue->head->value;

    // Sauvegarde de la cellule à libérer
    t_cell* toDelete = queue->head;

    // Avancement de head
    queue->head = queue->head->next;

    // Si la file devient vide, mettre tail à NULL aussi
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    // Libération de la cellule
    destroyCell(toDelete);

    // Marquer le joueur comme n'étant plus en file
    if (player != NULL) {
        player->inQueue = 0;
    }

    return player;
}

// Retire la cellule de tête sans la libérer
t_cell* removeHead(t_queue* queue) {
    if (queue == NULL || isQueueEmpty(queue)) {
        return NULL;
    }

    // Sauvegarde de la cellule de tête
    t_cell* head = queue->head;

    // Marquer le joueur comme n'étant plus en file
    if (head->value != NULL) {
        head->value->inQueue = 0;
    }

    // Avancement de head
    queue->head = queue->head->next;

    // Si la file devient vide, mettre tail à NULL aussi
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    // Déconnexion de la cellule retirée
    head->next = NULL;

    return head;
}

// Détruit la file et libère toute la mémoire
void destroyQueue(t_queue* queue) {
    if (queue == NULL) {
        return;
    }

    // Libération de toutes les cellules
    while (!isQueueEmpty(queue)) {
        t_cell* cell = removeHead(queue);
        destroyCell(cell);
    }

    // Libération de la structure de file
    free(queue);
}