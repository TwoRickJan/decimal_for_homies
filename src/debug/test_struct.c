#include "../s21_decimal.h"
#include <stdio.h>

void print_decimal(s21_decimal d) {
    // Создаём копию
    s21_decimal temp = d;
    s21_set_scale(&temp, 0);
    s21_set_sign(&temp, 0);
    
    // Массив для цифр
    char digits[100] = {0};
    int count = 0;
    
    // Делим на 10, пока не станет 0
    while (!s21_is_zero(temp)) {
        int remainder = s21_divide_by_10_with_remainder(&temp);
        digits[count++] = '0' + remainder;
    }
    
    if (count == 0) {
        printf("0");
        return;
    }
    
    // Выводим в обратном порядке
    for (int i = count - 1; i >= 0; i--) {
        putchar(digits[i]);
    }
}
int main()
{
    s21_decimal max = {0};
    max.bits[0] = 0xFFFFFFFFu;
    max.bits[1] = 0xFFFFFFFFu;
    max.bits[2] = 0xFFFFFFFFu;

    print_decimal(max);
    return 0;
}