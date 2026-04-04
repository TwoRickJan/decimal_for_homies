# Teamwork Manual — s21_decimal 

## Состав команды

| Участник | Роль | Функции API (цель задания) | Файл реализации | Файл тестов |
|----------|------|---------------------------|-----------------|-------------|
| **Человек 1** | Конвертация | `from_int_to_decimal`, `from_float_to_decimal`, `to_int`, `to_float` | `s21_decimal_convert.c` | `test_convert.c` |
| **Человек 2** | Округление | `truncate`, `floor`, `round`, `negate` | `s21_decimal_round.c` | `test_round.c` |
| **Человек 3** | Арифметика (mul/mod) |  `mul`, `mod` | `s21_decimal_arith.c` | `test_arith.c` |
| **mlinaho (Арутр Артур)** | Арифметика (div/add/sub) | `add`, `sub`,`div` | `s21_decimal_arith.c` | `test_arith.c` |

> **Уже реализовано (не трогать):** `s21_decimal_compare.c` (все 6 сравнений + внутренние), все `helpers/*.c`, внутренние функции в `arith.c` и `round.c`

---

## ⚠️ Текущий статус

> **Сейчас есть баги в normalize** — из-за этого могут не проходить тесты со scale и overflow. Граничные случаи. Автор будет допиливать (может уже допилил).
> Перед каждой работой синхронизируйте ветку с `develop` (автор helpers оповестит, если что-то меняется).

---


## Оценка нагрузки

| Участник | Функции | Объём кода | Сложность | Почему |
|----------|---------|------------|-----------|--------|
| **Человек 1** | `from_int_to_decimal`, `from_float_to_decimal`, `to_int`, `to_float` | ~250 строк | ⭐⭐ Средняя | `from_int` и `to_int` — простые (битовые операции). `from_float` — парсинг IEEE 754, подбор scale, 7 значимых цифр. `to_float` — деление мантиссы. |
| **Человек 2** | `truncate`, `floor`, `round`, `negate` , Тесты`compare` и дебаг `compare`| ~150 строк | ⭐⭐ Лёгкая | Внутренние функции округления уже готовы в файле (`bank_round` , `round_mantissa`, `shift_right` могут быть ошибки). Осталось связать логику API. `negate` — 3 строки. |
| **Человек 3** |, `mul`, `mod` | ~200 строк | ⭐⭐⭐ Сложная |  `mul` — 192-битное умножение, масштаб, банковское округление. |
| **Человек 4** | `div`, `add` ✅, `sub` ✅ | ~200 строк | ⭐⭐⭐ Сложная | Long division 192-bit, управление scale до 28, деление на 0, округление.|

### Итог по балансу

| Участник | Готово | Осталось | Свободно |
|----------|--------|----------|----------|
| **Человек 1** | — | 4 функции | — |
| **Человек 2** | Внутренние round | 4 API-функции | Самая лёгкая нагрузка, потэтому допом тесты compare и дебаг |
| **Человек 3** |  | `mul`, `mod` | Средняя нагрузка |
| **Человек 4** | `add`, `sub` | `div` | Высокая концентрация на одной функции |
---


## Правила работы

### 1. Пишем код
- Стандарт C11, стиль Google Style
- Без `__int128`
- Код только в своём `.c` файле
- Если нужна внутренняя функция — используй `static` в своём `.c`

### 2. Комментарии
- Не менять чужие функции без согласования
- Не менять `s21_decimal.h` без согласования
- **НЕ ТРОГАТЬ HELPERS** без согласования вообще
- Перед работой чекайте комиты в develop и сливайте себе в ветку
- Ветки делайте так же от develop как на string
- В девелоп запрещается коммитить 

### 3. Сборка
После каждого изменения:
```bash
cd src
make clean && make all
```

### 4. Тесты

**Правила написания тестов:**
- Использовать библиотеку **Check**
- Обязательные тонкие случаи и переполнения
- Покрытие **≥ 80%** каждой функции
- Запуск: `make test`
- Отчёт покрытия: `make gcov_report` → папка `gcov_report/`

---

## Доступные helper функции

Все helper функции находятся в `helpers/` и доступны для использования:

```
s21_get_sign          s21_set_sign
s21_get_scale         s21_set_scale
s21_zero_decimal      s21_is_zero
s21_get_mantissa      s21_set_mantissa
s21_left_shift_one    s21_left_shift_mantissa
s21_right_shift_one
s21_add_mantissa      s21_sub_mantissa
s21_compare_abs
s21_add_192           s21_mul_192_by_32   s21_div_192_by_32
s21_multiply_by_10    s21_power_of_10
s21_divide_by_10      s21_divide_by_10_with_remainder
s21_divide_mantissa_by_power_of_10
s21_round_mantissa    s21_bank_round
s21_shift_right_mantissa_with_round
s21_normalize         s21_remove_trailing_zeros
s21_handle_zeros      s21_handle_operation_overflow
s21_finalize_result   s21_add_sub_mantissa
s21_compare
```

---