#include <stdio.h>
#include <math.h>
#include <locale.h>
int main() {
	float x1, x2, y1, y2, r1, r2;
	//setlocale(LC_CTYPE, "Russiаn");
	setlocale(LC_ALL, "Rus");
	//scanf_s("Введите координаты и радиус первой окружности (x y r): %f %f %f", &x1, &y1, &r1);
	//scanf_s("Введите координаты и радиус второй окружности (x y r): %f %f %f", &x2, &y2, &r2);
	printf("Введите координаты и радиус первой окружности (x y r):");
	scanf_s("%f %f %f", &x1, &y1, &r1);
	printf("Введите координаты и радиус второй окружности (x y r):");
	scanf_s("%f %f %f", &x2, &y2, &r2);
	if ((r1 == 0) || (r2 == 0)) { printf("Радиус не может быть нулевым"); return 1; }
	if ((x1 == x2) && (y1 == y2)) { printf("Окружности равны"); return 0; }
	double d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	if (d > r1 + r2) { printf("Окружности удалены друг от друга"); return 0; }
	if (d == r1 + r2) { printf("Окружности внешне касаются друг друга"); return 0; }
	float rma = (r1 > r2) ? r1 : r2;
	float rmi = (r1 < r2) ? r1 : r2;
	if (d > rma) { printf("Окружности внешне пересекаются"); return 0; }
	if (d == rma) { printf("Окружности пересекаются, центр одной из них лежит на другой"); return 0; }
	if (d + rmi == rma) { printf("Окружности внутренне касаются друг друга"); return 0; }
	if (d + rmi > rma) { printf("Окружности внутренне пересекаются"); return 0; }
	printf("Одна окружность находится внутри другой"); return 0;
}