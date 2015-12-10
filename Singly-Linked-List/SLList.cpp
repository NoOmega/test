#include "SLList.h"

namespace Komissarov_Alexey
{
	SLList::SLList ()
	{
		begin = NULL;
	}
	
	/*
	Список был написан ранее, поэтому метод добавления элемента немного усложнен
	тем, что вставляет элементы в порядке возрастания. Также метод удаления элемента был исключен
	из программы за ненадобностью.
	*/
	
	void SLList::Add (const double num)
	{
		if (begin == NULL)
		{
			begin = new SLLData;
			begin->index = num;
			begin->next = NULL;
		}
		else
		{
			SLLData * temp;
			SLLData * new_element;
			temp = begin;

			new_element = new SLLData;
			new_element->index = num;

			if (begin->index >= num)
			{
				new_element->next = begin;
				begin = new_element;
			}
			else
			{
				while (temp->next)
				{
					if (temp->next->index >= num)
					{
						new_element->next = temp->next;
						temp->next = new_element;
						break;
					}
					else
						temp = temp->next;
				}
				
				if (temp->next == NULL)
				{
					temp->next = new_element;
					new_element->next = NULL;
				}
			}
		}
	}

	void SLList::Print ()
	{
		SLLData * temp;
		temp = begin;

		while (temp)
		{
			std::cout << temp->index << " -> ";
			temp = temp->next;
		}

		std::cout << "NULL\n\n";
	}

/*
Функция FLRepack переупаковывает список по типу "первый-последний-второй-предпоследний".
Данный алгоритм сначала записывает в вектор адреса всех элементов списка, а затем меняет их в нужном порядке.
Сложность предложенного алгоритма O(1.5*N).
*/

	void SLList::FLRepack ()				
	{
		if (begin == NULL)
			throw "List is empty.";
		
		std::vector<SLLData *> adr;
		SLLData * temp;

		adr.push_back(begin);
		temp = begin;

		while (temp)
		{
			adr.push_back(temp->next);		// Заполнение вектора адресами всех элементов списка
			temp = temp->next;
		}
		
		temp = begin;
		for (int i = 1, j = adr.size() - 2; i <= j; i++, j--)
		{
			if (i != j)
			{
				temp->next = adr[j];
				temp = temp->next;
				temp->next = adr[i];
				temp = temp->next;
			}
			else
			{
				temp->next = adr[i];
				temp = temp->next;
			}
		}
		temp->next = NULL;
	}

	SLList::~SLList (void)
	{
		while (begin != NULL)
		{
			SLLData *temp = begin;
			begin = begin->next;
			delete temp;
		}
		delete begin;
	}

}
