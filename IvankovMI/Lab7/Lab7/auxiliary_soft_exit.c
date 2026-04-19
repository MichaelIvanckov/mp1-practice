//./src/auxiliary_soft_exit.c

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "platform_compability.h"
#include "auxiliary_soft_exit.h"


// Глобальные состояния в рамкх всего процеса, реализуемые этим модулем:

// ссылки на все открытые файлы для мягкого закрытия при аварийном выходе
static FILE* opend_resources[MAX_FILES];

// информация о закрытых файлах для избежания овторного закрытия
static bool closed_resources[MAX_FILES];

// имена открытых файлов для вывода сообщения об ошибке
static char* file_names[MAX_FILES];

// счетчик открытых когда-либо файлов 
static unsigned files_counter = 0;


// Функции, опредленные здесь (в т. ч. статические, для устраения ошибок компиляци):

void soft_exit();
void soft_exit_no(errno_t errcode);
FILE* save_fopen(const char* filename, const char* mode);
void soft_fclose(FILE* file);

static bool file_cmp(FILE* one, FILE* another);
static void soft_i_fclose(int i);


// безопасное завершение программы с освобождением всех ресурсов с кодом выхода 1
void soft_exit() {
	soft_exit_no(EXIT_FAILURE);
}


// безопасное завершение программы с освобождением всех ресурсов с заданным кодом выхода
void soft_exit_no(errno_t errcode) {
	for (int i = 0; i < files_counter; i++)
		soft_i_fclose(i);
	exit(errcode);
}


// безопасное кроссплатформенное открытие файла, позволяющее потом его безопасно закрыть
// через soft_fclose или soft_exit/soft_exit_no
FILE* save_fopen(const char* filename, const char* mode) {
	FILE* file = NULL;
	errno_t err = 0;
	if (files_counter + 1 >= MAX_FILES) {
		perror("Превышен лимит открытых файлов");
		soft_exit();
	}
#if defined(_WIN32) && defined(_MSC_VER)
	err = fopen_s(&file, filename, mode);
#else
	file = fopen(filename, mode);
#endif
	if (file == NULL || err != 0) {
		perror("Не удалось открыть файл");
#if defined(_WIN32) && defined(_MSC_VER)
		soft_exit_no(err);
#else
		soft_exit();
#endif
	}
	opend_resources[files_counter] = file;
	closed_resources[files_counter] = false;
	files_counter++;
	return file;
}


// сравнение указателей на файл для мягкого закрытия (может быть не стабильно)
static bool file_cmp(FILE* one, FILE* another) {
	return (bool)(one->_Placeholder == another->_Placeholder);
}


// безопасное закрытие файла, исключающее повторное закрытие
void soft_fclose(FILE* file) {
	for (int i = 0; i < files_counter; i++)
		if (file_cmp(file, opend_resources[i]))
			soft_i_fclose(i);
}


// безопасное закрытие файла по номеру из opend_resources
static void soft_i_fclose(int i) {
	if (!closed_resources[i]) {
		closed_resources[i] = true;
		if (fclose(opend_resources[i]) != 0) {
			perror("Не удалось закрыть файл");
			soft_exit();
		}
	}
}
