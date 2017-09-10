#pragma once
#include <iostream>
#include <vector>


namespace Komissarov_Alexey
{
	struct SLLData
	{
		double index;
		SLLData *next;
	};

	class SLList
	{
	private:
		SLLData *begin;
	public:
		SLList ();
		void Add (const double);		//Добавляет элемент в порядке возрастания
		void Print (void);
		void FLRepack (void);		//Функция переупаковывает список (1->2->3->4 => 1->4->2->3)
		~SLList ();
	};
}
