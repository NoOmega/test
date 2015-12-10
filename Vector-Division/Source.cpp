#include <iostream>
#include <conio.h>

using namespace std;

/*
Данный алгоритм производит деление вектора размера N на M < N равных частей.
Результатом являются индексы начала и конца каждой части.
Результат выводится на экран.
*/

int main (void)
{
	int N, M, step, start;
	bool input = true;

	//-------------------Input-------------------

	while (input)
	{
		input = false;

		cout << "\n  N = ";
		cin >> N;
		cout << "  M = ";
		cin >> M;
	
		if (M <= 0 || M >= N)
		{
			cout << "\t!!!N and M must be greater then zero and M must be less then N. \n\tpress any key";
			_getch();
			system("cls");
			input = true;
		}
	}

	//--------------------------------------------

	if (N % M == 0)
	{
		step = N / M;
		start = 0;
	}
	else
	{
		step = (N - (N % M)) / M;
		start = (N % M) / 2;
	}

	//-------------------Output-------------------

	cout << "\n\t#\t|\ti1\t|\ti2\n----------------|---------------|----------------\n";

	for (int i = 0; i < M; i++)
	{
		cout << "\t" << i + 1 << "\t|\t" << start << "\t|\t" << start + step - 1 << endl;
		start += step;
	}


	_getch();
	return 0;
}
