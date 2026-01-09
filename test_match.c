/**
 * Tests unitaires - Partie 5
 * Module: match.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../unity/unity.h"
#include "../../headers/player.h"
#include "../../headers/list-utils.h"
#include "../../headers/match.h"

void setUp(void) {
    // Appelé avant chaque test
}

void tearDown(void) {
    // Appelé après chaque test
}

// ========================================
// Tests pour createMatch()
// ========================================

void test_createMatch_should_allocate_and_initialize(void) {
    Match* match = createMatch();
    TEST_ASSERT_NOT_NULL(match);
    TEST_ASSERT_NOT_NULL(match->players);
    TEST_ASSERT_NULL(match->results);
    TEST_ASSERT_EQUAL_INT(0, match->numPlayers);
    destroyMatch(match);
}

void test_createMatch_should_allocate_players_array(void) {
    Match* match = createMatch();
    TEST_ASSERT_NOT_NULL(match);
    if (match != NULL) {
        TEST_ASSERT_NOT_NULL(match->players);
        destroyMatch(match);
    }
}

// ========================================
// Tests pour destroyMatch()
// ========================================

void test_destroyMatch_should_free_memory(void) {
    Match* match = createMatch();
    TEST_ASSERT_NOT_NULL(match);
    destroyMatch(match);
    // Si on arrive ici sans crash, c'est OK
    TEST_ASSERT_TRUE(1);
}

void test_destroyMatch_should_handle_null(void) {
    // Ne doit pas planter si on passe NULL
    destroyMatch(NULL);
    TEST_ASSERT_TRUE(1); // Si on arrive ici, pas de crash
}

// ========================================
// Tests pour enqueue et integration queue
// ========================================

void test_addMatchPlayers_from_queue(void) {
    Match* match = createMatch();
    t_queue* queue = createQueue();

    // Create and add a player to the queue
    Player* player1 = createPlayer("Alice");
    TEST_ASSERT_NOT_NULL(player1);

    enqueue(queue, player1);

    // Add player from queue to match
    if (!isQueueEmpty(queue)) {
        Player* player = dequeue(queue);
        if (player != NULL && match->numPlayers < MATCH_MAX_PLAYERS) {
            match->players[match->numPlayers] = player;
            match->numPlayers++;
        }
    }

    TEST_ASSERT_EQUAL_INT(1, match->numPlayers);
    if (match->numPlayers > 0) {
        TEST_ASSERT_EQUAL_STRING("Alice", match->players[0]->username);
        destroyPlayer(match->players[0]);
    }

    destroyMatch(match);
    destroyQueue(queue);
}

void test_addMatchPlayers_multiple_players(void) {
    Match* match = createMatch();
    t_queue* queue = createQueue();

    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    enqueue(queue, p1);
    enqueue(queue, p2);
    enqueue(queue, p3);

    // Add all players from queue to match
    while (!isQueueEmpty(queue) && match->numPlayers < MATCH_MAX_PLAYERS) {
        Player* player = dequeue(queue);
        if (player != NULL) {
            match->players[match->numPlayers] = player;
            match->numPlayers++;
        }
    }

    TEST_ASSERT_EQUAL_INT(3, match->numPlayers);

    // Free all players in the match
    for (int i = 0; i < match->numPlayers; ++i) {
        destroyPlayer(match->players[i]);
    }

    destroyMatch(match);
    destroyQueue(queue);
}

// ========================================
// Tests pour simulateMatch()
// ========================================

void test_simulateMatch_should_allocate_results(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    match->players[0] = p1;
    match->players[1] = p2;
    match->players[2] = p3;
    match->numPlayers = 3;

    simulateMatch(match);

    TEST_ASSERT_NOT_NULL(match->results);

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_simulateMatch_should_create_valid_indices(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    Player* p3 = createPlayer("Player3");

    match->players[0] = p1;
    match->players[1] = p2;
    match->players[2] = p3;
    match->numPlayers = 3;

    simulateMatch(match);

    if (match->results != NULL) {
        // Check that results contain valid indices
        for (int i = 0; i < match->numPlayers; i++) {
            TEST_ASSERT_TRUE(match->results[i] >= 0 && match->results[i] < match->numPlayers);
        }
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_simulateMatch_with_two_players(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Winner");
    Player* p2 = createPlayer("Loser");

    match->players[0] = p1;
    match->players[1] = p2;
    match->numPlayers = 2;

    simulateMatch(match);

    TEST_ASSERT_NOT_NULL(match->results);
    if (match->results != NULL) {
        // Results should contain indices 0 and 1 in some order
        int contains_0 = 0, contains_1 = 0;
        for (int i = 0; i < 2; i++) {
            if (match->results[i] == 0) contains_0 = 1;
            if (match->results[i] == 1) contains_1 = 1;
        }
        TEST_ASSERT_TRUE(contains_0 && contains_1);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

// ========================================
// Tests pour updatePlayerStats()
// ========================================

void test_updatePlayerStats_should_update_games(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Winner");
    Player* p2 = createPlayer("Loser");

    p1->spicyIndex = 0;
    p2->spicyIndex = 0;
    p1->numGames = 0;
    p2->numGames = 0;
    p1->numWins = 0;
    p2->numWins = 0;
    p1->numLosses = 0;
    p2->numLosses = 0;

    match->players[0] = p1;
    match->players[1] = p2;
    match->numPlayers = 2;

    match->results = (int*)malloc(2 * sizeof(int));
    if (match->results != NULL) {
        match->results[0] = 0;  // p1 wins
        match->results[1] = 1;  // p2 loses

        updatePlayerStats(match);

        TEST_ASSERT_EQUAL_INT(1, p1->numGames);
        TEST_ASSERT_EQUAL_INT(1, p2->numGames);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_updatePlayerStats_should_update_wins_losses(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Winner");
    Player* p2 = createPlayer("Loser");

    p1->spicyIndex = 0;
    p2->spicyIndex = 0;
    p1->numGames = 0;
    p2->numGames = 0;
    p1->numWins = 0;
    p2->numWins = 0;
    p1->numLosses = 0;
    p2->numLosses = 0;

    match->players[0] = p1;
    match->players[1] = p2;
    match->numPlayers = 2;

    match->results = (int*)malloc(2 * sizeof(int));
    if (match->results != NULL) {
        match->results[0] = 0;  // p1 wins
        match->results[1] = 1;  // p2 loses

        updatePlayerStats(match);

        TEST_ASSERT_EQUAL_INT(1, p1->numWins);
        TEST_ASSERT_EQUAL_INT(0, p1->numLosses);
        TEST_ASSERT_EQUAL_INT(0, p2->numWins);
        TEST_ASSERT_EQUAL_INT(1, p2->numLosses);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_updatePlayerStats_should_update_spicyIndex(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Winner");
    Player* p2 = createPlayer("Loser");

    p1->spicyIndex = 0;
    p2->spicyIndex = 0;
    p1->numGames = 0;
    p2->numGames = 0;
    p1->numWins = 0;
    p2->numWins = 0;
    p1->numLosses = 0;
    p2->numLosses = 0;

    match->players[0] = p1;
    match->players[1] = p2;
    match->numPlayers = 2;

    match->results = (int*)malloc(2 * sizeof(int));
    if (match->results != NULL) {
        match->results[0] = 0;  // p1 wins
        match->results[1] = 1;  // p2 loses

        updatePlayerStats(match);

        // Winner should get +100, loser should get +0
        TEST_ASSERT_EQUAL_INT(100, p1->spicyIndex);
        TEST_ASSERT_EQUAL_INT(0, p2->spicyIndex);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_updatePlayerStats_scaling_with_three_players(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("First");
    Player* p2 = createPlayer("Second");
    Player* p3 = createPlayer("Third");

    p1->spicyIndex = 0;
    p2->spicyIndex = 0;
    p3->spicyIndex = 0;
    p1->numGames = 0;
    p2->numGames = 0;
    p3->numGames = 0;
    p1->numWins = 0;
    p2->numWins = 0;
    p3->numWins = 0;
    p1->numLosses = 0;
    p2->numLosses = 0;
    p3->numLosses = 0;

    match->players[0] = p1;
    match->players[1] = p2;
    match->players[2] = p3;
    match->numPlayers = 3;

    match->results = (int*)malloc(3 * sizeof(int));
    if (match->results != NULL) {
        match->results[0] = 0;  // p1 first (100)
        match->results[1] = 1;  // p2 second (50)
        match->results[2] = 2;  // p3 third (0)

        updatePlayerStats(match);

        TEST_ASSERT_EQUAL_INT(100, p1->spicyIndex);
        TEST_ASSERT_EQUAL_INT(50, p2->spicyIndex);
        TEST_ASSERT_EQUAL_INT(0, p3->spicyIndex);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_updatePlayerStats_scaling_with_five_players(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("P1");
    Player* p2 = createPlayer("P2");
    Player* p3 = createPlayer("P3");
    Player* p4 = createPlayer("P4");
    Player* p5 = createPlayer("P5");

    p1->spicyIndex = 0; p2->spicyIndex = 0; p3->spicyIndex = 0; p4->spicyIndex = 0; p5->spicyIndex = 0;
    p1->numGames = 0;   p2->numGames = 0;   p3->numGames = 0;   p4->numGames = 0;   p5->numGames = 0;
    p1->numWins = 0;    p2->numWins = 0;    p3->numWins = 0;    p4->numWins = 0;    p5->numWins = 0;
    p1->numLosses = 0;  p2->numLosses = 0;  p3->numLosses = 0;  p4->numLosses = 0;  p5->numLosses = 0;

    match->players[0] = p1;
    match->players[1] = p2;
    match->players[2] = p3;
    match->players[3] = p4;
    match->players[4] = p5;
    match->numPlayers = 5;

    match->results = (int*)malloc(5 * sizeof(int));
    if (match->results != NULL) {
        match->results[0] = 0;  // p1 first (100)
        match->results[1] = 1;  // p2 second (75)
        match->results[2] = 2;  // p3 third (50)
        match->results[3] = 3;  // p4 fourth (25)
        match->results[4] = 4;  // p5 fifth (0)

        updatePlayerStats(match);

        TEST_ASSERT_EQUAL_INT(100, p1->spicyIndex);
        TEST_ASSERT_EQUAL_INT(75, p2->spicyIndex);
        TEST_ASSERT_EQUAL_INT(50, p3->spicyIndex);
        TEST_ASSERT_EQUAL_INT(25, p4->spicyIndex);
        TEST_ASSERT_EQUAL_INT(0, p5->spicyIndex);
    }

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
    destroyPlayer(p4);
    destroyPlayer(p5);
}

// ========================================
// Tests pour displayMatchInfo()
// ========================================

void test_displayMatchInfo_should_handle_single_player(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("TestPlayer");
    match->players[0] = p1;
    match->numPlayers = 1;

    // This function only prints, so we just check it doesn't crash
    displayMatchInfo(match);
    TEST_ASSERT_TRUE(1);

    destroyMatch(match);
    destroyPlayer(p1);
}

void test_displayMatchInfo_should_handle_multiple_players(void) {
    Match* match = createMatch();

    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");
    match->players[0] = p1;
    match->players[1] = p2;
    match->numPlayers = 2;

    // This function only prints, so we just check it doesn't crash
    displayMatchInfo(match);
    TEST_ASSERT_TRUE(1);

    destroyMatch(match);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

// ========================================
// Tests pour launchBalancedMatch()
// ========================================

void test_launchBalancedMatch_with_priority_queue(void) {
    // Create a priority queue with players sorted by spicyIndex
    t_priority_queue* pqueue = createPriorityQueue();

    Player* p1 = createPlayer("High");
    Player* p2 = createPlayer("Mid");
    Player* p3 = createPlayer("Low");

    p1->spicyIndex = 1500;
    p2->spicyIndex = 1000;
    p3->spicyIndex = 500;

    // Insert into priority queue (highest spicyIndex first)
    insertWithPriority(pqueue, p1);
    insertWithPriority(pqueue, p2);
    insertWithPriority(pqueue, p3);

    // launchBalancedMatch will dequeue and use the priority order
    launchBalancedMatch(pqueue);

    // Queue should be empty after match
    TEST_ASSERT_TRUE(isQueueEmpty(pqueue));

    destroyPriorityQueue(pqueue);
    destroyPlayer(p1);
    destroyPlayer(p2);
    destroyPlayer(p3);
}

void test_launchBalancedMatch_fills_up_to_max_players(void) {
    // Create a priority queue with more than MATCH_MAX_PLAYERS
    t_priority_queue* pqueue = createPriorityQueue();

    Player* players[15];
    for (int i = 0; i < 15; i++) {
        char name[20];
        snprintf(name, sizeof(name), "Player%d", i);
        players[i] = createPlayer(name);
        players[i]->spicyIndex = 1500 - (i * 50);
        insertWithPriority(pqueue, players[i]);
    }

    // launchBalancedMatch will take only up to MATCH_MAX_PLAYERS
    launchBalancedMatch(pqueue);

    // Queue should still have remaining players (15 - 12 = 3)
    int remaining = 0;
    while (!isQueueEmpty(pqueue)) {
        Player* p = removeHighestPriority(pqueue);
        if (p != NULL) {
            remaining++;
        }
    }

    TEST_ASSERT_EQUAL_INT(3, remaining);

    // Clean up
    destroyPriorityQueue(pqueue);
    for (int i = 0; i < 15; i++) {
        destroyPlayer(players[i]);
    }
}

void test_launchBalancedMatch_with_small_queue(void) {
    // Test with only 2 players
    t_priority_queue* pqueue = createPriorityQueue();

    Player* p1 = createPlayer("Player1");
    Player* p2 = createPlayer("Player2");

    p1->spicyIndex = 1200;
    p2->spicyIndex = 800;

    insertWithPriority(pqueue, p1);
    insertWithPriority(pqueue, p2);

    // launchBalancedMatch will work with 2 players
    launchBalancedMatch(pqueue);

    TEST_ASSERT_TRUE(isQueueEmpty(pqueue));

    destroyPriorityQueue(pqueue);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_launchBalancedMatch_updates_player_stats(void) {
    // Create priority queue with players
    t_priority_queue* pqueue = createPriorityQueue();

    Player* p1 = createPlayer("Strong");
    Player* p2 = createPlayer("Weak");

    p1->spicyIndex = 1000;
    p2->spicyIndex = 500;
    p1->numGames = 0;
    p2->numGames = 0;
    p1->numWins = 0;
    p2->numWins = 0;
    p1->numLosses = 0;
    p2->numLosses = 0;

    insertWithPriority(pqueue, p1);
    insertWithPriority(pqueue, p2);

    // launchBalancedMatch will simulate and update stats
    launchBalancedMatch(pqueue);

    // Both players should have played a game
    TEST_ASSERT_EQUAL_INT(1, p1->numGames);
    TEST_ASSERT_EQUAL_INT(1, p2->numGames);

    // Exactly one should have won (results[0]) and one should have lost
    int total_wins = p1->numWins + p2->numWins;
    int total_losses = p1->numLosses + p2->numLosses;
    TEST_ASSERT_EQUAL_INT(1, total_wins);
    TEST_ASSERT_EQUAL_INT(1, total_losses);

    destroyPriorityQueue(pqueue);
    destroyPlayer(p1);
    destroyPlayer(p2);
}

void test_launchBalancedMatch_does_not_crash_on_empty_queue(void) {
    // Create an empty priority queue
    t_priority_queue* pqueue = createPriorityQueue();

    // launchBalancedMatch should handle empty queue gracefully
    launchBalancedMatch(pqueue);

    TEST_ASSERT_TRUE(isQueueEmpty(pqueue));

    destroyPriorityQueue(pqueue);
}

// ========================================
// MAIN - Test Runner
// ========================================

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_createMatch_should_allocate_and_initialize);
    RUN_TEST(test_createMatch_should_allocate_players_array);
    RUN_TEST(test_destroyMatch_should_free_memory);
    RUN_TEST(test_destroyMatch_should_handle_null);
    RUN_TEST(test_addMatchPlayers_from_queue);
    RUN_TEST(test_addMatchPlayers_multiple_players);
    RUN_TEST(test_simulateMatch_should_allocate_results);
    RUN_TEST(test_simulateMatch_should_create_valid_indices);
    RUN_TEST(test_simulateMatch_with_two_players);
    RUN_TEST(test_updatePlayerStats_should_update_games);
    RUN_TEST(test_updatePlayerStats_should_update_wins_losses);
    RUN_TEST(test_updatePlayerStats_should_update_spicyIndex);
    RUN_TEST(test_updatePlayerStats_scaling_with_three_players);
    RUN_TEST(test_updatePlayerStats_scaling_with_five_players);
    RUN_TEST(test_displayMatchInfo_should_handle_single_player);
    RUN_TEST(test_displayMatchInfo_should_handle_multiple_players);
    RUN_TEST(test_launchBalancedMatch_with_priority_queue);
    RUN_TEST(test_launchBalancedMatch_fills_up_to_max_players);
    RUN_TEST(test_launchBalancedMatch_with_small_queue);
    RUN_TEST(test_launchBalancedMatch_updates_player_stats);
    RUN_TEST(test_launchBalancedMatch_does_not_crash_on_empty_queue);
    return UNITY_END();
}