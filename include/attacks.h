#ifndef CHANI_ATTACKS
#define CHANI_ATTACKS

#include "utils.h"

extern const u64 not_a_file;
extern const u64 not_h_file;
extern const u64 not_ab_file;
extern const u64 not_hg_file;

extern const int bishop_relevant_bits[64];
extern const int rook_relevant_bits[64];

extern u64 pawn_attacks[2][64];
extern u64 knight_attacks[64];
extern u64 king_attacks[64];

u64 mask_pawn_attacks(int, int);
u64 mask_knight_attacks(int);
u64 mask_king_attacks(int);
u64 mask_bishop_attacks(int);
u64 mask_rook_attacks(int);
u64 bishop_attacks_on_the_fly(int, u64);
u64 rook_attacks_on_the_fly(int, u64);
void init_leaper_attacks();
u64 set_occupancy(int, int, u64);

#endif
