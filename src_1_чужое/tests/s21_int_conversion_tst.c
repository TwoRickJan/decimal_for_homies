#include "s21_decimal_tst.h"

START_TEST(s21_from_decimal_to_int_1) {
    s21_decimal var;
    int value = 0;
    s21_from_int_to_decimal(5, &var);
    s21_from_decimal_to_int(var, &value);
    ck_assert_int_eq(5, value);
}
END_TEST

START_TEST(s21_from_decimal_to_int_2) {
    int n = Rand_R(-8388608, 8388608);
    s21_decimal var;
    int value = 0;
    s21_from_int_to_decimal(n, &var);
    s21_from_decimal_to_int(var, &value);
    ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

Suite *suite_s21_from_decimal_to_int_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_int_conversion");
  tc = tcase_create("case_from_decimal_to_int");

  tcase_add_test(tc, s21_from_decimal_to_int_1);
  tcase_add_loop_test(tc, s21_from_decimal_to_int_2, 0, 1000);

  suite_add_tcase(s, tc);
  return s;
}
