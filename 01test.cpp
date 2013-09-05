#include <iostream>
#include <iomanip> // lib for io-format setting 
#include "stdlib.h"
#include "math.h"
#include <fstream>

#define SQRT_INTERPO
//#define DEBUG

double sqrt_interpo(double ub, double lb, double in,double in_concentration,bool isub);
double linear_interpo(double ub, double lb, double in,double in_concentration,bool isub);


using namespace std;

int main( int argc, char * argv[] )
{
  
  char tmp[256];                  // buffer for parsing
	ifstream fin;										// input stream
	ofstream fout;									// output stream

  // Input Parameters
  bool flag = false;            // flag 
  int in_isotope;
  float in_concentration;
  char in_c;
  char head[5];

  // calculation Parameters
  int in_temp;                      // input temperature
  int ub_temp;                      // upper bound temperature
  int lb_temp;                      // lower bound temperature
  int in_temp_idx;                  // input temperature index
  int ub_temp_idx;                  // upper bound temperature index
  int lb_temp_idx;                  // lower bound temperature index
  double ub_concentration;          // upper bound
  double lb_concentration;          // lower bound
//int up_idxtmp, lo_idxtmp;         // temperature index 

	// if input arguments < 4, leave
	if(argc < 5)
		return -1;

	// check input arguments
	if(!argv[1] || !argv[2])
		return -1;
	else
	{
		char tmp[256];
		//wcstombs(tmp,argv[1],sizeof(tmp));    // this may be unnecessary because argv is of type _tchar
		ub_temp = atoi(tmp);                  // 1st argument->upper temperature
		//wcstombs(tmp,argv[2],sizeof(tmp));    // this may be unnecessary because argv is of type _tchar
		lb_temp = atoi(tmp);                  // 2nd argument->lower temperature

#ifdef DEBUG
		cout << ub_temp << ' ' << lb_temp << endl;
#endif
	}
	if(!argv[3] || !argv[4])					      // 3rd srgument->input filename,4th srgument->output filename
		return -1;										
	else
	{
		fin.open(argv[3]);
		fout.open(argv[4]);
	}
		
	// build temperature map
	map<int,int> hm;
	hm[300] = 70;
	hm[350] = 35;
	hm[400] = 40;
	hm[450] = 45;
	hm[500] = 50;
	hm[550] = 55;
	hm[600] = 71;
	hm[650] = 70;
	hm[700] = 69;
	hm[750] = 75;
	hm[800] = 80;
	hm[850] = 85;
	hm[900] = 72;
	hm[950] = 95;
	hm[1000] = 16;
	hm[1050] = 21;
	hm[1100] = 22;
	hm[1150] = 23;
	hm[1200] = 73;
		 

	// decide input, upper and lower temp
	ub_temp_idx = hm[ub_temp];
	lb_temp_idx = hm[lb_temp];
	
	// no such temperature
	if(ub_temp_idx == 0 ||lb_temp_idx == 0)
	  return -1;

  while(fin.getline(tmp,256))
  {
    if(tmp[0] == 'm' && atoi(tmp+1))     // 'mx' format, where x is an integer
      flag = true;
    else if(tmp[0] == 'c')               // 'c' is comment
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
      
			// get input temperature
			for(map<int,int>::iterator i = hm.begin(); i != hm.end(); i++)
			{
				if(i->second == in_temp_idx)
				{
					in_temp = i->first;
					break;
				}
			}

      //calculation two concentration
#ifdef SQRT_INTERPO
      ub_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
      lb_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
      // verify the interpolation
      if ( (in_concentration - ub_concentration - lb_concentration) / in_concentration > 1e-3 )
      {  cout << "    Wrong SQRT interpolated\n"; }
      //cout << "SQRT interpolation\n";
#else
      ub_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
      lb_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
      // verify the interpolation
      if ( (in_concentration - ub_concentration - lb_concentration) / in_concentration > 1e-3 )
      {  cout << "    Wrong Linear interpolated\n"; }
      //cout << "Linear interpolation\n";
#endif

      // output
      switch (tmp[0])
      {
        case 'm':  
          fout <<  left << setw(5)  << head ;
          break;
        case ' ':
          fout << "     ";
          break;
      }
      fout << right << setw(7)  << in_isotope << '.' << lb_temp_idx << "c " 
           << right << setw(11) << scientific << uppercase << setprecision(4) << lb_concentration 
           << right << setw(7)  << in_isotope << '.' << ub_temp_idx << "c " 
           << right << setw(11) << scientific << uppercase << setprecision(4) << ub_concentration 
           << endl;
    }
    else
      fout << tmp << endl;
    
  }

  fin.close();
  fout.close();

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
