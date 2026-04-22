//./src/library.c

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "platform_compability.h"
#include "auxiliary_soft_exit.h"
#include "auxiliary.h"
#include "library.h"


// Основные ресурсы: библиотека и ее длина

book* library;
size_t lib_size;






// создание библиотеки из информации из файла, работает с глобалами library
void init_library(char* path) {
	FILE* source = save_fopen(path, "r");
	create_library(&library, &lib_size, K);
	int sz = fill_library(source, &library, &lib_size);
	printf("Из текстовой базы получено %d книг\n", sz);
	soft_fclose(source);
	if (sz <= 0) {
		perror("Библиотека не может быть пустой");
		soft_exit();
	}
}


// Удаление библиотеки - освобождение памяти всех книг, работает с глобалами library
void del_library_glob() {
	delete_library(&library, &lib_size);
}


// Основная функция поиска, возвращает указатель на массив указателей на книги в куче, изменяет f_cnt на кол-во найденных книг, завершает массив NULL
book** find_books(book* lib, size_t size, const char* substr, size_t* f_cnt) {
	// Разделители: пробельные символы и знаки пунктуации
	const char* delimiters = DELIMS;

	// Токенизация запроса
	int query_cnt;
	char** query_tokens = tokenize(substr, delimiters, &query_cnt);
	if (!query_tokens || query_cnt == 0) {
		if (query_tokens) free_tokens(query_tokens);
		return NULL;   // пустой запрос не считается совпадением
	}

	book** result = (book**)calloc(K, sizeof(book*)); // чтобы все лишние были нулями на всякий случай
	if (!result) {
		perror("Не удалось выделить память под список найденных книг (calloc)");
		soft_exit();
		return NULL; // чтоб статический не ругался
	}
	size_t res_l = K;
	size_t res_cnt = 0;

	for (size_t i = 0; i < size; ++i) {
		if (!lib[i].authors) continue;  // хотя такого по идее не должно быть

		int str_cnt;  // кол-во токенов в найденной строке
		char** str_tokens = tokenize(lib[i].authors, delimiters, &str_cnt);
		if (!str_tokens || str_cnt == 0) {
			if (str_tokens) free_tokens(str_tokens);
			continue;
		}

		// Проверяем, что все токены запроса встречаются среди токенов "найденной" строки
		int q_idx = 0;
		bool total_ok = true;   // по умолчанию, считаем запрос удовлетворенным
		for (; q_idx < query_cnt; ++q_idx) {
			bool ok = false;    // по умолч. мы пока не нашли подходящ. слов
			for (int s_idx = 0; s_idx < str_cnt; ++s_idx) {
				bool t_ok = contains_ignore_case(str_tokens[s_idx], query_tokens[q_idx]);
				if (t_ok)       // если хоть одно слово подошло, 
					ok = true;  // считаем данный токен запроса удовлетворенным
			}
			if (!ok)               // если хоть один токен запроса неудовлеворен,
				total_ok = false;  // считаем запрос неудовлетворенным
		}

		if (total_ok) {
			if (i >= res_l) {
				res_l *= 2;      // увеличим res_l
				result = (book**)realloc(result, res_l * sizeof(book*)); // и перенесем данные
				if (result == NULL) {
					perror("Не удалось релоцировать список найденных книг при составлении (realloc)");
					soft_exit();
					return NULL; // чтоб статический не ругался
				}
			}
			result[res_cnt] = &lib[i];
			res_cnt++;
			//free_tokens(str_tokens);
			//break;
		}
		free_tokens(str_tokens);
	}
	*f_cnt = res_cnt;
	free_tokens(query_tokens);
	//book* DEBB[10] = result; // не получилось
	return result;
}


// вывести инфо книги
static void print_book(book* bk) {
	printf("Название: %s\nАвтор(ы): %s\nИздательство: %s\nГод издания: %d\n\n", bk->name, bk->authors, bk->publ, bk->year);
}


// вывести инфо каждой книги из массива
void print_books(book** bks, size_t cnt) {
	for (int i = 0; (i < cnt) && (bks[i] != NULL); ++i) {
		print_book(bks[i]);
		bool deb1 = i + 1 < cnt;
		bool deb2 = bks[i + 1];
		bool deb3 = false;
	}
}


// тестовая функция для вывода книги по номеру
void test(book* lib, int n) {
	print_book(lib + n);
}

// тестовая функция для вывода всех книг
void test_all(book* lib, size_t n) {
	for (size_t i = 0; i < n; i++)
		print_book(lib + i);
}


// Вопрос в stdin о пути файла базы данных и заполнение библиотеки
void start_ask() {
	printf("Введите путь к файлу библиотеки:\n");
	bool valid = true;
	char* input = read_line(stdin, M, &valid);
	while (valid == false && input != NULL) {
		free(input);
		input = read_line(stdin, M, &valid);
	}
	if (input == NULL) {  // read_line возвращает NULL только в случае невозможности чтения из-за EOF
		perror("Не удалось прочитать пользовательский ввод пути (достигнут EOF)");
		soft_exit();
		return; // чтобы статический не ругался
	}
	init_library(input);
}


// Обработка запроса из stdin, возвращает false, если требуется выйти, работает с глобалами library
bool process_query() {
	printf("Введите подстроку для поиска книги по автору\n(можно несколько слов, например 'Пушкин А. С.' найдет книги с автором 'Пушкин Александр Сергеевич'):\n");
	bool valid = true;
	char* input = read_line(stdin, M, &valid);
	while (valid == false && input != NULL) {
		free(input);    // очищаем просто "\0", но что уж поделать
		input = read_line(stdin, M, &valid);
	}
	if (input == NULL) {  // read_line возвращает NULL только в случае невозможности чтения из-за EOF
		perror("Не удалось прочитать пользовательский запрос (достигнут EOF)");
		soft_exit();
		return false; // чтобы статический не ругался
	}
	if (strcmp(input, "exit") == 0 || strcmp(input, "выход") == 0) {
		free(input);
		return false;
	}
	int test_n;
	if (sscanf_s(input, "test %d", &test_n)) {  // по кодовому слову выводим книгу по номеру
		test(library, test_n);
		return true;
	}
	if (strcmp(input, "test all") == 0)
		test_all(library, lib_size);  // по другому кодовому слову выводим все книги
	size_t res_l;
	book** result = find_books(library, lib_size, input, &res_l);
	if (res_l) {
		printf("По вашему запросу найдено %zu книг:\n\n", res_l);
		print_books(result, res_l);
	}
	else
		printf("По вашему запросу ничего не найдено\n");
	free(input);
	free(result);
	return true;
}
