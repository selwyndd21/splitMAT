#include "stdio.h"
#include "mapclass.h"

using namespace std;

mapclass::mapclass(char *filename)
{
	ifstream fin(filename);
	char charline[256];
	int idx = 0;
	int temp = 0;

	while(fin.getline(charline,256))
	{
		sscanf(charline,"%d %d %*d", &temp, &idx);
		hm[temp] = idx;
	}
	fin.close();
}

int mapclass::get_value(int key)
{
	return hm.find(key)->second;
}

int mapclass::get_value(int key) const
{
	return hm.find(key)->second;
}

int mapclass::get_key(int value)
{
	for(map<int,int>::iterator i = hm.begin(); i != hm.end(); i++)
	{
		if(i->second == value)
		{
			return i->first;
		}
	}
	return -1;
}

int mapclass::get_key(int value) const
{
	for(map<int,int>::const_iterator i = hm.begin(); i != hm.end(); i++)
	{
		if(i->second == value)
		{
			return i->first;
		}
	}
	return -1;
}
