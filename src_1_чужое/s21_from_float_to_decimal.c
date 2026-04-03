#include "s21_decimal.h"
int s21_from_float_to_decimal(float src, s21_decimal *dst) { 
  int return_val = 0;
  s21_decl_to_null(dst);
  if (fabs(src) < powl(10.0, -1 * MAX_POW)) {
    return_val = 1;
    // dst->state = S21_NULL;
  } else if (src >= MAX_DECIMAL) {
    return_val = 1;
    // dst->state = S21_PLUS_INF;
  } else if (src <= MIN_DECIMAL) {
    return_val = 1;
    // dst->state = S21_MINUS_INF;
  } else {
    s21_decl_to_null(dst);
    int scale = 0;
    if (src < 0.0)
      s21_set_sign(dst, 1);
    src = fabsl(src);
    for(; !(int)src && scale < MAX_POW; src *=10)  // normalize
        scale++;
    int i = 0;
    for (; src < MAX_DECIMAL && scale < MAX_POW && i < FLOAT_ACCURACY; i++) {
      src *= (long double)10.0;
      scale++;
    }
    for (i = 0; src >= powl(10.0, -1 * (FLOAT_ACCURACY + 1)) && i < START_INFO; i++) {
      src = floorl(src) / 2;
      if (src - floorl(src) > powl(10.0, -1 * (FLOAT_ACCURACY + 1))) {
        s21_set_bit(dst, i, 1);
      } 
    }
    s21_set_scale(dst, scale);
    // dst->state = S21_NORMAL;
  }
  return return_val;
}
