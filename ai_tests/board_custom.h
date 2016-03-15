#ifndef __BOARD_CUSTOM_H__
#define __BOARD_CUSTOM_H__

#include <vector>
#include "common.h"

using namespace std;

class Dir {
public:
    int dx;
    int dy;

    Dir(int dx, int dy) {
        this->dx = dx;
        this->dy = dy;
    }
};

class Move {   
public:
    int x, y, type;
    vector<Dir *> *dirs;

    Move(int x, int y) {
        this->x = x;
        this->y = y;
        type = 0;
        dirs = new vector<Dir *>();
    }

    ~Move() {
        vector<Dir *>::iterator it;
        for(it = dirs->begin(); it != dirs->end(); it++){
            delete *it;
        }
        delete dirs;
    }

    int scalar() const {
        return x + y * 8;
    }
};

class Board {

private:
    vector<bool> black;
    vector<bool> taken;
    void print();
    
public:
    Board();
    Board *copy();
    int get(int x, int y);
    int count(Side);
    bool is_valid_move(const Move *m, const Side);
    static const int score_board[64];

    /**
     * after execution, m's dir vector will contain
     * the valid directions of replacement. If it 
     * is empty, then it is an invalid move.
     *
     * clears m->dirs first
     *
     * returns true if dir has directions
     * returns false if dir is empty
     */
    bool get_valid_dirs(const Move *m, const Side);
    unique_ptr<vector<Move *> > get_valid_moves(Side);
    void do_move(const Move *m, const Side);
};

#endif
