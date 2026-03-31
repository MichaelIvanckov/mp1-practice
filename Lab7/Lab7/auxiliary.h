#pragma once

#define N 100 // макс кол-во симв в строке в БД

// символы, которые надо удалить из начала и конца полей строкового представления книги после разбиения
#define FORBIDDEN_SYMBS " \t\"'«»"

#define lenof(sequence, type) sizeof(sequence)/sizeof(type)