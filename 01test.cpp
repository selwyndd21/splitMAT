#include <iostream>
#include "stdlib.h"
#include "math.h"
#include <fstream>

#define SQRT_INTEROP	


double sqrt_interop(double ub, double lb, double in,double in_concentration,bool isub);
double linear_interop(double ub, double lb, double in,double in_concentration,bool isub);


using namespace std;

int main( int argc, char * argv[] )
{
	
	char tmp[256];								// buffer for parsing
	ifstream fin("input.txt");				// input stream
	ofstream fout("out.txt");			// output stream

	// Input Parameters
	bool flag = false;						// flag 
	int in_isotope;
	float in_concentration;
	char in_c;
	char head[5];

	// calculation Parameters
	int in_temp;											// input temperature
	int ub_temp;											// upper bound temperature
	int lb_temp;											// lower bound temperature
	int in_temp_idx;									// input temperature index
	int ub_temp_idx;									// upper bound temperature index
	int lb_temp_idx;									// lower bound temperature index
	double ub_concentration;					// upper bound
	double lb_concentration;					// lower bound
//int up_idxtmp, lo_idxtmp;         //temperature index 

	cout << "Please select upper bound temperature & index" << endl;
	cin >> ub_temp ;//>> up_idxtmp ;
	cout << "Please select lower bound temperature & index" << endl;
	cin >> lb_temp ;//>> lo_idxtmp;

	// decide input, upper and lower temp
	// 99->300k, 92->600k, 88->900k,, 85->1200k
	switch (ub_temp)
	{
		case 300:
			ub_temp_idx = 99;
			break;
		case 600:
			ub_temp_idx = 92;
			break;
		case 900:
			ub_temp_idx = 88;
			break;
		case 1200:
			ub_temp_idx = 85;
			break;
	}
	switch (lb_temp)
	{
		case 300:
			lb_temp_idx = 99;
			break;
		case 600:
			lb_temp_idx = 92;
			break;
		case 900:
			lb_temp_idx = 88;
			break;
		case 1200:
			lb_temp_idx = 85;
			break;
	}

	while(fin.getline(tmp,256))
	{
		if(tmp[0] == 'm' && atoi(tmp+1))     // 'mx' format, where x is an integer
			flag = true;
		else if(tmp[0] == 'c')							 // 'c' is comment
			flag = false;

		// main calculation
		if(flag == true)											
		{
			// parsing input parameters
			switch (tmp[0])
			{
				case 'm':
					sscanf(tmp,"%5s %d.%d%c %e",&head, &in_isotope, &in_temp_idx, &in_c, &in_concentration);
					break;
				case ' ':
					sscanf(tmp,"%d.%d%c %e", &in_isotope, &in_temp_idx, &in_c, &in_concentration);
					break;
			}
			
			// input temperature
			switch (in_temp_idx)
			{
				case 99:
					in_temp = 300;
					break;
				case 92:
					in_temp = 600;
					break;
				case 88:
					in_temp = 900;
					break;
				case 85:
					in_temp = 1200;
					break;
			}

			//calculation two concentration
#ifdef SQRT_INTEROP
			ub_concentration = sqrt_interop(ub_temp,lb_temp,in_temp,in_concentration,true);
			lb_concentration = sqrt_interop(ub_temp,lb_temp,in_temp,in_concentration,false);
      // verify the interpolation
      if ( (in_concentration - ub_concentration - lb_concentration) / in_concentration > 1e-3 )
      {  cout << "    Wrong SQRT interpolated\n"; }
      //cout << "SQRT interpolation\n";
#else
			ub_concentration = linear_interop(ub_temp,lb_temp,in_temp,in_concentration,true);
			lb_concentration = linear_interop(ub_temp,lb_temp,in_temp,in_concentration,false);
      // verify the interpolation
      if ( (in_concentration - ub_concentration - lb_concentration) / in_concentration > 1e-3 )
      {  cout << "    Wrong Linear interpolated\n"; }
      //cout << "Linear interpolation\n";
#endif

			//output
			switch (tmp[0])
			{
				case 'm':
					fout << head << "   " << in_isotope << '.' << lb_temp_idx << "c " << scientific << lb_concentration 
							 << ' ' << in_isotope << '.' << ub_temp_idx << "c " << scientific << ub_concentration
							 << endl;
					break;
				case ' ':
					fout << "     " << in_isotope << '.' << lb_temp_idx << "c " << scientific << lb_concentration
							 << ' ' << in_isotope << '.' << ub_temp_idx << "c " << scientific << ub_concentration
							 << endl;
					break;
			}
		}
		else
			fout << tmp << endl;
		
	}

	fin.close();
	fout.close();

	return 0;
}

double sqrt_interop(double ub, double lb, double in,double in_concentration,bool isub)
{
	if(isub == true)
		return in_concentration / (sqrt(ub) - sqrt(lb)) * (sqrt(in) - sqrt(lb));
	else
		return in_concentration / (sqrt(ub) - sqrt(lb)) * (sqrt(ub) - sqrt(in));
}

double linear_interop(double ub, double lb, double in,double in_concentration,bool isub)
{
	if(isub == true)
		return in_concentration / (ub - lb) * (in - lb);
	else
		return in_concentration / (ub - lb) * (ub - in);
}
