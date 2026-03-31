//./src/auxiliary_soft_exit.c

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "auxiliary_soft_exit.h"


// √лобальные состо€ни€ в рамкх всего процеса, реализуемые этим модулем:

// ссылки на все открытые файлы дл€ м€гкого закрыти€ при аварийном выходе
static FILE* opend_resources[MAX_FILES];

// информаци€ о закрытых файлах дл€ избежани€ овторного закрыти€
static bool closed_resources[MAX_FILES];

// имена открытых файлов дл€ вывода сообщени€ об ошибке
static char* file_names[MAX_FILES];

// счетчик открытых когда-либо файлов 
static unsigned files_counter = 0;


// ‘ункции, опредленные здесь (в т. ч. статические, дл€ устраени€ ошибок компил€ци):

void soft_exit();
void soft_exit_no(errno_t errcode);
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


// сравнение указателей на файл дл€ м€гкого закрыти€ (может быть не стабильно)
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
			perror("Ќе удалось закрыть файл");
			soft_exit();
		}
	}
}