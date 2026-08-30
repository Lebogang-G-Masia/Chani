#include "../include/prng.h"
#include "../include/utils.h"

unsigned int state = 1804289383;

unsigned int get_random_u32_number() {
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    state = number;

    return number;
}

u64 get_random_u64_numbers() {
    u64 n1, n2, n3, n4;

    n1 = (u64)(get_random_u32_number()) & 0xFFFF;
    n2 = (u64)(get_random_u32_number()) & 0xFFFF;
    n3 = (u64)(get_random_u32_number()) & 0xFFFF;
    n4 = (u64)(get_random_u32_number()) & 0xFFFF;

    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);

}

u64 generate_magic_number() {
    return get_random_u64_numbers() & get_random_u64_numbers() & get_random_u64_numbers();
}
