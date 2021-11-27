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

	a.random(10);
	b.random(8);
	b.absolute();
	a = -a;
	//cout << a << " / " << b << " = " << a / b << endl;
	//cout << "‹óƒ‹[ƒv" << a << "‰ñ" << endl;

	cout << a << " + " << b << " = ";
	a += b;
	cout << a << endl;

	cout << a << " - " << b << " = " << a - b << endl;
	start = system_clock::now();

	//c = a / b;
	//for (LongInt i; i <= a; ++i);

	end = system_clock::now();

	double exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time:" << exetime << " ms" << endl;
}