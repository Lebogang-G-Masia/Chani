#ifndef CHANI_GENERATOR
#define CHANI_GENERATOR

#include "utils.h"
#include "attacks.h"
#include "bit_manipulation.h"
#include <stdio.h>

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
            if (piece == P) {
                while (bitboard) {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square - 8;

                    if (!(target_square < a8) && !GET_BIT(occupancies[BOTH], target_square)) {
                        // pawn promotion
                        if (source_square >= a7 && source_square <= h7) {
                            printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            // one square ahead pawn move
                             printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            // two squares ahead pawn move
                            if ((source_square >= a2 && source_square <= h2) && !GET_BIT(occupancies[BOTH], target_square - 8))
                                printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);
                            
                        }
                    }

                    POP_BIT(bitboard, source_square);
                }
            }
        } else {
            if (piece == p) {
                while (bitboard) {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square + 8;

                    if (!(target_square > h1) && !GET_BIT(occupancies[BOTH], target_square)) {
                        // pawn promotion
                        if (source_square >= a2 && source_square <= h2) {
                            printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            // one square ahead pawn move
                             printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            // two squares ahead pawn move
                            if ((source_square >= a7 && source_square <= h7) && !GET_BIT(occupancies[BOTH], target_square + 8))
                                printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + 8]);
                            
                        }
                    }

                    POP_BIT(bitboard, source_square);
                }
            }
        }
    }
}

#endif // CHANI_GENERATOR
