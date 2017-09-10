#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>


using namespace std;

/*
Алгоритм разбора параметров командной строки.
*/


struct option 
{
	char * name;
	int arg;
	bool required;
	bool found;
	vector<int> val;
};


void getopt_long_only (int, char * [], option *);
void help (const option *);

int main (int argc, char * argv[])
{
	option long_options[] =					//создание массива доступных параметров
	{
		{"--help", 0, false, false},
		{"--add", 2, true, false},
		{"--delete", 1, false, false},
		{"--fast", 0, false, false},
		{"--my-option", 1, false, false},
		{0, 0, false, false}				//последний элемент должен быть нулевым.
	};
	
	getopt_long_only(argc, argv, long_options);
	if (long_options[0].found)
		help(long_options);
	
	return 0;
}


void getopt_long_only(int argc, char * argv[], option * lo)
{
	for (int i = 1; i < argc; i++)
	{
		bool unknown_opt = true;
		for (int j = 0; lo[j].name != 0; j++)
		{
			if (!strcmp(argv[i], lo[j].name))
			{
				unknown_opt = false;
				int tmp, ii = 1;
				if (i + 1 < argc)
					if (!strcmp(argv[i + 1], "="))
						i++;

				if (i + lo[j].arg < argc)
					for (; lo[j].arg >= ii && sscanf(argv[i + ii], "%d", &tmp) == 1; ii++)
						lo[j].val.push_back(tmp);
				else
					ii = -1;
				if (lo[j].arg == ii - 1)
				{
					lo[j].found = true;
					i += ii - 1;
					break;
				}
				else
				{
					cout << "Option [" << lo[j].name << "] requires " << lo[j].arg;
					if (lo[j].arg > 1)
						cout << " parameters\n\tFor help type [--help]\n\n";
					else
						cout << " parameter\n\tFor help type [--help]\n\n";
					break;
				}
			}
				
		}
		if (unknown_opt)
			cout << "Unknown option: " << argv[i] << "\n\tFor help type [--help]\n\n";
	}
	
}

void help (const option * lo)
{
	cout << "\n\t\t-HELP-\n\n  Available options:\n";
	for (int i = 0; lo[i].name != 0; i++)
	{
		if (!lo[i].required)
			cout << " [" << lo[i].name << "]";
		else
			cout << "  " << lo[i].name;
		if (lo[i].arg > 0)
		{
			cout << ", requires " << lo[i].arg;
			if (lo[i].arg > 1)
				cout << " parameters";
			else
				cout << " parameter";
		}
		cout << endl;
	}
	cout << "\n Options in sqare brackets are not required.\n\nExamples of use:\n\t\"--option-name 35 12\"\n\t\"--option-name = 3\"\n\t\"--boolean-option\"\n";
}
