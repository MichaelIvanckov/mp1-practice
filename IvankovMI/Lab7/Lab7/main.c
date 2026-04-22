#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "library.h"
#include "auxiliary.h"


int main(int argc, char* argv[]) {
	//system("chcp 1251");
	//system("cls");
	setlocale(LC_ALL, "rus");
	if (argc == 2) {
		printf("Заполнение библиотеки из файла %s\n", argv[1]);
		init_library(argv[1]);
	}
	else if (argc > 2) {
		printf("Слишком много параметров.\n");
		return 1;
	}
	else
		start_ask();
	while (process_query());
	del_library_glob();
	printf("Выход...\n");
	return 0;
}
