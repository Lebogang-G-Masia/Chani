#ifndef CHANI_GENERATOR
#define CHANI_GENERATOR

#include "utils.h"
#include "attacks.h"

static inline int is_square_attacked(int square, int side) {
    if ((side == WHITE) && (pawn_attacks[BLACK][square] & bitboards[P])) return 1;
    if ((side == BLACK) && (pawn_attacks[WHITE][square] & bitboards[p])) return 1;
    if (knight_attacks[square] & ((side == WHITE) ? bitboards[N] : bitboards[n])) return 1;
    if (king_attacks[square] & ((side == WHITE) ? bitboards[K] : bitboards[k])) return 1;
    if (get_bishop_attacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[B] : bitboards[b])) return 1;
    if (get_rook_attacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[R] : bitboards[r])) return 1;
    if (get_queen_attacks(square, occupancies[BOTH]) & ((side == WHITE) ? bitboards[Q] : bitboards[q])) return 1;
    return 0;
}

static inline void generate_moves() {
    int source_square;
    int target_square;

    u64 bitboard;
    u64 attacks;

    for (int piece = P; piece <= k; piece++) {
        bitboard = bitboards[piece];

        if (side == WHITE) {
            
        } else {

        }
    }
}

#endif // CHANI_GENERATOR
