#ifndef MATCH_H
#define MATCH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "array-utils.h"
#include "list-utils.h"
#include "priority-queue.h"

#define MATCH_MAX_PLAYERS 12

typedef struct Match {
    Player ** players;
    int * results;
    int numPlayers;
} Match;

Match * createMatch();
void destroyMatch(Match * match);
void simulateMatch(Match * match);
void displayMatchResult(const Match * match);
void addMatchPlayers(t_queue * queue, Match * match);
void launchMatch(t_queue * queue);
void launchBalancedMatch(t_priority_queue * queue);
void updatePlayerStats(Match * match);
void displayMatchInfo(const Match * match);

#endif // MATCH_H
