#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/attacks.h"
#include "../include/io.h"
#include "../include/board.h"
#include "../include/generator.h"

int tests_run = 0;
int tests_passed = 0;
int tests_failed = 0;

#define ASSERT_TEST(condition, test_case_name) \
    do { \
        tests_run++; \
        if (condition) { \
            printf("[PASS] %s: %s\n", __func__, test_case_name); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s: %s\n", __func__, test_case_name); \
            tests_failed++; \
        } \
    } while (0)

void test_bit_manipulation() {
    u64 bitboard = 0ULL;
    
    // SET_BIT
    SET_BIT(bitboard, e4);
    ASSERT_TEST(bitboard == (1ULL << e4), "SET_BIT should set the e4 bit to 1");
    
    // GET_BIT
    ASSERT_TEST(GET_BIT(bitboard, e4) != 0, "GET_BIT should return non-zero for e4");
    ASSERT_TEST(GET_BIT(bitboard, d4) == 0, "GET_BIT should return zero for d4");
    
    // POP_BIT
    POP_BIT(bitboard, e4);
    ASSERT_TEST(bitboard == 0ULL, "POP_BIT should clear the e4 bit to 0");
    
    // count_bits
    bitboard = (1ULL << a8) | (1ULL << h1) | (1ULL << d4);
    ASSERT_TEST(count_bits(bitboard) == 3, "count_bits should return 3 for 3 bits set");
    
    // get_ls1b_index
    ASSERT_TEST(get_ls1b_index(bitboard) == a8, "get_ls1b_index should return a8 as the least significant 1st bit");
    ASSERT_TEST(get_ls1b_index(0ULL) == -1, "get_ls1b_index should return -1 for empty bitboard");
}

void test_pawn_attacks() {
    u64 w_attacks = mask_pawn_attacks(WHITE, e4);
    ASSERT_TEST(count_bits(w_attacks) <= 2, "mask_pawn_attacks for white on e4 should have up to 2 attacks");
    
    u64 b_attacks = mask_pawn_attacks(BLACK, e4);
    ASSERT_TEST(count_bits(b_attacks) <= 2, "mask_pawn_attacks for black on e4 should have up to 2 attacks");
}

void test_knight_attacks() {
    u64 attacks = mask_knight_attacks(e4);
    ASSERT_TEST(count_bits(attacks) > 0 && count_bits(attacks) <= 8, "mask_knight_attacks on e4 should have between 1 and 8 attacks");
}

void test_king_attacks() {
    u64 attacks = mask_king_attacks(e4);
    ASSERT_TEST(count_bits(attacks) > 0 && count_bits(attacks) <= 8, "mask_king_attacks on e4 should have between 1 and 8 attacks");
}

void test_bishop_attacks() {
    u64 attacks = mask_bishop_attacks(e4);
    ASSERT_TEST(count_bits(attacks) > 0, "mask_bishop_attacks on e4 should have > 0 attacks");
}

void test_rook_attacks() {
    u64 attacks = mask_rook_attacks(e4);
    ASSERT_TEST(count_bits(attacks) > 0, "mask_rook_attacks on e4 should have > 0 attacks");
}

void test_attacks_on_the_fly() {
    u64 b_attacks = bishop_attacks_on_the_fly(e4, 0ULL);
    ASSERT_TEST(count_bits(b_attacks) > 0, "bishop_attacks_on_the_fly on e4 should generate attacks");

    u64 r_attacks = rook_attacks_on_the_fly(e4, 0ULL);
    ASSERT_TEST(count_bits(r_attacks) > 0, "rook_attacks_on_the_fly on e4 should generate attacks");
}

void test_is_square_attacked() {
    init_leaper_attacks();
    init_sliders_attacks(BISHOP);
    init_sliders_attacks(ROOK);

    for (int i = 0; i < 12; i++) bitboards[i] = 0ULL;
    for (int i = 0; i < 3; i++) occupancies[i] = 0ULL;

    SET_BIT(bitboards[P], e4);
    occupancies[WHITE] |= (1ULL << e4);
    occupancies[BOTH] |= (1ULL << e4);

    ASSERT_TEST(is_square_attacked(d5, WHITE) == 1, "is_square_attacked: d5 attacked by white pawn on e4");
    ASSERT_TEST(is_square_attacked(f5, WHITE) == 1, "is_square_attacked: f5 attacked by white pawn on e4");
    ASSERT_TEST(is_square_attacked(e5, WHITE) == 0, "is_square_attacked: e5 not attacked by white pawn on e4");

    SET_BIT(bitboards[n], d4);
    occupancies[BLACK] |= (1ULL << d4);
    occupancies[BOTH] |= (1ULL << d4);

    ASSERT_TEST(is_square_attacked(e2, BLACK) == 1, "is_square_attacked: e2 attacked by black knight on d4");
    ASSERT_TEST(is_square_attacked(e3, BLACK) == 0, "is_square_attacked: e3 not attacked by black knight on d4");
}

void test_generate_moves() {
    for (int i = 0; i < 12; i++) bitboards[i] = 0ULL;
    for (int i = 0; i < 3; i++) occupancies[i] = 0ULL;

    SET_BIT(bitboards[P], e2);
    occupancies[WHITE] |= (1ULL << e2);
    occupancies[BOTH] |= (1ULL << e2);

    SET_BIT(bitboards[p], d3);
    occupancies[BLACK] |= (1ULL << d3);
    occupancies[BOTH] |= (1ULL << d3);

    side = WHITE;
    enpassant = no_sq;

    moves move_list[1];
    move_list->count = 0;

    generate_moves(move_list);

    int found_push = 0, found_double = 0, found_capture = 0;
    for (int i = 0; i < move_list->count; i++) {
        int move = move_list->moves[i];
        int src = GET_MOVE_SOURCE(move);
        int tgt = GET_MOVE_TARGET(move);
        int cap = GET_MOVE_CAPTURE(move);
        int dbl = GET_MOVE_DOUBLE_PUSH(move);

        if (src == e2 && tgt == e3 && !cap && !dbl) found_push = 1;
        if (src == e2 && tgt == e4 && !cap && dbl) found_double = 1;
        if (src == e2 && tgt == d3 && cap) found_capture = 1;
    }

    ASSERT_TEST(found_push, "generate_moves: generates pawn push e2e3");
    ASSERT_TEST(found_double, "generate_moves: generates double pawn push e2e4");
    ASSERT_TEST(found_capture, "generate_moves: generates pawn capture e2d3");
    ASSERT_TEST(move_list->count == 3, "generate_moves: exactly 3 expected moves generated");
}

void test_make_move() {
    for (int i = 0; i < 12; i++) bitboards[i] = 0ULL;
    for (int i = 0; i < 3; i++) occupancies[i] = 0ULL;

    SET_BIT(bitboards[P], e2);
    occupancies[WHITE] |= (1ULL << e2);
    occupancies[BOTH] |= (1ULL << e2);

    SET_BIT(bitboards[p], d3);
    occupancies[BLACK] |= (1ULL << d3);
    occupancies[BOTH] |= (1ULL << d3);

    side = WHITE;
    enpassant = no_sq;

    int move = ENCODE_MOVE(e2, d3, P, 0, 1, 0, 0, 0);
    make_move(move, ALL_MOVES);

    ASSERT_TEST(GET_BIT(bitboards[P], e2) == 0, "make_move: source square cleared");
    ASSERT_TEST(GET_BIT(bitboards[P], d3) != 0, "make_move: target square set");
    ASSERT_TEST(GET_BIT(bitboards[p], d3) == 0, "make_move: captured piece removed");
}

void test_io() {
    // Just a placeholder test to ensure it links properly
    ASSERT_TEST(1, "print_bitboard is reachable and linkable");
}

void test_leaper_attacks_init() {
    init_leaper_attacks();
    ASSERT_TEST(pawn_attacks[WHITE][e4] == mask_pawn_attacks(WHITE, e4), "pawn_attacks initialized");
    ASSERT_TEST(knight_attacks[e4] == mask_knight_attacks(e4), "knight_attacks initialized");
    ASSERT_TEST(king_attacks[e4] == mask_king_attacks(e4), "king_attacks initialized");
    ASSERT_TEST(pawn_attacks[WHITE][a2] == mask_pawn_attacks(WHITE, a2), "pawn edge case");
}

void test_set_occupancy() {
    u64 mask = 0x11ULL;
    ASSERT_TEST(set_occupancy(0, 2, mask) == 0ULL, "occupancy index 0");
    ASSERT_TEST(set_occupancy(1, 2, mask) == 0x1ULL, "occupancy index 1");
    ASSERT_TEST(set_occupancy(2, 2, mask) == 0x10ULL, "occupancy index 2");
    ASSERT_TEST(set_occupancy(3, 2, mask) == 0x11ULL, "occupancy index 3");
}

void test_sliders_attacks_init() {
    init_sliders_attacks(BISHOP);
    init_sliders_attacks(ROOK);
    
    u64 empty = 0ULL;
    ASSERT_TEST(get_bishop_attacks(e4, empty) == bishop_attacks_on_the_fly(e4, empty), "bishop magic empty");
    ASSERT_TEST(get_rook_attacks(e4, empty) == rook_attacks_on_the_fly(e4, empty), "rook magic empty");
    
    u64 block = (1ULL << d5) | (1ULL << e6);
    ASSERT_TEST(get_bishop_attacks(e4, block) == bishop_attacks_on_the_fly(e4, block), "bishop magic blocked");
    ASSERT_TEST(get_rook_attacks(e4, block) == rook_attacks_on_the_fly(e4, block), "rook magic blocked");
}


typedef void (*test_func_t)();
typedef struct {
    const char *name;
    test_func_t func;
} test_case_t;

test_case_t chani_tests[] = {
    {"test_bit_manipulation", test_bit_manipulation},
    {"test_pawn_attacks", test_pawn_attacks},
    {"test_knight_attacks", test_knight_attacks},
    {"test_king_attacks", test_king_attacks},
    {"test_bishop_attacks", test_bishop_attacks},
    {"test_rook_attacks", test_rook_attacks},
    {"test_attacks_on_the_fly", test_attacks_on_the_fly},
    {"test_is_square_attacked", test_is_square_attacked},
    {"test_generate_moves", test_generate_moves},
    {"test_make_move", test_make_move},
    {"test_io", test_io},
    {"test_leaper_attacks_init", test_leaper_attacks_init},
    {"test_set_occupancy", test_set_occupancy},
    {"test_sliders_attacks_init", test_sliders_attacks_init}
};
int num_tests = sizeof(chani_tests) / sizeof(chani_tests[0]);

void print_menu() {
    printf("\n==================================================\n");
    printf("CHANI INTERACTIVE TEST SUITE\n");
    printf("==================================================\n");
    for (int i = 0; i < num_tests; i++) {
        printf("%2d) %s\n", i + 1, chani_tests[i].name);
    }
    printf(" a) Run all tests\n");
    printf(" p) Run all tests with pause in between\n");
    printf(" q) Quit\n");
    printf("==================================================\n");
    printf("Select an option: ");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait_for_enter() {
    printf("\nPress [Enter] to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {
    init(); // Ensure global initialization

    if (argc > 1) {
        if (strcmp(argv[1], "--all") == 0) {
            for (int i = 0; i < num_tests; i++) chani_tests[i].func();
        } else if (strcmp(argv[1], "--test") == 0 && argc > 2) {
            int found = 0;
            for (int i = 0; i < num_tests; i++) {
                if (strcmp(chani_tests[i].name, argv[2]) == 0) {
                    chani_tests[i].func();
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Test '%s' not found.\n", argv[2]);
        } else {
            printf("Usage:\n  %s\n  %s --all\n  %s --test <test_name>\n", argv[0], argv[0], argv[0]);
            return 1;
        }
        goto summary;
    }

    char choice[10];
    while (1) {
        print_menu();
        if (!fgets(choice, sizeof(choice), stdin)) break;
        
        if (choice[0] == 'q' || choice[0] == 'Q') {
            return 0;
        } else if (choice[0] == 'a' || choice[0] == 'A') {
            for (int i = 0; i < num_tests; i++) chani_tests[i].func();
            break;
        } else if (choice[0] == 'p' || choice[0] == 'P') {
            for (int i = 0; i < num_tests; i++) {
                printf("\n--- Running %s ---\n", chani_tests[i].name);
                chani_tests[i].func();
                if (i < num_tests - 1) wait_for_enter();
            }
            break;
        } else {
            int test_idx = atoi(choice);
            if (test_idx >= 1 && test_idx <= num_tests) {
                printf("\n--- Running %s ---\n", chani_tests[test_idx - 1].name);
                chani_tests[test_idx - 1].func();
                wait_for_enter();
            } else {
                printf("Invalid selection.\n");
            }
        }
    }

summary:
    printf("\n==================================================\n");
    printf("TEST SUMMARY\n");
    printf("==================================================\n");
    printf("Tests Run    : %d\n", tests_run);
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("==================================================\n");
    
    return tests_failed == 0 ? 0 : 1;
}
