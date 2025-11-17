#include <stdio.h>
#include <locale.h>
#define t 0.015
#define pd 700  // kg/m3
#define pdsp 800
#define pdvp 735
int main() {
	double mass, h, w, d, m1, m2, m3, m4, m5;
	int k = 0, hi, wi, di;                     //использованы целочисленные дубликаты h, w, d, т к не получалось сравнивать их посредством < и >
	setlocale(LC_ALL, "Rus");

	do {
		printf("Введите высоту, ширину и глубину шкафа через пробел\n");
		scanf_s("%d %d %d", &hi, &wi, &di);
		if ((hi < 180) || (hi > 220) || (wi < 80) || (wi > 120) || (di < 50) || (di > 90))
			printf("Данные некоректны\n");
	} while ((hi < 180) || (hi > 220) || (wi < 80) || (wi > 120) || (di < 50) || (di > 90));
	h = hi;
	w = wi;
	d = di;
	h /= 100;                                                             //m
	w /= 100;                                                         
	d /= 100;                                                         
	m1 = h * w * pdvp * 0.005;                                            //Задняя стенка     ( m*m*m * (kg/m3) = kg)
	m2 = 2 * (h - 2 * t) * (d - 0.01) * t * pdsp;                         //Боковины
	m3 = 2 * w * d * t * pdsp;                                            //Крышки
	m4 = h * w * pd * 0.01;                                               //Дверцы
	k = (hi <= 202) ? 4 : 5;
	m5 = k * (w - t * 2) * (d - 0.01) * t * pdsp;                         //Полки
	printf("\nМасса вашего шкафа = %f kg\n", m1 + m2 + m3 + m4 + m5);
	printf("Полок в нем %d\n", k);
	return 0;
}