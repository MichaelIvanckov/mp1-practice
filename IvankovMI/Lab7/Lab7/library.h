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

typedef struct book_ book; // структура представления книги

extern book* library;
