#include <iostream>
#include <string>
using namespace std;

int result_cpp(int a, int c, int d) {
	return ((2 * c - d / 3) / (1 - a / 4));
}

int check_input() { //проверка введенных значений для длины массива
	//блок try catch исользуется для поиска ошибки, например, в нашем случае это недопустимого размера ввод длины
	try {
		cout << "Введите число: ";
		string length0 = " "; //вводим строковую переменную
		cin >> length0;
		while (length0.find_first_not_of("1234567890-") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите целое число: ";
			return check_input(); //повторный ввод
		}
		if (stoi(length0) == 0 || stoll(length0) == 9223373036854775807)
		{
			cout << "Введите целое число больше 0: ";
			return check_input(); //повторный ввод
		}
		else
			return stoi(length0);
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input();
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	int a, c, d;
	a = check_input();
	c = check_input();
	d = check_input();

	int result=0;
	int res;
	int flag=-1;

	__asm {

		mov eax, a; eax = a
		jo error_of; ошибка переполнения
		mov ebx, 4;
		cdq; eax = eax:edx
		idiv ebx; <eax> = a / 4
		mov ecx, eax; <ecx> = a / 4
		mov eax, 1;
		sub eax, ecx; eax = (1 - a / 4)
			jz error_zf; ошибка деление на ноль
		mov ecx, eax; ecx = eax = (1 - a / 4); здесь все верно считает
		mov eax, c; eax = c
			jo error_of; ошибка переполнения
		mov ebx, 2; 
		imul ebx; eax = 2 * c; здесь все верно считает
		jo error_of; ошибка переполнения
		mov res, eax; res = 2 * c
		mov eax, d;
		jo error_of; ошибка переполнения
		mov ebx, 3;
		cdq
		idiv ebx; eax = d / 3
		mov ebx, eax; ebx = d / 3
		mov eax, res;
		sub eax, ebx; eax = (2 * c - d / 3)
		cdq; eax = eax:edx
		idiv ecx; eax = (2 * c - d / 3) / (1 - a / 4)
		mov result, eax
		mov flag, 2
		jmp finish

		error_zf: 
		mov flag, 0
			jmp finish; метка перехода 
		error_of: 
		mov flag,1 
		finish:

	}

	switch (flag) {
		case 0: cout << "ошибка деление на ноль\n";
			break;
		case 1: cout << "ошибка переполнения\n";
			break;
		case 2: cout << "Assembler result: " << result << '\n';
			cout << "C++ result: " << result_cpp(a, c, d);
	}
	return 0;
}