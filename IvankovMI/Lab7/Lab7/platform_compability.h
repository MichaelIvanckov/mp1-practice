#pragma once

// Для компиляторов не MSVC используем strtok_r - POSIX-стандарт потокобезопасной функции
// А также strcpy (с "проглатыванием" одного параметра) вместо strcpy_s
// И strdup вместо его "MSVC-псевдонима" _strdup
#if !defined(_MSC_VER) || defined(__clang__)
// Отдельно проверяем clang, так как он определяет _MSVC_VER для совместимости
#define strtok_s strtok_r
#define strcpy_s(_Destination, _Fake_SizeInBytes, _Source) strcpy(_Destination, _Source)
#define _strdup strdup
#endif
