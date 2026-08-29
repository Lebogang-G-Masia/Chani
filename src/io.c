#include "../include/io.h"
#include "../include/bit_manipulation.h"
#include <stdio.h>

void print_bitboard(u64 bitboard) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;

            if (!file)
                printf("  %d  ", 8 - rank);
            printf(" %d ", (GET_BIT(bitboard, square) ? 1 : 0));
        }
        printf("\n");
    }
    printf("\n      a  b  c  d  e  f  g  h\n");
    printf("\n      Bitboard: %llu\n", bitboard);
}

