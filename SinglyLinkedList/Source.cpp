#include <iostream>
#include <conio.h>
#include "SLList.h"

using namespace Komissarov_Alexey;
	
int main (void)
{
	SLList a;

	a.Add (12.5);
	a.Add (38.98);
	a.Add (-7.34);
	a.Add (33.99);
	a.Add (105.987);
	a.Add (44);
	a.Add (100);
	a.Add (11);
	a.Add (720);

	a.Print ();
	
	try
	{
		a.FLRepack ();
		a.Print ();
	}
	catch (const char * message)
	{
		std::cout << message << std::endl;
	}

	_getch ();
	return 0;
}
