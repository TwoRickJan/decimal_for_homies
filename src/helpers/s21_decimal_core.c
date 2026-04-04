/*
 * s21_decimal_core.c — базовые операции с decimal
 *
 * Функции:
 * - s21_get_sign        — получение знака
 * - s21_set_sign        — установка знака
 * - s21_get_scale       — получение масштаба
 * - s21_set_scale       — установка масштаба
 * - s21_get_mantissa    — получение мантиссы
 * - s21_set_mantissa    — установка мантиссы
 * - s21_zero_decimal    — обнуление
 * - s21_is_zero         — проверка на ноль
 * - s21_left_shift_one  — сдвиг влево на 1 бит
 * - s21_left_shift_mantissa — сдвиг влево на N бит
 * - s21_right_shift_one — сдвиг вправо на 1 бит
 * - s21_right_shift_mantissa — сдвиг вправо на N бит
 * - s21_get_bit         — получение бита по индексу
 * - s21_set_bit         — установка бита по индексу
 * - s21_is_greater_mantissa — сравнение мантисс (>)
 * - s21_is_less_mantissa  — сравнение мантисс (<)
 */

#include "../s21_decimal.h"
#include <assert.h>

// =========================== ЗНАК И МАСШТАБ ===========================

/*
 * s21_get_sign — получение знака (0 = положительный, 1 = отрицательный)
 */
int s21_get_sign(s21_decimal d) {
    //`>> 31` == 0000 0000 0000 0000 0000 0000 0000 000x
    return (d.bits[3] >> 31) & 1; // `& 1` - для подстраховки. Так то и после сдвига все занулиться 
}
// Установка знака (0 = положительный, 1 = отрицательный)
void s21_set_sign(s21_decimal *d, int sign) {
    if (sign) {
        d->bits[3] |= S21_MASK_BIT31;
    } else {
        d->bits[3] &= ~S21_MASK_BIT31;
    }
}

/*
 * s21_get_scale — получение масштаба (степень 10, биты 16-23)
 */
int s21_get_scale(s21_decimal d) {
    return (d.bits[3] >> 16) & S21_MASK_8BIT;
}

/*
 * s21_set_scale — установка масштаба (0-28)
 */
void s21_set_scale(s21_decimal *d, int scale) {
    if (scale < 0 || scale > 28) {
        return;  // или установить 0, или игнорировать
    }
    d->bits[3] = (d->bits[3] & ~(S21_MASK_8BIT << 16)) | ((scale & S21_MASK_8BIT) << 16);
}

// =========================== МАНТИССА ===========================

/*
 * s21_get_mantissa — извлечение 96-битной мантиссы (3 слова)
 */
void s21_get_mantissa(s21_decimal d, unsigned int *low, unsigned int *mid, unsigned int *high) {
    *low = d.bits[0];
    *mid = d.bits[1];
    *high = d.bits[2];
}

/*
 * s21_set_mantissa — установка 96-битной мантиссы из 3 слов
 */
void s21_set_mantissa(s21_decimal *d, unsigned int low, unsigned int mid, unsigned int high) {
    d->bits[0] = low;
    d->bits[1] = mid;
    d->bits[2] = high;
}

// =========================== ТИП: ОБНУЛЕНИЕ И ПРОВЕРКА ===========================

/*
 * s21_zero_decimal — обнуление всех битов decimal
 */
void s21_zero_decimal(s21_decimal *d) {
    d->bits[0] = 0;
    d->bits[1] = 0;
    d->bits[2] = 0;
    d->bits[3] = 0;
}

/*
 * s21_is_zero — проверка на ноль (мантисса == 0)
 */
int s21_is_zero(s21_decimal d) {
    return (d.bits[0] == 0 && d.bits[1] == 0 && d.bits[2] == 0);
}

// =========================== СДВИГИ ===========================

/*
 * s21_left_shift_one — сдвиг мантиссы влево на 1 бит
 * Возвращает 1 если был перенос из старшего бита (бит 95)
 */
int s21_left_shift_one(s21_decimal *d) {
    unsigned int overflow = 0;
    for (int j = 0; j < 3; ++j) {
        unsigned int new_overflow = (d->bits[j] >> 31) & 1;
        d->bits[j] = (d->bits[j] << 1) | overflow;
        overflow = new_overflow;
    }
    /*TO-DO Логика переполнения (НАВЕРНОЕ КАКТО ОПОВЕЩАТЬ НАДО ЧТО ВЫШЛО ЗА ГРАНИЦЫ)
    * пока будет считать как ни в чем не бывало
    */
    if (overflow) {
        d->bits[2] |= (1u << 31);
    }
    return overflow;
}
/*
 * Сдвиг мантиссы влево на shift бит 
 * по принципу bits[2] ← bits[1] ← bits[0]
 * @example shift = 5
 * Вход:  bits[0]=0b1000 0000 0000 0000 0000 0000 0000 0001
 *        bits[1]=0b1000 0000 0000 0000 0000 0000 0000 0001
 *        bits[2]=0b1000 0000 0000 0000 0000 0000 0000 0001
 * 
 * Выход: bits[0]=0b0000 0000 0000 0000 0000 0000 0010 0000  
 *        bits[1]=0b0000 0000 0000 0000 0000 0000 0011 0000  
 *        bits[2]=0b1000 0000 0000 0000 0000 0000 0011 0000  
 * 
 * @example shift = 31
 * Вход:  bits[0]=0b1111 1111 1111 1111 1111 1111 1111 1111
 *        bits[1]=0b0000 0000 0000 0000 0000 0000 0000 0000
 *        bits[2]=0b0000 0000 0000 0000 0000 0000 0000 0000
 * 
 * Выход: bits[0]=0b1000 0000 0000 0000 0000 0000 0000 0000
 *        bits[1]=0b0111 1111 1111 1111 1111 1111 1111 1111
 *        bits[2]=0b0000 0000 0000 0000 0000 0000 0000 0000
 * 
 */
/*
 * s21_left_shift_mantissa — сдвиг мантиссы влево на shift бит
 */
void s21_left_shift_mantissa(s21_decimal *d, int shift) {
    for (int i = 0; i < shift; ++i) {
        s21_left_shift_one(d);
    }
}

/*
 * s21_right_shift_one — сдвиг мантиссы вправо на 1 бит
 * Возвращает 1 если был потерян младший бит
 */
int s21_right_shift_one(s21_decimal *d) {
    unsigned int overflow = 0;
    for (int j = 2; j >= 0; --j) {
        unsigned int new_overflow = d->bits[j] & 1;          // сохраняем младший бит
        d->bits[j] = (d->bits[j] >> 1) | (overflow << 31);  // сдвигаем и добавляем бит справа
        overflow = new_overflow;                            // передаём в следующее слово
    }
    // Восстанавливать нельзя — это изменит значение (округлит вверх, а не вниз)
    // overflow показывает, был ли потерян бит (для округления)
    return overflow;
}

/*
 * Сдвиг мантиссы вправо на shift бит
 * по принципу bits[2] → bits[1] → bits[0]
 */
void s21_right_shift_mantissa(s21_decimal *d, int shift) {
    for (int i = 0; i < shift; ++i) {
        s21_right_shift_one(d);
    }
}

/*
 * Получение бита мантиссы по индексу
 * @param d decimal число
 * @param index индекс бита (0-95)
 * @return значение бита (0 или 1)
 * @note Индекс 0 соответствует младшему биту bits[0]
 * @warning index должен быть в диапазоне 0-95
 */
int s21_get_bit(s21_decimal d, int index) {
    assert(index >= 0 && index <= 95);  // проверка
    int word = index / 32; // целую часть получаем и на дробную сдвигаем
    int bit = index % 32; 
    return (d.bits[word] >> bit) & 1; // удаляем остатки через умнажение на 0b0001
}

// Установка бита мантиссы
void s21_set_bit(s21_decimal *d, int index, int bit) {
    assert(index >= 0 && index <= 95);  // проверка
    int word = index / 32;
    int bit_pos = index % 32;
    if (bit) {
        d->bits[word] |= (1u << bit_pos); //когда двигаем влево, важно что unsigned
    } else {
        d->bits[word] &= ~(1u << bit_pos);
    }
}
// Сравнение мантисс (a > b)
int s21_is_greater_mantissa(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] > b.bits[2]) ? 1 : 0;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] > b.bits[1]) ? 1 : 0;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] > b.bits[0]) ? 1 : 0;
    }
    return 0;
}

// Сравнение мантисс (a < b)
int s21_is_less_mantissa(s21_decimal a, s21_decimal b) {
    if (a.bits[2] != b.bits[2]) {
        return (a.bits[2] < b.bits[2]) ? 1 : 0;
    }
    if (a.bits[1] != b.bits[1]) {
        return (a.bits[1] < b.bits[1]) ? 1 : 0;
    }
    if (a.bits[0] != b.bits[0]) {
        return (a.bits[0] < b.bits[0]) ? 1 : 0;
    }
    return 0;
}

