// This file is modified by 02test.cpp in
// commit: 5db1a19503f5d79101b2a68fd6abd9aa52469f43
// This is not compete yet.
//
// Problem: flag would reset after m-card and mt-card.
// 
#include "stdlib.h"
#include "math.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "mapclass.h"
#include <locale>

#define SQRT_INTERPO  
//#define DEBUG_pseudo
//#define DEBUG_argv
#define DEBUG_line1
#define DEBUG_line2

double sqrt_interpo(double ub, double lb, double in,double in_concentration,bool isub);
double linear_interpo(double ub, double lb, double in,double in_concentration,bool isub);


using namespace std;

int main( int argc, char * argv[] )
{
  
  char charline[256];               // buffer for parsing every input line
  ifstream fin;                     // input file stream
  ofstream fout;                    // output file stream

  // Input Parameters
  bool flag = false;                // flag 
  float in_concentration;

  // calculation Parameters
  int in_temp;                      // input temperature
  int ub_temp;                      // upper bound temperature
  int lb_temp;                      // lower bound temperature
  int in_temp_idx;                  // input temperature index
  int ub_temp_idx;                  // upper bound temperature index
  int lb_temp_idx;                  // lower bound temperature index
  double ub_concentration;          // upper bound
  double lb_concentration;          // lower bound


  // if input arguments < 7, leave
  if(argc < 7)
    return -1;

  // check arguments
  if(!argv[1] || !argv[2] || !argv[3] || !argv[4] || !argv[5] || !argv[6])
    return -1;

  ub_temp = atoi(argv[1]);
  lb_temp = atoi(argv[2]);
  if (lb_temp > ub_temp)                  // correct lb_temp and ub_temp relationship
  {
    in_temp = ub_temp;
    ub_temp = lb_temp;
    lb_temp = in_temp;
    in_temp = 0;
  }
  else if (lb_temp == ub_temp)
    return -1;
#ifdef DEBUG_argv
    cout << ub_temp << ' ' << lb_temp << endl;
#endif

  // open input and output file
  fin.open(argv[3]);
  fout.open(argv[4]);

  // build temperature map
  mapclass hm_m(argv[5]);      // map for 'm'
  mapclass hm_mt(argv[6]);    // map for "mt"

////////////////////////////////////////////////////////////////////////////////
//----- MAIN PROCESSING BLOCK: START
//  Line processing by flag == true
//  Condition for every line:
//  1. ^m[0-9]            : flag = true; calculate() and split();
//  2. ^mt                : flag = true; split();
//  3. ^\ && flag == true : calculate() and split();
//  else                  : print the line;
//
  while(fin.getline(charline,256))
  {
    #ifdef DEBUG_line1
    cout << "  flag is " << flag << endl;
    #endif
    string strline = charline;    // read line into string
    istringstream ss(strline);

    if (strline[0] == 'm' && isdigit(strline[1]))// "^m"+number
    {
      #ifdef DEBUG_line2
      cout << "    enter ^m[0-9];" ;
      #endif
      flag = true;                      // enable processing
      string strhead;
      string strisotope;
      ss >> strhead;                    // read "mx" head

      // calculate() and split()
      fout <<  left << setw(5)  << strhead ;
      while(ss >> strisotope)            // read input isotope
      {
        // calculate()
        ss >> in_concentration;          // read concentration
        in_temp_idx = stoi(strisotope.substr(strisotope.find('.')+1,2));
        in_temp = hm_m.get_key(in_temp_idx);

#ifdef SQRT_INTERPO
        ub_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
        lb_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
        #ifdef DEBUG_pseudo
        cout << "    SQRT interp:" << strhead << " " << strisotope << "= " 
             << ub_concentration << " + " << lb_concentration << endl;
        #endif
#else
        ub_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
        lb_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
        #ifdef DEBUG_pseudo  
        cout << "    LINE interp:" << strhead << " " << strisotope << "= " 
             << ub_concentration << " + " << lb_concentration << endl;
        #endif
#endif
        // split()
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_m.get_value(lb_temp)));
        fout << right << setw(7)  << strisotope
             << right << setw(13) << scientific << uppercase << setprecision(4) << lb_concentration;
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_m.get_value(ub_temp)));
        fout << right << setw(13) << strisotope
             << right << setw(13) << scientific << uppercase << setprecision(4) << ub_concentration 
             << endl;
      }
      #ifdef DEBUG_pseudo  
      cout << ub_temp << ": " << hm_m.get_value(ub_temp) << " + " << lb_temp << ": " << hm_m.get_value(lb_temp) << endl;
      #endif
    }
    else if (strline[0] == 'm' && strline[1] == 't')// "^mt"
    {
      #ifdef DEBUG_line2
      cout << "    enter ^mt[0-9];";
      #endif
      flag = true;                    // enable processing
      string strhead;
      string strisotope;
      ss >> strhead;                  // read "mtx" head

      // split()
      fout <<  left << setw(5)  << strhead ;
      while(ss >> strisotope)          // read input isotope
      {
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_mt.get_value(lb_temp)));
        fout << setw(10) << strisotope;
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_mt.get_value(ub_temp)));
        fout << setw(10) << strisotope;
      }
      fout << endl;

      #ifdef DEBUG_pseudo
      cout << hm_mt.get_value(lb_temp) << ' ' << hm_mt.get_value(ub_temp) << endl;
      #endif
    }
    else if (isspace(strline[0]) && flag == true)// "^ " and enabled
    {//only the line after m-card or mt-card belong to material
      #ifdef DEBUG_line2
      cout << "    enter ^[ ];" << endl;
      #endif
      string strisotope;

      // calculate() and split()
      while(ss >> strisotope)          // read input isotope
      {
        // calculate()
        ss >> in_concentration;        // read concentration
        in_temp_idx = stoi(strisotope.substr(strisotope.find('.')+1,2));
        in_temp = hm_m.get_key(in_temp_idx);

#ifdef SQRT_INTERPO
        ub_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
        lb_concentration = sqrt_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
        #ifdef DEBUG_pseudo
        cout << "    SQRT interp:" << " " << strisotope << "= " 
             << ub_concentration << " + " << lb_concentration << endl;
        #endif
#else
        ub_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,true);
        lb_concentration = linear_interpo(ub_temp,lb_temp,in_temp,in_concentration,false);
        #ifdef DEBUG_pseudo  
        cout << "    LINE interp:" << " " << strisotope << "= " 
             << ub_concentration << " + " << lb_concentration << endl;
        #endif
#endif
        // split()
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_m.get_value(lb_temp)));
        fout << right << setw(13)  << strisotope 
             << right << setw(13) << scientific << uppercase << setprecision(4) << lb_concentration;
        strisotope.replace(strisotope.find('.') + 1, 2, to_string(hm_m.get_value(ub_temp)));
        fout << right << setw(13)  << strisotope
             << right << setw(13) << scientific << uppercase << setprecision(4) << ub_concentration 
             << endl;
      }
    }
    else
    {
      flag = false;
      fout << strline << endl;
    }
  }
//----- MAIN PROCESSING BLOCK: END
////////////////////////////////////////////////////////////////////////////////

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

