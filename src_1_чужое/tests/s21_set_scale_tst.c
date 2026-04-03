#include "s21_decimal_tst.h"

START_TEST(s21_set_scale_test_1) {
    s21_decimal var;
    s21_decl_to_null(&var);
    s21_set_scale(&var, 6);
}
END_TEST

Suite *suite_s21_set_scale_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_set_scale");
  tc = tcase_create("case_set_scale");

  tcase_add_test(tc, s21_set_scale_test_1);

  suite_add_tcase(s, tc);
  return s;
}