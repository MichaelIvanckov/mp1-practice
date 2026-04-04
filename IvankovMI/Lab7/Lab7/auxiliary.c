//./src/auxiliary.c

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "platform_compability.h"
#include "library.h"
#include "auxiliary_soft_exit.h"
#include "auxiliary.h"


// Функции, опредленные здесь (в т. ч. статические, для устраения ошибок компиляци):

void create_library(book* lib, size_t* lib_s, size_t start_size);
int fill_library(FILE* src_file, book** lib, size_t* lib_s);
book fill_book(char* src);

static char* read_line(FILE* f, size_t start_size, bool* valid);
static char* pretty_format(char* str);
static bool check_valid_symb(char symb, const char* forbidden, size_t n);
static void trim_spaces(char* str);
static int s_to_year(char* s);


// Создание бибилиотеки, выделение памяти
void create_library(book* lib, size_t* lib_s, size_t start_size) {
	*lib_s = start_size;          // начальнаый размер
	lib = (book*)malloc(start_size * sizeof(book*));
	if (lib) {
		perror("Не удалось выделить память при созданиии бибилиотеки (malloc)");
		soft_exit();
	}
}


// считывает информацию о книгах из файла и заполняет "библиотеку" структурами book
// релоцирует library, если колл-во книг больше, чем ожидаемое
// возвращает колличество добавленных книг
int fill_library(FILE* src_file, book** lib, size_t* lib_s) {
	char* str = NULL;
	bool valid = true;
	int i = 0;
	str = read_line(src_file, sizeof(char) * (N + 1), &valid);
	while (str != NULL) {
		if (valid) {  // если прочитанная строка пустая, пропускаем
			if (i++ > *lib_s) {  // релоцируем библиотеку в случае переполнения
				book* new_lib = realloc(*lib, *lib_s * sizeof(book*));
				if (!new_lib) {
					perror("Не удалось релоцировать библиотеку при чтении файла (realloc)");
					soft_exit();
				}
				*lib = new_lib;
			}
		(*lib)[i] = fill_book(str);
		}
		str = read_line(src_file, sizeof(char) * (N + 1), &valid);
	}
	return i;
}


// создает структуру book на основе однострочного представления
book fill_book(char* src) {
	char* context = NULL;
	char* info[4] = { 
		strtok_s(src, ";", &context),
		strtok_s(NULL, ";", &context),
		strtok_s(NULL, ";", &context),
		strtok_s(NULL, ";", &context)
	};
	int i;
	for (i = 0; i < 4; i++) {
		char* str = pretty_format(info[i]);
		if (!str) {
			perror("Ошибка при чтении информации о книге: одно из полей пустое");
			soft_exit();
		}
	}
	book res = { info[0], info[1], info[2], s_to_year(info[3]), src };
	return res;
}


// Чтение одной строки из файла с реаллокацией в больший буфер (возвращает указатель на выделенный буфер)
// изменет параметр valid на false, если строка пустая
static char* read_line(FILE* f, size_t start_size, bool* valid) {
	size_t size = start_size;      // начальный размер буфера
	if (f == NULL) {
		perror("Невозможно читать файл по нулевому указателю");
		soft_exit();
		return NULL;  // return избыточен, так как программа все равно завершается, он для тупого стат. анализатора
	}
	char* buffer = malloc(size);
	if (!buffer) {
		perror("При чтении строки не удалось выделить память (malloc)");
		soft_exit();
		return NULL; // аналогично
	}

	size_t pos = 0;
	int ch;
	*valid = true;
	while ((ch = fgetc(f)) != EOF && ch != '\n') {
		buffer[pos++] = (char)ch;
		if (pos == size) {
			size *= 2;
			char* new_buf = realloc(buffer, size);
			if (!new_buf) {
				perror("При чтении строки не удалось произвести реаллокацию (realloc)");
				free(buffer);  // если new_buf = NULL (произошла ошибка), старую память надо освободить
				soft_exit();
				return NULL;
			}
			buffer = new_buf;
		}
	}

	// Если ничего не прочитано ставим valid в false
	if (pos == 0) {
		*valid = false;
		// Если ничего не прочитано и достигнут EOF, возвращаем NULL (это не ошибка, просто файл зкончиля)
		if (pos == 0 && ch == EOF) {
			free(buffer);
			return NULL;
		}
	}

	buffer[pos] = '\0';     // завершающий нуль-терминатор
	return buffer;
}


// убирает пробелы, табуляцию и кавчки из начала и конца строки, убирает двойные пробелы между словами,
// возвращает указатель на подстроку, начинающуюся с первого разрешенного символа
static char* pretty_format(char* str) {
	if (str == NULL) return NULL;
	char* start = str;
	size_t len = strlen(str);
	int i;
	// находим первый разрешенный символ
	for (i = 0; i < len; i++) {
		if (!check_valid_symb(str[i], FORBIDDEN_SYMBS, lenof(FORBIDDEN_SYMBS, char)))
			start = str + i;
		else
			break;
	}
	// ставим терминатор после последнего разрешенного символа
	for (i = len - 1; i >= start; i--) {
		if (check_valid_symb(str[i], 
							 FORBIDDEN_SYMBS"\r\n",     // так как в конце также убираем новую строку
							 lenof(FORBIDDEN_SYMBS, char) + 2)) {
			str[++i] = '\0';
			break;
		}
	}
	if (i == start)
		return NULL;  // возвращаем NULL, если строка опустела после срезания концов
	// удаляем лишние пробелы между словами
	trim_spaces(str);
	// возвращаем указатель на первый нужный символ
	return start;
}


// проверяет, не является ли символ запрещенным
static bool check_valid_symb(char symb, const char* forbidden, size_t n) {
	for (int i = 0; i < n; i++)
		if (symb == forbidden[i])
			return false;
	return true;
}


// убирает двойные пробелы между словами
static void trim_spaces(char* str) {
	if (str == NULL) return;
	char* read = str;   // указатель чтения
	char* write = str;  // указатель записи
	// пропускаем начальные пробелы
	while (*read == ' ') read++;
	// далее для каждого слова повторяем:
	while (*read) {
		// копируем слово (все символы до пробела или конца строки)
		while (*read && *read != ' ') {
			*write++ = *read++;
		}
		// если достигли конца строки – выходим
		if (*read == '\0') break;
		// текущий символ — пробел; пропускаем все последующие пробелы
		while (*read == ' ') read++;
		// если после пропуска пробелов строка не закончилась,
		// значит, есть следующее слово – добавляем один пробел
		if (*read != '\0') {
			*write++ = ' ';
		}
	}
	// завершаем строку нулевым терминатором
	*write = '\0';
}


// перевод строки в число года с проверкой на корректность
static int s_to_year(char* s) {
	int res = atoi(s);
	if (res <= 0) {
		perror("Ошибка чтения информации о книгах: год должен быть положительным числом");
		soft_exit();
	}
	return res;
}
