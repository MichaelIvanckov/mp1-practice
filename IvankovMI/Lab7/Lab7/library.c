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

library_t library_; // не используется, можно убрать




// создание библиотеки из информации из файла, работает с глобалами library
void init_library(char* path) {
	FILE* source = save_fopen(path, "r");
	create_library(&library, &lib_size, K);
	int sz = fill_library(source, &library, &lib_size);
	printf("Из текстовой базы получено %d книг", sz);
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
	size_t res_l = K * sizeof(book*);

	for (size_t i = 0; i < size; ++i) {
		if (!lib[i].authors) continue;

		int str_cnt;  // кол-во токенов в найденной строке
		char** str_tokens = tokenize(lib[i].authors, delimiters, &str_cnt);
		if (!str_tokens || str_cnt == 0) {
			if (str_tokens) free_tokens(str_tokens);
			continue;
		}

		// Проверяем, что все токены запроса встречаются среди токенов "найденной" строки
		int q_idx = 0;
		bool total_ok = true;
		for (; q_idx < query_cnt; ++q_idx) {
			bool ok = true;
			for (int s_idx = 0; s_idx < str_cnt; ++s_idx)
				ok = contains_ignore_case(str_tokens[s_idx], query_tokens[q_idx]);
			if (ok)
				q_idx++;   // переходим к следующему токену запроса
		}

		if (total_ok) {
			if (i >= res_l) {
				result = (book**)realloc(result, res_l *= sizeof(book*) * 2); // увеличим res_l
				if (!result) {
					perror("Не удалось релоцировать список найденных книг при составлении (realloc)");
					soft_exit();
					return NULL; // чтоб статический не ругался
				}
			}
			result[i] = &lib[i];
			free_tokens(str_tokens);
			break;
		}
		free_tokens(str_tokens);
	}

	free_tokens(query_tokens);
	return result;
}


// вывести инфо книги
static void print_book(book* bk) {
	printf("Название: %s\nАвтор(ы): %s\n Издательство: %s\nГод издания %d\n\n", bk->name, bk->authors, bk->publ, bk->year);
}


// вывести инфо каждой книги из массива
void print_books(book** bks, size_t cnt) {
	for (int i = 0; i < cnt && !bks[i]; ++i)
		print_book(bks[i]);
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
	size_t res_l;
	book** result = find_books(library, lib_size, input, &res_l);
	if (res_l) {
		printf("По вашему запросу найдено %zu книг:", res_l);
		print_books(result, res_l);
	}
	else
		printf("По вашему запросу ничего не найдено");
	free(input);
	free(result);
	return true;
}
