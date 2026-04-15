#include <stdio.h>
#include <locale.h>
#include "library.h"
#include "auxiliary.h"


int main() {
	setlocale(LC_ALL, "rus");
	start_ask();
	while (process_query());
	printf("Выход...");
	return 0;
}
