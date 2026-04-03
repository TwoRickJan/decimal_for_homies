#include "s21_decimal_tst.h"

void negate_test_function(float n, float accuracy) {
    s21_decimal var1, var2, var3;
    s21_decl_to_null(&var1);
    s21_decl_to_null(&var2);
    s21_decl_to_null(&var3);
    float value1 = 0.0, value2 = 0.0;
    s21_from_float_to_decimal(n, &var1);
    s21_negate(var1, &var2);
    s21_from_decimal_to_float(var2, &value2);
    value1 = -1 * n;
    s21_from_float_to_decimal(value1, &var3);
    if (s21_is_not_equal(var2, var3))
      printf("n = %f, value1 = %f, value2 = %f\n", n, value1, value2);
    ck_assert_float_eq_tol(value1, value2, accuracy);
    return;
}
START_TEST(s21_negate_1) {
    float n = 0.9;
    negate_test_function(n, 1);
}
END_TEST

START_TEST(s21_negate_2) {
    float n = Rand_R(-8388608, 8388608);
    negate_test_function(n, 10);
}
END_TEST

Suite *suite_s21_negate_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_negate");
  tc = tcase_create("case_negate");

  tcase_add_test(tc, s21_negate_1);
  tcase_add_loop_test(tc, s21_negate_2, 0, 1000);

  suite_add_tcase(s, tc);
  return s;
}
