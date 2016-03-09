#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
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
    void setBoard(char data[64]);
private:
	Move *simpleAIMove();
	Move *minimaxMove();
	int minimax(Board* b, int current_depth, int max_depth);
	int score_board(Board *b);
	int minimax_score_board(Board *b);

};

#endif
