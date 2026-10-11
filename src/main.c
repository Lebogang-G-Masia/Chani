#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include "../include/board.h"
#include "../include/generator.h"
#include "../include/perft.h"
#include "../include/uci.h"

#include <string.h>
#include <stdio.h>

int main() {

    init();

    parse_fen("r3k2r/p11pqpb1/bn2pnp1/2pPN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    print_board();
    getchar();

    int move = parse_move("d5c6");

    if (move) {
        make_move(move, ALL_MOVES);
        print_board();
        print_move(move);
        getchar();
    }
    else
        printf("Illegal move!\n");


    return 0;
}
