//./src/auxiliary_soft_exit.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "platform_compability.h"

// макс кол-во открытых файлов (нужно дл€ м€гкого аварийного выхода из программы) 
#define MAX_FILES 20

// безопасное завершение программы с освобождением всех ресурсов с кодом выхода 1
void soft_exit();

// безопасное завершение программы с освобождением всех ресурсов с заданным кодом выхода
void soft_exit_no(errno_t errcode);

// безопасное кроссплатформенное открытие файла, позвол€ющее потом его безопасно закрыть
// через soft_fclose или soft_exit/soft_exit_no
FILE* save_fopen(const char* filename, const char* mode);

// безопасное закрытие файла, исключающее повторное закрытие
void soft_fclose(FILE* file);
