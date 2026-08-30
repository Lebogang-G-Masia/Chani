#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/io.h"
#include "../include/attacks.h"
#include <stdio.h>

int main() {
    init_leaper_attacks();

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            printf(" %d, ", count_bits(mask_bishop_attacks(square)));
        }
        printf("\n");
    }

    return 0;
}
