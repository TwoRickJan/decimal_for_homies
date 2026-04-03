#include "s21_decimal_tst.h"

START_TEST(s21_div_tests_1) {
    int a = 5; // (int)Rand_R(-10e5, 10e1);
    int b = 2; ///(int)Rand_R(-10e5, 10e1);
    printf("a, b: %d %d\n", a, b);
    int c = -1;
    printf("c: %d\n", c);
    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res; // = {{0,0,0,0}, 0};
    //print_dec(res);
    s21_from_int_to_decimal(a, &_a);
    s21_from_int_to_decimal(b, &_b);
    s21_div(_a, _b, &res);
    s21_from_decimal_to_int(res, &c);
    //print_dec(res);
    // printf("c: %d\n", c);

    // printf("%d + %d ||||| %d\n", a, b, c);

    ck_assert_int_eq(a / b, c);
}
END_TEST

START_TEST(s21_div_tests_2) {
    int a = (int)Rand_R(-1e5, 1e5);
    int b = (int)Rand_R(-1e5, 1e5);
    
    int c = 0;

    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;

    s21_from_int_to_decimal(a, &_a);
    s21_from_int_to_decimal(b, &_b);
    s21_div(_a, _b, &res);
    s21_from_decimal_to_int(res, &c);

    if (c != a / b)
        printf("%d %d | %d %d\n", a, b, a / b, c);
    ck_assert_int_eq(a / b, c);
}
END_TEST

START_TEST(s21_div_tests_3) {
    float a = 0.6; //Rand_R(-1e2, 1e2);
    float b = 3.0;//Rand_R(-1e2, 1e2);
    
    float c = 0;

    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;

    s21_from_float_to_decimal(a, &_a);
    s21_from_float_to_decimal(b, &_b);
    s21_div(_a, _b, &res);
    s21_from_decimal_to_float(res, &c);

    if (c != a / b)
        printf("%f %f | %f %f\n", a, b, a / b, c);
    ck_assert_float_eq(a / b, c);
}
END_TEST

START_TEST(s21_div_tests_4) {
    float a = 6; //Rand_R(-1e2, 1e2);
    float b = 0.3;//Rand_R(-1e2, 1e2);
    
    float c = 0;

    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;

    s21_from_float_to_decimal(a, &_a);
    s21_from_float_to_decimal(b, &_b);
    s21_div(_a, _b, &res);
    s21_from_decimal_to_float(res, &c);

    if (c != a / b)
        printf("%f %f | %f %f\n", a, b, a / b, c);
    ck_assert_float_eq(a / b, c);
}
END_TEST

START_TEST(s21_div_tests_5) {
    float a = 1; //Rand_R(-1e2, 1e2);
    float b = 0.2;//Rand_R(-1e2, 1e2);
    
    float c = 0;

    s21_decimal _a;
    s21_decimal _b;
    s21_decimal res;

    s21_from_float_to_decimal(a, &_a);
    s21_from_float_to_decimal(b, &_b);
    s21_div(_a, _b, &res);

    s21_from_decimal_to_float(res, &c);

    if (c != a / b)
        printf("%f %f | %f %f\n", a, b, a / b, c);
    ck_assert_float_eq(a / b, c);
}
END_TEST

Suite *suite_s21_div_ste(void) {
    Suite *s;
    TCase *tc;
    s = suite_create("s21_div");
    tc = tcase_create("case_div");

    // tcase_add_loop_test(tc, s21_div_tests_1, 0, 1);
    // tcase_add_loop_test(tc, s21_div_tests_2, 0, 10);
    // tcase_add_loop_test(tc, s21_div_tests_3, 0, 1);
    // tcase_add_loop_test(tc, s21_div_tests_4, 0, 1);
    tcase_add_loop_test(tc, s21_div_tests_5, 0, 1);
    // tcase_add_loop_test(tc, s21_mul_tests_4, 0, 1000);
    // tcase_add_loop_test(tc, s21_mul_tests_5, 0, 1000);
    // tcase_div_test(tc, s21_mul_tests_6);


    suite_add_tcase(s, tc);
    return s;
}
