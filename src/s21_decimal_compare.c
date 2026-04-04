// =========================== ОПЕРАТОРЫ СРАВНЕНИЯ  ===========================

#include "s21_decimal.h"



/*
 * Сравнение двух decimal чисел
 * Возвращает:
 *   0 — S21_OK (нормализация успешна, результат в cmp)
 *   4 — S21_NORMOLIZATION_ERROR (ошибка нормализации)
 * 
 * Результат сравнения передаётся через указатель cmp:
 *   1 — если a > b
 *   0 — если a == b
 *   -1 — если a < b
 */
int s21_compare(s21_decimal a, s21_decimal b, int *cmp) {
    // Получить знаки и масштабы
    int sign_a = s21_get_sign(a);
    int sign_b = s21_get_sign(b);
    int scale_a = s21_get_scale(a);
    int scale_b = s21_get_scale(b);

    // Если знаки разные
    if (sign_a != sign_b) {
        if (sign_a == 0 && sign_b == 1) {
            *cmp = 1;  // a > b
            return S21_OK;
        }
        if (sign_a == 1 && sign_b == 0) {
            *cmp = -1;  // a < b
            return S21_OK;
        }
    }
    
    // Нормализация масштабов
    if (scale_a != scale_b) {
        int norm_result = s21_normalize(&a, &b);
        if (norm_result != 0) {
            return S21_NORMOLIZATION_ERROR;
        }
    }
    
    // Сравнение мантисс
    if (a.bits[2] != b.bits[2]) {
        *cmp = (a.bits[2] > b.bits[2]) ? 1 : -1;
    } else if (a.bits[1] != b.bits[1]) {
        *cmp = (a.bits[1] > b.bits[1]) ? 1 : -1;
    } else if (a.bits[0] != b.bits[0]) {
        *cmp = (a.bits[0] > b.bits[0]) ? 1 : -1;
    } else {
        *cmp = 0;
    }
    
    // Если оба отрицательные — результат инвертировать
    if (sign_a == 1) {
        *cmp = -(*cmp);
    }
    
    return S21_OK;
}


// Сравнение абсолютных значений мантисс (без учёта знака и масштаба)
int s21_compare_abs(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] > b.bits[2]) ? 1 : -1;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] > b.bits[1]) ? 1 : -1;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] > b.bits[0]) ? 1 : -1;
    }
    return 0;
}



int s21_is_less(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp == -1;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp == 0;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp == 1;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp == -1 || cmp == 0;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp == 1 || cmp == 0;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
    int cmp;
    if (s21_compare(a, b, &cmp) != S21_OK) {
        return S21_FALSE;
    }
    return cmp != 0;
}
