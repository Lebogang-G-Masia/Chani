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

    parse_fen(tricky_position);
    print_board();

    int start = get_time_ms();

    //perft_test(5);
    //getchar();

    print_move(ENCODE_MOVE(e2, e4, P, 0, 0, 0, 0, 0));
    print_move(ENCODE_MOVE(e7, e7, P, Q, 0, 0, 0, 0));
    return 0;
}
