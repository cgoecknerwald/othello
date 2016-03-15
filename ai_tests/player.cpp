#include <stdio.h>
#include <random>
#include <algorithm>
#include <ctime> 
#include <memory>
#include "player.h"
#include "common.h"
#include "board_custom.h"

#define HEURISTIC false
#define NUM_SIMS 1000
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) : rng(rd()) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    srand(time(NULL));
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
        unique_ptr<vector<Move *> > moves = this->board.get_valid_moves(this->side);
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
                Board *copy_ = copy->copy();
                double w = this->simulate_rand(copy_);
                delete copy_;
                wins += w;
            }
            //fprintf(stderr, "sum:%f\n", wins);
            if (wins > best_wins) {
                best_wins = wins;
                best_move = m;
            }
            else if (wins == best_wins) {
                int bs = Board::score_board[best_move->x + best_move->y * 8];
                int ms = Board::score_board[m->x + m->y * 8];
                if(ms > bs) {
                    best_wins = wins;
                    best_move = m;
                }
            }
            if(best_move != *it) {
                delete *it;
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
    
    unique_ptr<vector<Move *> > moves = b->get_valid_moves(p1);
    double bonus = this->calc_bonus(b, p1, moves, NULL);

    while (moves->size()) {

        shuffle(moves->begin(), moves->end(), this->rng);

        Move *m = this->pick_move(b, p1, moves);
        //bonus += this->calc_bonus(b, p1, NULL, m);

        b->do_move(m, p1);
        delete m;
        //swaps sides
        temp = p1;
        p1 = p2;
        p2 = temp;

        moves = b->get_valid_moves(p1);

        if (moves->size() == 0) {
            temp = p1;
            p1 = p2;
            p2 = temp;

            moves = b->get_valid_moves(p1);
            bonus += this->calc_bonus(b, p1, moves, NULL);
        }
    }
    return 10 * ((b->count(this->side) - b->count(other)) > 0) + bonus;
}

/*
 * Create a random integer from "from" to "to".
 * Uses a uniform_int_distribution.
 */
int Player::rand_int(int from, int to) {
    uniform_int_distribution<int> uni(from, to-1);
    return uni(this->rng);
}

/*
 * Calculates the bonus (detailed in AI.txt). This bonus improves AI function-
 * ality by maxmizing the impact of "good" states, and punishing for "bad". 
 */
double Player::calc_bonus(Board *b, Side s, unique_ptr<vector<Move *> > &moves, Move *m) {

    double bonus = 0.0;
    int c = (this->side == s) ? 1 : -1;
    if (m != NULL) {
        int x = m->x;
        int y = m->y;
        if ((x == 0 && y == 0)  ||
            (x == 7 && y == 0)  ||
            (x == 0 && y == 7)  ||
            (x == 7 && y == 7) ){
            bonus = 1;
            return bonus*c;
        }
    }
    if (moves == NULL) {
        return 0;
    }

    //mobility bonus
    bonus += moves->size() * 0.01; 

    //score bonus
    //bonus += (b->count(s) - b->count(s == BLACK ? WHITE : BLACK)) * 0.02;

    /* 
     * Iterate through each of the corners, (0,0), (0,7), (7,0), and (7,7). 
     */
    // Either 0 or 7. 
    for (int x = 0; x <= 7; x += 7) {
        // Either 0 or 7.
        for (int y = 0; y <= 7; y += 7) {

            int corner_int = b->get(x, y);
            // If it's empty, continue.
            if (!corner_int) {
                continue;
            }
            // Corner bonus: we want to have corners, and keep them from the enemy.
            Side corner = b->get(x, y) == 1 ? BLACK : WHITE;
            if (s) {
                bonus += (s == corner) ? 5 : -5;
            }
        }
    }
    return bonus * c;
}

/*
 * This is the simulation that runs using the Roxanne chart (as opposed to Monte
 * Carlo random sampling). 
 * 
 * In our simulation, we pick for our AI and enemy AI.
 */
Move *Player::pick_move(Board *b, Side s, unique_ptr<vector<Move *> > &moves) {

    shuffle(moves->begin(), moves->end(), this->rng);
    vector<Move *>::iterator it; 
    int best_score = -10;
    Move *best = NULL;

    //return *(moves->begin());

    for (it = moves->begin(); it != moves->end(); it++) {

        Move *m = *it;
        int b = Board::score_board[m->x + m->y * 8];
        if (b > best_score) {
            best_score = b;
            best = m;
        }
        else {
            delete *it;
        }
    }
    return best;
    
    /* 
     * Move "m"-n indicates the n-th priority square based on the Roxanne chart.
     * There's no move m1 because we will return immediately upon finding it. 
     */    
    Move *m2 = NULL;
    Move *m3 = NULL;
    Move *m4 = NULL;
    Move *m5 = NULL;

    //vector<Move *>::iterator it; 
    for (it = moves->begin(); it != moves->end(); it++) {



        Move *m = *it;
        int x = m->x;
        int y = m->y;
        if ((x == 0 && y == 0)  ||
            (x == 7 && y == 0)  ||
            (x == 0 && y == 7)  ||
            (x == 7 && y == 7) ){
            m->type = 1;
            return m;
        }
        else if ((2 <= x && x <= 5) && (2 <= y && y <= 5)) {
            m->type = 2;
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
            m->type = 5;
            m5 = m;
        }
        else if(x == 0 || x == 7 || y == 0 || y == 7) {
            m->type = 3;
            m3 = m;
        }
        else {
            m->type = 4;
            m4 = m;
        }
    }

    // Select the best move based on Roxanne Priority Chart. (For the random 
    // sampling simulation - a modification of Monte Carlo.)
    if (m2) {
        return m2;
    }
    else if (m3) {
        return m3;
    }
    else if (m4) {
        return m4;
    }
    return m5;
}


