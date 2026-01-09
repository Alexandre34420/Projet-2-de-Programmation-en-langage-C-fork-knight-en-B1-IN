#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/player.h"
#include "../headers/array-utils.h"
#include "../headers/list-utils.h"
#include "../headers/priority-queue.h"
#include "../headers/match.h"
#include "../headers/simulation.h"

// ---- Internal helpers -------------------------------------------------------

static int queueSize(t_queue* q) {
    int n = 0;
    for (t_cell* c = q->head; c != NULL; c = c->next) n++;
    return n;
}

static int getNthSpicyIndex(t_queue* q, int n /*0-based*/) {
    int i = 0;
    for (t_cell* c = q->head; c != NULL; c = c->next) {
        if (i == n) return c->value->spicyIndex;
        i++;
    }
    return -1; // not found
}

static void launchMatchFromQueue(t_queue* q) {
    // Launch a match by taking up to MATCH_MAX_PLAYERS via dequeue (FIFO)
    Match* match = createMatch();
    if (!match) return;
    while (!isQueueEmpty(q) && match->numPlayers < MATCH_MAX_PLAYERS) {
        Player* p = dequeue(q);
        if (p) {
            match->players[match->numPlayers++] = p;
        }
    }
    displayMatchInfo(match);
    simulateMatch(match);
    updatePlayerStats(match);
    displayMatchResult(match);
    destroyMatch(match);
}

static void launchMatchFromPriority(t_priority_queue* pq) {
    // Launch a match by taking up to MATCH_MAX_PLAYERS via removeHighestPriority
    Match* match = createMatch();
    if (!match) return;
    while (!isQueueEmpty(pq) && match->numPlayers < MATCH_MAX_PLAYERS) {
        Player* p = removeHighestPriority(pq);
        if (p) {
            match->players[match->numPlayers++] = p;
        }
    }
    displayMatchInfo(match);
    simulateMatch(match);
    updatePlayerStats(match);
    displayMatchResult(match);
    destroyMatch(match);
}

static int pickRandomNotEnqueuedIndex(Player** players, int count) {
    // Try up to 'count' attempts to find a player with inQueue==0
    if (count <= 0) return -1;
    for (int tries = 0; tries < count; tries++) {
        int idx = rand() % count;
        if (players[idx] != NULL && players[idx]->inQueue == 0) return idx;
    }
    // fall back to linear scan
    for (int i = 0; i < count; i++) {
        if (players[i] != NULL && players[i]->inQueue == 0) return i;
    }
    return -1; // none available
}

// ---- Public API -------------------------------------------------------------

void runSimulation(const SimulationConfig* cfg) {
    if (cfg == NULL || cfg->inputPath == NULL || cfg->outputPath == NULL || cfg->rounds <= 0) {
        fprintf(stderr, "Invalid SimulationConfig.\n");
        return;
    }

    if (cfg->seed != 0) srand(cfg->seed); else srand((unsigned int)time(NULL));

    int numPlayers = 0;
    Player** players = loadPlayers(cfg->inputPath, &numPlayers);
    if (!players) {
        fprintf(stderr, "Failed to load players from %s\n", cfg->inputPath);
        return;
    }

    t_queue* fifoQ = createQueue();
    t_priority_queue* prioQ = createQueue();
    if (!fifoQ || !prioQ) {
        fprintf(stderr, "Failed to create queues.\n");
        if (fifoQ) destroyQueue(fifoQ);
        if (prioQ) destroyQueue(prioQ);
        destroyPlayerArray(players, numPlayers);
        return;
    }

    const int maxWait = (cfg->maxWaitCyclesPriority > 0) ? cfg->maxWaitCyclesPriority : 10;

    for (int round = 1; round <= cfg->rounds; round++) {
        printf("========== Round %d ==========%s\n", round, "");
        int fifoLaunched = 0;
        int prioLaunched = 0;
        int waitCycles = 0;

        // Keep enqueuing until both matches are launched in this round
        while (!(fifoLaunched && prioLaunched)) {
            int fsz = queueSize(fifoQ);
            int psz = queueSize(prioQ);

            // Enqueue one random available player to the queue that needs it most
            int target = -1; // 0 => fifo, 1 => prio
            if (!fifoLaunched && fsz < MATCH_MAX_PLAYERS) target = 0;
            else if (!prioLaunched && psz < MATCH_MAX_PLAYERS) target = 1;
            else if (!fifoLaunched) target = 0;
            else if (!prioLaunched) target = 1;

            if (target != -1) {
                int idx = pickRandomNotEnqueuedIndex(players, numPlayers);
                if (idx != -1) {
                    if (target == 0) {
                        enqueue(fifoQ, players[idx]);
                    } else {
                        insertWithPriority(prioQ, players[idx]);
                    }
                } else {
                    // No more players available to enqueue
                }
            }

            // Check launch conditions
            if (!fifoLaunched && queueSize(fifoQ) >= MATCH_MAX_PLAYERS) {
                printf("Launching FIFO match...\n");
                launchMatchFromQueue(fifoQ);
                fifoLaunched = 1;
            }

            if (!prioLaunched && queueSize(prioQ) >= MATCH_MAX_PLAYERS) {
                int headSpicy = getNthSpicyIndex(prioQ, 0);
                int twelfthSpicy = getNthSpicyIndex(prioQ, MATCH_MAX_PLAYERS - 1);
                int spreadOK = (headSpicy >= 0 && twelfthSpicy >= 0 && (headSpicy - twelfthSpicy) <= 500);
                if (spreadOK || waitCycles >= maxWait) {
                    if (!spreadOK) {
                        printf("Priority queue spread>500 after %d waits; forcing launch.\n", waitCycles);
                    } else {
                        printf("Spicy spread OK (head=%d, 12th=%d). Launching priority match...\n", headSpicy, twelfthSpicy);
                    }
                    launchMatchFromPriority(prioQ);
                    prioLaunched = 1;
                } else {
                    waitCycles++;
                }
            }

            // Break safety to avoid accidental infinite loop
            if (fifoLaunched && prioLaunched) break;
        }
    }

    // Save final player states
    savePlayers(cfg->outputPath, players, numPlayers);

    // Cleanup
    destroyQueue(fifoQ);
    destroyQueue(prioQ);
    destroyPlayerArray(players, numPlayers);

    printf("Simulation complete. Saved players to %s\n", cfg->outputPath);
}