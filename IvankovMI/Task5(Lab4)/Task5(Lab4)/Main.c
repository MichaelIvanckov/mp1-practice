#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <conio.h>

//Ќј—“–ќ… »:                          --------------------------------------------------------------------------------------|
#define N   1000     //макс длина ввода                                                                                     |
#define NPR 9999     //макс длина таблицы продуктов (не больше 9999)                                                        |
//--------------------------------------------------------------------------------------------------------------------------|


#define R(f) (strcmp(inp, f) == 0)                //просто дл€ удобства
#define R2(f) (strcmp(inp2, f) == 0)              //сравнение второго инпута
#define ISCOMMAND (strcmp(inp[0], ".") == 0)
#define ISCOM ISCOMMAND 
#define ISLW is_last_word()
#define scan {printf("--------\b\b\b\b\b\b\b\b"); scanf_s(format, inp, sizeof(char) * N);} //”–јјјјј!!! –јЅќ“ј≈“!!!! ¬¬ќƒ ѕќч»Ќ»Ћ»!!!
#define scan_t scanf_s(format, inp, sizeof(char) * N)
#define old_scan_with_error {printf("-----\b\b\b\b\b"); scanf_s(format, inp, sizeof(inp));}  //вот она, где собака была зарыта: "sizeof(inp)" возвращает размер ” ј«ј“≈Ћя, как раз, видимо, равный 8 байтам, а вот "sizeof(char) * N" возвращает размер байтового массива длиной N
//деклассированные элементы:    (возможно, никогда не используютс€)
#define scan_old {printf("----\b\b\b\b"); scanf_s(format, inp, sizeof(inp), inp2, sizeof(inp2));}
#define FGscan {char buff[N + 1]; printf("----\b\b\b\b"); fgets(buff, N, stdin); sscanf_s(buff, format, inp, sizeof(inp), inp2, sizeof(inp2));}
#define scan_(fl) input(format, b, inp, inp2, fl)
#define NEWscan ic = scan_(ic)


//ѕоддерж. команды: 
// купон
// инфо
// помощь
// позв. персонал
// отмена поз.
// отмена покупки
// финиш (завершение покупки)
// выход.

void choose(char* inp, char* inp2, int* coup, int ic);   //+
void coupon(char* inp, int* coup);   //+
void info() { ; }
//void help();                        //+
//void callthecashier();              //+
void Galya() { ; }   // отмена поз.
void Galina() { ; }  // отмена покупки
void final() { ; }
void barcode(char* inp);
void test(char* inp);

void file_to_sortedmatrix(FILE* crt, char* dst[][4], int n);
int input(char format[], char buff[], char inp[], char inp2[], int flag);   //стереть ?????
int is_last_word();

char helpi[] = "¬водите ниже цифры \"отсканированных штрихкодов\" и специальные команды, а программа \nсформирует чек и расчитает итоговую стоимость и размер скидки в рубл€х (без копеек). \n÷ифры \"штрихкода\" вводите слитно (без пробелов), в дес€тичной системе счислени€, \nкоманды и \"штрихкоды\" раздел€йте пробелами и/или переносами строк. \n—писок команд: \n.coupon           Ч предъ€вить скидочный купон (затем попрос€т ввести номер купона) \n.info <штрихкод>  Ч получить информацию о товаре, не добавл€€ его в корзину \n(обратите внимание, команду и \"штрихкод\" надо писать раздельно, пример: .info 0123) \n.. / .fin         Ч завершить \"сканирование товаров\" и перейти к оплате \n.. / .fin (после оплаты) Ч закончить просмотр чека и завершить покупку \n.callthecashier   Ч позвать сотрудника \n.Galya            Ч отменить уже добавленный к покупке товар \n.Galina           Ч отменить весь процесс покупки \n.quit             Ч выйти из программы и завершить процесс \n.help             Ч вывести эту инструкцию ещЄ раз \n*просто введЄнный штрихкод добавл€ет товар в корзину и выводит базовую информацию о нЄм \n*\"касса\" обслуживает покупателей непрерывно: после завершени€ одной покупки начнетс€ следующа€\n\n";
char format[14];                         //строка формата дл€ scanf_s вида "%Ns", где N - максимально разрешенна€ длина ввода
char* products[NPR][4];                  //двумерный массив указателей на строки, представл€ющий таблицу товаров
//char* b;



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
	char inp[N + 1], inp2[N + 1]; //buff[N + 1];    //format[9];
	int n, coup = 0, error, ic = 0;
	
	//b = buff;
	
	size_t szf = sizeof(format);
	snprintf(format, szf, "%%%ds", N);     //формирование строки формата дл€ scanf_s вида "%Ns", где N - максимально разрешенна€ длина ввода

	setlocale(LC_ALL, "Rus");

	"list ="; error = fopen_s(&list, "list_of_items.txt", "r");

	printf("ƒобро пожаловать в имитацию кассового аппарата \n\n");
	printf(helpi);
	if (error != 0) {
		printf("ќшибка чтени€ файла. ѕроверьте наличие файла.");
		return error;
	}
	fscanf_s(list, "%d", &n);
	file_to_sortedmatrix(list, products, n);
	printf("\n------------------ ƒобро пожаловать в магазин \"ћагазин\"! ¬водите свои товары ------------------\n\n");

	//printf("%d\n", n);
	//scanf_s(888, format, inp, inp2, sizeof(inp), sizeof(inp2));
	scan;
	while (strcmp(inp, ".quit") != 0) {
		
		choose(inp, inp2, &coup, ic);
		scan;

	}
	fclose(list);
	return 0;
}


int is_last_word() {            //странна€, конечно, реализаци€. но зато проста€, как топор
	char c = fgetc(stdin);
	while (c == ' ') {
		c = fgetc(stdin); 
	}
	if (c == '\n' || c == EOF) {
		return 1;
	}
	else {
		ungetc(c, stdin);
		return 0;
	}
}


void file_to_sortedmatrix(FILE* crt, char* dst[][4], int n) { 
	;
}


//стереть ?????
int input(char format[], char buff[], char inp[], char inp2[], int flag) {
	char c;
	if (flag == 0)
		fgets(buff, N + 1, stdin);
	sscanf_s(buff, format, inp, sizeof(inp));
	if (fgetc(buff) == '\n')
		return 0;
	else
		return 1;
}


void choose(char* inp, char* inp2, int* coup, int ic) {
	if (R(".help"))
		printf(helpi);
	else if (R(".callthecashier"))
		printf("—коро к вам подойдет сотрудник\n");
	else if (R(".coupon"))
		coupon(inp, coup);
	else if (R(".info"))
		info();
	else if (R(".Galya"))
		Galya();
	else if (R(".Galina"))
		Galina();
	else if (R("..") && R(".fin"))
		final();
	else if (R(".test"))
		test(inp);
	else
		barcode(inp);
}


void coupon(char* inp, int* coup) {
	//char coupon[21];     //если нужна будет действительно проверка купона
	if (is_last_word()) {
		printf("¬ведите номер своего купона: ");
	}
	scan;
	//strcpy(coupon, inp);
	printf("ќтлично, теперь на некоторые товары вы получите скидки!\n");
	printf("є купона: %s\n", inp);
	*coup = 1;
}


void barcode(char* inp) {
	printf("“≈—“: Ўтрихкод %s прин€т\n", inp);
}


void test(char* inp){
	scan;
	printf("“≈—“: %s\n", inp);
}
