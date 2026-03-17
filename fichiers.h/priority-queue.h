#ifndef FORKKNIGHT_PRIORITY_QUEUE_H
#define FORKKNIGHT_PRIORITY_QUEUE_H

#include "list-utils.h"
#include "player.h"

typedef t_queue t_priority_queue;

t_priority_queue* createPriorityQueue();
void insertWithPriority(t_priority_queue* queue, Player* player);
Player* removeHighestPriority(t_priority_queue* queue);
void displayPriorityQueue(t_priority_queue* queue);
void destroyPriorityQueue(t_priority_queue* queue);
// Optionnel :
void formBalancedMatch(t_priority_queue* queue);

#endif // PRIORITY_QUEUE_H
