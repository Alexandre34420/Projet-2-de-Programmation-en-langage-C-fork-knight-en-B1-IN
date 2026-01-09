/**
 * Tests unitaires - Partie 3
 * Module: list-utils.c
 */

#include "../unity/unity.h"
#include "../../headers/list-utils.h"
#include "../../headers/player.h"

void setUp(void) {
    // Appelé avant chaque test
}

void tearDown(void) {
    // Appelé après chaque test
}

// ========================================
// Tests pour createCell()
// ========================================

void test_createCell_should_allocate_memory(void) {
    Player* p = createPlayer("TestUser");
    t_cell* cell = createCell(p);

    TEST_ASSERT_NOT_NULL(cell);

    if (cell != NULL) {
        destroyCell(cell);
    }
    destroyPlayer(p);
}

void test_createCell_should_initialize_value(void) {
    Player* p = createPlayer("TestUser");
    t_cell* cell = createCell(p);

    TEST_ASSERT_NOT_NULL(cell);
    if (cell != NULL) {
        TEST_ASSERT_EQUAL_PTR(p, cell->value);
        destroyCell(cell);
    }
    destroyPlayer(p);
}

void test_createCell_should_initialize_next_to_null(void) {
    Player* p = createPlayer("TestUser");
    t_cell* cell = createCell(p);

    TEST_ASSERT_NOT_NULL(cell);
    if (cell != NULL) {
        TEST_ASSERT_NULL(cell->next);
        destroyCell(cell);
    }
    destroyPlayer(p);
}

void test_createCell_should_handle_null_player(void) {
    t_cell* cell = createCell(NULL);

    // Should still create a cell, just with NULL value
    TEST_ASSERT_NOT_NULL(cell);
    if (cell != NULL) {
        TEST_ASSERT_NULL(cell->value);
        destroyCell(cell);
    }
}

// ========================================
// Tests pour destroyCell()
// ========================================

void test_destroyCell_should_not_crash_with_null(void) {
    // Ne doit pas planter si on passe NULL
    destroyCell(NULL);
    TEST_ASSERT_TRUE(1); // Si on arrive ici, pas de crash
}

void test_destroyCell_should_not_free_player(void) {
    // Vérifie que le Player n'est pas libéré par destroyCell
    Player* p = createPlayer("TestUser");
    t_cell* cell = createCell(p);

    if (cell != NULL) {
        destroyCell(cell);
        // Le player doit toujours être valide
        TEST_ASSERT_EQUAL_STRING("TestUser", p->username);
        destroyPlayer(p);
    } else {
        destroyPlayer(p);
    }
}

// ========================================
// Tests pour createQueue()
// ========================================

void test_createQueue_should_allocate_memory(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);

    if (queue != NULL) {
        free(queue);
    }
}

void test_createQueue_should_initialize_head_to_null(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);
    if (queue != NULL) {
        TEST_ASSERT_NULL(queue->head);
        free(queue);
    }
}

void test_createQueue_should_initialize_tail_to_null(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);
    if (queue != NULL) {
        TEST_ASSERT_NULL(queue->tail);
        free(queue);
    }
}

// ========================================
// Tests pour isQueueEmpty()
// ========================================

void test_isQueueEmpty_should_return_true_for_new_queue(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);
    if (queue != NULL) {
        TEST_ASSERT_TRUE(isQueueEmpty(queue));
        free(queue);
    }
}

void test_isQueueEmpty_should_return_false_after_enqueue(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("TestUser");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);
    TEST_ASSERT_FALSE(isQueueEmpty(queue));
    destroyQueue(queue);
    destroyPlayer(p);
}

void test_isQueueEmpty_should_return_true_after_dequeue_all(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("TestUser");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);
    dequeue(queue);
    TEST_ASSERT_TRUE(isQueueEmpty(queue));
    free(queue);
    destroyPlayer(p);
}

// ========================================
// Tests pour enqueue()
// ========================================

void test_enqueue_should_add_to_empty_queue(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);

    TEST_ASSERT_NOT_NULL(queue->head);
    TEST_ASSERT_NOT_NULL(queue->tail);
    TEST_ASSERT_EQUAL_PTR(queue->head, queue->tail);
    TEST_ASSERT_EQUAL_PTR(p, queue->head->value);

    destroyQueue(queue);
    destroyPlayer(p);
}

void test_enqueue_should_set_inQueue_flag(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    TEST_ASSERT_EQUAL_INT(0, p->inQueue);
    enqueue(queue, p);
    TEST_ASSERT_EQUAL_INT(1, p->inQueue);

    destroyQueue(queue);
    destroyPlayer(p);
}

void test_enqueue_should_add_multiple_players(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    TEST_ASSERT_EQUAL_PTR(p1, queue->head->value);
    TEST_ASSERT_EQUAL_PTR(p3, queue->tail->value);

    destroyQueue(queue);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_enqueue_should_maintain_fifo_order(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    // Vérifier l'ordre via les pointeurs next
    TEST_ASSERT_EQUAL_PTR(p1, queue->head->value);
    TEST_ASSERT_EQUAL_PTR(p2, queue->head->next->value);
    TEST_ASSERT_EQUAL_PTR(p3, queue->head->next->next->value);
    TEST_ASSERT_NULL(queue->tail->next);

    destroyQueue(queue);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

// ========================================
// Tests pour dequeue()
// ========================================

void test_dequeue_should_return_null_for_empty_queue(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);

    Player* p = dequeue(queue);
    TEST_ASSERT_NULL(p);
    free(queue);
}

void test_dequeue_should_remove_and_return_head(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);

    enqueue(queue, p1);
    enqueue(queue, p2);

    Player* dequeued = dequeue(queue);
    TEST_ASSERT_EQUAL_PTR(p1, dequeued);
    TEST_ASSERT_EQUAL_PTR(p2, queue->head->value);

    destroyQueue(queue);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_dequeue_should_clear_inQueue_flag(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);
    TEST_ASSERT_EQUAL_INT(1, p->inQueue);

    dequeue(queue);
    TEST_ASSERT_EQUAL_INT(0, p->inQueue);

    free(queue);
    destroyPlayer(p);
}

void test_dequeue_should_empty_queue_after_last_element(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);
    dequeue(queue);

    TEST_ASSERT_TRUE(isQueueEmpty(queue));
    TEST_ASSERT_NULL(queue->head);
    TEST_ASSERT_NULL(queue->tail);

    free(queue);
    destroyPlayer(p);
}

void test_dequeue_should_respect_fifo_order(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    TEST_ASSERT_EQUAL_PTR(p1, dequeue(queue));
    TEST_ASSERT_EQUAL_PTR(p2, dequeue(queue));
    TEST_ASSERT_EQUAL_PTR(p3, dequeue(queue));
    TEST_ASSERT_TRUE(isQueueEmpty(queue));

    free(queue);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

// ========================================
// Tests pour removeHead()
// ========================================

void test_removeHead_should_return_null_for_empty_queue(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);

    t_cell* cell = removeHead(queue);
    TEST_ASSERT_NULL(cell);
    free(queue);
}

void test_removeHead_should_return_head_cell(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);

    t_cell* cell = removeHead(queue);
    TEST_ASSERT_NOT_NULL(cell);
    TEST_ASSERT_EQUAL_PTR(p, cell->value);

    destroyCell(cell);
    free(queue);
    destroyPlayer(p);
}

void test_removeHead_should_clear_inQueue_flag(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);
    TEST_ASSERT_EQUAL_INT(1, p->inQueue);

    t_cell* cell = removeHead(queue);
    TEST_ASSERT_EQUAL_INT(0, p->inQueue);

    destroyCell(cell);
    free(queue);
    destroyPlayer(p);
}

void test_removeHead_should_not_free_cell(void) {
    t_queue* queue = createQueue();
    Player* p = createPlayer("Player1");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p);

    enqueue(queue, p);

    t_cell* cell = removeHead(queue);
    // La cellule doit être valide (pas libérée)
    TEST_ASSERT_NOT_NULL(cell);
    TEST_ASSERT_EQUAL_PTR(p, cell->value);

    // On doit pouvoir la libérer manuellement
    destroyCell(cell);
    free(queue);
    destroyPlayer(p);
}

// ========================================
// Tests pour destroyQueue()
// ========================================

void test_destroyQueue_should_free_empty_queue(void) {
    t_queue* queue = createQueue();

    TEST_ASSERT_NOT_NULL(queue);
    destroyQueue(queue);
    TEST_ASSERT_TRUE(1); // Si pas de crash, c'est bon
}

void test_destroyQueue_should_free_all_cells(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    destroyQueue(queue);

    // Les joueurs doivent toujours être valides
    TEST_ASSERT_EQUAL_STRING("Player1", p1->username);
    TEST_ASSERT_EQUAL_STRING("Player2", p2->username);
    TEST_ASSERT_EQUAL_STRING("Player3", p3->username);

    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_destroyQueue_should_clear_inQueue_flags(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);

    enqueue(queue, p1);
    enqueue(queue, p2);

    TEST_ASSERT_EQUAL_INT(1, p1->inQueue);
    TEST_ASSERT_EQUAL_INT(1, p2->inQueue);

    destroyQueue(queue);

    TEST_ASSERT_EQUAL_INT(0, p1->inQueue);
    TEST_ASSERT_EQUAL_INT(0, p2->inQueue);

    destroyPlayer(p1);
    destroyPlayer(p2);
}

// ========================================
// Tests d'intégration
// ========================================

void test_integration_enqueue_dequeue_cycle(void) {
    t_queue* queue = createQueue();
    Player* p1 = createPlayer("Alice");
    Player* p2 = createPlayer("Bob");
    Player* p3 = createPlayer("Charlie");

    TEST_ASSERT_NOT_NULL(queue);
    TEST_ASSERT_NOT_NULL(p1);
    TEST_ASSERT_NOT_NULL(p2);
    TEST_ASSERT_NOT_NULL(p3);

    // Ajouter 3 joueurs
    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    // Retirer 2 joueurs
    Player* d1 = dequeue(queue);
    Player* d2 = dequeue(queue);

    TEST_ASSERT_EQUAL_PTR(p1, d1);
    TEST_ASSERT_EQUAL_PTR(p2, d2);
    TEST_ASSERT_FALSE(isQueueEmpty(queue));

    // Ajouter 2 nouveaux joueurs
    Player* p4 = createPlayer("David");
    Player* p5 = createPlayer("Eve");
    TEST_ASSERT_NOT_NULL(p4);
    TEST_ASSERT_NOT_NULL(p5);

    enqueue(queue, p4);
    enqueue(queue, p5);

    // Retirer tous
    TEST_ASSERT_EQUAL_PTR(p3, dequeue(queue));
    TEST_ASSERT_EQUAL_PTR(p4, dequeue(queue));
    TEST_ASSERT_EQUAL_PTR(p5, dequeue(queue));
    TEST_ASSERT_TRUE(isQueueEmpty(queue));

    free(queue);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
    destroyPlayer(p4);
    destroyPlayer(p5);
}

// ========================================
// Main runner
// ========================================

int main(void) {
    UNITY_BEGIN();

    // Tests createCell
    RUN_TEST(test_createCell_should_allocate_memory);
    RUN_TEST(test_createCell_should_initialize_value);
    RUN_TEST(test_createCell_should_initialize_next_to_null);
    RUN_TEST(test_createCell_should_handle_null_player);

    // Tests destroyCell
    RUN_TEST(test_destroyCell_should_not_crash_with_null);
    RUN_TEST(test_destroyCell_should_not_free_player);

    // Tests createQueue
    RUN_TEST(test_createQueue_should_allocate_memory);
    RUN_TEST(test_createQueue_should_initialize_head_to_null);
    RUN_TEST(test_createQueue_should_initialize_tail_to_null);

    // Tests isQueueEmpty
    RUN_TEST(test_isQueueEmpty_should_return_true_for_new_queue);
    RUN_TEST(test_isQueueEmpty_should_return_false_after_enqueue);
    RUN_TEST(test_isQueueEmpty_should_return_true_after_dequeue_all);

    // Tests enqueue
    RUN_TEST(test_enqueue_should_add_to_empty_queue);
    RUN_TEST(test_enqueue_should_set_inQueue_flag);
    RUN_TEST(test_enqueue_should_add_multiple_players);
    RUN_TEST(test_enqueue_should_maintain_fifo_order);

    // Tests dequeue
    RUN_TEST(test_dequeue_should_return_null_for_empty_queue);
    RUN_TEST(test_dequeue_should_remove_and_return_head);
    RUN_TEST(test_dequeue_should_clear_inQueue_flag);
    RUN_TEST(test_dequeue_should_empty_queue_after_last_element);
    RUN_TEST(test_dequeue_should_respect_fifo_order);

    // Tests removeHead
    RUN_TEST(test_removeHead_should_return_null_for_empty_queue);
    RUN_TEST(test_removeHead_should_return_head_cell);
    RUN_TEST(test_removeHead_should_clear_inQueue_flag);
    RUN_TEST(test_removeHead_should_not_free_cell);

    // Tests destroyQueue
    RUN_TEST(test_destroyQueue_should_free_empty_queue);
    RUN_TEST(test_destroyQueue_should_free_all_cells);
    RUN_TEST(test_destroyQueue_should_clear_inQueue_flags);

    // Tests d'intégration
    RUN_TEST(test_integration_enqueue_dequeue_cycle);

    return UNITY_END();
}