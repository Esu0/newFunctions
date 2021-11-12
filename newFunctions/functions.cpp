#include"functions.h"

using namespace MyFunctions1;

LongInt::LongInt() :data(1, 0), sign{0}
{}

LongInt::LongInt(int num):data(), sign{num < 0}
{
	while (num > 0)
	{
		data.push_back((unsigned char)(num % 10));
		num /= 10;
	}
}