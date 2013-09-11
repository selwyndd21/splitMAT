#include <map>
#include <fstream>

using namespace std;

class mapclass
{
public:

	mapclass(char *);       // constructor

	int get_value(int);     // get value from key
	int get_key(int);       // get key from value

	int get_value(int) const;
	int get_key(int) const;

private:
	map<int,int> hm;
};
