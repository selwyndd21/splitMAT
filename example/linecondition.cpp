#include <iostream>
#include <iomanip> // lib for io-format setting 
#include "stdlib.h"
#include "math.h"
#include <fstream>
#include <map>
#include <locale>


using namespace std;

int main( int argc, char * argv[] )
{   
  string strline = "m26   5010.71c 3.6372E-08";
  strline = "mt26 graph.70t";
  //strline = "mode  n";
  //strline = "kcode 50000 1 50 1050";
  if (strline[0] == 'm' && isdigit(strline[1]) == true)// "^m"+number
  {
    //calculate();
    //split()
    cout << "calculate and split\n";
  }
  else if (strline[0] == 'm' && strline[1] == 't')// "^mt"
  {
    //split();
    cout << "split\n";
  }
  else if (isspace(strline[1]) == true)// "^ "
  {
    //split()
    cout << "split by calculated ratio\n";
  }
  else
  {
    cout << strline << endl;
    cout << "only print line\n";
  }

  return 0;
}

