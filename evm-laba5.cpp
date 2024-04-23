#include <iostream>
#include <string>
using namespace std;
void showTask() {
	cout << "Вариант 8. Найти ((2 * c - d / 3) / (1 - a / 4))." << endl;
}

double result_cpp(double a, double c, double d) { //int a, int c, int d
	if ((1 - a / 4) == 0) {
		cout << "ошибка деления на ноль\n";
		return 0;
	}
	else {
		return ((2 * c - d / 3) / (1 - a / 4));
	}
}

double check_input() {
	try {
		cout << "Введите число : ";
		string length0 = " ";
		cin >> length0;
		while (length0.find_first_not_of("1234567890-.") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите число: ";
			return check_input(); //повторный ввод
		}
		double res = stod(length0);
		return res;
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input();
	}
}

double asm_res(double a, double c, double d) {
	//((2 * c - d / 3) / (1 - a / 4))
	const int c1 = 1;
	const int	c2 = 2;
	const int	c3 = 3;
	const int	c4 = 4;
	double res;
	int flag = -1;
	__asm {
		finit; инициализация процессора

		fld qword ptr[c]; добавление c = st(0)
		fimul qword ptr[c2]; st(0) = st(0) * 2 = c * 2
		fld qword ptr[d]; добавление d = st(0); st(1) = c * 2
		fidiv qword ptr[c3]; st(0) = st(0) / c3 = d / 3
		fsub; st(0) = st(1) - st(0) = c * 2 - d / 3
		fld qword ptr[a]; st(0) = a; st(1) = c * 2 - d / 3; st(2) = c * 2
		fidiv qword ptr[c4]; st(0) = a / 4; st(1) = c * 2 - d / 3; st(2) = c * 2
		fchs; st(0) = -a / 4; st(1) = c * 2 - d / 3; st(2) = c * 2
		fiadd qword ptr[c1]; st(0) = 1 - a / 4; st(1) = c * 2 - d / 3; st(2) = c * 2
		fldz; st(0) = 0;  st(1) = 1 - a / 4; st(2) = c * 2 - d / 3; st(3) = c * 2
		fucomp; Сравнить ST(0) и ST(1), из регистрового стека выталкивается два верхних элемента st(0)
		fnstsw ax; Сохранить значение регистра SW в AX
		sahf; Загрузить биты SF, ZF, AF, PF и CF регистра флагов FLAGS из регистра AH
		jz error_0; проверка zero - флага;  st(0) = 1 - a / 4; st(1) = c * 2 - d / 3; st(2) = c * 2
		fdivp st(1), st; ST(1) = ST(1) / ST(0), верхний элемент выталкивается из регистрового стека ST(0)
		fxam; Проанализировать и классифицировать значение в новом ST(0)
		mov flag, 1
		fst res;
		jmp finish

		error_0 :
		mov flag, 0
			jmp finish; метка перехода

		finish :
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
	double a = check_input();
	double c = check_input();
	double d = check_input();

	double result = 0;
	double res;
	cout << "Assembler result: " << asm_res(a, c, d) << '\n';
	cout << "C++ result: " << result_cpp(a, c, d);
}
