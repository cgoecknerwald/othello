#include <stdio.h>
#include <random>
#include "player.h"
#include "common.h"
#include "board_custom.h"

#define HEURISTIC false
#define NUM_SIMS 500
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) : rng(rd()) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     this->side = side;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    Side other = this->side == BLACK ? WHITE : BLACK;

    if(opponentsMove != NULL) {
        this->board.do_move(opponentsMove, other);
        //fprintf(stderr, "did op move: (%i, %i)\n", opponentsMove->x, opponentsMove->y);
    }
    
    // vector<Move *> *moves = this->board.get_valid_moves(this->side);
    // Move *m = (*moves)[0];
    // //fprintf(stderr, "doing move: (%i, %i)\n", m->x, m->y);
    // this->board.do_move(m, this->side);
    // return m;

    if (HEURISTIC) {
        return NULL;
    }
    else {
        vector<Move *> *moves = this->board.get_valid_moves(this->side);
        vector<Move *>::iterator it;
        
        if (moves->size() == 0) {
            return NULL;
        }

        double best_wins = 0;
        Move *best_move = (*moves)[0];
        for (it = moves->begin(); it != moves->end(); it++) {
            Board *copy = this->board.copy();
            Move *m = *it;
            copy->do_move(m, this->side);
            double wins = 0;
            for (int i = 0; i < NUM_SIMS; i += 1) {
                wins += this->simulate_rand(copy);
            }
            if (wins > best_wins) {
                best_wins = wins;
                best_move = m;
            }
            delete copy;
        }
        this->board.do_move(best_move, this->side);
        return best_move;
    }
}

/* 
 * simulate match starting with this board and not self as first player
 * assumes there is a valid move.
 */
double Player::simulate_rand(Board *b) {
    Side p1 = this->side == BLACK ? WHITE : BLACK;
    Side p2 = this->side;

    Side other = this->side == BLACK ? WHITE : BLACK;

    Side temp;
    
    vector<Move *> *moves = b->get_valid_moves(p1);
    double bonus = this->calc_bonus(b, p1, moves);

    while (moves->size()) {
        Move *m = this->pick_move(b, p1, moves);

        b->do_move(m, p1);
        
        //swaps sides
        temp = p1;
        p1 = p2;
        p2 = temp;

        delete moves;
        moves = b->get_valid_moves(p1);
        bonus += this->calc_bonus(b, p1, moves);

        if (moves->size() == 0) {

            temp = p1;
            p1 = p2;
            p2 = temp;

            delete moves;
            moves = b->get_valid_moves(p1);
            bonus += this->calc_bonus(b, p1, moves);
        }
    }
    return ((b->count(this->side) - b->count(other)) > 0) + bonus;
}

int Player::rand_int(int from, int to) {
    uniform_int_distribution<int> uni(from, to-1);
    return uni(this->rng);
}

double Player::calc_bonus(Board *b, Side s, vector<Move *> *moves) {
    return 0.0;
}

Move *Player::pick_move(Board *b, Side s, vector<Move *> *moves) {
    
    Move *m2 = NULL;
    Move *m3 = NULL;
    Move *m4 = NULL;
    Move *m5 = NULL;

    for (int i = 0; i < moves->size(); i += 1) {
        Move *m = (*moves)[i];
        int x = m->x;
        int y = m->y;
        if ((x == 0 && y == 0)  ||
            (x == 7 && y == 0)  ||
            (x == 0 && y == 7)  ||
            (x == 7 && y == 7) ){
            return m;
        }
        else if ((2 <= x && x <= 5) && (2 <= y && y <= 5)) {
            m2 = m;
        }
        else if ((x == 0 && y == 1) ||
                 (x == 1 && y == 0) ||
                 (x == 1 && y == 1) ||

                 (x == 0 && y == 6) ||
                 (x == 1 && y == 6) ||
                 (x == 1 && y == 7) ||

                 (x == 6 && y == 6) ||
                 (x == 6 && y == 7) ||
                 (x == 7 && y == 6) ||

                 (x == 6 && y == 0) ||
                 (x == 6 && y == 1) ||
                 (x == 7 && y == 1)) {
            m5 = m;
        }
        else if(x == 0 || x == 7 || y == 0 || y == 7) {
            m3 = m;
        }
        else {
            m4 = m;
        }
    }

    //return (*moves)[rand_int(0, moves->size())];

    if(m2) {
        return m2;
    }
    else if(m3) {
        return m3;
    }
    else if(m4) {
        return m4;
    }
    return m5;
}
