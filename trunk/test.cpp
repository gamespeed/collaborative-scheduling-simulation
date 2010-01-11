#include <iostream.h>
#include <fstream.h>

using namespace std;

int main()
{
	ofstream i_file;
	i_file.open("test.in");
	i_file<<"Hello world"<< endl;
	i_file.close();
	return 0;
}