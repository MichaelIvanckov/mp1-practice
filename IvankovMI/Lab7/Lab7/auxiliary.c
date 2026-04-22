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

void create_library(book** lib, size_t* lib_s, size_t start_size);
int fill_library(FILE* src_file, book** lib, size_t* lib_s);
book fill_book(char* src);
char* read_line(FILE* f, size_t start_size, bool* valid);

static char* pretty_format(char* str);
static bool check_valid_symb(char symb, const char* forbidden, size_t n);
static void trim_spaces(char* str);
static int s_to_year(char* s);



// Создание бибилиотеки, выделение памяти начального размера, изменение lib_s
void create_library(book** lib, size_t* lib_s, size_t start_size) {
	*lib_s = start_size;          // начальнаый размер
	*lib = (book*)malloc(start_size * sizeof(book));
	if (!*lib) {
		perror("Не удалось выделить память при созданиии бибилиотеки (malloc)");
		soft_exit();
	}
}


// Удаление библиотеки - освобождение памяти всех книг и установка указателя в NULL (на всякий случай)
void delete_library(book** lib, size_t* lib_s) {
	for (int i = 0; i < *lib_s; i++)
		free((*lib)[i].str);
	free(*lib);
	*lib = NULL;
	*lib_s = 0;
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
			if (i >= *lib_s) {  // релоцируем библиотеку в случае переполнения
				book* new_lib = (book*)realloc(*lib, *lib_s * sizeof(book) * 2);
				if (!new_lib) {
					perror("Не удалось релоцировать библиотеку при чтении файла (realloc)");
					soft_exit();
					return -1; // чтоб статический не ругался
				}
				*lib = new_lib;
				*lib_s *= 2;
			}
			book bk = fill_book(str);
			if (bk.name == NULL || bk.authors == NULL || bk.publ == NULL) {
				printf("ПРЕДУПРЕЖДЕНИЕ: Не удалось записать в библиотеку книгу с %d строчки в базе\n", i + 1);
				i++;
			}
			else if (bk.year == 0) {
				printf("ПРЕДУПРЕЖДЕНИЕ: Не удалось прочитать год издания книги в %d строчки в базе, книга пропущена\n", i + 1);
				i++;
			}
			else
				(*lib)[i++] = bk;
		}
		// free(str); - критическая ошибка, книга "забирает" себе выделенную строку и ее нельзя чистить
		str = read_line(src_file, sizeof(char) * (N + 1), &valid);
	}
	book* new_lib = (book*)realloc(*lib, i * sizeof(book));  // уменьшаем выделенную память обратно под колличество книг
	if (!new_lib) {                                          // да, не оптимально, но что поделать, зато кондово
		perror("Не удалось релоцировать библиотеку при освобождении лишнего (realloc)");
		soft_exit();
		return -1;
	}
	*lib = new_lib;
	*lib_s = i;
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
		if (!str) {  // str = NULL в случае отсутствия токена (strtok_s) или в случае "схлопывания" сторки после удаления пробелов (pretty_format)
			perror("Ошибка при чтении информации о книге: одно из полей отсутствует или пустое");
			if (FALL_FAST)
				soft_exit();
			else
				return (book) { NULL, NULL, NULL, 0, src };
		}
		info[i] = str;
	}
	book res = { info[0], info[1], info[2], s_to_year(info[3]), src };
	return res;
}


// Чтение одной строки из файла с реаллокацией в больший буфер (возвращает указатель на выделенный буфер)
// изменет параметр valid на false, если строка пустая
char* read_line(FILE* f, size_t start_size, bool* valid) {
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
	while ((ch = fgetc(f)) != EOF && ch != '\n' && ch != '\0') {
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
	if (str == NULL) return NULL;  // в случае NULL возвращаем NULL для проверок в fill_book
	char* start = str;
	size_t len = strlen(str);
	int i;
	// находим первый разрешенный символ
	for (i = 0; i < len; i++) {
		if (!check_valid_symb(str[i], FORBIDDEN_SYMBS, lenof(FORBIDDEN_SYMBS, char)))
			start = str + i + 1;
		else
			break;
	}
	// ставим терминатор после последнего разрешенного символа
	for (i = len - 1; i >= 0; i--) {
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
		if (FALL_FAST)
			soft_exit();
		else
			return 0;
	}
	return res;
}


// перевод строки в нижний регистр на месте для поиска
static void str_to_lower(char* s) {
	for (; *s; ++s) *s = tolower(*s);
}


// Токенизация строки с копированием с заданными разделителями.
// Возвращает массив указателей на копии токенов (в нижнем регистре),
// заканчивающийся NULL. Количество токенов сохраняется в *count.
char** tokenize(const char* str, const char* delimiters, int* count) {
	if (!str) return NULL;

	char* copy = _strdup(str);
	if (!copy) return NULL;

	char* saveptr = NULL;

	// Первый проход: подсчёт токенов
	int n = 0;
	char* token = strtok_s(copy, delimiters, &saveptr);
	while (token) {
		n++;
		token = strtok_s(NULL, delimiters, &saveptr);
	}

	if (n == 0) {
		free(copy);
		*count = 0;
		return NULL;
	}

	// Выделение памяти под массив указателей (n+1 для NULL)
	char** tokens = malloc((n + 1) * sizeof(char*));
	if (!tokens) {
		free(copy);
		return NULL;
	}

	// Второй проход: заполнение массива
	strcpy_s(copy, strlen(str) + 1, str);  // восстанавливаем исходную строку
	int i = 0;
	saveptr = NULL; // на всякий
	token = strtok_s(copy, delimiters, &saveptr);
	while (token) {
		tokens[i] = _strdup(token);
		
		if (!tokens[i]) {  //если ошибка при _strdup возвращаем NULL
			// Освобождаем уже выделенные токены
			for (int j = 0; j < i; ++j) free(tokens[j]);
			free(tokens);
			free(copy);
			soft_exit();
			return NULL;
		}
		str_to_lower(tokens[i]);
		i++;
		token = strtok_s(NULL, delimiters, &saveptr);
	}
	tokens[i] = NULL;
	*count = n;
	free(copy);
	return tokens;
}


// Освобождение массива токенов
void free_tokens(char** tokens) {
	if (!tokens) return;
	for (int i = 0; tokens[i]; ++i) free(tokens[i]);
	free(tokens);
}


// Проверка, является ли needle подстрокой haystack (без учёта регистра)
bool contains_ignore_case(const char* haystack, const char* needle) {
	char* h_lower = _strdup(haystack);
	char* n_lower = _strdup(needle);
	if (!h_lower || !n_lower) {
		free(h_lower);
		free(n_lower);
		return 0;
	}
	str_to_lower(h_lower);
	str_to_lower(n_lower);
	bool found = (strstr(h_lower, n_lower) != NULL);
	free(h_lower);
	free(n_lower);
	if (found != NULL)
		return true;
	else
		return false;
}
