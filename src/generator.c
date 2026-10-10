#include "../include/generator.h"
#include <stdlib.h>

char promoted_pieces[] = {
    [Q] = 'q',
    [R] = 'r',
    [B] = 'b',
    [N] = 'n',
    [q] = 'q',
    [r] = 'r',
    [b] = 'b',
    [n] = 'n',
};

const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};



void print_move(int move) {
    int source = GET_MOVE_SOURCE(move),
        target = GET_MOVE_TARGET(move),
        piece = GET_MOVE_PIECE(move),
        promoted = GET_MOVE_PROMOTED(move),
        capture = GET_MOVE_CAPTURE(move),
        castling = GET_MOVE_CASTLING(move);

    if (promoted && !capture) {
        printf("%s=%c\n",
                square_to_coordinates[target],
                ascii_pieces[promoted]);
        return;
    }

    if (capture) {
        if (ascii_pieces[piece] == 'P' || ascii_pieces[piece] == 'p') {
            if (promoted) {
                printf("%cx%s=%c\n",
                        square_to_coordinates[source][0],
                        square_to_coordinates[target],
                        ascii_pieces[promoted]);
                return;
            }
            printf("%cx%s\n",
                square_to_coordinates[source][0],
                square_to_coordinates[target]);

            return;
        }

        printf("%cx%s\n",
                ascii_pieces[piece],
                square_to_coordinates[target]);
        return;
    }

    if (castling) {
        if (target == g1 || target == g8)
            printf("O-O\n");
        else
            printf("O-O-O\n");

        return;
    }

    if (ascii_pieces[piece] == 'P' || ascii_pieces[piece] == 'p')
        printf("%s\n",
            square_to_coordinates[target]);
    else 
        printf("%c%s\n",
            ascii_pieces[piece],
            square_to_coordinates[target]);
}

char* get_move(int move) {
    int source = GET_MOVE_SOURCE(move),
        target = GET_MOVE_TARGET(move),
        piece = GET_MOVE_PIECE(move),
        promoted = GET_MOVE_PROMOTED(move),
        capture = GET_MOVE_CAPTURE(move),
        castling = GET_MOVE_CASTLING(move);

    char* move_notation = malloc(20 * sizeof(char)); 

    if (promoted && !capture) {
        snprintf(move_notation, 
                20,
                "%s=%c",
                square_to_coordinates[target],
                ascii_pieces[promoted]);
        return move_notation;
    }

    if (capture) {
        if (ascii_pieces[piece] == 'P' || ascii_pieces[piece] == 'p') {
            if (promoted) {
                snprintf(move_notation,
                        20,
                        "%cx%s=%c",
                        square_to_coordinates[source][0],
                        square_to_coordinates[target],
                        ascii_pieces[promoted]);
                return move_notation;
            }
            snprintf(move_notation,
                    20,
                    "%cx%s",
                    square_to_coordinates[source][0],
                    square_to_coordinates[target]);

            return move_notation;
        }

        snprintf(move_notation,
                20,
                "%cx%s",
                ascii_pieces[piece],
                square_to_coordinates[target]);
        return move_notation;
    }

    if (castling) {
        if (target == g1 || target == g8)
            snprintf(move_notation,
                    20,
                    "O-O");
        else
            snprintf(move_notation,
                    20,
                    "O-O-O");

        return move_notation;
    }

    if (ascii_pieces[piece] == 'P' || ascii_pieces[piece] == 'p')
        snprintf(move_notation,
                20,
                "%s",
                square_to_coordinates[target]);
    else 
        snprintf(move_notation,
                20,
                "%c%s",
                ascii_pieces[piece],
                square_to_coordinates[target]);

    return move_notation;
}

void print_move_list(moves* move_list) {

    if (move_list->count == 0) {
        printf("\n    No moves in move list!\n");
        return;
    }

    printf("\n    move        piece        capture        double push        enpassant        castling\n");
    for (int move_count = 0; move_count < move_list->count; move_count++) {
        int move = move_list->moves[move_count];
#ifdef WIN64
        printf("    %s%s%c       %c            %d              %d                  %d                %d\n", 
                square_to_coordinates[GET_MOVE_SOURCE(move)], 
                square_to_coordinates[GET_MOVE_TARGET(move)], 
                GET_MOVE_PROMOTED(move) ? promoted_pieces[GET_MOVE_PROMOTED(move)] : ' ',
                ascii_pieces[GET_MOVE_PIECE(move)],
                GET_MOVE_CAPTURE(move) ? 1 : 0,
                GET_MOVE_DOUBLE_PUSH(move) ? 1 : 0,
                GET_MOVE_ENPASSANT(move) ? 1 : 0,
                GET_MOVE_CASTLING(move) ? 1 : 0
              );
#else
        printf("    %s%s%c       %s            %d              %d                  %d                %d\n", 
                square_to_coordinates[GET_MOVE_SOURCE(move)], 
                square_to_coordinates[GET_MOVE_TARGET(move)], 
                GET_MOVE_PROMOTED(move) ? promoted_pieces[GET_MOVE_PROMOTED(move)] : ' ',
                unicode_pieces[GET_MOVE_PIECE(move)],
                GET_MOVE_CAPTURE(move) ? 1 : 0,
                GET_MOVE_DOUBLE_PUSH(move) ? 1 : 0,
                GET_MOVE_ENPASSANT(move) ? 1 : 0,
                GET_MOVE_CASTLING(move) ? 1 : 0
              );
#endif
    };
    printf("\n\n    Total number of moves: %d\n", move_list->count);

}


