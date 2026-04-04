/*
 * test_div.c — тесты деления
 * Файл реализации: s21_decimal_arith.c
 * Ответственный: Человек 4
 * Функции: s21_div
 */

#include "../s21_decimal.h"
#include <check.h>

Suite *test_div_suite(void) {
    Suite *s = suite_create("div");
    return s;
}

int main(void) {
    Suite *s = test_div_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
