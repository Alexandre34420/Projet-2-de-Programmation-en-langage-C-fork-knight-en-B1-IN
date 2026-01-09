#ifndef SIMULATION_H
#define SIMULATION_H

/*
Simulation module: orchestrates player loading, queuing, matchmaking, and saving.

Overview
--------
This module simulates a game environment with two matchmaking strategies:
- FIFO queue: launch a match as soon as 12 players are queued.
- Priority queue: launch a match when the top 12 players' spicyIndex spread
  (head spicyIndex - 12th spicyIndex) is <= 500. If the condition cannot be met
  after a configurable number of wait cycles (while adding more players), it launches anyway.

Key behaviors
-------------
1) Load players from a file into a dynamic array (loadPlayers).
2) Randomly enqueue players (not already enqueued) into one of the queues
   (FIFO or Priority), balancing until both can launch matches.
3) When a queue reaches 12 players, launch a match:
   - FIFO: dequeue() players
   - Priority: removeHighestPriority() players
   Matches use simulateMatch() (random results), updatePlayerStats(), and display results.
4) Run for a configurable number of rounds.
5) Save resulting player states to an output file.

Notes
-----
- Players have a single inQueue flag in Player; we never add the same player to both queues
  simultaneously. This ensures consistency with existing enqueue/remove functions.
- Priority queue is a typedef of t_queue; we still remove with removeHighestPriority()
  to respect ordering.
*/

#include "player.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SimulationConfig {
    const char* inputPath;         // e.g., "input/players.txt"
    const char* outputPath;        // e.g., "output/players_saved.txt"
    int rounds;                    // number of full cycles to run
    unsigned int seed;             // RNG seed (0 => use time(NULL))
    int maxWaitCyclesPriority;     // max cycles to wait for spread<=500 before forcing a match
} SimulationConfig;

// Run the full simulation based on the config; prints progress to stdout.
void runSimulation(const SimulationConfig* cfg);

#ifdef __cplusplus
}
#endif

#endif // SIMULATION_H