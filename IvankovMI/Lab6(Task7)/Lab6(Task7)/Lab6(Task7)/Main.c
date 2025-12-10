#include <stdio.h>
#include <locale.h>

int find2(unsigned short src, int number) {
	unsigned short arr[1] = { src };
	unsigned char* ret = (unsigned char*) arr;
	return ret[number - 1];
}

int find1(unsigned short src, int number) {
	union { 
		unsigned short src;
		unsigned char ret[2];
	} ret = {src};
	return ret.ret[number - 1];
}



int main() {
	unsigned short inp;
	setlocale(LC_ALL, "Rus");
	printf("¬ведите число от 0 до 65535:  ");
	scanf_s("%d", &inp);
	printf("ѕервый байт: %d\n¬торой байт: %d\n", find1(inp, 1), find1(inp, 2));
	return 0;
}