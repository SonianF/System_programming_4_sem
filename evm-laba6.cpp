#include <iostream>
#include <string>
using namespace std;


void showTask() {
	cout << "Вариант 8" << endl;
	cout << "a > b: (a + 3) / b - 21" << endl;
	cout << "a = b: -135" << endl;
	cout << "a < b: (19*b - 7) / (3 - a)" << endl;
}

double result_cpp(double a, double b) {
	if (a > b && b!=0) {
		return (a + 3) / b - 21;
	}
	else if (a < b && a!=3) {
		return (19 * b - 7) / (3 - a);
	}
	else if (a == 3 || b == 0) {
		cout << "ошибка деления на ноль\n";
		return 0;
	}
	else
		return -135;
}


double check_input(string num) {
	try {
		cout << "Введите число "+num+": ";
		string length0 = " ";
		cin >> length0;
		while (length0.find_first_not_of("1234567890-.") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите число: ";
			return check_input(num); //повторный ввод
		}
		double res = stod(length0);
		return res;
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input(num);
	}
}

//	fst res; самая полезная команда - выводит значение из вершины стека

double asm_res(double a, double b) {
	int status; 
	const int c3 = 3; 
	const int c21 = 21; 
	const int c135 = -135;
	const int c19 = 19;
	const int c7 = 7;
	double res;
	int flag = -1;
	__asm {
		finit; инициализация сопроцессора
		fld qword ptr[b]; st(0) = b
		fld qword ptr[a]; st(0) = a; st(1) = b
		fcom st(1); сравниваем a и b
		fstsw status; сохраняем регистр флагов сопроцессора
		mov ah, byte ptr[status + 1]
		sahf; записываем в регистр флагов процессора
		ja a_bigger; переход если a больше
		jb b_bigger; переход если b больше
		; если равны
		fild c135; st(0) = -135; st(1) = a; st(2) = b
		jmp finish_calc

		a_bigger : //(a + 3) / b - 21;
		fldz; st(0) = 0; st(1) = a; st(2) = b
		fcomp st(2); сравнение b с 0 и выбивание 0 из верхушки стека
		fnstsw ax; сохраняем регистр флагов сопроцессора
		sahf; записываем в регистр флагов процессора
		je error_0; переход если b = 0
		fiadd qword ptr[c3]; st(0) = a + 3;  st(1) = b
		fxch;  st(0) = b; st(1) = a + 3;
		fdiv; st(0) = (a + 3) / b;
		fisub c21; st(0) = (a + 3) / b - 21
		jmp finish_calc

		b_bigger : //(19*b - 7) / (3 - a)
		fild c3; st(0) = 3; st(1) = a; st(2) = b
		fcomp st(1); сравнение a с 3, st(0) = a; st(1) = b
		fnstsw ax; сохраняем регистр флагов сопроцессора
		sahf; записываем в регистр флагов процессора
		je error_0; переход если a = 3
		fchs; st(0) = -a; st(1) = b 
		fiadd qword ptr[c3]; st(0) = 3-a; st(1) = b
		fxch; st(0) = b;  st(1) = 3 - a; 
		fimul c19; st(0) = 19*b;  st(1) = 3 - a;
		fisub c7;  st(0) = 19 * b - 7;  st(1) = 3 - a;
		fxch; st(0) = 3 - a; st(1) = 19 * b - 7;
		fdiv; st(0) = (19 * b - 7) / (3 - a);
		jmp finish_calc
		error_0 : 
		fldz; формируем результат ошибки - загрузка 0 в вершину стека
		mov flag, 0
		jmp finish_calc; метка перехода

		finish_calc : fst res ;анализируем результат
	}
	if (flag == 0) {
		cout << "ошибка деления на ноль\n";
	}
	else
		return res;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "eng");
	showTask();
	double a = check_input("a");
	double b = check_input("b");

	double result = 0;
	double res;
	cout << "Assembler result: " << asm_res(a, b) << '\n';
	cout << "C++ result: " << result_cpp(a,b);
}
