#include "../include/generator.h"

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

void print_move(int move) {
    printf("%s%s%c\n", square_to_coordinates[GET_MOVE_SOURCE(move)], 
            square_to_coordinates[GET_MOVE_TARGET(move)], 
            promoted_pieces[GET_MOVE_PROMOTED(move)]);
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


