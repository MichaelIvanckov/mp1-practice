//./src/library.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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



extern book* library;
extern size_t lib_size;




// Функции:

// создание библиотеки из информации из файла, работает с глобалами library
void init_library(char* path);

// Удаление библиотеки - освобождение памяти всех книг, работает с глобалами library
void del_library_glob();

// Основная функция поиска, возвращает указатель на массив указателей на книги в куче, изменяет f_cnt на кол-во найденных книг, завершает массив NULL
book** find_books(book* lib, size_t size, const char* substr, size_t* f_cnt);

// вывести инфо каждой книги из массива
void print_books(book** bks, size_t cnt);

// Вопрос в stdin о пути файла базы данных и заполнение библиотеки
void start_ask();

// Обработка запроса из stdin, возвращает false, если требуется выйти, работает с глобалами library
bool process_query();

// тестовая функция для вывода книги по номеру
void test(book* lib, int n);

// тестовая функция для вывода всех книг
void test_all(book* lib, size_t n);
