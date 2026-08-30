#ifndef CHANI_PRNG
#define CHANI_PRNG

#include "utils.h"

extern unsigned int state; 

unsigned int get_random_u32_number();
u64 get_random_u64_numbers();
u64 generate_magic_number();

#endif // CHANI_PRNG
