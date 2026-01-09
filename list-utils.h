#ifndef FORKKNIGHT_LIST_UTILS_H
#define FORKKNIGHT_LIST_UTILS_H

#endif //FORKKNIGHT_LIST_UTILS_H

#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include "player.h"

typedef struct s_cell {
    Player * value;           // On stocker l'adresse du joueur
    struct s_cell * next;     // Pointeur vers la cellule suivante
} t_cell;

typedef struct s_queue {
    t_cell * head;   // Pointeur vers le premier élément
    t_cell * tail;   // Pointeur vers le dernier élément
} t_queue;

// Fonctions de gestion des cellules
t_cell* createCell(Player* player);
void destroyCell(t_cell* cell);

// Fonctions de gestion de la file
t_queue* createQueue();
int isQueueEmpty(t_queue* queue);
void displayQueue(t_queue* queue);
void enqueue(t_queue* queue, Player* player);
Player* dequeue(t_queue* queue);
t_cell* removeHead(t_queue* queue);
void destroyQueue(t_queue* queue);

#endif // LIST_UTILS_H