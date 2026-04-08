//./src/library.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "platform_compability.h"


// структура представления книги
struct book_ {
	char* authors;  // авторы
	char* name;     // название
	char* publ;     // издательство
	unsigned year;  // год издания
	char* str;      // указатель на строку с исходной информацией о книге для очистки памяти
};
typedef struct book_ book; // структура представления книги

// структура представления библиотеки  (не используется)
struct library_s {
	book* list;
	size_t lenth;
	size_t size;
};
typedef struct library_s library_t; // структура представления библиотеки

extern book* library;
extern size_t lib_size;

extern library_t library_;


// Функции:

// Основная функция поиска, возвращает указатель на массив указателей на книги в куче, изменяет f_cnt на кол-во найденных книг, завершает массив NULL
book* find_books(book* lib, size_t size, const char* substr, size_t* f_cnt);

void print_books(book** bks, size_t cnt);
