#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "library.h"
#include "auxiliary.h"


// считывает информацию о книгах из файла и заполняет "библиотеку" структурами book
int fill_library(FILE* src_file, book* lib) {
	char* str = (char*)malloc(sizeof(char) * (N + 1));
	int err = 0;
	int i = 0;
	while (err != 0) {  // поменять
		err = fgets(str, N, src_file);
		lib[i++] = fill_book(str);
	}
	return i;
}


// создает структуру book на основе однострочного представления
book fill_book(char* src) {
	char* context = NULL;
	char* info[4] = { strtok_s(src, ";", &context),
		strtok_s(NULL, ";", &context),
		strtok_s(NULL, ";", &context) };
	int i;
	for (i = 0; i < 4; i++)
		info[i] = pretty_format(info[i]);
	book res = { info[0], info[1], info[2], s_to_year(info[3]), src };
	return res;
}


// убирает пробелы, табуляцию и кавчки из начала и конца строки, убирает двойные пробелы между словами
static char* pretty_format(char* str) {
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
	for (i = len - 1; i >= 0; i--) {
		if (check_valid_symb(str[i], FORBIDDEN_SYMBS, lenof(FORBIDDEN_SYMBS, char))) {
			str[++i] = '\0';
			break;
		}
	}
	// удаляем лишние пробелы между словами
	trim_spaces(str);
	// возвращаем указатель на первый нужный символ
	return start;
}


// проверяет, не является ли символ запрещенным
static bool check_valid_symb(char symb, const char* forbidden, size_t n) {
	for (int i; i < n; i++)
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
		exit(1);
	}
	return res;
}