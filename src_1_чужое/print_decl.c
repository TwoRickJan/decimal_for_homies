#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

void print_dec(s21_decimal decl) {
  char color[20];

  for (int i = 127; i >= 0; i--) {
    printf("%d", s21_get_bit(decl, i));
    if (i % 32 == 0) {
      printf(" ");
    }
  }
  printf("\n");
}
