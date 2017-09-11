#include <iostream>
#include <conio.h>
#include <vector>
#include <cmath>


using namespace std;


void prime_factors (const int);


int main ()
{
	prime_factors(10000);
	_getch();
	return 0;
}

/*
Функция разложения натуральных чисел от 1 до N на простые множители.
Результат выводится на экран.
В данном алгоритме сначала вычисляются все простые числа 
от 1 до квадратного корня из N и записываются в вектор.
Далее каждое натуральное число от 1 до N проходит проверку деления без остатка
на уже вычисленные простые числа меньшие или равные квадратному корню из текущего числа.
*/

void prime_factors (const int N)
{
	if (N < 1)
	{
		cout << "Wrong number N.";
		return;
	}
	
	vector<int> prime_num(1, 2);
	int maxPN, current_num;

	//-----------------Searching prime numbers from 1 to sqrt(N)-----------------

	prime_num.push_back(3);
	maxPN = prime_num[1];
	current_num = maxPN;

	while ((maxPN * maxPN) < N)
	{
		current_num += 2;
		bool isPrime = true;

		for (int i = 1; prime_num[i] <= sqrt(current_num); i++)
		{
			if ((current_num % prime_num[i]) == 0)
			{	
				isPrime = false;
				break;
			}
		}
		
		if (isPrime)
		{
			prime_num.push_back(current_num);
			maxPN = current_num;
		}
	}

	//--------------Searching prime factors--------------

	for (current_num = 1; current_num <= N; current_num++)
	{
		int tmp = current_num;
		
		cout << current_num << ": ";

		for (int i = 0; prime_num[i] <= sqrt(current_num); i++)
		{
			if (tmp % prime_num[i] == 0)
			{
				cout << prime_num[i] << " ";
				tmp /= prime_num[i];
				i = -1;
			}
		}

		if (current_num == tmp || sqrt(current_num) < tmp) 
			cout << tmp;

		cout << endl;
	}
}
