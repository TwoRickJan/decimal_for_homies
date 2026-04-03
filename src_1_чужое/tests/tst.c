#include "s21_decimal_tst.h"


START_TEST(s21_decimal_test_1) {
    s21_decimal decl = {0, 1, 2, 3};
    print_dec(decl);
}
END_TEST

START_TEST(s21_decimal_test_2) {
    print_float(5.0);
}
END_TEST

Suite *suite_s21_decimal_test(void) {
    Suite *s = suite_create("s21_decl");
    TCase *tc = tcase_create("Core");

    tcase_add_test(tc, s21_decimal_test_1);
    tcase_add_test(tc, s21_decimal_test_2);

    suite_add_tcase(s, tc);
    return s;
}