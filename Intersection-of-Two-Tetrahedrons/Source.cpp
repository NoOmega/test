#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <map>
#include "vector.h" //Класс математического вектора. К сожалению написан не мной, поэтому не могу предоставить его исходный код.

/*
Алгоритм вычисления объема пересечения двух произвольных тетраэдров.
Алгоритм основан на том, что любой тетраэдр можно представить 
в виде пересечения четырех полупространств. Следовательно задача сводится к
вычислению пересечения тетраэдра с полупространством.
*/

struct tetr							//Tetrahedron
{
	vector<3> a;
	vector<3> b;
	vector<3> c;
	vector<3> d;
};

double tetr_volume (tetr);			// - Volume of tetrahedron.
vector<3> normal (tetr, int);		// - returns normal vector to choosen face of tetrahedron (second parametr).
double intersection(tetr, tetr);	// - intersection of two tetrahedrons.
vector<3> cross_point (vector<3>, vector<3>, vector<3>, vector<3>);
int intersection_HS_T (tetr*, vector<3>, vector<3>);



int main()
{
	tetr A, B;
	
	A.a = vector<3>::comps(10, 10, 10);
	A.b = vector<3>::comps(0, 30, 10);
	A.c = vector<3>::comps(0, 0, 10);
	A.d = vector<3>::comps(0, 10, 30);

	B.a = vector<3>::comps(9, 10, 11);
	B.b = vector<3>::comps(1, 20, 11);
	B.c = vector<3>::comps(1, 10, 11);
	B.d = vector<3>::comps(1, 20, 30);
	
	double intersection_volume = intersection(A, B);

	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//Volume of tetrahedron.															//
//////////////////////////////////////////////////////////////////////////////////////

inline double tetr_volume (tetr T)
{
	return (1./6.) * fabs(((T.b-T.a) % (T.c-T.a)) * (T.d-T.a));
}


//////////////////////////////////////////////////////////////////////////////////////
//Returns normal vector to choosen face of tetrahedron (second parametr).			//
//////////////////////////////////////////////////////////////////////////////////////

vector<3> normal (tetr T, int direction)
{
	vector<3> normal;

	switch(direction)
	{
		//CASE 1 - ABC-D
		//CASE 2 - ABD-C
		//CASE 3 - ACD-B
		//CASE 4 - BCD-A
	case 1: {
				normal = (T.b - T.a) % (T.c - T.a);
				if (((T.d - T.a) * normal) < 0)
					normal = -normal;
				break;
			}
	case 2: {
				normal = (T.b - T.a) % (T.d - T.a);
				if (((T.c - T.a) * normal) < 0)
					normal = -normal;
				break;
			}
	case 3: {
				normal = (T.c - T.a) % (T.d - T.a);
				if (((T.b - T.a) * normal) < 0)
					normal = -normal;
				break;
			}
	case 4: {
				normal = (T.c - T.b) % (T.d - T.b);
				if (((T.a - T.b) * normal) < 0)
					normal = -normal;
				break;
			}
	}

	normal /= fabs(normal);

	return normal;
}


//////////////////////////////////////////////////////////////////////////////////////////
//Intersection of two tetrahedrons.														//
//Returns 'double' volume of intersection of two tetrahedrons.							//
//T1 - ABCD, T2 - A'B'C'D'																//
//T1 - points, T2 - half-spaces															//
//////////////////////////////////////////////////////////////////////////////////////////

double intersection(tetr T1, tetr T2)
{
	vector<3> norm, R;
    std::vector<tetr> TET(1, T1), TET_New;
    tetr tmp[3];			        	
	double volume = 0;

	for (int i = 1; i <= 4; i++)		//цикл по полупространствам
	{
		norm = normal(T2, i);

		if (i != 4)
			R = T2.a;
		else
			R = T2.b;

        for (int jj = 0; jj < TET.size(); jj++)	//цикл по массиву тетраэдров
		{
			int number;
			tmp[0] = TET[jj];
			number = intersection_HS_T(&tmp[0], R, norm);
            TET_New.insert(TET_New.end(), tmp, tmp + number);
		}
        TET.clear();
        TET.swap(TET_New);   
        
		if (TET.empty())
            break;
	}

	for (int k = 0; k < TET.size(); k++)
		volume += tetr_volume(TET[k]);
	return volume;
}


inline int sign(double x)
{
	return (x == 0.) ? 0 : ((x < 0.) ? -1 : 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//AB - rib(points), A - belongs to another tetrahedron, R - point on plane of half-space.	//
//////////////////////////////////////////////////////////////////////////////////////////////

vector<3> cross_point (vector<3> A, vector<3> B, vector<3> R, vector<3> norm)
{
	const double s1 = (A - R) * norm, s2 = (B - R) * norm;
	if (sign(s1) != sign(s2))
	{
		const double theta = s2 / (s2 - s1);  
									          
		return theta * A + (1. - theta) * B;
	}
}



//////////////////////////////////////////////////////////////////////////////////////////
//Intersection of half-space and tetrahedron.											//
//Input:																				//
//1 - addres of 'tetr' array (3 elements minimum),										//
//2 - 'vector<3>' point that belongs to the border of half-space,						//
//3 - 'vector<3>' normal vector of half-space.											//
//Writing 0, 1 or 3 tetrahedrons to array (3 means that their sum is the intersection).	//
//Returns number of written tetrahedrons.												//
//////////////////////////////////////////////////////////////////////////////////////////

int intersection_HS_T (tetr* T, vector<3> R, vector<3> norm)
{
	double arn, brn, crn, drn;
	int membership[5] = {0};	//[1] - A, [2] - B, [3] - C, [4] - D, [0] - number of points 
								//that belongs to half-space. 
	
	arn = ((T->a - R) * norm);
	if (arn >= 0)
	{
		membership[1] = 1;
		membership[0]++;
	}
	brn = ((T->b - R) * norm);
	if (brn >= 0)
	{
		membership[2] = 1;
		membership[0]++;
	}
	crn = ((T->c - R) * norm);
	if (crn >= 0)
	{		
		membership[3] = 1;
		membership[0]++;
	}
	drn = ((T->d - R) * norm);
	if (drn >= 0)
	{			
		membership[4] = 1;
		membership[0]++;
	}


	switch (membership[0])
		{
		case 0: {
					return 0;
					break;
				}
		case 1: {
					if (membership[1] == 1)
					{
						T->b += brn / (brn - arn) * (T->a - T->b);
						T->c += crn / (crn - arn) * (T->a - T->c);
						T->d += drn / (drn - arn) * (T->a - T->d);
					}
					if (membership[2] == 1)
					{
						T->a += arn / (arn - brn) * (T->b - T->a);
						T->c += crn / (crn - brn) * (T->b - T->c);
						T->d += drn / (drn - brn) * (T->b - T->d);
					}
					if (membership[3] == 1)
					{
						T->a += arn / (arn - crn) * (T->c - T->a);
						T->b += brn / (brn - crn) * (T->c - T->b);
						T->d += drn / (drn - crn) * (T->c - T->d);
					}
					if (membership[4] == 1)
					{
						T->a += arn / (arn - drn) * (T->d - T->a);
						T->b += brn / (brn - drn) * (T->d - T->b);
						T->c += crn / (crn - drn) * (T->d - T->c);
					}
					
					return 1;
					break;
				}
		case 2: {
					tetr TMP1, TMP2;

					if (membership[1] == 1 && membership[2] == 1)		//AB
					{
						TMP1.a = cross_point(T->a, T->c, R, norm);
						TMP1.b = cross_point(T->a, T->d, R, norm);
						TMP1.c = cross_point(T->b, T->d, R, norm);
						TMP1.d = cross_point(T->b, T->c, R, norm);
						TMP2.a = T->a;
						TMP2.b = T->b;
					}
					if (membership[1] == 1 && membership[3] == 1)		//AC
					{
						TMP1.a = cross_point(T->a, T->d, R, norm);
						TMP1.b = cross_point(T->a, T->b, R, norm);
						TMP1.c = cross_point(T->c, T->b, R, norm);
						TMP1.d = cross_point(T->c, T->d, R, norm);
						TMP2.a = T->a;
						TMP2.b = T->c;
					}
					if (membership[1] == 1 && membership[4] == 1)		//AD
					{
						TMP1.a = cross_point(T->a, T->b, R, norm);
						TMP1.b = cross_point(T->a, T->c, R, norm);
						TMP1.c = cross_point(T->d, T->c, R, norm);
						TMP1.d = cross_point(T->d, T->b, R, norm);
						TMP2.a = T->a;
						TMP2.b = T->d;
					}
					if (membership[2] == 1 && membership[3] == 1)		//BC
					{
						TMP1.a = cross_point(T->b, T->a, R, norm);
						TMP1.b = cross_point(T->b, T->d, R, norm);
						TMP1.c = cross_point(T->c, T->d, R, norm);
						TMP1.d = cross_point(T->c, T->a, R, norm);
						TMP2.a = T->b;
						TMP2.b = T->c;
					}
					if (membership[2] == 1 && membership[4] == 1)		//BD
					{
						TMP1.a = cross_point(T->b, T->c, R, norm);
						TMP1.b = cross_point(T->b, T->a, R, norm);
						TMP1.c = cross_point(T->d, T->a, R, norm);
						TMP1.d = cross_point(T->d, T->c, R, norm);
						TMP2.a = T->b;
						TMP2.b = T->d;
					}
					if (membership[3] == 1 && membership[4] == 1)		//CD
					{
						TMP1.a = cross_point(T->c, T->a, R, norm);
						TMP1.b = cross_point(T->c, T->b, R, norm);
						TMP1.c = cross_point(T->d, T->b, R, norm);
						TMP1.d = cross_point(T->d, T->a, R, norm);
						TMP2.a = T->c;
						TMP2.b = T->d;
					}

					T->a = TMP2.b;
					T->b = TMP1.b;
					T->c = TMP1.c;
					T->d = TMP1.d;

					(T + 1)->a = TMP2.a;
					(T + 1)->b = TMP2.b;
					(T + 1)->c = TMP1.b;
					(T + 1)->d = TMP1.d;

					(T + 2)->a = TMP2.a;
					(T + 2)->b = TMP1.a;
					(T + 2)->c = TMP1.b;
					(T + 2)->d = TMP1.d;
					
					return 3;
					break;
				}
		case 3: {
					tetr TMP1, TMP2;		
					
					if (membership[1] == 0)								//BCD
					{
						TMP1.b = cross_point(T->b, T->a, R, norm);
						TMP1.c = cross_point(T->c, T->a, R, norm);
						TMP1.d = cross_point(T->d, T->a, R, norm);
						TMP2.b = T->b;
						TMP2.c = T->c;
						TMP2.d = T->d;
					}
					if (membership[2] == 0)								//ACD
					{
						TMP1.b = cross_point(T->c, T->b, R, norm);
						TMP1.c = cross_point(T->d, T->b, R, norm);
						TMP1.d = cross_point(T->a, T->b, R, norm);
						TMP2.b = T->c;
						TMP2.c = T->d;
						TMP2.d = T->a;
					}
					if (membership[3] == 0)								//ABD
					{
						TMP1.b = cross_point(T->d, T->c, R, norm);
						TMP1.c = cross_point(T->a, T->c, R, norm);
						TMP1.d = cross_point(T->b, T->c, R, norm);
						TMP2.b = T->d;
						TMP2.c = T->a;
						TMP2.d = T->b;
					}
					if (membership[4] == 0)								//ABC
					{
						TMP1.b = cross_point(T->a, T->d, R, norm);
						TMP1.c = cross_point(T->b, T->d, R, norm);
						TMP1.d = cross_point(T->c, T->d, R, norm);
						TMP2.b = T->a;
						TMP2.c = T->b;
						TMP2.d = T->c;
					}
					
					T->a = TMP2.b;
					T->b = TMP2.c;
					T->c = TMP1.c;
					T->d = TMP2.d;

					(T + 1)->a = TMP2.b;
					(T + 1)->b = TMP1.b;
					(T + 1)->c = TMP1.c;
					(T + 1)->d = TMP2.d;

					(T + 2)->a = TMP1.b;
					(T + 2)->b = TMP1.c;
					(T + 2)->c = TMP1.d;
					(T + 2)->d = TMP2.d;
					
					return 3;
					break;
				}
		case 4: {
					return 1;
					break;
				}
		}
}
