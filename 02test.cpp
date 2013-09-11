#include "stdlib.h"
#include "math.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "mapclass.h"

#define SQRT_INTERPO	
//#define DEBUG

double sqrt_interpo(double ub, double lb, double in,double in_concentration,bool isub);
double linear_interpo(double ub, double lb, double in,double in_concentration,bool isub);


using namespace std;

int _tmain( int argc, char * argv[] )
{
	
	char charline[256];									// buffer for parsing every input line
	ifstream fin;												// input file stream
	ofstream fout;											// output file stream

	// Input Parameters
	bool flag = false;									// flag 
	float in_concentration;

	// calculation Parameters
	int in_temp;											// input temperature
	int ub_temp;											// upper bound temperature
	int lb_temp;											// lower bound temperature
	int in_temp_idx;									// input temperature index
	int ub_temp_idx;									// upper bound temperature index
	int lb_temp_idx;									// lower bound temperature index
	double ub_concentration;					// upper bound
	double lb_concentration;					// lower bound


	// if input arguments < 7, leave
	if(argc < 7)
		return -1;

	// check arguments
	if(!argv[1] || !argv[2] || !argv[3] || !argv[4] || !argv[5] || !argv[6])
		return -1;

	ub_temp = atoi(argv[2]);
	lb_temp = atoi(argv[3]);

  // open input and output file
	fin.open(argv[3]);
	fout.open(argv[4]);

	// build temperature map
	mapclass hm_m(argv[5]);			// map for 'm'
	mapclass hm_mt(argv[6]);		// map for "mt"

	// main procedure
	while(fin.getline(charline,256))
	{
		string strline = charline;		// read line into string
		istringstream ss(strline);

		if(strline.find("mt") == 0)				// "mt" as head of line
		{
			string strhead;
			string strisotope;

			flag = true;										// enable processing
			ss >> strhead;									// read "mtx" head

			fout <<  left << setw(5)  << strhead ;
			while(ss >> strisotope)					// read input isotope
			{
				strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_mt.get_value(lb_temp)));
				fout << setw(10) << strisotope;
				strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_mt.get_value(ub_temp)));
				fout << setw(10) << strisotope;
			}
			fout << endl;

#ifdef DEBUG
			cout << hm_mt.get_value(lb_temp) << ' ' << hm_mt.get_value(ub_temp) << endl;
#endif
		}
		else if(strline.find('m') == 0 && !strline.find("mode") == 0)		// "m" as head of line, ignore "mode"
		{
			string strhead;
			string strisotope;

			flag = true;										  // enable processing
			ss >> strhead;										// read "mx" head

			fout <<  left << setw(5)  << strhead ;
			while(ss >> strisotope)					  // read input isotope
			{
				ss >> in_concentration;					// read concentration
				in_temp_idx = stoi(strisotope.substr(strisotope.find('.')+1,2));
				in_temp = hm_m.get_key(in_temp_idx);

#ifdef SQRT_INTERPO
				ub_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
				lb_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
#else
				ub_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
				lb_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
#endif
				strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_m.get_value(lb_temp)));
				fout << right << setw(7)  << strisotope
					 << right << setw(13) << scientific << uppercase << setprecision(4) << lb_concentration;
				strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_m.get_value(ub_temp)));
				fout << right << setw(13) << strisotope
				     << right << setw(13) << scientific << uppercase << setprecision(4) << ub_concentration 
				     << endl;
			}
#ifdef DEBUG
			cout << lb_temp << ' ' << ub_temp << endl;
			cout << hm_m.get_value(lb_temp) << ' ' << hm_m.get_value(ub_temp) << endl;
#endif
		}
		else if(strline.find('c') == 0)		// "c" as head of line
		{
			flag = false;
			fout << strline << endl;
		}
		else
		{
			if(flag == true)
			{
				string strisotope;
				flag = true;										// enable processing

				while(ss >> strisotope)					// read input isotope
				{
					ss >> in_concentration;				// read concentration
					in_temp_idx = stoi(strisotope.substr(strisotope.find('.')+1,2));
					in_temp = hm_m.get_key(in_temp_idx);

	#ifdef SQRT_INTERPO
					ub_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
					lb_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
	#else
					ub_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
					lb_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
	#endif
					strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_m.get_value(lb_temp)));
					fout << right << setw(13)  << strisotope 
						 << right << setw(13) << scientific << uppercase << setprecision(4) << lb_concentration;
					strisotope.replace(strisotope.find('.') + 1, 2, stoi(hm_m.get_value(ub_temp)));
					fout << right << setw(13)  << strisotope
						 << right << setw(13) << scientific << uppercase << setprecision(4) << ub_concentration 
						 << endl;
				}
			}
			else
				fout << strline << endl;
		}
	}

	fin.close();
	fout.close();

	getchar();
	return 0;
}

double sqrt_interpo(double ub, double lb, double in,double in_concentration,bool isub)
{
	if(isub == true)
		return in_concentration / (sqrt(ub) - sqrt(lb)) * (sqrt(in) - sqrt(lb));
	else
		return in_concentration / (sqrt(ub) - sqrt(lb)) * (sqrt(ub) - sqrt(in));
}

double linear_interpo(double ub, double lb, double in,double in_concentration,bool isub)
{
	if(isub == true)
		return in_concentration / (ub - lb) * (in - lb);
	else
		return in_concentration / (ub - lb) * (ub - in);
}
