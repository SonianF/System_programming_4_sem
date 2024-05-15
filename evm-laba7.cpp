#include <iostream>
#include <string>
using namespace std;

int k_final = 0;
void showTask() {
	cout << "Вариант 8. Посчитать сумму членов ряда для функции ln(x) = sum[n=0; inf] ((-1)^n)*((x-1)^(n+1))/(n+1) " << endl;
}

double input_correct_x() {// проверка на введеное значение х
	double n;
	cout << "Enter x: ";
	while (!(cin >> n) || cin.peek() != '\n' || n <= 0 || n > 2) {
		cin.clear(); //очищаем поток от флага ошибки
		cin.ignore(32767, '\n'); //очищаем поток от символов
		cout << "Error, enter a number between 0 and 2: ";
	}
	return n;
}

double check_input_eps() { //проверка введенных значений для длины массива
	//блок try catch исользуется для поиска ошибки, например, в нашем случае это недопустимого размера ввод длины
	try {
		cout << "Введите значение eps : ";
		string length0 = " "; //вводим строковую переменную
		cin >> length0;
		size_t pos = length0.find("-");
		while (length0.find_first_not_of("1234567890.") != -1) { //если введенное значение содержит что-то кроме цифр, то выдается ошибка
			std::cout << "ОШИБКА! Введите натуральное число число: ";
			return check_input_eps(); //повторный ввод
		}
		return stod(length0);
	}
	catch (...) {
		cout << "Значение больше допустимого значения типа. Пожалуйста, введите число поменьше " << endl;
		return check_input_eps();
	}
}

double result_cpp(double x) {
	return log(x);
};

double asm_res(double x, double eps) {
	double iteration;
	int n = 0;
	double alfa = (-1) * (x - 1) * (n + 1) / (n + 2);
	double first = x - 1;
	double sum = first;
	n++;
	int c1 = 1;
	int m1 = -1;
	
	__asm {
		finit; инициализация сопроцессора
		fld qword ptr[eps]; st(0) = eps
		fld qword ptr[sum];  st(0) = first(sum), st(1) = eps;
		fld qword ptr[first]; st(0) = first st(1) = first(sum) st(2) = eps
		fild n; st(0) = 1 (n), st(1) = first(sum), st(2) = first, st(3) = eps;

	begin_loop:
		fld qword ptr[x]; st(0) = x, st(1) = n+1, st(2) = sum, st(3) = first, st(4) = eps;
		fisub c1; st(0) = x-1, st(1) = n+1, st(2) = sum, st(3) = first, st(4) = eps;
		fild m1; st(0) = -1, st(1) = x - 1, st(2) = n + 1, st(3) = sum, st(4) = first, st(5) = eps;
		fmulp st(1), st; st(0) = -1 * (x - 1), st(1) = n + 1, st(2) = sum, st(3) = first, st(4) = eps;
		fxch st(1); st(0) = n + 1, st(1) = -1 * (x - 1),  st(2) = sum, st(3) = first, st(4) = eps;
		fmul st(1), st; st(0) = n + 1, st(1) = -1 * (x - 1)*(n+1), st(2) = sum, st(3) = first, st(4) = eps;

		fiadd c1; st(0) = n + 2, st(1) = -1 * (x - 1) * (n + 1); st(2) = sum, st(3) = first, st(4) = eps;
		fdiv st(1), st; st(0) = n + 2, st(1) = -1 * (x - 1) * (n + 1) / (n + 2); st(2) = sum, st(3) = first, st(4) = eps;
		fst iteration; нашли alfa = st(1)
		fxch st(3); st(0) = first, st(1) = -1 * (x - 1) * (n + 1) / (n + 2); st(2) = sum, st(3) = n + 2, st(4) = eps;
		fmulp st(1), st; st(0) = first* -1 * (x - 1) * (n + 1) / (n + 2); st(1) = sum, st(2) = n + 2, st(3) = eps;

		fadd st(1), st; st(0) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(1) = sum +xn, st(2) = n + 2, st(3) = eps;

		fld1; st(0) = 1; st(1) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(2) = sum + xn, st(3) = n + 2, st(4) = eps;
		fxch st(1); st(0) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(1) = 1; st(2) = sum + xn, st(3) = n + 2, st(4) = eps;
		fmul st(1), st; st(0) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(1) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(2) = sum + xn, st(3) = n + 2, st(4) = eps;
		fabs; st(0) = |first * -1 * (x - 1) * (n + 1) / (n + 2)|; st(1) = first * -1 * (x - 1) * (n + 1) / (n + 2); st(2) = sum + xn, st(3) = n + 2, st(4) = eps;


		fcomip st, st(4);
		fxch st(2); st(0) = n + 2; st(1) = sum + xn; st(2) = first * -1 * (x - 1) * (n + 1) / (n + 2), st(3) = eps;
		; если член ряда больше погрешности, позвращаемся в начало цикла;
		ja begin_loop;

		fistp k_final;
	}
}


int main()
{
	setlocale(LC_ALL, "Russian");
	setlocale(LC_NUMERIC, "eng");
	showTask();
	double x = input_correct_x();

	double eps = check_input_eps();

	double analitic = result_cpp(x);
	double sum_of_row = asm_res(x, eps);

	cout << "Result on c++: " << analitic << endl;
	cout << "Result on allembler: " << sum_of_row << endl;
	cout << "It took " << k_final - 2 << " iterations.\n"; 
	cout << "Error rate: " << analitic - sum_of_row;
}
