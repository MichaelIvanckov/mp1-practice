#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>

#define N //как€-то шл€па

void help();
void final();
void check();

int main() {
	char inp[1000]; //какие-нибудь переменные

	setlocale(LC_ALL, "Rus");

	printf("¬ступительные слова");

	scanf_s("%s", inp, sizeof(inp));

	while (strcmp(inp, ".quit") != 0) {
		//switch (inp){
		//case ".help":
		//	help();
		//case ".":
		//	final();
		//default:
		//	check();
		//}
		scanf_s("%s", inp, sizeof(inp));
	}

	return 0;
}