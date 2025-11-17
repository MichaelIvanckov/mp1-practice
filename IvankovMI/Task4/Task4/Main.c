#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <math.h>
#include <string.h>


int глобальный_костыль = 1;      //КОСТЫЛЬ (осторожно) !!!

int unique(int a[5], int i);
void get_int_arr_lenth_n(int a[5],int n);
int arr_is_digit(char a[], int n);

int main() {
	int n, sl[5], mysl[5], i, j, cows = 0, bulls = 0, ok = 0, tryn = 1, tryp = 0;

	setlocale(LC_ALL, "Rus");
	srand(time(NULL));

	restart:                                                //ну не охота так в цикл все заворачивать - и так понятно же
	printf("      --- Вы запустили игру «Быки и коровы­» --- \nВ ней вам надо будет угадывать слово \nпо необычным подсказкам\n\n");
	printf("Введите длину загадываемого слова (2, 3, 4 или 5)\n");
	scanf_s("%d", &n);
	if (n > 5 && n < 11)
		printf("Вы просите загадать число больше 5, но делаете это без уважения. \nТак что я не гарантирую корректную работу программы\n");
	if (n == 1)
		printf("Игра, конечно, запустится, но будет ли в этом смысл...\n");
	if (n > 10) {
		printf("Не-не-не, так мы точно не договаривались. Я вам где %d разных цифр возьму? \n(Я пробовал, прога падала, ломая все на своем пути) \nТак что давайте начнем сначала\n\n\n\n\n", n);
		goto restart;
	}
	sl[0] = rand() % 10;
	for (i = 1; i < n; i++) {
		sl[i] = rand() % 10;
		while (! unique(sl, i))
			sl[i] = rand() % 10;
	}
	
	printf("\nOk'эй, я загадал \"слово\" из %d уникальных цифр. Вводите свою отгадку, \nа я отвечу, сколько в ней коров (цифра есть, но не на своем месте) \nи быков (цифра и её положение угаданы верно).\n", n);
	get_int_arr_lenth_n(mysl, n);
	while (!ok) {
		for (i = 0; i < n; i++) {
			int t = mysl[i];
			for (j = 0; j < n; j++)
				if (t == sl[j]) {
					if (i == j)
						bulls++;
					else
						cows++;
				}
		}
		if (bulls == n)
			ok = 1;
		else {
			if (!(bulls == 0 && cows == 0 && tryn < 2))
				printf("%s %d бык%s и %d коров%s%s\n",
					(tryn < 2 + tryp) ? "Вы не отгадали слово, но в нем случайно затесались" : "В вашем слове",
					bulls, (bulls == 0 || bulls == 5) ? "ов" : (bulls == 1) ? "" : "а",
					cows, (cows == 0 || cows == 5) ? "" : (cows == 1) ? "а" : "ы",
					(tryn < 2 + tryp) ? ". \nВы на верном пути, продолжайте отгадывать" : "");
			else {
				printf("Вы не отгадали слово, и в нем нет быков и коров. \nНо не отчаивайтесь и продолжайте отгадывать\n");
				tryp = 1; 
			}
			bulls = 0;
			cows = 0;
			tryn++;
			get_int_arr_lenth_n(mysl, n);
		}
	}
	printf("Поздравляю! Вы отгадали слово, и потратили на это всего %d попыток!\n\n", tryn);
	return 0;
}

int unique(int a[5], int i) {
	//ф-я проверяет уникальность i-того элемента в срезе массива от 0 до i
	int j = 0;
	for (; j < i; j++)
		if (a[j] == a[i])
			return 0;
	return 1;
}

void get_int_arr_lenth_n(int a[5], int n) {
	char inp[100];
	int i = 0, caps = 0;
	
	if (глобальный_костыль == 1) {
		fgets(inp, sizeof(inp), stdin); глобальный_костыль = 0;         //КОСТЫЛЬ --\_/(- _ - )\_/--
	}
	
	fgets(inp, sizeof(inp), stdin);
	while ((strlen(inp) - 1 != n) || (caps = !arr_is_digit(inp, n))) {
		printf("Введите \"слово\" из %d %s\n", n, (caps)? "ЦИФР":"цифр"); caps = 0;
		fgets(inp, sizeof(inp), stdin);
	}
	for (i = 0; i < n; i++)
		switch (inp[i]){
		case '0': a[i] = 0; break;
		case '1': a[i] = 1; break;
		case '2': a[i] = 2; break;
		case '3': a[i] = 3; break;
		case '4': a[i] = 4; break;
		case '5': a[i] = 5; break;
		case '6': a[i] = 6; break;
		case '7': a[i] = 7; break;
		case '8': a[i] = 8; break;
		case '9': a[i] = 9; break;
		}
}

int arr_is_digit(char a[], int n) {
	char digits[10] = "0123456789";
	int i = 0, j, ok;
	for (; i < n; i++) {
		ok = 0;
		for (j = 0; j < 10; j++)
			if (a[i] == digits[j]) {
				ok = 1; 
				break;
			}
		if (ok == 0)
			return 0;
	}
	return 1;
}