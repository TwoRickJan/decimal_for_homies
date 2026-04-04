/*
 * test_normalize.c — тесты функций нормализации
 * Файл реализации: helpers/s21_decimal_normalize.c
 * Ответственный: Общий
 * Функции: normalize, remove_trailing_zeros, handle_zeros,
 *          handle_operation_overflow, finalize_result
 */

#include "../s21_decimal.h"
#include <check.h>

/*
    КАК ДОБАВЛЯТЬ ТЕСТЫ:

    1. Один тест = один кейс
    2. Не используйте test1, test2 — давайте осмысленные имена:
       test_normalize_basic
       test_remove_trailing_zeros
       test_handle_overflow
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
    Тесты для функции normalize
================================================*/

/*==============================================
    Тесты для функции remove_trailing_zeros
================================================*/

/*==============================================
    Тесты для функции handle_zeros
================================================*/

/*==============================================
    Тесты для функции handle_operation_overflow
================================================*/

/*==============================================
    Тесты для функции finalize_result
================================================*/

START_TEST(test_normalize_same_scale) {
    s21_decimal a = create_decimal(123, 0, 0, 2, 0);
    s21_decimal b = create_decimal(456, 0, 0, 2, 0);

    s21_decimal a_orig = a;
    s21_decimal b_orig = b;

    int result = s21_normalize(&a, &b);

    ck_assert_msg(result == S21_OK,
                  "result == %s, ожидалось %s",
                  result == S21_OK ? "S21_OK" : "S21_NORMOLIZATION_ERROR",
                  "S21_OK");
    ck_assert_int_eq(s21_get_scale(a), 2);
    ck_assert_int_eq(s21_get_scale(b), 2);
    assert_decimal_eq(a_orig, a);
    assert_decimal_eq(b_orig, b);
}
END_TEST

START_TEST(test_normalize_a_less_scale) {
    s21_decimal a = create_decimal(123, 0, 0, 2, 0);
    s21_decimal b = create_decimal(45, 0, 0, 0, 0);

    s21_decimal expected_b = create_decimal(4500, 0, 0, 2, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_msg(result == S21_OK,
                  "result == %s, ожидалось %s",
                  result == S21_OK ? "S21_OK" : "S21_NORMOLIZATION_ERROR",
                  "S21_OK");
    ck_assert_int_eq(s21_get_scale(a), 2);
    ck_assert_int_eq(s21_get_scale(b), 2);
    ck_assert_uint_eq(b.bits[0], 4500);
}
END_TEST

START_TEST(test_normalize_b_less_scale) {
    s21_decimal a = create_decimal(45, 0, 0, 0, 0);
    s21_decimal b = create_decimal(123, 0, 0, 2, 0);

    s21_decimal expected_a = create_decimal(4500, 0, 0, 2, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_int_eq(result, S21_OK);
    ck_assert_int_eq(s21_get_scale(a), 2);
    ck_assert_int_eq(s21_get_scale(b), 2);
    ck_assert_uint_eq(a.bits[0], 4500);
}
END_TEST

START_TEST(test_normalize_a_less_scale_multiple) {
    s21_decimal a = create_decimal(1, 0, 0, 0, 0);
    s21_decimal b = create_decimal(1, 0, 0, 4, 0);

    s21_decimal expected_a = create_decimal(10000, 0, 0, 4, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_int_eq(result, S21_OK);
    ck_assert_int_eq(s21_get_scale(a), 4);
    ck_assert_int_eq(s21_get_scale(b), 4);
    ck_assert_uint_eq(a.bits[0], 10000);
    ck_assert_uint_eq(b.bits[0], 1);
}
END_TEST

START_TEST(test_normalize_with_overflow_and_reduction) {
    s21_decimal a = create_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
    s21_decimal b = create_decimal(1, 0, 0, 28, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_msg(result == S21_OK,
                  "result == %s, ожидалось %s",
                  result == S21_OK ? "S21_OK" : "S21_NORMOLIZATION_ERROR",
                  "S21_OK");
    ck_assert_int_eq(s21_get_scale(a), s21_get_scale(b));
}
END_TEST

START_TEST(test_normalize_a_overflow_no_scale) {
    s21_decimal a = create_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
    s21_decimal b = create_decimal(1, 0, 0, 1, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_int_eq(result, S21_NORMOLIZATION_ERROR);
}
END_TEST

START_TEST(test_normalize_b_overflow_no_scale) {
    s21_decimal b = create_decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
    s21_decimal a = create_decimal(1, 0, 0, 1, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_int_eq(result, S21_NORMOLIZATION_ERROR);
}
END_TEST

START_TEST(test_normalize_with_negative_numbers) {
    s21_decimal a = create_decimal(123, 0, 0, 2, 1);
    s21_decimal b = create_decimal(45, 0, 0, 0, 0);

    int result = s21_normalize(&a, &b);

    ck_assert_int_eq(result, S21_OK);
    ck_assert_int_eq(s21_get_scale(a), 2);
    ck_assert_int_eq(s21_get_scale(b), 2);
    ck_assert_uint_eq(b.bits[0], 4500);
    ck_assert_int_eq(s21_get_sign(a), 1);
    ck_assert_int_eq(s21_get_sign(b), 0);
}
END_TEST

Suite *test_normolize_suite(void) {
    Suite *s = suite_create("normolize");

    TCase *normalize = tcase_create("normalize");
    tcase_add_test(normalize, test_normalize_same_scale);
    tcase_add_test(normalize, test_normalize_a_less_scale);
    tcase_add_test(normalize, test_normalize_b_less_scale);
    tcase_add_test(normalize, test_normalize_a_less_scale_multiple);
    tcase_add_test(normalize, test_normalize_with_overflow_and_reduction);
    tcase_add_test(normalize, test_normalize_a_overflow_no_scale);
    tcase_add_test(normalize, test_normalize_b_overflow_no_scale);
    tcase_add_test(normalize, test_normalize_with_negative_numbers);

    suite_add_tcase(s, normalize);

    return s;
}

int main(void) {
    Suite *s = test_normolize_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
