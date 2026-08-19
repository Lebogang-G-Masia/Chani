
/*************************************************
---------------------------------------------------
    
            CHANI BITBOARD CHESS ENGINE
                
                        by
            
                 Lebogang Masia


*************************************************/

// System headers
#include <stdio.h>

// define bitboard data type
#define u64 unsigned long long

// board squares
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1,
};

// sides to move (colors)
enum {
    WHITE,
    BLACK
};

/*"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
*/

/*********************************************************
 *********************************************************

            BIT MANIPULATION

**********************************************************
*********************************************************/

// set/get/pop macros
#define set_bit(bitboard, square) (bitboard |= (1ULL << square));
#define get_bit(bitboard, square) (bitboard & (1ULL << square)) 
#define pop_bit(bitboard, square) (get_bit(bitboard, f2) ? bitboard ^= (1ULL << f2) : 0)

/*********************************************************
 *********************************************************

            INPUT OUTPUT 

**********************************************************
*********************************************************/



// print bitboard
void print_bitboard(u64 bitboard) {
    printf("\n");
    // loop over the board ranks
    for (int rank = 0; rank < 8; rank++) {
        // loop over the board files
        for (int file = 0; file < 8; file++) {
            // convert file and rank into square index
            int square = rank * 8 + file;
            
            // print ranks
            if (!file)
                printf("  %d  ", 8 - rank);

            // print bit state (either 1 or 0)
            printf(" %d", get_bit(bitboard, square) ? 1 : 0);
        }
        // print new line every rank
        printf("\n");
    }
    // print board files
    printf("\n      a b c d e f g h\n\n");
    // print bitboard as unsigned decimal number
    printf("      Bitboard: %llud\n\n", bitboard);
}

/*********************************************************
 *********************************************************

            ATTACKS 

**********************************************************
*********************************************************/

/*
 *   NOT A FILE
 *
  8   0 1 1 1 1 1 1 1
  7   0 1 1 1 1 1 1 1
  6   0 1 1 1 1 1 1 1
  5   0 1 1 1 1 1 1 1
  4   0 1 1 1 1 1 1 1
  3   0 1 1 1 1 1 1 1
  2   0 1 1 1 1 1 1 1
  1   0 1 1 1 1 1 1 1

      a b c d e f g h
 */

/* 
 * NOT H FILE
 
  8   1 1 1 1 1 1 1 0
  7   1 1 1 1 1 1 1 0
  6   1 1 1 1 1 1 1 0
  5   1 1 1 1 1 1 1 0
  4   1 1 1 1 1 1 1 0
  3   1 1 1 1 1 1 1 0
  2   1 1 1 1 1 1 1 0
  1   1 1 1 1 1 1 1 0

      a b c d e f g h
*/

/*
 * NOT HG FILE
 *
 *
  8   1 1 1 1 1 1 0 0
  7   1 1 1 1 1 1 0 0
  6   1 1 1 1 1 1 0 0
  5   1 1 1 1 1 1 0 0
  4   1 1 1 1 1 1 0 0
  3   1 1 1 1 1 1 0 0
  2   1 1 1 1 1 1 0 0
  1   1 1 1 1 1 1 0 0

      a b c d e f g h
*/

/*
 * NOT AB FILE
 *
 *
  8   0 0 1 1 1 1 1 1
  7   0 0 1 1 1 1 1 1
  6   0 0 1 1 1 1 1 1
  5   0 0 1 1 1 1 1 1
  4   0 0 1 1 1 1 1 1
  3   0 0 1 1 1 1 1 1
  2   0 0 1 1 1 1 1 1
  1   0 0 1 1 1 1 1 1

      a b c d e f g h
*/



// not_a_file constant
const u64 not_a_file = 18374403900871474942ULL;

// not_ab_file constant
const u64 not_ab_file = 18229723555195321596ULL;

// not_h_file const
const u64 not_h_file = 9187201950435737471ULL;

// not_hg_file constant
const u64 not_hg_file = 4557430888798830399ULL;

// pawn attacks table [side][square]
u64 pawn_attacks[2][64];

// knight attacks table [square]
u64 knight_attacks[64];

// king attacks table [square]
u64 king_attacks[64];

// generate pawn attacks
u64 mask_pawn_attacks(int side, int square) {
    // define result attacks bitboard
    u64 attacks = 0ULL;
        
    // define piece bitboard 
    u64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);
    //print_bitboard(bitboard);

    // white pawns
    if (!side) {
        // generate pawn attacks
        if ((bitboard >> 7) & not_a_file) {
            attacks |= (bitboard >> 7); 
        }
        if ((bitboard >> 9) & not_h_file) {
            attacks |= (bitboard >> 9);
        }
    }
    // black pawns
    else {
         // generate pawn attacks       
        if ((bitboard << 7) & not_h_file) {
            attacks |= (bitboard << 7); 
        }
        if ((bitboard << 9) & not_a_file) {
            attacks |= (bitboard << 9);
        }
 
    }

    // return attack map
    return attacks;
}

// generate knight attacks
u64 mask_knight_attacks(int square) {
    // result attacks bitboard
    u64 attacks = 0ULL;

    // piece bitboard
    u64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate knight attacks: 17, 15, 10, 6
    if ((bitboard >> 17) & not_h_file) attacks |= (bitboard >> 17);
    if ((bitboard >> 15) & not_a_file) attacks |= (bitboard >> 15);
    if ((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10);
    if ((bitboard >> 6) & not_ab_file) attacks |= (bitboard >> 6);

    if ((bitboard << 17) & not_a_file) attacks |= (bitboard << 17);
    if ((bitboard << 15) & not_h_file) attacks |= (bitboard << 15);
    if ((bitboard << 10) & not_ab_file) attacks |= (bitboard << 10);
    if ((bitboard << 6) & not_hg_file) attacks |= (bitboard << 6);


    // return attacks map
    return attacks;
}

// generate king attacks
u64 mask_king_attacks(int square) {
    // result attacks bitboard
    u64 attacks = 0ULL;

    // piece bitboard
    u64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // generate king attacks: 1, 7, 8, 9 
    if ((bitboard >> 1) & not_h_file) attacks |= (bitboard >> 1);
    if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
    if ((bitboard >> 8)) attacks |= (bitboard >> 8);
    if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);

    if ((bitboard << 1) & not_a_file) attacks |= (bitboard << 1);
    if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
    if ((bitboard << 8)) attacks |= (bitboard << 8);
    if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);

    // return attack map
    return attacks;
}

// mask bishop attacks
u64 mask_bishop_attacks(int square) {
     // result attacks bitboard
    u64 attacks = 0ULL;

    // initialize ranks & files
    int rank, file;

    // initialize target ranks & files
    int target_rank = square / 8;
    int target_file = square % 8;

    // mask relevent bishop occupancy squares
    for (rank = target_rank + 1, file = target_file + 1; rank <= 6 && file <= 6; rank++, file++)
        attacks |= (1ULL << (rank * 8 + file));

    for (rank = target_rank - 1, file = target_file + 1; rank >= 1 && file <= 6; rank--, file++)
        attacks |= (1ULL << (rank * 8 + file));
    
    for (rank = target_rank + 1, file = target_file - 1; rank <= 6 && file >= 1; rank++, file--)
        attacks |= (1ULL << (rank * 8 + file));

    for (rank = target_rank - 1, file = target_file - 1; rank >= 1 && file >= 1; rank--, file--)
        attacks |= (1ULL << (rank * 8 + file));


    // piece bitboard
    u64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // return attack map
    return attacks;

}

// mask rook attacks
u64 mask_rook_attacks(int square) {
     // result attacks bitboard
    u64 attacks = 0ULL;

    // initialize ranks & files
    int rank, file;

    // initialize target ranks & files
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

    // piece bitboard
    u64 bitboard = 0ULL;

    // set piece on board
    set_bit(bitboard, square);

    // return attack map
    return attacks;
}

// initialize leaper pieces attacks
void init_leaper_attacks() {
    // loop over 64 board squares
    for (int square = 0; square < 64; square++) {
        // init pawn attacks
        pawn_attacks[WHITE][square] = mask_pawn_attacks(WHITE, square);
        pawn_attacks[BLACK][square] = mask_pawn_attacks(BLACK, square);
        // init knight attacks
        knight_attacks[square] = mask_knight_attacks(square);
        // init king attacks
        king_attacks[square] = mask_king_attacks(square);
    }
}

/*********************************************************
 *********************************************************

            MAIN DRIVER 

**********************************************************
*********************************************************/



int main() {
    // initialzie leaper attacks
    init_leaper_attacks();
    // loop over 64 squares
    for (int square = 0; square < 64; square++)
        print_bitboard(mask_rook_attacks(square));

    

 
    
    
    return 0;
}


















