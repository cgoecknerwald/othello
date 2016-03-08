#include "player.h"
#include "common.h"

#define RANDOM_MOVES false
#define SIMPLE_MOVES true

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
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
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    Side opposide_side = this->side == WHITE ? BLACK : WHITE;
    board.doMove(opponentsMove, opposide_side);

    if(!board.hasMoves(this->side))
        return NULL;

    if (RANDOM_MOVES) {
        

        for (int x = 0; x < 8; x += 1) {
            for (int y = 0; y < 8; y += 1) {
                Move *m = new Move(x, y);
                if (board.checkMove(m, this->side)) {
                    board.doMove(m, this->side);
                    return m;
                }
            }
        }
        return NULL;
    }
    else {
        Move *m = getBestMove();
        board.doMove(m, this->side);
        return m;
    }
}

Move *Player::getBestMove() {
    Board *copy = board.copy();
    
    Move *best_move = NULL;
    int best_score = -1000000000;

    for (int x = 0; x < 8; x += 1) {
        for (int y = 0; y < 8; y += 1) {
            Move *m = new Move(x, y);
            if (copy->checkMove(m, this->side)) {
                copy->doMove(m, this->side);
                int temp = 0;
                temp = score_board(copy);
                
                if(best_score < temp) {
                    best_score = temp;
                    best_move = m;
                }
                else {
                    delete m;
                }
                delete copy;
                copy = board.copy();
            }
            else {
                delete m;
            }
        }
    }
    return best_move;
}

int Player::score_board(Board *b) {
    int score = 0;
    score += b->countBlack();
    score += 4 * b->get(BLACK, 0, 0);
    score += 4 * b->get(BLACK, 7, 0);
    score += 4 * b->get(BLACK, 0, 7);
    score += 4 * b->get(BLACK, 7, 7);

    //10 01 60 06 71 17 76 67 
    score -= 4 * b->get(BLACK, 1, 0);
    score -= 4 * b->get(BLACK, 0, 1);
    score -= 4 * b->get(BLACK, 6, 0);
    score -= 4 * b->get(BLACK, 0, 6);
    score -= 4 * b->get(BLACK, 7, 1);
    score -= 4 * b->get(BLACK, 1, 7);
    score -= 4 * b->get(BLACK, 7, 6);
    score -= 4 * b->get(BLACK, 6, 7);

    score -= b->countWhite();
    score -= 4 * b->get(WHITE, 0, 0);
    score -= 4 * b->get(WHITE, 7, 0);
    score -= 4 * b->get(WHITE, 0, 7);
    score -= 4 * b->get(WHITE, 7, 7);

    score += 4 * b->get(WHITE, 1, 0);
    score += 4 * b->get(WHITE, 0, 1);
    score += 4 * b->get(WHITE, 6, 0);
    score += 4 * b->get(WHITE, 0, 6);
    score += 4 * b->get(WHITE, 7, 1);
    score += 4 * b->get(WHITE, 1, 7);
    score += 4 * b->get(WHITE, 7, 6);
    score += 4 * b->get(WHITE, 6, 7);
    if(this->side != BLACK) {
        score *= -1;
    }
    return score;
}
