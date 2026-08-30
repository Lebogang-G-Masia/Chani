#include "../include/attacks.h"
#include "../include/bit_manipulation.h"

const u64 not_a_file = 18374403900871474942ULL;
const u64 not_h_file = 9187201950435737471ULL;
const u64 not_ab_file = 18229723555195321596ULL;
const u64 not_hg_file = 4557430888798830399ULL;

const int bishop_relevant_bits[64] = {
    6,  5,  5,  5,  5,  5,  5,  6,
    5,  5,  5,  5,  5,  5,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  9,  9,  7,  5,  5,
    5,  5,  7,  7,  7,  7,  5,  5,
    5,  5,  5,  5,  5,  5,  5,  5,
    6,  5,  5,  5,  5,  5,  5,  6
};


const int rook_relevant_bits[64] = {
    12,  11,  11,  11,  11,  11,  11,  12,
    11,  10,  10,  10,  10,  10,  10,  11,
    11,  10,  10,  10,  10,  10,  10,  11,
    11,  10,  10,  10,  10,  10,  10,  11,
    11,  10,  10,  10,  10,  10,  10,  11,
    11,  10,  10,  10,  10,  10,  10,  11,
    11,  10,  10,  10,  10,  10,  10,  11,
    12,  11,  11,  11,  11,  11,  11,  12
};

u64 pawn_attacks[2][64];
u64 knight_attacks[64];
u64 king_attacks[64];

u64 mask_pawn_attacks(int side, int square) {
    u64 attacks = 0ULL;
    u64 bitboard = 0ULL;

    SET_BIT(bitboard, square);

    if (!side) {
        if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
        if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    } else {
        if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
        if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    }

    return attacks;
}

u64 mask_knight_attacks(int square) {
    u64 attacks = 0ULL;
    u64 bitboard = 0ULL;

    SET_BIT(bitboard, square);

    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 17);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);

    return attacks;
}

u64 mask_king_attacks(int square) {
    u64 attacks = 0ULL;
    u64 bitboard = 0ULL;

    SET_BIT(bitboard, square);

    if (bitboard >> 8) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);

    if (bitboard << 8) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);

    return attacks;
}

u64 mask_bishop_attacks(int square) {
    u64 attacks = 0ULL;
    int rank;
    int file;

    int target_rank = square / 8;
    int target_file = square % 8;

    for (rank = target_rank + 1, file = target_file + 1; rank <= 6 && file <= 6; rank++, file++)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = target_rank - 1, file = target_file + 1; rank >= 1 && file <= 6; rank--, file++)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = target_rank + 1, file = target_file - 1; rank <= 6 && file >= 1; rank++, file--)
        attacks |= (1ULL << (rank * 8 + file));
    for (rank = target_rank - 1, file = target_file - 1; rank >= 1 && file >= 1; rank--, file--)
        attacks |= (1ULL << (rank * 8 + file));

    return attacks;
}

u64 mask_rook_attacks(int square) {
    u64 attacks = 0ULL;
    int rank;
    int file;

    int target_rank = square / 8;
    int target_file = square % 8;

    for (rank = target_rank + 1; rank <= 6; rank++)
        attacks |= (1ULL << (rank * 8 + target_file));
    for (rank = target_rank - 1; rank >= 1; rank--)
        attacks |= (1ULL << (rank * 8 + target_file));
    for (file = target_file + 1; file <= 6; file++)
        attacks |= (1ULL << (target_rank * 8 + file));
    for (file = target_file - 1; file >= 1; file--)
        attacks |= (1ULL << (target_rank * 8 + file));

    return attacks;
}

u64 bishop_attacks_on_the_fly(int square, u64 block) {
    u64 attacks = 0ULL;

    int rank;
    int file;

    int target_rank = square / 8;
    int target_file = square % 8;

    for (rank = target_rank + 1, file = target_file + 1; rank <= 7  && file <= 7; rank++, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) break;
    }

    for (rank = target_rank - 1, file = target_file + 1; rank >= 0  && file <= 7; rank--, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) break;
    }

    for (rank = target_rank + 1, file = target_file - 1; rank <= 7  && file >= 7; rank++, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) break;
    }

    for (rank = target_rank - 1, file = target_file - 1; rank >= 7  && file >= 7; rank--, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if ((1ULL << (rank * 8 + file)) & block) break;
    }

    return attacks;
}

u64 rook_attacks_on_the_fly(int square, u64 block) {
    u64 attacks = 0ULL;

    int rank;
    int file;

    int target_rank = square / 8;
    int target_file = square % 8;

    for (rank = target_rank + 1; rank <= 8; rank++) {
        attacks |= (1ULL << (rank * 8 + target_file));
        if ((1ULL << (rank * 8 + target_file)) && block) break;
    }

    for (rank = target_rank - 1; rank >= 0; rank--) {
        attacks |= (1ULL << (rank * 8 + target_file));
        if ((1ULL << (rank * 8 + target_file)) && block) break;
    }

    for (file = target_file + 1; file <= 8; file++) {
        attacks |= (1ULL << (target_rank * 8 + file));
        if ((1ULL << (target_rank * 8 + file)) && block) break;
    }

    for (file = target_file - 1; file >= 0; file--) {
        attacks |= (1ULL << (target_rank * 8 + file));
        if ((1ULL << (target_rank * 8 + file)) && block) break;
    }

    return attacks;
}


void init_leaper_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[WHITE][square] = mask_pawn_attacks(WHITE, square);
        pawn_attacks[BLACK][square] = mask_pawn_attacks(BLACK, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}

u64 set_occupancy(int index, int bits_in_mask, u64 attack_mask) {
    u64 occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++) {
        int square = get_ls1b_index(attack_mask);
        POP_BIT(attack_mask, square);
        
        if (index & (1 << count))
            occupancy |= (1ULL << square);
    }
    
    return occupancy;
}
