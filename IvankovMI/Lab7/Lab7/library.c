//./src/library.c

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "platform_compability.h"
#include "auxiliary.h"
#include "library.h"

book* library;
size_t lib_size;

library_t library_;



// Основная функция поиска, возвращает указатель на массив указателей на книги в куче, изменяет f_cnt на кол-во найденных книг, завершает массив NULL
book* find_books(book* lib, size_t size, const char* substr, size_t* f_cnt) {
	// Разделители: пробельные символы и знаки пунктуации
	const char* delimiters = " \t\n\r\f\v.,;:!?()\"'—–";

	// Токенизация запроса
	int query_cnt;
	char** query_tokens = tokenize(substr, delimiters, &query_cnt);
	if (!query_tokens || query_cnt == 0) {
		if (query_tokens) free_tokens(query_tokens);
		return NULL;   // пустой запрос не считается совпадением
	}

	book** result = (book*)calloc(K, sizeof(book*)); // чтобы все лишние были нулями на всякий случай
	size_t res_l = K;

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
			if (i >= res_l)
				result = (book*)realloc(result, res_l * 2);
			result[i] = &lib[i];
			free_tokens(str_tokens);
			break;
		}
		free_tokens(str_tokens);
	}

	free_tokens(query_tokens);
	return result;
}

static void print_book(book* bk) {
	fprint("Название: %s\nАвтор(ы): %s\n Издательство: %s\nГод издания %d\n\n", bk->name, bk->authors, bk->publ, bk->year);
}

void print_books(book** bks, size_t cnt) {
	for (int i = 0; i < cnt && !bks[i]; ++i)
		print_book(bks[i]);
}

void process_query(); // Дописать