#include"functions.h"

using namespace MyFunctions1;
#define MULTI_DNUM 4

LongInt::LongInt()noexcept:data{}, sign{ 0 }
{}


LongInt::LongInt(int num) : data{}, sign{ num < 0 }
{
	num = abs(num);
	while (num > 0)
	{
		data.push_back((byte)(num % 10));
		num /= 10;
	}
	if (data.empty())data.push_back(0);
	digit_num = data.size();
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

	if (data.empty())
	{
		data.push_back(0);
		digit_num = 0;
	}
	else
	{
		digit_num = data.size() - 1;
		while (data[digit_num] == 0 && --digit_num != 0);
		++digit_num;
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

	if (data.empty())
	{
		data.push_back(0);
		digit_num = 0;
	}
	else
	{
		digit_num = data.size() - 1;
		while (data[digit_num] == 0 && --digit_num != 0);
		++digit_num;
	}
}

LongInt::LongInt(std::size_t size, bool):data(size)
{}

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
	for (long long i = digit_num - 1; i >= 0; --i)
	{
		str.push_back((char)(data[i] + '0'));
	}
}

bool LongInt::operator<(const LongInt& num2)const&
{
	if (sign == num2.sign)
	{
		if (digit_num == num2.digit_num)
		{
			for (std::size_t i = digit_num; i > 0;)
			{
				--i;
				if (data[i] < num2.data[i])return true;
				if (data[i] > num2.data[i])return false;
			}
			return false;
		}
		else return digit_num < num2.digit_num;
	}
	else return sign;
}

bool LongInt::operator>(const LongInt& num2)const&
{
	if (sign == num2.sign)
	{
		if (digit_num == num2.digit_num)
		{
			for (std::size_t i = digit_num; i > 0;)
			{
				--i;
				if (data[i] > num2.data[i])return true;
				if (data[i] < num2.data[i])return false;
			}
			return false;
		}
		else return digit_num > num2.digit_num;
	}
	else return num2.sign;
}

bool LongInt::operator>=(const LongInt& num2)const&
{
	return !operator<(num2);
}

bool LongInt::operator<=(const LongInt& num2)const&
{
	return !operator>(num2);
}


void LongInt::addabs(const LongInt& num2, LongInt& result)const&
{
	byte carry = 0;
	std::size_t i;
	for (i = 0; i < result.data.size() - 1; ++i)
	{
		result.data[i] = (*this)[i] + num2[i] + carry;
		carry = result.data[i] / (byte)10;
		result.data[i] %= (byte)10;
	}
	result.data[i] = carry;
	if (carry == 0)result.digit_num = i;
	else result.digit_num = ++i;
}

void LongInt::subtractabs(const LongInt& num2, LongInt& result)const&
{
	signed char brw = 0;
	signed char tmp;
	for (std::size_t i = 0; i < result.data.size(); ++i)
	{
		tmp = (char)(*this)[i] - (char)num2[i] + brw;
		if (tmp >= 0)
		{
			result.data[i] = (byte)tmp;
			brw = 0;
			if (tmp != 0)result.digit_num = i + 1;
		}
		else
		{
			result.data[i] = (byte)(tmp + 10);
			brw = -1;
			if (tmp != -10)result.digit_num = i + 1;
		}
	}
}

//*this > num2
bool LongInt::cmpabs(const LongInt& num2)const&
{
	if (digit_num == num2.digit_num)
	{
		for (std::size_t i = digit_num; i > 0;)
		{
			--i;
			if (data[i] > num2.data[i])return true;
			if (data[i] < num2.data[i])return false;
		}
		return false;
	}
	else return digit_num > num2.digit_num;
}

void LongInt::multiplyabs(long long num, LongInt& result)const&
{
	constexpr byte dnum = 12;
	std::size_t i;
	static std::vector<std::vector<byte>> addnum(dnum, std::vector<byte>(dnum + 1, 0));
	byte addnumhead = 0, j;

	long long tmp;

	for (i = 0; i < digit_num; ++i)
	{
		tmp = num * data[i];
		for (j = 0; j < dnum; ++j)
		{
			addnum[(addnumhead + j) % dnum][j] = (byte)(tmp % 10);
			tmp /= 10;
		}
		for (j = 0; j <= dnum; ++j)
		{
			result.data[i] += addnum[addnumhead][j];
			addnum[addnumhead][j] = 0;
		}
		addnumhead++;
		addnumhead %= dnum;
		addnum[addnumhead][dnum] = result.data[i] / (byte)10;
		result.data[i] %= (byte)10;
	}

	for (byte cnt = 0; cnt < dnum - 1; ++cnt)
	{
		for (j = 0; j <= dnum; ++j)
		{
			result.data[i + cnt] += addnum[addnumhead][j];
		}
		addnumhead++;
		addnumhead %= dnum;
		addnum[addnumhead][dnum] = result.data[i + cnt] / (byte)10;
		result.data[i + cnt] %= (byte)10;
		if (result.data[i + cnt] != 0)result.digit_num = i + cnt + 1;
	}
}

unsigned char LongInt::operator[](std::size_t index)const&
{
	return index < data.size() ? data[index] : 0;
}

LongInt LongInt::operator+(const LongInt& num)const&
{
	LongInt answer((std::max)(digit_num, num.digit_num) + 1, true);
	if (sign == num.sign)
	{
		addabs(num, answer);
		answer.sign = sign;
	}
	else if(cmpabs(num))
	{
		subtractabs(num, answer);
		answer.sign = sign;
	}
	else if(num.cmpabs(*this))
	{
		num.subtractabs(*this, answer);
		answer.sign = num.sign;
	}
	else
	{
		answer.sign = false;
	}
	return answer;
}

LongInt LongInt::operator-(const LongInt& num)const&
{
	LongInt answer((std::max)(digit_num, num.digit_num) + 1, true);
	if (sign == !num.sign)
	{
		addabs(num, answer);
		answer.sign = sign;
	}
	else if (cmpabs(num))
	{
		subtractabs(num, answer);
		answer.sign = sign;
	}
	else if (num.cmpabs(*this))
	{
		num.subtractabs(*this, answer);
		answer.sign = !sign;
	}
	else
	{
		answer.sign = false;
	}
	return answer;
}

LongInt LongInt::operator*(const LongInt& num)const&
{
	LongInt answer(digit_num + num.digit_num, true);
	std::size_t j;
	byte carry;
	for (std::size_t i = 0; i < num.digit_num; ++i)
	{
		carry = 0;
		for (j = 0; j < digit_num; ++j)
		{
			answer.data[i + j] += num.data[i] * data[j] + carry;
			carry = answer.data[i + j] / 10;
			answer.data[i + j] %= 10;
		}
		answer.data[i + j] = carry;
	}
	answer.sign = sign ^ num.sign;
	answer.digit_num = carry == 0 ? answer.data.size() - 1 : answer.data.size();
	return answer;
}

//“r’†
LongInt LongInt::operator/(const LongInt& num2)const&
{
	LongInt answer(digit_num - num2.digit_num + 1, true);
	LongInt r = *this;
	byte multi = 0, pivot = num2.data[num2.digit_num - 1];
	byte brw = 0;
	signed short tmp;
	for (std::size_t i = digit_num - num2.digit_num + 1; i <= 0;)
	{
		--i;
		multi = (r[i + num2.digit_num] * 10 + r.data[i + num2.digit_num - 1]) / (pivot + 1);
		if (multi != 0)
		{
			for (std::size_t j = 0; j < num2.digit_num; ++j)
			{
				tmp = (short)r[j + i] - (short)(num2[j] * multi);
			}
		}
		for (std::size_t j = num2.digit_num; j <= 0;)
		{
			--j;
		}
	}
	return answer;
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