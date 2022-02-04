#include"functions.h"
#include<time.h>
#include"number_theoretic_transform.h"

using namespace MyFunctions1;


LongInt::LongInt()noexcept:data(1), sign{0}
{}

#if VERSION == 1
LongInt::LongInt(int num): data{}, sign{ num < 0 }
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
#elif VERSION == 2
LongInt::LongInt(int num) : data(1, num), sign{0}
{
	fix_carry();
}
#endif

LongInt::LongInt(const char* snum):data{}, sign{0}
{
	byte tmp;
	std::size_t i = 0;
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
#if VERSION == 1
		data.push_back((byte)(*snum - '0'));
#elif VERSION == 2
		data.push_back(*snum - '0');
#endif
	}

	//èáèòÇãtÇ…Ç∑ÇÈ
	while ((std::size_t)i < data.size() / 2)
	{
		tmp = data[i];
		data[i] = data[data.size() - i - 1];
		data[data.size() - i - 1] = tmp;
		++i;
	}
#if VERSION == 1
	if (data.empty())
	{
		data.push_back(0);
		digit_num = 1;
	}
	else
	{
		digit_num = data.size() - 1;
		while (data[digit_num] == 0 && digit_num != 0 && --digit_num != 0);
		++digit_num;
	}
#elif VERSION == 2
	del_zero();
#endif
}

#if VERSION == 1
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
#endif

LongInt::LongInt(std::size_t size, bool):data(size)
{}

#if VERSION == 1
LongInt& LongInt::operator=(int num)&
{
	if (!num)
	{
		*this = LongInt();
		return *this;
	}
	std::vector<byte> tmp;
	sign = num < 0;
	num *= sign ? -1 : 1;
	while (num > 0)
	{
		tmp.push_back((unsigned char)(num % 10));
		num /= 10;
	}
	data = std::move(tmp);
	digit_num = data.size();
	return *this;
}
#endif

void LongInt::tostring(std::string& str)const&
{
	if (sign)str.push_back('-');
	for (long long i = data.size() - 1; i >= 0; --i)
	{
		str.push_back((char)(data[i] + '0'));
	}
}


bool LongInt::operator<(const LongInt& num2)const& noexcept
{
	if (sign == num2.sign)
	{
		if (data.size() == num2.data.size())
		{
			for (std::size_t i = data.size(); i > 0;)
			{
				--i;
				if (data[i] < num2.data[i])return true;
				if (data[i] > num2.data[i])return false;
			}
			return false;
		}
		else return data.size() < num2.data.size();
	}
	else return sign;
}

bool LongInt::operator>(const LongInt& num2)const& noexcept
{
	if (sign == num2.sign)
	{
		if (data.size() == num2.data.size())
		{
			for (std::size_t i = data.size(); i > 0;)
			{
				--i;
				if (data[i] > num2.data[i])return true;
				if (data[i] < num2.data[i])return false;
			}
			return false;
		}
		else return data.size() > num2.data.size();
	}
	else return num2.sign;
}

bool LongInt::operator>=(const LongInt& num2)const& noexcept
{
	return !operator<(num2);
}

bool LongInt::operator<=(const LongInt& num2)const& noexcept
{
	return !operator>(num2);
}

#if VERSION == 1
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
#endif

#if VERSION == 1
unsigned char LongInt::operator[](std::size_t index)const& noexcept
{
	return index < data.size() ? data[index] : 0;
}
#elif VERSION == 2
int LongInt::operator[](std::size_t index)const& noexcept
{
	return index < data.size() ? data[index] : 0;
}
#endif

LongInt& LongInt::operator++()&
{
	if (sign)
	{
		if (data.size() == 1 && data[0] == 1)
		{
			data[0] = 0;
			sign = 0;
			return *this;
		}
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			if (!(data[i]))
			{
				data[i] = 9;
			}
			else
			{
				--(data[i]);
				return *this;
			}
		}
	}
	else
	{
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			++(data[i]);
			if (data[i] < 10)return *this;
			else data[i] %= 10;
		}
		data.push_back(1);
	}
	return *this;
}

LongInt LongInt::operator++(int)&
{
	LongInt tmp = *this;
	this->operator++();
	return tmp;
}

LongInt& LongInt::operator--()&
{
	if (sign)
	{
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			++(data[i]);
			if (data[i] < 10)return *this;
			else data[i] %= 10;
		}
		data.push_back(1);
	}
	else
	{
		if (data.size() == 1 && data[0] == 0)
		{
			data[0] = 1;
			sign = 1;
			return *this;
		}
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			if (!(data[i]))
			{
				data[i] = 9;
			}
			else
			{
				--(data[i]);
				return *this;
			}
		}
	}
	return *this;
}

LongInt LongInt::operator--(int)&
{
	LongInt tmp = *this;
	this->operator--();
	return tmp;
}


LongInt LongInt::operator+()const&
{
	return *this;
}

LongInt LongInt::operator-()const&
{
	LongInt tmp = *this;
	if (!(tmp.data.size() == 1 && tmp.data[0] == 0))tmp.sign = !tmp.sign;
	return tmp;
}

#if VERSION == 1
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
#elif VERSION == 2
LongInt LongInt::operator+(LongInt num)const&
{
	std::size_t i;
	if (num.sign != sign)
	{
		for (i = 0; i < num.data.size(); ++i)
		{
			num.data[i] -= (*this)[i];
		}
		for (; i < data.size(); ++i)
		{
			num.data.push_back(-(this->data[i]));
		}
	}
	else
	{
		for (i = 0; i < num.data.size(); ++i)
		{
			num.data[i] += (*this)[i];
		}
		for (; i < data.size(); ++i)
		{
			num.data.push_back(this->data[i]);
		}
	}
	num.fix_carry();
	return num;
}
#endif

#if VERSION == 1
LongInt& LongInt::operator+=(const LongInt& num)&
{
	if (sign == num.sign)
	{
		std::size_t i;
		byte carry = 0;
		for (i = 0; i < digit_num; ++i)
		{
			if (i >= num.digit_num)
			{
				data[i] += carry;
				if (data[i] >= 10)
				{
					data[i] -= 10;
					carry = 1;
					continue;
				}
				else
				{
					carry = 0;
					++i;
					goto additionEnd;
				}
			}
			data[i] += num.data[i] + carry;
			carry = data[i] / 10;
			data[i] %= 10;
		}
		for (; i < data.size(); ++i)
		{
			if (i >= num.digit_num)
			{
				data[i] = carry;
				digit_num += carry;
				carry = 0;
				break;
			}
			data[i] += num.data[i] + carry;
			carry = data[i] / 10;
			data[i] %= 10;
			++digit_num;
		}
		for (; i < num.digit_num; ++i)
		{
			data.push_back(num[i] + carry);
			carry = data[i] / 10;
			data[i] %= 10;
			++digit_num;
		}
	additionEnd:
		if (carry)
		{
			if (i >= data.size())data.push_back(1);
			else data[i] = 1;
			++digit_num;
		}
	}
	else
	{
		signed char cmpresult = 0;
		byte brw = 0;
		std::size_t i;
		if (digit_num == num.digit_num)
		{
			for (i = digit_num; i > 0;)
			{
				--i;
				if (data[i] != num.data[i])
				{
					cmpresult = data[i] > num.data[i] ? 1 : -1;
					break;
				}
			}
		}
		else cmpresult = digit_num > num.digit_num ? 1 : -1;
		if (!cmpresult)*this = LongInt();
		else if (cmpresult == 1)
		{
			for (i = 0; i < num.digit_num; ++i)
			{
				data[i] += 10 - num.data[i] - brw;
				brw = (data[i] / 10) ? 0 : 1;
				data[i] %= 10;
			}
			if (brw)
			{
				for (; i < digit_num; ++i)
				{
					if (data[i])
					{
						--data[i];
						break;
					}
					else data[i] = 9;
				}
			}
			check_dnum(digit_num - 1);
		}
		else
		{
			for (i = 0; i < digit_num; ++i)
			{
				data[i] = 10 + num.data[i] - data[i] - brw;
				brw = (data[i] / 10) ? 0 : 1;
				data[i] %= 10;
			}
			for (; i < data.size() && i < num.digit_num; ++i)
			{
				if (num.data[i])
				{
					data[i] = num.data[i] - brw;
					brw = 0;
				}
				else
				{
					data[i] = brw ? 9 : 0;
				}
			}
			for (; i < num.digit_num; ++i)
			{
				if (num.data[i])
				{
					data.push_back(num.data[i] - brw);
					brw = 0;
				}
				else
				{
					data.push_back(brw ? 9 : 0);
				}
			}
			check_dnum(num.digit_num - 1);
			sign = num.sign;
		}
	}
	return *this;
}
#elif VERSION == 2
LongInt& LongInt::operator+=(const LongInt& num)&
{
	unsigned long long i;
	if (sign == num.sign)
	{
		for (i = 0; i < (std::min)(num.data.size(), data.size()); ++i)data[i] += num.data[i];
		for (; i < num.data.size(); ++i)data.push_back(num.data[i]);
	}
	else
	{
		for (i = 0; i < (std::min)(data.size(), num.data.size()); ++i)data[i] -= num.data[i];
		for (; i < num.data.size(); ++i)data.push_back(-num.data[i]);
	}
	fix_carry();
	return *this;
}
#endif

#if VERSION == 1
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
#elif VERSION == 2
LongInt LongInt::operator-(LongInt num)const&
{
	std::size_t i;
	num.sign = !num.sign;
	if (sign != num.sign)
	{
		for (i = 0; i < num.data.size(); ++i)
		{
			num.data[i] -= (*this)[i];
		}
		for (; i < data.size(); ++i)
		{
			num.data.push_back(-(this->data[i]));
		}
	}
	else
	{
		for (i = 0; i < num.data.size(); ++i)
		{
			num.data[i] += (*this)[i];
		}
		for (; i < data.size(); ++i)
		{
			num.data.push_back(this->data[i]);
		}
	}
	num.fix_carry();
	return num;
}
#endif

#if VERSION == 1
LongInt& LongInt::operator-=(const LongInt& num)&
{
	if (sign == num.sign)
	{
		signed char cmpresult = 0;
		byte brw = 0;
		std::size_t i;
		if (digit_num == num.digit_num)
		{
			for (i = digit_num; i > 0;)
			{
				--i;
				if (data[i] != num.data[i])
				{
					cmpresult = data[i] > num.data[i] ? 1 : -1;
					break;
				}
			}
		}
		else cmpresult = digit_num > num.digit_num ? 1 : -1;
		if (!cmpresult)*this = LongInt();
		else if (cmpresult == 1)
		{
			for (i = 0; i < num.digit_num; ++i)
			{
				data[i] += 10 - num.data[i] - brw;
				brw = (data[i] / 10) ? 0 : 1;
				data[i] %= 10;
			}
			if (brw)
			{
				for (; i < digit_num; ++i)
				{
					if (data[i])
					{
						--data[i];
						break;
					}
					else data[i] = 9;
				}
			}
			check_dnum(digit_num - 1);
		}
		else
		{
			for (i = 0; i < digit_num; ++i)
			{
				data[i] = 10 + num.data[i] - data[i] - brw;
				brw = (data[i] / 10) ? 0 : 1;
				data[i] %= 10;
			}
			for (; i < data.size() && i < num.digit_num; ++i)
			{
				if (num.data[i])
				{
					data[i] = num.data[i] - brw;
					brw = 0;
				}
				else
				{
					data[i] = brw ? 9 : 0;
				}
			}
			for (; i < num.digit_num; ++i)
			{
				if (num.data[i])
				{
					data.push_back(num.data[i] - brw);
					brw = 0;
				}
				else
				{
					data.push_back(brw ? 9 : 0);
				}
			}
			check_dnum(num.digit_num - 1);
			sign = !num.sign;
		}
	}
	else
	{
		std::size_t i;
		byte carry = 0;
		for (i = 0; i < digit_num; ++i)
		{
			if (i >= num.digit_num)
			{
				data[i] += carry;
				if (data[i] >= 10)
				{
					data[i] -= 10;
					carry = 1;
					continue;
				}
				else
				{
					carry = 0;
					++i;
					goto additionEnd;
				}
			}
			data[i] += num.data[i] + carry;
			carry = data[i] / 10;
			data[i] %= 10;
		}
		for (; i < data.size(); ++i)
		{
			if (i >= num.digit_num)
			{
				data[i] = carry;
				digit_num += carry;
				carry = 0;
				break;
			}
			data[i] += num.data[i] + carry;
			carry = data[i] / 10;
			data[i] %= 10;
			++digit_num;
		}
		for (; i < num.digit_num; ++i)
		{
			data.push_back(num[i] + carry);
			carry = data[i] / 10;
			data[i] %= 10;
			++digit_num;
		}
	additionEnd:
		if (carry)
		{
			if (i >= data.size())data.push_back(1);
			else data[i] = 1;
			++digit_num;
		}
	}
	return *this;
}
#elif VERSION == 2
LongInt& LongInt::operator-=(const LongInt& num)&
{
	unsigned long long i;
	if (sign != num.sign)
	{
		for (i = 0; i < (std::min)(num.data.size(), data.size()); ++i)data[i] += num.data[i];
		for (; i < num.data.size(); ++i)data.push_back(num.data[i]);
	}
	else
	{
		for (i = 0; i < (std::min)(num.data.size(), data.size()); ++i)data[i] -= num.data[i];
		for (; i < num.data.size(); ++i)data.push_back(-num.data[i]);
	}
	fix_carry();
	return *this;
}
#endif

#if VERSION == 1
LongInt LongInt::operator*(const LongInt& num)const&
{
	if ((data[0] == 0 && digit_num == 1) || (num.data[0] == 0 && num.digit_num == 1))return LongInt();
	LongInt answer(digit_num + num.digit_num, true);
	std::size_t j;
	byte carry = 0;
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
	answer.check_dnum(answer.data.size() - 1);
	return answer;
}

LongInt& LongInt::operator*=(const LongInt& num)&
{
	if ((data[0] == 0 && digit_num == 1) || (num.data[0] == 0 && num.digit_num == 1))
	{
		*this = LongInt();
		return *this;
	}
	std::vector<byte> answer(digit_num + num.digit_num);
	byte carry = 0;
	for (std::size_t i = 0; i < num.digit_num; ++i)
	{
		std::size_t j;
		carry = 0;
		for (j = 0; j < digit_num; ++j)
		{
			answer[i + j] += num.data[i] * data[j] + carry;
			carry = answer[i + j] / 10;
			answer[i + j] %= 10;
		}
		answer[i + j] = carry;
	}
	sign ^= num.sign;
	data = std::move(answer);
	check_dnum(data.size() - 1);
	return *this;
}

LongInt LongInt::operator/(const LongInt& num)const&
{
	if (num.digit_num == 1 && num.data[0] == 0)throw std::out_of_range("èúêîÇ™0");
	if (digit_num < num.digit_num)return LongInt();

	LongInt answer(digit_num - num.digit_num + 1, true);
	std::vector<byte> vtmp(digit_num + 1);
	byte pivot = num.data[num.digit_num - 1];
	signed char brw = 0;
	signed char tmp;

	for (std::size_t i = 0; i < digit_num; ++i)
	{
		vtmp[i] = data[i];
	}

	for (std::size_t i = digit_num - num.digit_num + 1; i > 0;)
	{
		--i;
		std::size_t j;
		answer.data[i] = (vtmp[i + num.digit_num] * 10 + vtmp[i + num.digit_num - 1]) / (pivot + 1);
		brw = 0;
		for (j = i; j < i + num.digit_num; ++j)
		{
			tmp = (char)vtmp[j] - (char)(num.data[j - i] * answer.data[i]) + brw + 100;
			brw = tmp / 10 - 10;
			vtmp[j] = (byte)(tmp % 10);
		}
		vtmp[j] += brw;
		while (true)
		{
			if (vtmp[j] == 0)
			{
				while (j > i)
				{
					--j;
					if (vtmp[j] < num.data[j - i])goto loopend;
					else if (vtmp[j] > num.data[j - i])break;
				}
			}
			brw = 0;
			for (j = i; j < i + num.digit_num; ++j)
			{
				tmp = (char)vtmp[j] - (char)num.data[j - i] + brw;
				if (tmp >= 0)
				{
					vtmp[j] = (byte)tmp;
					brw = 0;
				}
				else
				{
					vtmp[j] = (byte)(tmp + 10);
					brw = -1;
				}
			}
			vtmp[j] += brw;
			++(answer.data[i]);
		}
	loopend:
		;
	}
	answer.digit_num = answer.data.size();
	if (*(answer.data.end() - 1) == 0)--(answer.digit_num);
	answer.sign = sign ^ num.sign;
	return answer;
}

LongInt LongInt::operator%(const LongInt& num)const&
{
	if (num.digit_num == 1 && num.data[0] == 0)throw std::out_of_range("èúêîÇ™0");
	if (digit_num < num.digit_num)return *this;

	byte pivot = num.data[num.digit_num - 1];
	signed char brw = 0;
	signed char tmp;
	LongInt answer = *this;
	byte multi;
	answer.data.push_back(0);

	for (std::size_t i = digit_num - num.digit_num + 1; i > 0;)
	{
		--i;
		std::size_t j;
		multi = (answer.data[i + num.digit_num] * 10 + answer.data[i + num.digit_num - 1]) / (pivot + 1);
		brw = 0;
		for (j = i; j < i + num.digit_num; ++j)
		{
			tmp = (char)answer.data[j] - (char)(num.data[j - i] * multi) + brw + 100;
			brw = tmp / 10 - 10;
			answer.data[j] = (byte)(tmp % 10);
		}
		answer.data[j] += brw;
		while (true)
		{
			if (answer.data[j] == 0)
			{
				while (j > i)
				{
					--j;
					if (answer.data[j] < num.data[j - i])goto loopend;
					else if (answer.data[j] > num.data[j - i])break;
				}
			}
			brw = 0;
			for (j = i; j < i + num.digit_num; ++j)
			{
				tmp = (char)answer.data[j] - (char)num.data[j - i] + brw;
				if (tmp >= 0)
				{
					answer.data[j] = (byte)tmp;
					brw = 0;
				}
				else
				{
					answer.data[j] = (byte)(tmp + 10);
					brw = -1;
				}
			}
			answer.data[j] += brw;
			++multi;
		}
	loopend:
		;
	}
	answer.check_dnum(num.digit_num - 1);
	answer.sign = sign ^ num.sign;
	return answer;
}
#elif VERSION == 2

#if defined(MULTI_REVISED)
LongInt& LongInt::operator*=(const LongInt& num)&
{
	unsigned long long size = 1;
	unsigned long long i, k = 0;
	while (size < data.size() + num.data.size())size <<= 1;
	std::vector<mycomplex> tmp1(size), tmp2(size);
	if ((std::min)(data.size(), num.data.size()) > size >> 2)
	{
		for (i = 0; i < data.size(); ++i)tmp1[i].real = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp2[i].real = num.data[i];
		mycomplex* copy = new mycomplex[size];
		fft(tmp1, 1, copy);
		fft(tmp2, 1, copy);
		for (i = 0; i < size; ++i)tmp1[i].times(tmp2[i]);
		fft(tmp1, -1, copy);
		delete[] copy;
		this->data.resize(data.size() + num.data.size());
		for (i = 0; i < data.size(); ++i)data[i] = (int)(tmp1[i].real / size + 0.5);
	}
	else if (num.data.size() < data.size())
	{
		std::vector<int> ntmp(data.size() + num.data.size());
		while (num.data.size() << 2 > size)size >>= 1;
		for (i = 0; i < num.data.size(); ++i)tmp2[i].real = num.data[i];
		mycomplex* copy = new mycomplex[size];
		fft(tmp2, 1, copy);
		while (k < data.size())
		{
			for (i = 0; i + k < data.size() && i < size >> 1; ++i)
			{
				tmp1[i].real = data[i + k];
				tmp1[i].imag = 0;
			}
			for (; i < size; ++i)
			{
				tmp1[i].real = 0;
				tmp1[i].imag = 0;
			}
			fft(tmp1, 1, copy);
			for (i = 0; i < size; ++i)tmp1[i].times(tmp2[i]);
			fft(tmp1, -1, copy);
			for (i = 0; i + k < ntmp.size() && i < size; ++i)ntmp[i + k] += (int)(tmp1[i].real / size + 0.5);
			k += size >> 1;
		}
		data = std::move(ntmp);
	}
	else
	{
		while (data.size() << 2 > size)size >>= 1;
		for (i = 0; i < data.size(); ++i)tmp2[i].real = data[i];
		for (auto& tmp : data)tmp = 0;
		data.resize(data.size() + num.data.size());
		mycomplex* copy = new mycomplex[size];
		fft(tmp2, 1, copy);
		while (k < num.data.size())
		{
			for (i = 0; i + k < num.data.size() && i < size >> 1; ++i)
			{
				tmp1[i].real = num.data[i + k];
				tmp1[i].imag = 0;
			}
			for (; i < size; ++i)
			{
				tmp1[i].real = 0;
				tmp1[i].imag = 0;
			}
			fft(tmp1, 1, copy);
			for (i = 0; i < size; ++i)tmp1[i].times(tmp2[i]);
			fft(tmp1, -1, copy);
			for (i = 0; i + k < data.size() && i < size; ++i)data[i + k] += (int)(tmp1[i].real / size + 0.5);
			k += size >> 1;
		}
	}
	sign ^= num.sign;
	fix_carry();
	return *this;
}
#else
#if defined(USE_NTT)
#define NTT_PRIME1 2013265921
#define NTT_ROOT1 137
#define NTT_PRIME2 1811939329
#define NTT_ROOT2 136

void LongInt::karatsuba(std::vector<int>& num1, std::vector<int>& num2, std::vector<int>& result)
{
	if (num1.size() == 1)
	{
		int tmp = num1[0] * num2[0];
		result.push_back(tmp % 10);
		result.push_back(tmp / 10);
		return;
	}
	std::vector<int> a, b, c, d;
	std::vector<int> p, q, r;
	std::size_t size = num1.size() >> 1, i;
	for (i = 0; i < size; ++i)
	{
		b.push_back(num1[i]);
	}
	for (; i < num1.size(); ++i)
	{
		a.push_back(num1[i]);
	}
	if (num2.size() <= size)
	{
		d.resize(size);
		for (i = 0; i < num2.size(); ++i)
		{
			d[i] = num2[i];
		}
		LongInt::karatsuba(a, d, q);
		LongInt::karatsuba(b, d, r);
		result = r;
		result.resize(num1.size() + num2.size());
		for (i = 0; i < q.size(); ++i)
		{
			result[i + size] += q[i];
		}
		while (result.size() >= 1 && !result.back())
		{
			result.pop_back();
		}
	}
	else
	{
		for (i = 0; i < size; ++i)
		{
			d.push_back(num2[i]);
		}
		for (; i < num2.size(); ++i)
		{
			c.push_back(num2[i]);
		}
		LongInt::karatsuba(a, c, p);
		LongInt::karatsuba(b, d, r);
		for (i = 0; i < size; ++i)
		{
			a[i] += b[i];
			c[i] += d[i];
		}
		LongInt::karatsuba(a, c, q);
		for (i = 0; i < q.size(); ++i)
		{
			q[i] -= p[i] + r[i];
		}
		result = r;
		result.resize(num1.size() * 2);
		for (i = 0; i < q.size(); ++i)
		{
			result[i + size] += q[i];
		}
		size *= 2;
		for (i = 0; i < p.size(); ++i)
		{
			result[i + size] += p[i];
		}
	}
}

LongInt LongInt::operator*(const LongInt& num)const&
{
	LongInt tmp(data.size() + num.data.size(), true);
	unsigned long long size = 1;
	int cnt = 0;
	unsigned long long i, k = 0;
	while (size < data.size() + num.data.size())
	{
		size <<= 1;
		cnt += 1;
	}
	if ((unsigned long long)(cnt * 20) > num.data.size())
	{
		num.multiply_nr(*this, tmp);

	}
	else if ((unsigned long long)(cnt * 20) > data.size())
	{
		multiply_nr(num, tmp);
	}
	else
	{
		static Number_Theoretic_Transform<NTT_PRIME1> ntt1(NTT_ROOT1);
		static Number_Theoretic_Transform<NTT_PRIME2> ntt2(NTT_ROOT2);
		std::vector<unsigned long long> tmp11(size), tmp21(size);
		for (i = 0; i < data.size(); ++i)tmp11[i] = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp21[i] = num.data[i];
		std::vector<unsigned long long> tmp12, tmp22;
		if (cnt > 12)
		{
			tmp12 = tmp11;
			tmp22 = tmp21;
			ntt2.trans_f(tmp12, cnt - 1, 0, size - 1);
			ntt2.trans_f(tmp22, cnt - 1, 0, size - 1);
			for (i = 0; i < size; ++i)
			{
				tmp12[i] *= tmp22[i];
				tmp12[i] %= NTT_PRIME2;
			}
			ntt2.itrans_t(tmp12, cnt - 1, 0, size - 1);
		}

		ntt1.trans_f(tmp11, cnt - 1, 0, size - 1);
		ntt1.trans_f(tmp21, cnt - 1, 0, size - 1);
		for (i = 0; i < size; ++i)
		{
			tmp11[i] *= tmp21[i];
			tmp11[i] %= NTT_PRIME1;
		}
		ntt1.itrans_t(tmp11, cnt - 1, 0, size - 1);

		if (cnt > 12)
			for (i = 0; i < tmp.data.size(); ++i)tmp.data[i] = (int)(garner((long long)tmp11[i], (long long)tmp12[i], NTT_PRIME1, NTT_PRIME2) / size);
		else
			for (i = 0; i < tmp.data.size(); ++i)tmp.data[i] = (int)(tmp11[i] / size);
	}
	tmp.sign = sign ^ num.sign;
	tmp.fix_carry();
	return tmp;
}

LongInt& LongInt::operator*=(const LongInt& num)&
{
	unsigned long long size = 1;
	int cnt = 0;
	unsigned long long i, k = 0;
	while (size < data.size() + num.data.size())
	{
		size <<= 1;
		cnt += 1;
	}
	if ((unsigned long long)(cnt * 20) > num.data.size())
	{
		LongInt tmp(data.size() + num.data.size(), true);
		num.multiply_nr(*this, tmp);
		tmp.sign = sign ^ num.sign;
		tmp.fix_carry();
		*this = std::move(tmp);
	}
	else if ((unsigned long long)(cnt * 20) > data.size())
	{
		LongInt tmp(data.size() + num.data.size(), true);
		multiply_nr(num, tmp);
		tmp.sign = sign ^ num.sign;
		tmp.fix_carry();
		*this = std::move(tmp);
	}
	else
	{
		static Number_Theoretic_Transform<NTT_PRIME1> ntt1(NTT_ROOT1);
		static Number_Theoretic_Transform<NTT_PRIME2> ntt2(NTT_ROOT2);
		std::vector<unsigned long long> tmp11(size), tmp21(size);
		for (i = 0; i < data.size(); ++i)tmp11[i] = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp21[i] = num.data[i];
		std::vector<unsigned long long> tmp12, tmp22;
		if (cnt > 12)
		{
			tmp12 = tmp11;
			tmp22 = tmp21;
			ntt2.trans_f(tmp12, cnt - 1, 0, size - 1);
			ntt2.trans_f(tmp22, cnt - 1, 0, size - 1);
			for (i = 0; i < size; ++i)
			{
				tmp12[i] *= tmp22[i];
				tmp12[i] %= NTT_PRIME2;
			}
			ntt2.itrans_t(tmp12, cnt - 1, 0, size - 1);
		}
		ntt1.trans_f(tmp11, cnt - 1, 0, size - 1);
		ntt1.trans_f(tmp21, cnt - 1, 0, size - 1);
		for (i = 0; i < size; ++i)
		{
			tmp11[i] *= tmp21[i];
			tmp11[i] %= NTT_PRIME1;
		}
		ntt1.itrans_t(tmp11, cnt - 1, 0, size - 1);
		this->data.resize(data.size() + num.data.size());
		if (cnt > 12)
			for (i = 0; i < data.size(); ++i)data[i] = (int)(garner((long long)tmp11[i], (long long)tmp12[i], NTT_PRIME1, NTT_PRIME2) / size);
		else
			for (i = 0; i < data.size(); ++i)data[i] = (int)(tmp11[i] / size);
		sign ^= num.sign;
		fix_carry();
	}
	return *this;
}
#else
LongInt LongInt::operator*(const LongInt& num)const&
{
	unsigned long long size = 1, cnt = 0, i, k = 0;
	LongInt tmp(data.size() + num.data.size(), true);
	while (size < data.size() + num.data.size())
	{
		size <<= 1;
		cnt += 20;
	}
	if (cnt > num.data.size())
	{
		num.multiply_nr(*this, tmp);
	}
	else if (cnt > data.size())
	{
		multiply_nr(num, tmp);
	}
	else
	{
		std::vector<mycomplex> tmp1(size), tmp2(size);
		for (i = 0; i < data.size(); ++i)tmp1[i].real = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp2[i].real = num.data[i];
		mycomplex* copy = new mycomplex[size];
		fft(tmp1, 1, copy);
		fft(tmp2, 1, copy);
		for (i = 0; i < size; ++i)tmp1[i].times(tmp2[i]);
		fft(tmp1, -1, copy);
		delete[] copy;
		for (i = 0; i < data.size(); ++i)tmp.data[i] = (int)(tmp1[i].real / size + 0.5);
	}
	tmp.sign = sign ^ num.sign;
	tmp.fix_carry();
	return tmp;
}

LongInt& LongInt::operator*=(const LongInt& num)&
{
	unsigned long long size = 1, cnt = 0;
	unsigned long long i, k = 0;
	while (size < data.size() + num.data.size())
	{
		size <<= 1;
		cnt += 20;
	}
	if (cnt > num.data.size())
	{
		LongInt tmp(data.size() + num.data.size(), true);
		num.multiply_nr(*this, tmp);
		tmp.sign = sign ^ num.sign;
		tmp.fix_carry();
		*this = std::move(tmp);
	}
	else if (cnt > data.size())
	{
		LongInt tmp(data.size() + num.data.size(), true);
		multiply_nr(num, tmp);
		tmp.sign = sign ^ num.sign;
		tmp.fix_carry();
		*this = std::move(tmp);
	}
	else
	{
		std::vector<mycomplex> tmp1(size), tmp2(size);
		for (i = 0; i < data.size(); ++i)tmp1[i].real = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp2[i].real = num.data[i];
		mycomplex* copy = new mycomplex[size];
		fft(tmp1, 1, copy);
		fft(tmp2, 1, copy);
		for (i = 0; i < size; ++i)tmp1[i].times(tmp2[i]);
		fft(tmp1, -1, copy);
		delete[] copy;
		this->data.resize(data.size() + num.data.size());
		for (i = 0; i < data.size(); ++i)data[i] = (int)(tmp1[i].real / size + 0.5);
		sign ^= num.sign;
		fix_carry();
	}
	return *this;
}
#endif
LongInt LongInt::operator/(const LongInt& num)const&
{
	//éü
	return LongInt();
}
#endif
#endif

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
	numin = tmp.c_str();
	return input;
}