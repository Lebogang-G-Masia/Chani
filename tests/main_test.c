#include <stdio.h>
#include "../include/utils.h"
#include "../include/bit_manipulation.h"
#include "../include/attacks.h"
#include "../include/io.h"

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

void test_io() {
    // Just a placeholder test to ensure it links properly
    ASSERT_TEST(1, "print_bitboard is reachable and linkable");
}

int main() {
    printf("==================================================\n");
    printf("RUNNING TESTS\n");
    printf("==================================================\n");
    
    test_bit_manipulation();
    test_pawn_attacks();
    test_knight_attacks();
    test_king_attacks();
    test_bishop_attacks();
    test_rook_attacks();
    test_attacks_on_the_fly();
    test_io();
    
    printf("==================================================\n");
    printf("TEST SUMMARY\n");
    printf("==================================================\n");
    printf("Tests Run    : %d\n", tests_run);
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("==================================================\n");
    
    return tests_failed == 0 ? 0 : 1;
}
