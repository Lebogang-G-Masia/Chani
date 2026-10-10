#ifndef CHANI_PERFT
#define CHANI_PERFT

#include "generator.h"

extern long nodes;

static inline void perft_driver(int depth) {
    if (depth == 0) {
        nodes++;
        return;
    }

    moves move_list[1];
    generate_moves(move_list);

    for (int move_count = 0; move_count < move_list->count; move_count++) {
        COPY_BOARD();

        if (!make_move(move_list->moves[move_count], ALL_MOVES)) continue;

        perft_driver(depth - 1);

        RESTORE_BOARD();
    }
}

#endif // CHANI_PERFT
