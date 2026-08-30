#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include "../include/prng.h"
#include <stdio.h>

int main() {
    init_leaper_attacks();
    
    print_bitboard((u64)(get_random_u32_number()));
    print_bitboard((u64)(get_random_u32_number() & 0xFFFF));
    print_bitboard(get_random_u64_numbers());
    print_bitboard(generate_magic_number());

    return 0;
}
