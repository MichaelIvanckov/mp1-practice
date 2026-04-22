//./src/library.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "platform_compability.h"


// структура представлени€ книги
struct book_ {
	char* authors;  // авторы
	char* name;     // название
	char* publ;     // издательство
	unsigned year;  // год издани€
	char* str;      // указатель на строку с исходной информацией о книге дл€ очистки пам€ти
};
typedef struct book_ book; // структура представлени€ книги



extern book* library;
extern size_t lib_size;




// ‘ункции:

// создание библиотеки из информации из файла, работает с глобалами library
void init_library(char* path);

// ”даление библиотеки - освобождение пам€ти всех книг, работает с глобалами library
void del_library_glob();

// ќсновна€ функци€ поиска, возвращает указатель на массив указателей на книги в куче, измен€ет f_cnt на кол-во найденных книг, завершает массив NULL
book** find_books(book* lib, size_t size, const char* substr, size_t* f_cnt);

// вывести инфо каждой книги из массива
void print_books(book** bks, size_t cnt);

// ¬опрос в stdin о пути файла базы данных и заполнение библиотеки
void start_ask();

// ќбработка запроса из stdin, возвращает false, если требуетс€ выйти, работает с глобалами library
bool process_query();

// тестова€ функци€ дл€ вывода книги по номеру
void test(book** lib, int n);
