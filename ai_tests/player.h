#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <random>
#include <iostream>
#include "common.h"
#include "board_custom.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board board;
    Side side;

private:
    double simulate_rand(Board* b);
    random_device rd;
    std::mt19937 rng;

    int rand_int(int from, int to);
    double calc_bonus(Board *b, Side s, unique_ptr<vector<Move *> > &moves, Move *m);
    Move *pick_move(Board *b, Side s, unique_ptr<vector<Move *> > &moves);
};

#endif
