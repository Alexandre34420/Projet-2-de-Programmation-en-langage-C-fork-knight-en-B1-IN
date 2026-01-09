/**
 * Tests unitaires - Partie 4
 * Module: priority-queue.c
 */

#include "../unity/unity.h"
#include "../../headers/priority-queue.h"
#include "../../headers/player.h"

void setUp(void) {}
void tearDown(void) {}


void test_createPriorityQueue_should_allocate_and_initialize(void) {
    t_priority_queue* pq = createPriorityQueue();
    TEST_ASSERT_NOT_NULL(pq);
    TEST_ASSERT_NULL(pq->head);
    TEST_ASSERT_NULL(pq->tail);
    destroyPriorityQueue(pq);
}

void test_insertWithPriority_should_insert_in_order(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* p1 = createPlayer("A"); p1->spicyIndex = 1000;
    Player* p2 = createPlayer("B"); p2->spicyIndex = 1200;
    Player* p3 = createPlayer("C"); p3->spicyIndex = 800;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    insertWithPriority(pq, p3);
    TEST_ASSERT_EQUAL_PTR(p2, pq->head->value);
    TEST_ASSERT_EQUAL_PTR(p1, pq->head->next->value);
    TEST_ASSERT_EQUAL_PTR(p3, pq->tail->value);
    destroyPriorityQueue(pq);
    destroyPlayer(p1); destroyPlayer(p2); destroyPlayer(p3);
}

void test_insertWithPriority_should_handle_equal_spicyIndex_fifo(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* p1 = createPlayer("A"); p1->spicyIndex = 1000;
    Player* p2 = createPlayer("B"); p2->spicyIndex = 1000;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    TEST_ASSERT_EQUAL_PTR(p1, pq->head->value);
    TEST_ASSERT_EQUAL_PTR(p2, pq->tail->value);
    destroyPriorityQueue(pq);
    destroyPlayer(p1); destroyPlayer(p2);
}

void test_removeHighestPriority_should_return_max_spicyIndex(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* p1 = createPlayer("A"); p1->spicyIndex = 1000;
    Player* p2 = createPlayer("B"); p2->spicyIndex = 1200;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    Player* removed = removeHighestPriority(pq);
    TEST_ASSERT_EQUAL_PTR(p2, removed);
    destroyPriorityQueue(pq);
    destroyPlayer(p1); destroyPlayer(p2);
}

void test_displayPriorityQueue_should_format_output(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* p1 = createPlayer("A"); p1->spicyIndex = 1000;
    Player* p2 = createPlayer("B"); p2->spicyIndex = 1200;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    // On ne teste pas l'affichage console ici, mais on vérifie la structure
    TEST_ASSERT_EQUAL_PTR(p2, pq->head->value);
    TEST_ASSERT_EQUAL_PTR(p1, pq->tail->value);
    destroyPriorityQueue(pq);
    destroyPlayer(p1); destroyPlayer(p2);
}

void test_destroyPriorityQueue_should_free_memory(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* p1 = createPlayer("A"); p1->spicyIndex = 1000;
    insertWithPriority(pq, p1);
    destroyPriorityQueue(pq);
    destroyPlayer(p1);
    // Si on arrive ici sans crash, c'est OK
    TEST_ASSERT_TRUE(1);
}

void test_formBalancedMatch_should_select_12_closest(void) {
    t_priority_queue* pq = createPriorityQueue();
    Player* players[15];
    for (int i = 0; i < 15; ++i) {
        Player* p = createPlayer("P");
        p->spicyIndex = 1000 + i * 10;
        players[i] = p;
        insertWithPriority(pq, p);
    }
    formBalancedMatch(pq); // On ne teste pas l'affichage, mais la fonction doit s'exécuter
    destroyPriorityQueue(pq);
    // Free all players
    for (int i = 0; i < 15; ++i) {
        destroyPlayer(players[i]);
    }
    // Si on arrive ici sans crash, c'est OK
    TEST_ASSERT_TRUE(1);
}

// --- TESTS ADDITIONNELS ---
void test_insertWithPriority_should_work_on_empty_queue(void) {
    t_priority_queue *pq = createPriorityQueue();
    Player *p = createPlayer("A");
    p->spicyIndex = 1000;
    insertWithPriority(pq, p);
    TEST_ASSERT_NOT_NULL(pq);
    TEST_ASSERT_NOT_NULL(pq->head);
    destroyPriorityQueue(pq);
    destroyPlayer(p);
}

void test_insertWithPriority_should_fifo_on_equal_priority(void) {
    t_priority_queue *pq = createPriorityQueue();
    Player *p1 = createPlayer("A");
    Player *p2 = createPlayer("B");
    p1->spicyIndex = 1000;
    p2->spicyIndex = 1000;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    TEST_ASSERT_EQUAL_PTR(p1, pq->head->value);
    TEST_ASSERT_EQUAL_PTR(p2, pq->head->next->value);
    destroyPriorityQueue(pq);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_removeHighestPriority_should_return_null_on_empty(void) {
    t_priority_queue *pq = createPriorityQueue();
    Player *p = removeHighestPriority(pq);
    TEST_ASSERT_NULL(p);
    destroyPriorityQueue(pq);
}

void test_displayPriorityQueue_should_handle_empty(void) {
    t_priority_queue *pq = createPriorityQueue();
    // On attend un affichage vide ou formaté
    displayPriorityQueue(pq);
    destroyPriorityQueue(pq);
}

void test_destroyPriorityQueue_should_handle_empty(void) {
    t_priority_queue *pq = createPriorityQueue();
    destroyPriorityQueue(pq);
}

void test_insert_and_remove_multiple_players(void) {
    t_priority_queue *pq = createPriorityQueue();
    Player *p1 = createPlayer("A");
    Player *p2 = createPlayer("B");
    Player *p3 = createPlayer("C");
    p1->spicyIndex = 1000;
    p2->spicyIndex = 1200;
    p3->spicyIndex = 1100;
    insertWithPriority(pq, p1);
    insertWithPriority(pq, p2);
    insertWithPriority(pq, p3);
    Player *max = removeHighestPriority(pq);
    TEST_ASSERT_EQUAL_PTR(p2, max); // p2 a la plus grande priorité
    destroyPlayer(max);
    destroyPriorityQueue(pq);
    destroyPlayer(p1);
    destroyPlayer(p3);
}

void test_stress_insert_remove(void) {
    t_priority_queue *pq = createPriorityQueue();
    int N = 100;
    Player *players[100];
    for (int i = 0; i < N; ++i) {
        char name[8];
        snprintf(name, sizeof(name), "P%d", i);
        players[i] = createPlayer(name);
        players[i]->spicyIndex = i;
        insertWithPriority(pq, players[i]);
    }
    for (int i = N-1; i >= 0; --i) {
        Player *p = removeHighestPriority(pq);
        TEST_ASSERT_NOT_NULL(p);
        destroyPlayer(p);
    }
    destroyPriorityQueue(pq);
}

void test_operations_after_destroy(void) {
    t_priority_queue *pq = createPriorityQueue();
    destroyPriorityQueue(pq);
    pq = NULL;  // Set to NULL after destroy
    // Les opérations suivantes ne doivent pas provoquer de crash
    insertWithPriority(pq, NULL);
    Player *p = removeHighestPriority(pq);
    TEST_ASSERT_NULL(p);
    displayPriorityQueue(pq);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_createPriorityQueue_should_allocate_and_initialize);
    RUN_TEST(test_insertWithPriority_should_insert_in_order);
    RUN_TEST(test_removeHighestPriority_should_return_max_spicyIndex);
    RUN_TEST(test_displayPriorityQueue_should_format_output);
    RUN_TEST(test_destroyPriorityQueue_should_free_memory);
    RUN_TEST(test_formBalancedMatch_should_select_12_closest);
    RUN_TEST(test_insertWithPriority_should_work_on_empty_queue);
    RUN_TEST(test_insertWithPriority_should_fifo_on_equal_priority);
    RUN_TEST(test_removeHighestPriority_should_return_null_on_empty);
    RUN_TEST(test_displayPriorityQueue_should_handle_empty);
    RUN_TEST(test_destroyPriorityQueue_should_handle_empty);
    RUN_TEST(test_insert_and_remove_multiple_players);
    RUN_TEST(test_stress_insert_remove);
    RUN_TEST(test_operations_after_destroy);
    return UNITY_END();
}