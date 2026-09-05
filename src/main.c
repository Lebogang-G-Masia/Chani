#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include "../include/board.h"
#include <stdio.h>

int main() {

    init();
    
    SET_BIT(bitboards[P], e2);
    print_bitboard(bitboards[P]);

#ifdef WIN64
    printf("piece: %c\n", ascii_pieces[P]);
    printf("piece: %c\n", ascii_pieces[char_pieces['K']]);
#else
    printf("piece: %s\n", unicode_pieces[P]);
    printf("piece: %s\n", unicode_pieces[char_pieces['K']]);
#endif

    return 0;
}
