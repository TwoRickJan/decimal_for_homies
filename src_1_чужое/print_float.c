#include <stdio.h>

#include "s21_decimal.h"

void print_float(float f_num) {
  union float_bits decl_bits;
  decl_bits.num_f = f_num;

  for (int i = 0; i < 32; ++i) {
    printf("%s%5d%s", YELLOW, i, RESET);
  }
  printf("\n");
  for (int i = 0; i < 32; ++i) {
    if ((decl_bits.bits & (1 << i)) >> i) {
      printf("%s%5d%s", GREEN, 1, RESET);
    } else {
      printf("%s%5d%s", RED, 0, RESET);
    }
  }
  printf("\n");
}
