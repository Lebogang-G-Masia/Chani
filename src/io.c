#include "../include/io.h"
#include "../include/bit_manipulation.h"
#include "../include/board.h"
#include <stdio.h>
#include <string.h>

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

void parse_fen(char *fen) {
    memset(bitboards, 0ULL, sizeof(bitboards));
    memset(occupancies, 0ULL, sizeof(occupancies));
    side = 0;
    enpassant = no_sq;
    castle = 0;

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z')) {
                int piece = char_pieces[(unsigned char)*fen];
                SET_BIT(bitboards[piece], square);

                fen++;
            }

            if (*fen >= '0' && *fen <= '9') {
                int offset = *fen - '0';
                int piece = -1;

                for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                    if (GET_BIT(bitboards[bb_piece], square))
                        piece = bb_piece;
                }

                if (piece == -1)
                    file--;

                file += offset;
                fen++;
            }

            if (*fen == '/') fen++;
            
        }
    }
    fen++;
    side = (*fen == 'w') ? WHITE : BLACK;

    fen += 2;
    while (*fen != ' ') {
        switch (*fen) {
            case 'K':
                castle |= WK;
                break;
            case 'Q':
                castle |= WQ;
                break;
            case 'k':
                castle |= BK;
                break;
            case 'q':
                castle |= BQ;
                break;
            case '-':
                break;
        }
        fen++;
    }

    fen++;
    if (*fen != '-') {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');
        enpassant = rank * 8 + file;
    } else {
        enpassant = no_sq;
    }

    for (int piece = P; piece <= K; piece++) {
        occupancies[WHITE] |= bitboards[piece];
    }

    for (int piece = p; piece <= k; piece++) {
        occupancies[BLACK] |= bitboards[piece];
    }

    occupancies[BOTH] = occupancies[WHITE] | occupancies[BLACK];
}
