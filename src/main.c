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

    moves move_list[1];

    move_list->count = 0;

    int move = ENCODE_MOVE(e7, e8, p, n, 1, 1, 1, 1);

    add_move(move_list, move);

    print_move_list(move_list);
   
    return 0;
}
