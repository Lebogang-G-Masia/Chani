#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include "../include/board.h"
#include "../include/generator.h"

#include <string.h>
#include <stdio.h>

int main() {

    init();

    parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/Pp2P3/2N2Q1p/1PPBBPPP/R3K2R b KQkq a3 0 1");
    //print_board();

    moves move_list[1];

    generate_moves(move_list);

    for (int move_count = 0; move_count < move_list->count; move_count++) {
        int move = move_list->moves[move_count];

        COPY_BOARD();

        make_move(move, ALL_MOVES);
       
        print_board();
        if (GET_MOVE_CAPTURE(move)) {
            printf("Move: %cx%s",
                    ascii_pieces[GET_MOVE_PIECE(move)],
                    square_to_coordinates[GET_MOVE_TARGET(move)]);
        } else {
            printf("Move: %c%s",
                    ascii_pieces[GET_MOVE_PIECE(move)],
                    square_to_coordinates[GET_MOVE_TARGET(move)]);
        }
 
        getchar();
        RESTORE_BOARD();
        print_board();
        getchar();
    }

    return 0;
}
