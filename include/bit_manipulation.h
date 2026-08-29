#ifndef CHANI_BIT_MANIPULATION
#define CHANI_BIT_MANIPULATION

#include "utils.h"

#define SET_BIT(bitboard, square) (bitboard |= (1ULL << square))
#define GET_BIT(bitboard, square) (bitboard & (1ULL << square))
#define POP_BIT(bitboard, square) (GET_BIT(bitboard, square) ? bitboard ^= (1ULL << square) : 0)

static inline int count_bits(u64 bitboard) {
    int count = 0;

    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;
    }

    return count;
}

static inline int get_ls1b_index(u64 bitboard) {
    if (bitboard) {
        return count_bits((bitboard & -bitboard) - 1);
    }

    return -1;
}

#endif // CHANI_BIT_MANIPULATION
