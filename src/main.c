#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include "../include/magics.h"
#include <stdio.h>

int main() {

    init();

    u64 occupancy = 0ULL;

    SET_BIT(occupancy, c5);
    SET_BIT(occupancy, f2);
    SET_BIT(occupancy, g7);
    SET_BIT(occupancy, b2);
    SET_BIT(occupancy, g5);
    SET_BIT(occupancy, e2);
    SET_BIT(occupancy, e7);

    print_bitboard(occupancy);
    print_bitboard(get_bishop_attacks(d4, occupancy));


    print_bitboard(get_rook_attacks(e5, occupancy));
     
    return 0;
}
