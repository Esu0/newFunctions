#include"functions.h"
#include<chrono>

using namespace MyFunctions1;
using std::cout;
using std::endl;
using std::cin;
using std::chrono::system_clock;
int main()
{
	system_clock::time_point start, end;
	LongInt a, b, c;
	a.random(200000);
	b.random(200000);

	start = system_clock::now();

	c = a + b;

	end = system_clock::now();

	double exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time:" << exetime << " ms" << endl;
}