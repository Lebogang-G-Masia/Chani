#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include <stdio.h>

int main() {
    init_leaper_attacks();
    u64 attack_mask = mask_bishop_attacks(b4);

    for (int index = 0; index < 100; index++) {
        print_bitboard(set_occupancy(index, count_bits(attack_mask), attack_mask));
        getchar();
    }

    return 0;
}
