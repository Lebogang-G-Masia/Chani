#ifndef CHANI_ATTACKS
#define CHANI_ATTACKS

#include "utils.h"
#include "magics.h"

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

void init_sliders_attacks(int);


#endif
