/*
 * test_mul_mod.c — тесты умножения и остатка
 * Файл реализации: s21_decimal_arith.c
 * Ответственный: Человек 3
 * Функции: s21_mul, s21_mod
 */

#include "../s21_decimal.h"
#include <check.h>

Suite *test_mul_mod_suite(void) {
    Suite *s = suite_create("mul_mod");
    return s;
}

int main(void) {
    Suite *s = test_mul_mod_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
