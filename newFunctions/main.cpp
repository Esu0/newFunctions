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
	LongInt a, b, c(20, true);
	a.random(5);
	b.random(20);

	a.multiplyabs(23793000000, c);
	cout << a << " * " << 23793000000 << " = " << c << endl;
	start = system_clock::now();

	c = a + b;

	end = system_clock::now();

	double exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time:" << exetime << " ms" << endl;
}