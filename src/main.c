#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include "../include/board.h"
#include "../include/generator.h"

#include <stdio.h>

int main() {

    init();

    parse_fen("r3k2r/ppppqpb1/bn2pn11/3PN3/Pp2P3/2N2Q1p/1PPBBPpP/R3K2R b KQkq a3 0 1 ");
    print_board();
    generate_moves();    
    return 0;

}
