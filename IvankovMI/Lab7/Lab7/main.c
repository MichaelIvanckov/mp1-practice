#include <stdio.h>
#include <locale.h>
#include "library.h"
#include "auxiliary.h"


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "rus");
	if (argc == 2) {
		printf("Заполнение библиотеки из файла %s", argv[1]);
		init_library(argv[1]);
	}
	else if (argc > 2) {
		printf("Слишком много параметров.");
		return 1;
	}
	else
		start_ask();
	while (process_query());
	printf("Выход...");
	return 0;
}
