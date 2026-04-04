/*
 * test_round.c — тесты округления
 * Файл реализации: s21_decimal_round.c
 * Ответственный: Человек 2
 * Функции: s21_truncate, s21_floor, s21_round
 */

#include "../s21_decimal.h"
#include <check.h>

/*
    КАК ДОБАВЛЯТЬ ТЕСТЫ:

    1. Один тест = один кейс
    2. Не используйте test1, test2 — давайте осмысленные имена:
       test_add_positive
       test_add_negative
       test_add_overflow
    3. На каждый новый кейс — новый START_TEST
    4. Не забывайте добавлять новые тесты в свои подгруппы
*/

/*==============================================
    Вспомогательные функции для тестов
================================================*/

s21_decimal create_decimal(unsigned int low, unsigned int mid, unsigned int high, int scale, int sign) {
    s21_decimal d = {0};
    d.bits[0] = low;
    d.bits[1] = mid;
    d.bits[2] = high;
    s21_set_scale(&d, scale);
    s21_set_sign(&d, sign);
    return d;
}

void assert_decimal_eq(s21_decimal a, s21_decimal b) {
    ck_assert_msg(a.bits[0] == b.bits[0] &&
                  a.bits[1] == b.bits[1] &&
                  a.bits[2] == b.bits[2] &&
                  a.bits[3] == b.bits[3],
                  "Decimal не равны: {0x%X, 0x%X, 0x%X, 0x%X} != {0x%X, 0x%X, 0x%X, 0x%X}",
                  a.bits[0], a.bits[1], a.bits[2], a.bits[3],
                  b.bits[0], b.bits[1], b.bits[2], b.bits[3]);
}

/*==============================================
    Тесты для функций округления truncate
================================================*/

/*==============================================
    Тесты для функций округления floor
================================================*/

/*==============================================
    Тесты для функций округления round
================================================*/

/*==============================================
    Создание набора тестов
================================================*/

Suite *test_round_suite(void) {
    Suite *s = suite_create("round");

    TCase *truncate = tcase_create("truncate");

    TCase *floor_tc = tcase_create("floor");

    TCase *round_tc = tcase_create("round");

    suite_add_tcase(s, truncate);
    suite_add_tcase(s, floor_tc);
    suite_add_tcase(s, round_tc);

    return s;
}

int main(void) {
    Suite *s = test_round_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
