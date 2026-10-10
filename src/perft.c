#include "../include/perft.h"
#include <math.h>

void perft_test(int depth) {

    printf("\n    Performance Test\n\n");

    moves move_list[1];
    generate_moves(move_list);

    int start = get_time_ms();

    for (int move_count = 0; move_count < move_list->count; move_count++) {
        COPY_BOARD();

        int move = move_list->moves[move_count];
        if (!make_move(move, ALL_MOVES)) continue;

        long cummulative_nodes = nodes;

        perft_driver(depth - 1);
        
        long old_nodes = nodes - cummulative_nodes;

        RESTORE_BOARD();

        printf("    move: %s%s%c    nodes: %ld\n", square_to_coordinates[GET_MOVE_SOURCE(move)], 
            square_to_coordinates[GET_MOVE_TARGET(move)], 
            promoted_pieces[GET_MOVE_PROMOTED(move)],
            old_nodes);

    }
    int end = get_time_ms();
    float total_time = (end - start) / 1000;

    printf("\n\n    Depth: %d\n", depth);
    printf("    Nodes: %ld\n", nodes);
    printf("    Time: %.2lf seconds \n\n", total_time);
}

long nodes;
