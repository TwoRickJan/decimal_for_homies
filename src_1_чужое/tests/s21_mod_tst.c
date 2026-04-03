#include "s21_decimal_tst.h"

START_TEST(s21_mod_int_tests) {
    int a = (int)Rand_R(-10e5, 10e5);
    int b = (int)Rand_R(-10e5, 10e5);
    int c = 0;
    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;
    s21_from_int_to_decimal(a, &_a);
    s21_from_int_to_decimal(b, &_b);
    s21_mod(_a, _b, &res);
    s21_from_decimal_to_int(res, &c);

    ck_assert_int_eq(a % b, c);
}
END_TEST

START_TEST(s21_mod_float_standard_tests) {
    float a = Rand_R(-10e3, 10e3);
    float b = Rand_R(-10e3, 10e3);
    float c = 0;
    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;
    s21_from_float_to_decimal(a, &_a);
    s21_from_float_to_decimal(b, &_b);
    s21_mod(_a, _b, &res);
    s21_from_decimal_to_float(res, &c);
    if (fmod(a, b) != c)
        printf("%f %f | %f %f\n", a, b, fmod(a, b), c);
    ck_assert_float_eq(fmod(a, b), c);
}
END_TEST

Suite *suite_s21_mod_ste(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("s21_mod");
    tc = tcase_create("case_div");

    tcase_add_loop_test(tc, s21_mod_int_tests, 0, 1000);
    tcase_add_loop_test(tc, s21_mod_float_standard_tests, 0, 10);



    suite_add_tcase(s, tc);
    return s;
}
