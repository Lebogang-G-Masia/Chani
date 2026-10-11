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
    
    parse_position("position startpos moves e2e4 e7e6");
    print_board();

    parse_go("go depth 6");

    return 0;
}
