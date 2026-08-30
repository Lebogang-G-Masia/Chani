#ifndef CHANI_MAGICS
#define CHANI_MAGICS

#include "utils.h"

extern u64 rook_magic_numbers[64];
extern u64 bishop_magic_numbers[64];

u64 find_magic_number(int, int, int);
void init_magic_numbers();

#endif // CHANI_MAGICS
