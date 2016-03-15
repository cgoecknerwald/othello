#include <vector>
#include <assert.h>
#include "board_custom.h"
#include "common.h"
#include <stdio.h>
#include <memory>

using namespace std;

const int Board::score_board[] = 
				  { 5, -1, 2, 2, 2, 2, -1,  5,
                   -1, -2, 1, 1, 1, 1, -2, -1,
                    2,  1, 2, 2, 2, 2,  1,  2,
                    2,  1, 2, 2, 2, 2,  1,  2,
                    2,  1, 2, 2, 2, 2,  1,  2,
                    2,  1, 2, 2, 2, 2,  1,  2,
                   -1, -2, 1, 1, 1, 1, -2, -1,
                    5, -1, 2, 2, 2, 2, -1,  5};

Board::Board() : black(64), taken(64) {

	//fprintf(stderr, "board init\n");

	for(int i = 0; i < 64; i += 1) {
		black[i] = 0;
		taken[i] = 0;
	}

	// (4, 3)
	black[4 + 3 * 8] = 1;
	taken[4 + 3 * 8] = 1;
	
	// (3, 4)
	black[3 + 4 * 8] = 1; 
	taken[3 + 4 * 8] = 1;

	// (3, 3) & (4, 4)
	taken[3 + 3 * 8] = 1;
	taken[4 + 4 * 8] = 1;
}

Board *Board::copy() {
	Board *b = new Board();
	b->black = this->black;
	b->taken = this->taken;
	return b;
}

int Board::get(int x, int y) {
	if (black[x + y * 8]) {
		return 1;
	}
	else if (taken[x + y * 8]) {
		return -1;
	}
	else {
		return 0;
	}
}

int Board::count(Side side) {
	int black_c = 0;
	int taken_c = 0;
	for (int i = 0; i < 64; i += 1) {
		black_c += black[i];
		taken_c += taken[i];
	}

	if(side == BLACK) {
		return black_c;
	}
	else {
		return taken_c - black_c;
	}
}

bool Board::is_valid_move(const Move *m, const Side s) {
	int x = m->x;
	int y = m->y;

	// 1 if black, -1 if white
	int side_int = s == BLACK ? 1 : -1;

	if (taken[m->scalar()]) {
		return false;
	}

	for (int dx = -1; dx <= 1; dx += 1) {
		for (int dy = -1; dy <= 1; dy += 1) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			int x_ = x + dx;
			int y_ = y + dy;

			int count = 0;
			int cell = 0;

			while ((0 <= x_ && x_ < 8) && (0 <= y_ && y_ < 8)) {
				// 1 if black, 0 if empty, -1 if white
				cell = 2 * black[x_ + y_ * 8] - taken[x_ + y_ * 8];

				// if cell is the same as side or cell is empty
				if (cell == side_int || cell == 0) {
					break;
				}

				// the cell being looked at is not empty and is
				// not the same as the piece being played.
				count += 1;

				x_ += dx;
				y_ += dy;
			}

			// if we ended on a piece the same as the one being 
			// played and we counted more than 0 of the other 
			// pieces, this direction is a valid move.
			if (cell == side_int && count > 0) {
				//fprintf(stderr, "\t v-move: (%i, %i) d: (%i, %i) cell: %i cou: %i\n", x, y, dx, dy, cell, count);
				//this->print();
				return true;
			}
		}
	}

	return false;
}

void Board::print() {
	for (int i = 0; i < 64; i += 1) {
		if (i % 8 == 0) {
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "%i", black[i] + taken[i]);
	}
	fprintf(stderr, "\n");
}

bool Board::get_valid_dirs(const Move *m, const Side s) {
	int x = m->x;
	int y = m->y;

	// 1 if black, -1 if white
	int side_int = s == BLACK ? 1 : -1;

	if (taken[m->scalar()]) {
		return false;
	}

	m->dirs->clear();

	for (int dx = -1; dx <= 1; dx += 1) {
		for (int dy = -1; dy <= 1; dy += 1) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			int x_ = x + dx;
			int y_ = y + dy;

			int count = 0;
			int cell = 0;

			while ((0 <= x_ && x_ < 8) && (0 <= y_ && y_ < 8)) {
				// 1 if black, 0 if empty, -1 if white
				cell = 2 * black[x_ + y_ * 8] - taken[x_ + y_ * 8];

				// if cell is the same as side or cell is empty
				if (cell == side_int || cell == 0) {
					break;
				}

				// the cell being looked at is not empty and is
				// not the same as the piece being played.
				count += 1;

				x_ += dx;
				y_ += dy;
			}

			// if we ended on a piece the same as the one being 
			// played and we counted more than 0 of the other 
			// pieces, this direction is a valid move.
			if (cell == side_int && count > 0) {
				m->dirs->push_back(new Dir(dx, dy));
			}
		}
	}

	// true if m->dirs->size() is NOT zero
	return m->dirs->size() != 0;
}

unique_ptr<vector<Move *> > Board::get_valid_moves(const Side s) {
	unique_ptr<vector<Move *> > moves(new vector<Move *>());
	for (int x = 0; x < 8; x += 1) {
		for (int y = 0; y < 8; y += 1) {
			Move *move = new Move(x, y);
			if (this->is_valid_move(move, s)) {
				moves->push_back(move);
			}
			else {
				delete move;
			}
		}
	}
	return moves;
}

void Board::do_move(const Move *m, const Side s) {
	//fprintf(stderr, "board do_move m = (%i, %i)", m->x, m->y);

	int num_dirs = m->dirs->size();
	if (!num_dirs) {
		this->get_valid_dirs(m, s);
		num_dirs = m->dirs->size();
	}
	if (num_dirs == 0) {
		//fprintf(stderr, "for move (%i, %i):\n", m->x, m->y);
		assert(("move has no directions",  num_dirs != 0));	
	}
	

	int x = m->x;
	int y = m->y;
	assert(("invalid move; piece already here", taken[x + y * 8] == 0));
	//fprintf(stderr, "doing move (%i, %i) before:\n", m->x, m->y);
	//this->print();
	vector<Dir *> *dirs = m->dirs;
	vector<Dir *>::iterator it;

	int side_int = s == BLACK ? 1 : -1;

	taken[x + y * 8] = 1;
	black[x + y * 8] = (s == BLACK);

	for (it = dirs->begin(); it != dirs->end(); it++) {
		int dx = (*it)->dx;
		int dy = (*it)->dy;

		int x_ = x + dx;
		int y_ = y + dy;

		while ((0 <= x_ && x_ < 8) && (0 <= y_ && y_ < 8)) {
			int cell = 2 * black[x_ + y_ * 8] - taken[x_ + y_ * 8];
			// 1 if black, 0 if empty, -1 if white

			if (cell == side_int) {
				break;
			}

			black[x_ + y_ * 8] = (s == BLACK);
			x_ += dx;
			y_ += dy;
		}
	}
	//fprintf(stderr, "doing move (%i, %i) after:\n", m->x, m->y);
	//this->print();
}
