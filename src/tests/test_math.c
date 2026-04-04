/*
 * test_math.c — тесты helper функций (математика мантисс)
 * Файл реализации: helpers/s21_decimal_math.c, helpers/s21_decimal_core.c
 * Ответственный: Общий
 * Функции: арифметика мантисс, 192-битные операции, битовые сдвиги
 */

#include "../s21_decimal.h"
#include <check.h>

/*
    КАК ДОБАВЛЯТЬ ТЕСТЫ:

    1. Один тест = один кейс
    2. Не используйте test1, test2 — давайте осмысленные имена:
       test_add_mantissa_basic
       test_shift_left_one
       test_div_192_by_32_remainder
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
    Тесты для функции add_mantissa / sub_mantissa
================================================*/

/*==============================================
    Тесты для функции compare_abs
================================================*/

/*==============================================
    Тесты для функции left_shift / right_shift
================================================*/

/*==============================================
    Тесты для функции multiply_by_10 / divide_by_10
================================================*/

/*==============================================
    Тесты для функции add_192 / mul_192_by_32 / div_192_by_32
================================================*/

/*==============================================
    Создание набора тестов
================================================*/

Suite *test_math_suite(void) {
    Suite *s = suite_create("math");

    TCase *mantissa_arith = tcase_create("mantissa_arith");

    TCase *compare_abs = tcase_create("compare_abs");

    TCase *shift_ops = tcase_create("shift");

    TCase *power_of_10 = tcase_create("power_of_10");

    TCase *ops_192 = tcase_create("ops_192");

    suite_add_tcase(s, mantissa_arith);
    suite_add_tcase(s, compare_abs);
    suite_add_tcase(s, shift_ops);
    suite_add_tcase(s, power_of_10);
    suite_add_tcase(s, ops_192);

    return s;
}

int main(void) {
    Suite *s = test_math_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
