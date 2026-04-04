/*
 * test_convert.c — тесты преобразователей
 * Файл реализации: s21_decimal_convert.c
 * Ответственный: Человек 1
 * Функции: s21_from_int_to_decimal, s21_from_float_to_decimal,
 *          s21_from_decimal_to_int, s21_from_decimal_to_float
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
    Тесты для преобразования from_int_to_decimal
================================================*/

/*==============================================
    Тесты для преобразования from_float_to_decimal
================================================*/

/*==============================================
    Тесты для преобразования from_decimal_to_int
================================================*/

/*==============================================
    Тесты для преобразования from_decimal_to_float
================================================*/

/*==============================================
    Создание набора тестов
================================================*/

Suite *test_convert_suite(void) {
    Suite *s = suite_create("convert");

    TCase *from_int = tcase_create("from_int");

    TCase *from_float = tcase_create("from_float");

    TCase *to_int = tcase_create("to_int");

    TCase *to_float = tcase_create("to_float");

    suite_add_tcase(s, from_int);
    suite_add_tcase(s, from_float);
    suite_add_tcase(s, to_int);
    suite_add_tcase(s, to_float);

    return s;
}

int main(void) {
    Suite *s = test_convert_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
