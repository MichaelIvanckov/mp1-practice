//./src/library.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>



// структура представления книги
struct book_ {
	char* authors;  // авторы
	char* name;     // название
	char* publ;     // издательство
	unsigned year;  // год издания
	char* str;      // указатель на строку с исходной информацией о книге для очистки памяти
};

struct library_s { // структура представления библиотеки
	book* list;
	size_t lenth;
	size_t size;
};

typedef struct book_ book; // структура представления книги
typedef struct library_s library_t; // структура представления библиотеки

extern book* library_;
extern size_t lib_size;

extern library_t library;
