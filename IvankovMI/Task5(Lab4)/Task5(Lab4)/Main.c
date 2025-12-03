#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <conio.h>

#define N 1000       //макс длина ввода
#define NPR          //макс длина таблицы продуктов (не больше 9999)
#define R(f) (strcmp(inp, f) == 0)                //просто для удобства
#define R2(f) (strcmp(inp2, f) == 0)              //сравнение второго инпута
#define ISCOMMAND (strcmp(inp[0], ".") == 0)
#define scan {char buff[N + 1]; printf("----\b\b\b\b"); fgets(buff, N, stdin); sscanf_s(buff, format, inp, sizeof(inp), inp2, sizeof(inp2));}


//Поддерж. команды: 
// купон
// инфо
// помощь
// позв. персонал
// отмена поз.
// отмена покупки
// финиш
// выход.

void choose(char* inp, char* inp2, int* coup);
void coupon(char* inp, char* inp2, char* coup);   //+
void info() { ; }
//void help();                        //+
//void callthecashier();              //+
void Galya() { ; }
void Galina() { ; }
void final() { ; }
void barcode(char *inp);

void file_to_sortedmatrix(FILE* crt, char* dst[][3], int n) { ; }

char helpi[] = "Вводите ниже цифры \"отсканированных штрихкодов\" и специальные команды, а программа \nсформирует чек и расчитает итоговую стоимость и размер скидки в рублях (без копеек). \nЦифры \"штрихкода\" вводите слитно (без пробелов), в десятичной системе счисления, \nкоманды и \"штрихкоды\" разделяйте пробелами и/или переносами строк. \nСписок команд: \n.coupon           — предъявить скидочный купон (затем попросят ввести номер купона) \n.info <штрихкод>  — получить информацию о товаре, не добавляя его в корзину \n(обратите внимание, команду и \"штрихкод\" надо писать раздельно, пример: .info 0123) \n.. / .fin         — завершить \"сканирование товаров\" и перейти к оплате \n.. / .fin (после оплаты) — закончить просмотр чека и завершить покупку \n.callthecashier   — позвать сотрудника \n.Galya            — отменить уже добавленный к покупке товар \n.Galina           — отменить весь процесс покупки \n.quit             — выйти из программы и завершить процесс \n.help             — вывести эту инструкцию ещё раз \n*просто введённый штрихкод добавляет товар в корзину и выводит базовую информацию о нём \n*\"касса\" обслуживает покупателей непрерывно: после завершения одной покупки начнется следующая\n\n";
char format[14];                         //строка формта для scanf_s вида "%Ns", где N - максимально разрешенная длина ввода
char* products[NPR][3];                  //двумерный массив указателей на строки, представляющий таблтцу товаров



//printf("****");
//_getch();
//printf("\b\b\b\b");

int main0() {
	int n, e;
	char s[20];
	e = scanf_s("%d %5s", &n, s, sizeof(s));
	printf("%d, %s\n", n, s);
	printf("%d", e);
	return 0;
}

int main() {
	
	FILE* list;
	char inp[N + 1], inp2[N + 1];    //format[9];
	int n, coup = 0, error;
	
	size_t szf = sizeof(format);
	snprintf(format, szf, "%%%ds %%%ds", N, N);

	setlocale(LC_ALL, "Rus");

	"list ="; error = fopen_s(&list, "list_of_items.txt", "r");

	printf("Добро пожаловать в имитацию кассового аппарата \n\n");
	printf(helpi);
	if (error != 0) {
		printf("Ошибка чтения файла. Проверьте наличие файла.");
		return 0;
	}
	fscanf_s(list, "%d", &n);
	printf("\n------------------ Добро пожаловать в магазин \"Магазин\"! Вводите свои товары ------------------\n\n");

	//printf("%d\n", n);
	//scanf_s(888, format, inp, inp2, sizeof(inp), sizeof(inp2));
	scan;
	while (strcmp(inp, ".quit") != 0) {
		
		choose(inp, inp2, &coup);
		scan;

	}
	return 0;
}


void choose(char* inp, char* inp2, int* coup) {
	if (R(".help"))
		printf(helpi);
	else if (R(".callthecashier"))
		printf("скоро к вам подойдет сотрудник");
	else if (R(".coupon"))
		coupon(inp, inp2, coup);
	else if (R(".info"))
		info();
	else if (R(".Galya"))
		Galya();
	else if (R(".Galina"))
		Galina();
	else if (R("..") && R(".fin"))
		final();
	else
		barcode(inp, inp);
}


void coupon(char* inp, char* inp2, char* coup) {
	char coupon[21];     //если нужна будет действительно проверка купона
	if (R(".coupon")) {
		printf("Введите номер своего купона: ");
		scan;
	}
	printf("Отлично, теперь на некоторые товары вы получите скидки!\n");
	*coup = 1;
}


void barcode(char *inp) {
	;
}