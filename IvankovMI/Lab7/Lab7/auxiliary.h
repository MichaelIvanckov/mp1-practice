//./src/auxiliary.h

#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "library.h"

#define N 20 // ожидаемое кол-во симв в строке в БД

#define K 20 // ожидаемое кол-во строк в БД

// символы, которые надо удалить из начала и конца полей строкового представления книги после разбиения
#define FORBIDDEN_SYMBS " \t\"'«»"

#define lenof(sequence, type) sizeof(sequence)/sizeof(type)

// шаблон сообщения коммита: "Иванков М. И. - Lab7 - библиотека - ..."

// Создание бибилиотеки, выделение памяти
void create_library(book* lib, size_t* lib_s, size_t start_size);

// считывает информацию о книгах из файла и заполняет "библиотеку" структурами book
int fill_library(FILE* src_file, book** lib, size_t* lib_s);

// создает структуру book на основе однострочного представления
book fill_book(char* src);
