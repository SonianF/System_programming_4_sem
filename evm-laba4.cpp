#include <iostream>
#include <string>
using namespace std;


void showTask() {
	cout << "Вариант 8. Найти разность суммы положительных и суммы отрицательных элементов массива A = { a[i] }." << endl;
}

int result_cpp(int *array, int n) {
	int pos_sum=0;
	int neg_sum=0;
	for (int i = 0; i < n; i++) {
		if (array[i] > 0) {
			pos_sum += array[i];
		}
		else
			neg_sum += array[i];
	}
	int res = pos_sum - neg_sum;
	return res;
}

int check_input(bool flag) { //проверка введенных значений для длины массива
	//блок try catch исользуется для поиска ошибки, например, в нашем случае это недопустимого размера ввод длины
	try {
		if (flag == 1) {
			string length0 = " "; //вводим строковую переменную
			cin >> length0;
			size_t pos = length0.find("-");
			while (length0.find_first_not_of("1234567890") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
				std::cout << "ОШИБКА! Введите натуральное целое число: ";
				return check_input(1); //повторный ввод
			}
			return stoi(length0);
		}
		string length0 = " "; //вводим строковую переменную
		cin >> length0;
		size_t pos = length0.find("-");
		while (length0.find_first_not_of("1234567890-") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите целое число: ";
			return check_input(0); //повторный ввод
		}
		return stoi(length0);
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input(0);
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	showTask();
	cout << "Введите значение n : " << endl;
	int n = check_input(1);
	int* array = new int[n];
	for (int j = 0; j < n; j++) {
		cout << "Введите значение a[" <<j+1<<" ] : " << endl;
		array[j] = check_input(0);
	}

	int pos_sum=0;
	int neg_sum=0;
	int res=0;
	int flag = -1;

	__asm {
		xor esi, esi; подготовим регистр индекса в массиве
		mov ebx, array; ebx указывает на начало массива
		mov ecx, n; счётчик цикла по всем элементам
		; массива
		jecxz finish; завершить если длина массива 0

	begin_loop:
		mov eax, [ebx + esi * 4]; определяем текущий элемент
			cmp eax, 0; сравнение a[i] и 0; в операциях сложения, вычитания и сравнения второй операнд может быть задан сразу
			jg positive_sum; если элемент положительный
			add neg_sum, eax; добавляем отрицательный элемент
			jo error_of; ошибка переполнения
			; увеличиваем счётчик
			jmp end_loop;
	positive_sum:
		add pos_sum, eax; добавляем положительный элемент
		jo error_of; ошибка переполнения
		jmp end_loop;
	end_loop:
		inc esi; переходим к следующему элементу
			loop begin_loop; повторяем цикл для всех элементов
			; массива

	mov eax, pos_sum;
	mov ebx, neg_sum;
	sub eax, ebx; считаем разность
	jmp finish;

	error_of:
		mov flag, 1
		jmp finish_prog; завершаем работу 

	finish: 
		mov flag, 2
		mov res, eax; возвращаем разность суммы положительных и суммы отрицательных элементов массива A = { a[i] }
	finish_prog:

	}

	switch (flag) {
	case 1: cout << "ошибка переполнения\n";
		break;
	case 2: cout << "Assembler result: " << res << '\n';
		cout << "C++ result: " << result_cpp(array, n);
	}
	return 0;
}
