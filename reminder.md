# Qwen Code + Ollama — Напоминание и Инструкция

## Быстрые команды

```bash
qwen-local    # запустить с локальной моделью Ollama
qwen-cloud    # запустить с облачной моделью
qwen-status   # проверить текущую конфигурацию
```

---

## Ключевые файлы и пути

| Файл | Назначение |
|------|-----------|
| `~/.env` | Переменные окружения для Ollama |
| `~/.qwen/settings.json` | Настройки Qwen Code + MCP серверы |
| `~/.bashrc` | Алиасы (qwen-local, qwen-cloud, qwen-status) |
| `~/docs/QWEN.md` | Правила поведения Qwen Code |
| `~/docs/OLLAMA_WORKFLOW.md` | Инструкция по взаимодействию |
| `~/docs/decimal_for_homies/README_for_Qwen.md` | Документация проекта s21_decimal |

---

## Модели

| Модель | Размер | Назначение |
|--------|--------|------------|
| `qwen2.5-coder:14b` | ~9 GB | Генерация кода |
| `deepseek-r1:14b` | ~9 GB | Ревью кода, поиск багов, исправления |

## Роль распределение

| Роль | Модель | Расход токенов |
|------|--------|----------------|
| **Координатор** | Qwen Cloud (облачный) | ~3K на цикл |
| **Генерация кода** | qwen2.5-coder:14b | Бесплатно (GPU) |
| **Ревью кода** | deepseek-r1:14b | Бесплатно (GPU) |

---

## Правила работы с локальными моделями

### КРИТИЧЕСКОЕ: Разбивай задачи на подзадачи

- **НЕ** отправляй один промпт на все файлы сразу
- Работай **по одному файлу** за раз
- Проверяй результат → переходи к следующему

### Порядок обработки файлов проекта

1. `s21_decimal.h` — заголовочный файл (первый!)
2. `s21_decimal_arith.c` — арифметика
3. `s21_decimal_compare.c` — сравнение
4. `s21_decimal_convert.c` — конвертация
5. `s21_decimal_round.c` — округление
6. `helpers/s21_decimal_core.c` — базовые операции
7. `helpers/s21_decimal_math.c` — математика
8. `helpers/s21_decimal_normalize.c` — нормализация

---

## Полная инструкция по настройке

### Шаг 1: Проверить Ollama

```bash
# Убедиться что Ollama запущен
ollama list

# Если не запущен:
ollama serve

# Проверить доступные модели
ollama list
```

### Шаг 2: Скачать модели

```bash
# Генерация кода
ollama pull qwen2.5-coder:14b

# Ревью кода
ollama pull deepseek-r1:14b
```

### Шаг 3: Проверить эндпоинт

```bash
curl http://localhost:11434/api/tags
```

Должен вернуть JSON со списком моделей.

### Шаг 4: Настроить ~/.qwen/settings.json

```json
{
  "model": {
    "name": "qwen2.5-coder:14b"
  },
  "mcpServers": {
    "ollama-local": {
      "httpUrl": "http://localhost:11434/v1",
      "env": {
        "OPENAI_BASE_URL": "http://localhost:11434/v1",
        "OPENAI_API_KEY": "ollama"
      }
    }
  },
  "$version": 3
}
```

### Шаг 5: Добавить алиасы в ~/.bashrc

```bash
# В конец ~/.bashrc:
alias qwen-local='source ~/.env && echo "[LOCAL] Using Ollama model: $OPENAI_MODEL" && qwen'
alias qwen-cloud='unset OPENAI_BASE_URL OPENAI_API_KEY OPENAI_MODEL && echo "[CLOUD] Using cloud model" && qwen'
alias qwen-status='if [ -n "$OPENAI_BASE_URL" ]; then echo "[LOCAL] Ollama: $OPENAI_MODEL ($OPENAI_BASE_URL)"; else echo "[CLOUD] Cloud model (OAuth)"; fi'
```

### Шаг 6: Применить алиасы

```bash
source ~/.bashrc
```

---

## Как пользоваться

### Локальный режим (Ollama)
```bash
qwen-local
```
- Использует `qwen2.5-coder:14b` локально
- Приватно, бесплатно
- Требует запущенный Ollama

### Облачный режим (платный Qwen)
```bash
qwen-cloud
```
- Использует облачную модель через OAuth
- Мощнее, но платный
- Нужен интернет

### Проверить режим
```bash
qwen-status
```

---

## Алгоритм полной настройки с нуля

1. **Установить Ollama** (если ещё нет):
   ```bash
   curl -fsSL https://ollama.com/install.sh | sh
   ```

2. **Запустить Ollama**:
   ```bash
   ollama serve
   ```

3. **Скачать модели**:
   ```bash
   ollama pull qwen2.5-coder:14b
   ollama pull deepseek-r1:14b
   ```

4. **Проверить эндпоинт**:
   ```bash
   curl http://localhost:11434/api/tags
   ```

5. **Настроить `~/.qwen/settings.json`** (см. Шаг 4)

6. **Добавить алиасы в `~/.bashrc`** (см. Шаг 5)

7. **Применить**:
   ```bash
   source ~/.bashrc
   ```

8. **Проверить**:
   ```bash
   qwen-status
   qwen-local
   ```

---

## Решение проблем

### Ollama не запускается
```bash
# Проверить занятость порта
ss -tlnp | grep 11434

# Убить процесс
pkill ollama
ollama serve
```

### Модель не отвечает
```bash
# Проверить список моделей
ollama list

# Перезапустить Ollama
pkill ollama && ollama serve

# Протестировать напрямую
ollama run qwen2.5-coder:14b "test"
```

### Qwen Code не подключается
```bash
# Проверить переменные
env | grep OPENAI

# Проверить curl
curl http://localhost:11434/api/tags

# Перезапустить Qwen Code
qwen-cloud  # потом qwen-local
```

### Ollama не использует GPU
```bash
# Проверить GPU
nvidia-smi

# Если нет вывода — драйверы не стоят
# Windows: обновить с nvidia.com

# Перезапустить Ollama
pkill ollama && ollama serve
```

---

*Дата создания: 2026-04-04*
*Среда: WSL Ubuntu | VS Code | Qwen Code | Ollama | GPU RTX 5060 Ti 16GB*
