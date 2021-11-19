#include"functions.h"

using namespace MyFunctions1;

LongInt::LongInt() :data{}, sign{ 0 }
{}


LongInt::LongInt(int num) : data{}, sign{ num < 0 }
{
	num = abs(num);
	while (num > 0)
	{
		data.push_back((byte)(num % 10));
		num /= 10;
	}
}


LongInt::LongInt(const char* snum):data{}, sign{0}
{
	byte tmp;
	long long i = 0;
	if (*snum == '-')
	{
		++snum;
		sign = 1;
	}
	if (*snum == '+')
	{
		++snum;
	}
	for (; *snum != '\0'; ++snum)
	{
		data.push_back((byte)(*snum - '0'));
	}

	//‡˜‚ğ‹t‚É‚·‚é
	while ((std::size_t)i < data.size() / 2)
	{
		tmp = data[i];
		data[i] = data[data.size() - i - 1];
		data[data.size() - i - 1] = tmp;
		++i;
	}
}


LongInt::LongInt(const std::string& snum):data{}
{
	byte to = 0;
	if (snum[0] == '-')
	{
		to = 1;
		sign = 1;
	}
	else if (snum[0] == '+')
	{
		to = 1;
	}
	for (long long i = snum.length() - 1; i >= to; --i)
	{
		data.push_back((byte)(snum.at(i) - '0'));
	}
}

LongInt& LongInt::operator=(int num)&
{
	LongInt tmp;
	while (num > 0)
	{
		tmp.data.push_back((unsigned char)(num % 10));
		num /= 10;
	}
	return *this;
}

void LongInt::tostring(std::string& str)const&
{
	if (sign)str.push_back('-');
	for (long long i = data.size() - 1; i >= 0; --i)
	{
		str.push_back((char)(data[i] + '0'));
	}
}

std::ostream& operator<<(std::ostream& output, const MyFunctions1::LongInt& numout)
{
	std::string tmp;
	numout.tostring(tmp);
	output << tmp;
	return output;
}


std::istream& operator>>(std::istream& input, LongInt& numin)
{
	std::string tmp;
	input >> tmp;
	numin = tmp;
	return input;
}