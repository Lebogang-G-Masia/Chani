#include "../include/io.h"
#include "../include/bit_manipulation.h"
#include "../include/board.h"
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

void print_board() {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {

            if (!file)
                printf("  %d  ", 8 - rank);

            int square = rank * 8 + file; 
            int piece = -1;

            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (GET_BIT(bitboards[bb_piece], square))
                    piece = bb_piece;
            }
#ifdef WIN64
            printf(" %c ", (piece == -1) ? '.' : ascii_pieces[piece]);
#else
            printf(" %s ", (piece == -1) ? "." : unicode_pieces[piece]);
#endif
        }
        printf("\n");
    }
    printf("\n      a  b  c  d  e  f  g  h\n\n");
    printf("      S:\t%s\n", (!side) ? "White" : "Black");
    printf("      E:\t%s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "no");
    printf("      C:\t%c%c%c%c\n\n", (castle & WK) ? 'K' : '-', (castle & WQ) ? 'Q' : '-', (castle & BK) ? 'k' : '-', (castle & BQ) ? 'q' : '-');
}
