#include "s21_decimal_tst.h"
// проблема потому что еще нули после точки при конвертации картину могут портить
void round_test_function(float n, float accuracy) {
    s21_decimal var1, var2;
    s21_decl_to_null(&var1);
    s21_decl_to_null(&var2);
    float value1 = 0.0, value2 = 0.0;
    s21_from_float_to_decimal(n, &var1);
    s21_round(var1, &var2);
    s21_from_decimal_to_float(var2, &value2);
    value1 = roundf(n);
    ck_assert_float_eq_tol(value1, value2, accuracy);
    return;
}
START_TEST(s21_round_1) {
    float n = 0.9;
    round_test_function(n, 1);
}
END_TEST

START_TEST(s21_round_2) {
    float n = Rand_R(-8388608, 8388608);
    round_test_function(n, 10);
}
END_TEST

Suite *suite_s21_round_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_round");
  tc = tcase_create("case_round");

  tcase_add_test(tc, s21_round_1);
  tcase_add_loop_test(tc, s21_round_2, 0, 10);
//   tcase_add_loop_test(tc, s21_round_3, 0, 100);

  suite_add_tcase(s, tc);
  return s;
}
