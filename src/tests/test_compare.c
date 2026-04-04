/*
 * test_compare.c — тесты операций сравнения
 * Файл реализации: s21_decimal_compare.c
 * Ответственный: Общий
 * Функции: s21_is_less, s21_is_less_or_equal, s21_is_greater,
 *          s21_is_greater_or_equal, s21_is_equal, s21_is_not_equal
 */

#include "../s21_decimal.h"
#include <check.h>

/*
    КАК ДОБАВЛЯТЬ ТЕСТЫ:

    1. Один тест = один кейс
    2. Не используйте test1, test2 — давайте осмысленные имена:
       test_is_less_positive
       test_is_equal_negative
       test_is_not_equal_boundary
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
    Тесты для сравнения is_less
================================================*/

/*==============================================
    Тесты для сравнения is_less_or_equal
================================================*/

/*==============================================
    Тесты для сравнения is_greater
================================================*/

/*==============================================
    Тесты для сравнения is_greater_or_equal
================================================*/

/*==============================================
    Тесты для сравнения is_equal
================================================*/


START_TEST(test_is_equal_equal) {
    s21_decimal a = create_decimal(12345, 0, 0, 2, 0);
    s21_decimal b = create_decimal(12345, 0, 0, 2, 0);

    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_different_scale) {
    s21_decimal a = create_decimal(12345, 0, 0, 2, 0);
    s21_decimal b = create_decimal(1234500, 0, 0, 4, 0);

    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_not_equal) {
    s21_decimal a = create_decimal(100, 0, 0, 0, 0);
    s21_decimal b = create_decimal(200, 0, 0, 0, 0);

    ck_assert_int_eq(s21_is_equal(a, b), S21_FALSE);
}
END_TEST

START_TEST(test_is_equal_negative) {
    s21_decimal a = create_decimal(100, 0, 0, 0, 1);
    s21_decimal b = create_decimal(100, 0, 0, 0, 1);

    ck_assert_int_eq(s21_is_equal(a, b), S21_TRUE);
}
END_TEST

START_TEST(test_is_equal_different_sign) {
    s21_decimal a = create_decimal(100, 0, 0, 0, 0);
    s21_decimal b = create_decimal(100, 0, 0, 0, 1);

    ck_assert_int_eq(s21_is_equal(a, b), S21_FALSE);
}
END_TEST

/*==============================================
    Создание набора тестов
================================================*/

Suite *test_compare_suite(void) {
    Suite *s = suite_create("compare");

    TCase *is_equal = tcase_create("is_equal");
    tcase_add_test(is_equal, test_is_equal_equal);
    tcase_add_test(is_equal, test_is_equal_different_scale);
    tcase_add_test(is_equal, test_is_equal_not_equal);
    tcase_add_test(is_equal, test_is_equal_negative);
    tcase_add_test(is_equal, test_is_equal_different_sign);

    suite_add_tcase(s, is_equal);

    TCase *is_not_equal = tcase_create("is_not_equal");

    TCase *is_less = tcase_create("is_less");

    TCase *is_less_or_equal = tcase_create("is_less_or_equal");

    TCase *is_greater = tcase_create("is_greater");

    TCase *is_greater_or_equal = tcase_create("is_greater_or_equal");

    suite_add_tcase(s, is_not_equal);
    suite_add_tcase(s, is_less);
    suite_add_tcase(s, is_less_or_equal);
    suite_add_tcase(s, is_greater);
    suite_add_tcase(s, is_greater_or_equal);

    return s;
}

int main(void) {
    Suite *s = test_compare_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
