#ifndef CHANI_ATTACKS
#define CHANI_ATTACKS

#include "utils.h"
#include "magics.h"
#include "io.h"

enum {
    ROOK,
    BISHOP
};

extern const u64 not_a_file;
extern const u64 not_h_file;
extern const u64 not_ab_file;
extern const u64 not_hg_file;

extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];

extern u64 pawn_attacks[2][64];
extern u64 knight_attacks[64];
extern u64 king_attacks[64];
extern u64 bishop_masks[64];
extern u64 rook_masks[64];
extern u64 bishop_attacks[64][512];
extern u64 rook_attacks[64][4096];

u64 mask_pawn_attacks(int, int);
u64 mask_knight_attacks(int);
u64 mask_king_attacks(int);
u64 mask_bishop_attacks(int);
u64 mask_rook_attacks(int);
u64 bishop_attacks_on_the_fly(int, u64);
u64 rook_attacks_on_the_fly(int, u64);
void init_leaper_attacks();
u64 set_occupancy(int, int, u64);

static inline u64 get_bishop_attacks(int square, u64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];
    return bishop_attacks[square][occupancy];
}

static inline u64 get_rook_attacks(int square, u64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];
    return rook_attacks[square][occupancy];

}

static inline u64 get_queen_attacks(int square, u64 occupancy) {
    u64 queen_attacks = 0ULL;
    u64 bishop_occupancies = occupancy;
    u64 rook_occupancies = occupancy;

    bishop_occupancies &= bishop_masks[square];
    bishop_occupancies *= bishop_magic_numbers[square];
    bishop_occupancies  >>= 64 - bishop_relevant_bits[square];

    queen_attacks = bishop_attacks[square][bishop_occupancies];

    rook_occupancies &= rook_masks[square];
    rook_occupancies *= rook_magic_numbers[square];
    rook_occupancies >>= 64 - rook_relevant_bits[square];

    queen_attacks |= rook_attacks[square][rook_occupancies];

    return queen_attacks;
}

void init_sliders_attacks(int);


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


#endif
