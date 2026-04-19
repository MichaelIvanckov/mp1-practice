#pragma once

// Для компиляторов не MSVC используем strtok_r - POSIX-стандарт потокобезопасной функции
#if !defined(_MSC_VER) || defined(__clang__)
// Отдельно проверяем clang, так как он определяет _MSVC_VER для совместимости
#define strtok_s strtok_r
#endif
