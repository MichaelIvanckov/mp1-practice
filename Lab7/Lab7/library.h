#pragma once

// структура представления книги
struct book {
	char* authors;  // авторы
	char* name;     // название
	char* publ;     // издательство
	unsigned year;  // год издания
	char* str;      // указатель на строку с исходной информацией о книге для очистки памяти
};

typedef struct book book; // структура представления книги

extern book* library;