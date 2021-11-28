#include"functions.h"
#include<chrono>


#define DEBUG

#ifndef DEBUG
#define TIME
#endif

using namespace MyFunctions1;
using std::cout;
using std::endl;
using std::cin;
using std::chrono::system_clock;


int main()
{
	system_clock::time_point start, end;
	LongInt a, b, c(20, true);
	

	std::random_device seed;
	std::mt19937_64 mt(seed());
	std::uniform_int_distribution<> rand(10000000, 99999999);

	int ia = rand(mt), ib = -23547;
	a.random(40);
	b.random(20);

	//cout << a << " / " << b << " = " << a / b << endl;
	//cout << "空ループ" << a << "回" << endl;

#ifdef DEBUG
	/*
	cout << a << " + " << b << " = ";
	a += b;
	cout << a << endl;

	cout << a << " - " << b << " = ";
	a -= b;
	cout << a << endl;
	*/
	cout << ia << " % " << ib << " = " << ia % ib << endl;
	cout << a << " % " << b << " = " << a % b << endl;

	cout << a << " * " << b << " = ";
	a = a * b;
	cout << a << "\n";

#endif

#ifdef TIME
	start = system_clock::now();

	c = a * b;

	end = system_clock::now();

	double exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time of c = a * b : " << exetime << " ms\n";

	start = system_clock::now();

	//c = a / b;
	//for (LongInt i; i <= a; ++i);
	//a -= b;
	a *= b;
	end = system_clock::now();

	exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time of a *= b : " << exetime << " ms\n";
#endif
}