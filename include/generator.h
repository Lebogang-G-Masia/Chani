#ifndef CHANI_GENERATOR
#define CHANI_GENERATOR

#include "utils.h"
#include "attacks.h"
#include "bit_manipulation.h"
#include <stdio.h>

#define ENCODE_MOVE(source, target, piece, promoted, capture, double_push, enpassant, castling) \
    (source) | \
    (target << 6) | \
    (piece << 12) | \
    (promoted << 16) | \
    (capture << 20) | \
    (double_push << 21) | \
    (enpassant << 22) | \
    (castling << 23) \

#define GET_MOVE_SOURCE(move) (move & 0x3f)
#define GET_MOVE_TARGET(move) ((move & 0xfc0) >> 6)
#define GET_MOVE_PIECE(move) ((move & 0xf000) >> 12)
#define GET_MOVE_PROMOTED(move) ((move & 0xf0000) >> 16)
#define GET_MOVE_CAPTURE(move) (move & 0x100000) 
#define GET_MOVE_DOUBLE_PUSH(move) (move & 0x200000)
#define GET_MOVE_ENPASSANT(move) (move & 0x400000)
#define GET_MOVE_CASTLING(move) (move & 0x800000)


typedef struct {
    int moves[256];
    int count;
} moves;

extern char promoted_pieces[];

void print_move(int);
void print_move_list(moves*);


static inline void add_move(moves* move_list, int move) {
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

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
                        if (source_square >= a7 && source_square <= h7) {
                            printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        if ((source_square >= a2 && source_square <= h2) && !GET_BIT(occupancies[BOTH], target_square - 8)) 
                            printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square - 8]);
                    }


                    attacks = pawn_attacks[side][source_square] & occupancies[BLACK];

                    while (attacks) {
                        target_square = get_ls1b_index(attacks);
                        if (source_square >= a7 && source_square <= h7) {
                            printf("pawn promotion capture: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            printf("pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        POP_BIT(attacks, target_square);
                    }

                    if (enpassant != no_sq) {
                        u64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        if (enpassant_attacks) {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            printf("pawn enpassant capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
                        }
                    }
                    POP_BIT(bitboard, source_square);
                }
            }
            if (piece == K) {
                if (castle & WK) {
                    if (!GET_BIT(occupancies[BOTH], f1) &&
                            !GET_BIT(occupancies[BOTH], g1)) {
                        if (!is_square_attacked(e1, BLACK) &&
                                !is_square_attacked(f1, BLACK))
                            printf("castling move: e1g1\n");
                        
                    }
                }

                if (castle & WQ) {
                    if (!GET_BIT(occupancies[BOTH], d1) &&
                            !GET_BIT(occupancies[BOTH], c1) &&
                            !GET_BIT(occupancies[BOTH], b1)) {
                        if (!is_square_attacked(e1, BLACK) &&
                                !is_square_attacked(d1, BLACK)) {
                            printf("castling move: e1c1\n");
                        }
                    }
                }
            }
        } else {
             if (piece == p) {
                while (bitboard) {
                    source_square = get_ls1b_index(bitboard);
                    target_square = source_square + 8;

                    if (!(target_square > h1) && !GET_BIT(occupancies[BOTH], target_square)) {
                        if (source_square >= a2 && source_square <= h2) {
                            printf("pawn promotion: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            printf("pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        if ((source_square >= a7 && source_square <= h7) && !GET_BIT(occupancies[BOTH], target_square + 8)) 
                            printf("double pawn push: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square + 8]);
                    }


                    attacks = pawn_attacks[side][source_square] & occupancies[WHITE];

                    while (attacks) {
                        target_square = get_ls1b_index(attacks);
                        if (source_square >= a2 && source_square <= h2) {
                            printf("pawn promotion capture: %s%sq\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sb\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sn\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                            printf("pawn promotion capture: %s%sr\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        } else {
                            printf("pawn capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                        }
                        POP_BIT(attacks, target_square);
                    }

                    if (enpassant != no_sq) {
                        u64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        if (enpassant_attacks) {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            printf("pawn enpassant capture: %s%s\n", square_to_coordinates[source_square], square_to_coordinates[target_enpassant]);
                        }
                    }
                    POP_BIT(bitboard, source_square);
                }
            }
            if (piece == k) {
                if (castle & BK) {
                    if (!GET_BIT(occupancies[BOTH], f8) &&
                            !GET_BIT(occupancies[BOTH], g8)) {
                        if (!is_square_attacked(e8, WHITE) &&
                                !is_square_attacked(f8, WHITE))
                            printf("castling move: e8g8\n");
                        
                    }
                }

                if (castle & BQ) {
                    if (!GET_BIT(occupancies[BOTH], d8) &&
                            !GET_BIT(occupancies[BOTH], c8) &&
                            !GET_BIT(occupancies[BOTH], b8)) {
                        if (!is_square_attacked(e8, WHITE) &&
                                !is_square_attacked(d8, WHITE)) {
                            printf("castling move: e8c8\n");
                        }
                    }
                }
            }
        }

        if ((side == WHITE) ? piece == N : piece == n) {
            while (bitboard) {
                source_square = get_ls1b_index(bitboard);
                attacks = knight_attacks[source_square] & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                while (attacks) {
                    target_square = get_ls1b_index(attacks);
                    
                    if (!GET_BIT(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), target_square))
                            printf("%s%s\tknight move\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    else
                             printf("%s%s\tknight capture\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);                   

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }

        if ((side == WHITE) ? piece == B : piece == b) {
            while (bitboard) {
                source_square = get_ls1b_index(bitboard);
                attacks = get_bishop_attacks(source_square, occupancies[BOTH]) & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                while (attacks) {
                    target_square = get_ls1b_index(attacks);
                    
                    if (!GET_BIT(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), target_square))
                            printf("%s%s\tbishop move\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    else
                             printf("%s%s\tbishop capture\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);                   

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }

        if ((side == WHITE) ? piece == R : piece == r) {
            while (bitboard) {
                source_square = get_ls1b_index(bitboard);
                attacks = get_rook_attacks(source_square, occupancies[BOTH]) & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                while (attacks) {
                    target_square = get_ls1b_index(attacks);
                    
                    if (!GET_BIT(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), target_square))
                            printf("%s%s\trook move\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    else
                             printf("%s%s\trook capture\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);                   

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }
        if ((side == WHITE) ? piece == Q : piece == q) {
            while (bitboard) {
                source_square = get_ls1b_index(bitboard);
                attacks = get_queen_attacks(source_square, occupancies[BOTH]) & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                while (attacks) {
                    target_square = get_ls1b_index(attacks);
                    
                    if (!GET_BIT(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), target_square))
                            printf("%s%s\tqueen move\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    else
                             printf("%s%s\tqueen capture\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);                   

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }

        if ((side == WHITE) ? piece == K : piece == k) {
            while (bitboard) {
                source_square = get_ls1b_index(bitboard);
                attacks = king_attacks[source_square] & ((side == WHITE) ? ~occupancies[WHITE] : ~occupancies[BLACK]);
                while (attacks) {
                    target_square = get_ls1b_index(attacks);
                    
                    if (!GET_BIT(((side == WHITE) ? occupancies[BLACK] : occupancies[WHITE]), target_square))
                            printf("%s%s\tking move\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);
                    else
                             printf("%s%s\tking capture\n", square_to_coordinates[source_square], square_to_coordinates[target_square]);                   

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }
    }
}

#endif // CHANI_GENERATOR
