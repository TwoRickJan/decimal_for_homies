#include "s21_decimal_tst.h"

void truncate_test_function(float n, float accuracy) {
    float value0 = 0.0;
    s21_decimal var1, var2;
    s21_decl_to_null(&var1);
    s21_decl_to_null(&var2);
    int value1 = 0, value2 = 0;
    s21_from_float_to_decimal(n, &var1);
    s21_truncate(var1, &var2);
    s21_from_decimal_to_float(var2, &value0);
    value2 = (int)value0;
    value1 = truncf(n);
    ck_assert_float_eq_tol(value1, value2, accuracy);
    return;
}

START_TEST(s21_truncate_1) {
    float n = 0.9;
    truncate_test_function(n, 1);
}
END_TEST

START_TEST(s21_truncate_2) {
    float n = Rand_R(-8388608, 8388608);
    truncate_test_function(n, 10);
}
END_TEST

Suite *suite_s21_truncate_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_truncate");
  tc = tcase_create("case_truncate");

  tcase_add_test(tc, s21_truncate_1);
  tcase_add_loop_test(tc, s21_truncate_2, 0, 1000);
  suite_add_tcase(s, tc);

  return s;
}
