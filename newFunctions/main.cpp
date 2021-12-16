#include"functions.h"
#include"LongIntBin.h"
#include<chrono>
#include"number_theoretic_transform.h"

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

	int ia = 3313822, ib = -10002030;
#if defined(DEBUG)
	a.random(10);
	b.random(5);
	a = 3;
	c = 1;
#else
	a.random(100000);
	b.random(100000);
#endif

	//cout << a << " / " << b << " = " << a / b << endl;
	//cout << "‹óƒ‹[ƒv" << a << "‰ñ" << endl;

#ifdef DEBUG
	/*
	cout << a << " + " << b << " = ";
	a += b;
	cout << a << endl;

	cout << a << " - " << b << " = ";
	a -= b;
	cout << a << endl;
	*/
	//cout << ia << " % " << ib << " = " << ia % ib << endl;
	//cout << a << " % " << b << " = " << a % b << endl;

	//cout << a << " * " << b << " = ";
	//a = a * b;
	//cout << a << "\n";
	//cin >> a;
	//cin >> b;
	//a = ia;
	//b = ib;

	//cout << "ia = " << ia << endl;
	//cout << "a = " << a << endl;
	//cout << "ib = " << ib << endl;
	//cout << "b = " << b << endl;
	//cout << "ia + ib = " << ia + ib << endl;
	//cout << "a + b = " << a + b << endl;
	//a += b;
	//cout << "a + b = " << a << endl;
	//cout << "a - b = " << a - b << endl;
	//a -= b;
	//cout << "a - b = " << a << endl;
	//cout << "a * b = " << a * b << endl;
	//x.outhex();
	Number_Theoretic_Transform<> ntt;
	std::vector<unsigned long long> ar = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
	unsigned long long* copy = new unsigned long long[16];
	ntt(ar, 3, false, copy);
	ntt(ar, 3, true, copy);
	for (auto tmp : ar)cout << tmp << " ";
#endif

#ifdef TIME
	LongInt::mycomplex za(1, 1.2), zb(1.01, 1.03), zc;
	std::complex<double> za2(1, 1.2), zb2(1.01, 1.03), zc2;
	start = system_clock::now();

	//c = a + b;

	end = system_clock::now();

	double exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time of c = a * b : " << exetime << " ms\n";

	start = system_clock::now();

	//c = a / b;
	//for (LongInt i; i <= a; ++i);
	//a -= b;
	c = a * b;
	/*
	for (int i = 0; i < 100000000; ++i)
	{

	}
	*/
	end = system_clock::now();
	cout << etime << endl;
	exetime = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) / 1000.0;
	cout << "execution time of a *= b : " << exetime << " ms\n";
#endif
}