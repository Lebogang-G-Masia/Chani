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
    
    parse_position("position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves c3b5");
    print_board();

    return 0;
}
