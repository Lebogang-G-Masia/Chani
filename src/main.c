#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include "../include/board.h"
#include "../include/generator.h"
#include "../include/perft.h"

#include <string.h>
#include <stdio.h>

int main() {

    init();

    parse_fen(start_position);
    print_board();

    int start = get_time_ms();

    perft_driver(6);

    printf("Time taken to execute: %d ms \n", get_time_ms() - start);
    printf("Nodes: %ld\n", nodes);

    return 0;
}
