#include <iostream>
#include <string>
using namespace std;


void showTask() {
	cout << "Вариант 14" << endl;
	cout << "a > b: (2b - 2a) / (a + 7)" << endl;
	cout << "a = b: -89" << endl;
	cout << "a < b: (-b + 5a) / b" << endl;
}

int result_cpp(int a, int b) {
	if (a > b) {
		return (2*b - 2*a) / (a + 7);
	}
	else if (a < b) {
		return (-b + 5*a) / b;
	}
	else
		return -89;
}

int check_input(string num) { //проверка введенных значений для длины массива
	//блок try catch исользуется для поиска ошибки, например, в нашем случае это недопустимого размера ввод длины
	try {
		cout << "Введите значение " << num << " : ";
		string length0 = " "; //вводим строковую переменную
		cin >> length0;
		size_t pos = length0.find("-");
		while (length0.find_first_not_of("1234567890-") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите целое число: ";
			return check_input(num); //повторный ввод
		}
		return stoi(length0);
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input(num);
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	showTask();
	int a, b;
	a = check_input("a");
	b = check_input("b");

	int result = 0;
	int res;
	int flag = -1;

	__asm {
		mov eax, a; eax = a
		mov ebx, b;
		cmp eax, ebx; сравнение a и b
		jg l_bigger; переход если a > b
		jl l_smaller; переход если a < b
		je equally; переход если a = b

	equally:
		mov result, -89; result = -89
		mov flag, 2
		jmp finish; переход на конец программы

	l_bigger :
		sub ebx, eax; ebx = b - a
		mov ecx, 7; ecx = 7
		add ecx, eax; ecx = a + 7
		jz error_zf; ошибка деление на ноль
		mov eax, ebx; eax = b - a
		mov ebx, 2; ebx = 2
		imul ebx; eax = (b - a) * 2
		cdq; подготовка деления <edx:eax> = (b - a) * 2; обработка ситуации «частное велико»
		idiv ecx; eax = 2 * (b - a) / (a + 7)
		mov result, eax; result = 2 * (b - a) / (a + 7)
		mov flag, 2
		jmp finish; переход на конец программы
	l_smaller :
		mov eax, a; eax = a
		jo error_of; ошибка переполнения
		mov ecx, 5; ecx = 5
		imul ecx; eax = 5 * a
		sub eax, ebx; eax = 5 * a - b
		cdq; подготовка деления <edx:eax> = (5 * a - b); обработка ситуации «частное велико»
		idiv ebx; eax = (5 * a - b) * b
		mov result, eax; result = (5 * a - b) / b
		mov flag, 2
		jmp finish; переход на конец программы

			error_zf :
		mov flag, 0
			jmp finish; метка перехода
			error_of :
		mov flag, 1

			finish :
	}

	switch (flag) {
	case 0: cout << "ошибка деление на ноль\n";
		break;
	case 1: cout << "ошибка переполнения\n";
		break;
	case 2: cout << "Assembler result: " << result << '\n';
		cout << "C++ result: " << result_cpp(a, b);
	}
	return 0;
}
