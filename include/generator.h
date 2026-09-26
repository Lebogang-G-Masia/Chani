#ifndef CHANI_GENERATOR
#define CHANI_GENERATOR

#include "utils.h"
#include "attacks.h"
#include "bit_manipulation.h"

#include <string.h>
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

#define COPY_BOARD() \
    u64 bitboards_copy[12], occupancies_copy[3]; \
    int side_copy, enpassant_copy, castle_copy; \
    memcpy(bitboards_copy, bitboards, 96); \
    memcpy(occupancies_copy, occupancies, 24); \
    side_copy = side; \
    enpassant_copy = enpassant; \
    castle_copy = castle;

#define RESTORE_BOARD() \
    memcpy(bitboards, bitboards_copy, 96); \
    memcpy(occupancies, occupancies_copy, 24); \
    side = side_copy; \
    enpassant = enpassant_copy; \
    castle = castle_copy;

enum {
    ALL_MOVES,
    ONLY_CAPTURES
};

static inline int make_move(int move, int move_flag) {
    if (move_flag == ALL_MOVES) {
        COPY_BOARD();

        int source_square = GET_MOVE_SOURCE(move);
        int target_square = GET_MOVE_TARGET(move);
        int piece = GET_MOVE_PIECE(move);
        int promoted_piece = GET_MOVE_PROMOTED(move);
        int capture_flag = GET_MOVE_CAPTURE(move);
        int double_push_flag = GET_MOVE_DOUBLE_PUSH(move);
        int enpassant_flag = GET_MOVE_ENPASSANT(move);
        int castling_flag = GET_MOVE_CASTLING(move);

        POP_BIT(bitboards[piece], source_square);
        SET_BIT(bitboards[piece], target_square);

    } else {
        if (GET_MOVE_CAPTURE(move)) 
            make_move(move, ALL_MOVES);
        else return 0;
    }
}


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

static inline void generate_moves(moves* move_list) {
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
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, Q, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, R, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, B, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, N, 0, 0, 0, 0));
                        } else {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                        }
                        if ((source_square >= a2 && source_square <= h2) && !GET_BIT(occupancies[BOTH], target_square - 8)) 
                            add_move(move_list, ENCODE_MOVE(source_square, (target_square - 8), piece, 0, 0, 1, 0, 0));
                    }


                    attacks = pawn_attacks[side][source_square] & occupancies[BLACK];

                    while (attacks) {
                        target_square = get_ls1b_index(attacks);
                        if (source_square >= a7 && source_square <= h7) {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, Q, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, R, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, B, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, N, 1, 0, 0, 0));
                        } else {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        POP_BIT(attacks, target_square);
                    }

                    if (enpassant != no_sq) {
                        u64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        if (enpassant_attacks) {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, ENCODE_MOVE(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
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
                            add_move(move_list, ENCODE_MOVE(e1, g1, piece, 0, 0, 0, 0, 1));
                    }
                }

                if (castle & WQ) {
                    if (!GET_BIT(occupancies[BOTH], d1) &&
                            !GET_BIT(occupancies[BOTH], c1) &&
                            !GET_BIT(occupancies[BOTH], b1)) {
                        if (!is_square_attacked(e1, BLACK) &&
                                !is_square_attacked(d1, BLACK)) {
                            add_move(move_list, ENCODE_MOVE(e1, c1, piece, 0, 0, 0, 0, 1));
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
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, q, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, r, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, b, 0, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, n, 0, 0, 0, 0));
 
                        } else {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                        }
                        if ((source_square >= a7 && source_square <= h7) && !GET_BIT(occupancies[BOTH], target_square + 8)) 
                            add_move(move_list, ENCODE_MOVE(source_square, (target_square + 8), piece, 0, 0, 1, 0, 0));
                    }


                    attacks = pawn_attacks[side][source_square] & occupancies[WHITE];

                    while (attacks) {
                        target_square = get_ls1b_index(attacks);
                        if (source_square >= a2 && source_square <= h2) {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, q, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, r, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, b, 1, 0, 0, 0));
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, n, 1, 0, 0, 0));
                        } else {
                            add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));
                        }
                        POP_BIT(attacks, target_square);
                    }

                    if (enpassant != no_sq) {
                        u64 enpassant_attacks = pawn_attacks[side][source_square] & (1ULL << enpassant);

                        if (enpassant_attacks) {
                            int target_enpassant = get_ls1b_index(enpassant_attacks);
                            add_move(move_list, ENCODE_MOVE(source_square, target_enpassant, piece, 0, 1, 0, 1, 0));
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
                            add_move(move_list, ENCODE_MOVE(e8, g8, piece, 0, 0, 0, 0, 1));
                    }
                }

                if (castle & BQ) {
                    if (!GET_BIT(occupancies[BOTH], d8) &&
                            !GET_BIT(occupancies[BOTH], c8) &&
                            !GET_BIT(occupancies[BOTH], b8)) {
                        if (!is_square_attacked(e8, WHITE) &&
                                !is_square_attacked(d8, WHITE)) {
                            add_move(move_list, ENCODE_MOVE(e8, c8, piece, 0, 0, 0, 0, 1));
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
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    else
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));


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
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    else
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));

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
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    else
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));

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
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    else
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));

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
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 0, 0, 0, 0));
                    else
                        add_move(move_list, ENCODE_MOVE(source_square, target_square, piece, 0, 1, 0, 0, 0));

                    POP_BIT(attacks, target_square);
                }

                POP_BIT(bitboard, source_square);
            }
        }
    }
}

#endif // CHANI_GENERATOR
