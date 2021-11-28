#include"functions.h"

using namespace MyFunctions1;

LongInt::LongInt()noexcept:data(1), sign{0}
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

	//èáèòÇãtÇ…Ç∑ÇÈ
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
		digit_num = 1;
	}
	else
	{
		digit_num = data.size() - 1;
		while (data[digit_num] == 0 && digit_num != 0 && --digit_num != 0);
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


void LongInt::tostring(std::string& str)const&
{
	if (sign)str.push_back('-');
	for (long long i = digit_num - 1; i >= 0; --i)
	{
		str.push_back((char)(data[i] + '0'));
	}
}


bool LongInt::operator<(const LongInt& num2)const& noexcept
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

bool LongInt::operator>(const LongInt& num2)const& noexcept
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

bool LongInt::operator>=(const LongInt& num2)const& noexcept
{
	return !operator<(num2);
}

bool LongInt::operator<=(const LongInt& num2)const& noexcept
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


unsigned char LongInt::operator[](std::size_t index)const& noexcept
{
	return index < data.size() ? data[index] : 0;
}


LongInt& LongInt::operator++()&
{
	if (sign)
	{
		if (digit_num == 1 && data[0] == 1)
		{
			data[0] = 0;
			sign = 0;
			return *this;
		}
		for (std::size_t i = 0; i < digit_num; ++i)
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
		for (std::size_t i = 0; i < digit_num; ++i)
		{
			++(data[i]);
			if (data[i] < 10)return *this;
			else data[i] %= 10;
		}
		if (digit_num != data.size())data[digit_num++] = 1;
		else
		{
			data.push_back(1);
			++digit_num;
		}
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
		for (std::size_t i = 0; i < digit_num; ++i)
		{
			++(data[i]);
			if (data[i] < 10)return *this;
			else data[i] %= 10;
		}
		if (digit_num != data.size())data[digit_num++] = 1;
		else
		{
			data.push_back(1);
			++digit_num;
		}
	}
	else
	{
		if (digit_num == 1 && data[0] == 0)
		{
			data[0] = 1;
			sign = 1;
			return *this;
		}
		for (std::size_t i = 0; i < digit_num; ++i)
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
	if (!(tmp.digit_num == 1 && tmp.data[0] == 0))tmp.sign = !tmp.sign;
	return tmp;
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