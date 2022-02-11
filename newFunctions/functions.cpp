#include"functions.h"

using namespace MyFunctions1;
LongInt::LongInt()noexcept :data(1), sign{ 0 }
{}

LongInt::LongInt(int num) : data(1, num), sign{ 0 }
{
	fix_carry();
}

LongInt::LongInt(const char* snum) : data{}, sign{ 0 }
{
	int tmp = 0, m = 1;
	std::size_t i = 0;
	std::size_t size = strlen(snum);

	if (*snum == '-')
	{
		++i;
		sign = 1;
	}
	if (*snum == '+')
	{
		++i;
	}
	for (std::size_t j = size; j > i;)
	{
		--j;
		tmp += m * (snum[j] - '0');
		m *= 10;
		if ((size - j) % LINT_BASE_DNUM == 0)
		{
			data.push_back(tmp);
			tmp = 0;
			m = 1;
		}
	}
	if (tmp != 0)data.push_back(tmp);
	del_zero();
}

LongInt::LongInt(std::size_t size, bool) :data(size)
{}

int LongInt::operator[](std::size_t index)const& noexcept
{
	return index < data.size() ? data[index] : 0;
}

void LongInt::tostring(std::string& str)const&
{
	if (sign)str.push_back('-');
	std::ostringstream oss;
	oss << data.back();
	for (unsigned long long i = data.size() - 1; i > 0;)
	{
		--i;
		oss << std::setfill('0') << std::setw(5) << data[i];
	}
	str += oss.str();
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

bool LongInt::operator==(const LongInt& num2)const& noexcept
{
	if (sign == num2.sign && data.size() == num2.data.size())
	{
		for (std::size_t i = 0; i < data.size(); ++i)
		{
			if (data[i] != num2.data[i])return false;
		}
		return true;
	}
	else return false;
}

bool LongInt::operator!=(const LongInt& num2)const& noexcept
{
	return !operator==(num2);
}
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
				data[i] = LINT_BASE - 1;
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
			if (data[i] < LINT_BASE)return *this;
			else data[i] %= LINT_BASE;
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
			if (data[i] < LINT_BASE)return *this;
			else data[i] %= LINT_BASE;
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
				data[i] = LINT_BASE - 1;
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



#define NTT_PRIME1 2013265921
#define NTT_ROOT1 137
#define NTT_PRIME2 1811939329
#define NTT_ROOT2 136

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
	std::vector<unsigned long long> tmp11(size);
	if ((unsigned long long)(cnt * 20) > num.data.size())
	{
		num.multiply_nr(this->data, tmp11);
	}
	else if ((unsigned long long)(cnt * 20) > data.size())
	{
		multiply_nr(num.data, tmp11);
	}
	else
	{
		static Number_Theoretic_Transform<NTT_PRIME1, NTT_ROOT1> ntt1;
		static Number_Theoretic_Transform<NTT_PRIME2, NTT_ROOT2> ntt2;
		std::vector<unsigned long long> tmp21(size);
		unsigned long long size_inv1 = (unsigned long long)minv((long long)size, NTT_PRIME1);
		unsigned long long size_inv2 = (unsigned long long)minv((long long)size, NTT_PRIME2);
		for (i = 0; i < data.size(); ++i)tmp11[i] = data[i];
		for (i = 0; i < num.data.size(); ++i)tmp21[i] = num.data[i];
		std::vector<unsigned long long> tmp12, tmp22;

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

		ntt1.trans_f(tmp11, cnt - 1, 0, size - 1);
		ntt1.trans_f(tmp21, cnt - 1, 0, size - 1);
		for (i = 0; i < size; ++i)
		{
			tmp11[i] *= tmp21[i];
			tmp11[i] %= NTT_PRIME1;
		}
		ntt1.itrans_t(tmp11, cnt - 1, 0, size - 1);

		for (i = 0; i < tmp.data.size(); ++i)tmp11[i] = (unsigned long long)(garner((long long)((tmp11[i] * size_inv1) % NTT_PRIME1), (long long)((tmp12[i] * size_inv2) % NTT_PRIME2), NTT_PRIME1, NTT_PRIME2));
	}
	unsigned long long carry = 0;
	for (i = 0; i < tmp.data.size(); ++i)
	{
		tmp11[i] += carry;
		carry = tmp11[i] / LINT_BASE;
		tmp.data[i] = (int)(tmp11[i] % LINT_BASE);
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
	std::vector<unsigned long long> tmp11(size);
	if ((unsigned long long)(cnt * 20) > num.data.size())
	{
		num.multiply_nr(this->data, tmp11);
	}
	else if ((unsigned long long)(cnt * 20) > data.size())
	{
		multiply_nr(num.data, tmp11);
	}
	else
	{
		static Number_Theoretic_Transform<NTT_PRIME1, NTT_ROOT1> ntt1;
		static Number_Theoretic_Transform<NTT_PRIME2, NTT_ROOT2> ntt2;
		unsigned long long size_inv1 = (unsigned long long)minv((long long)size, NTT_PRIME1);
		unsigned long long size_inv2 = (unsigned long long)minv((long long)size, NTT_PRIME2);
		std::vector<unsigned long long> tmp21(size);
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
		for (i = 0; i < data.size(); ++i)tmp11[i] = (unsigned long long)(garner((long long)((tmp11[i] * size_inv1) % NTT_PRIME1), (long long)((tmp12[i] * size_inv2) % NTT_PRIME2), NTT_PRIME1, NTT_PRIME2));
	}
	unsigned long long carry = 0;
	this->data.resize(data.size() + num.data.size());
	for (i = 0; i < data.size(); ++i)
	{
		tmp11[i] += carry;
		carry = tmp11[i] / LINT_BASE;
		data[i] = (int)(tmp11[i] % LINT_BASE);
	}
	sign ^= num.sign;
	fix_carry();
	return *this;
}

LongInt LongInt::operator/(const LongInt& num)const&
{
	//ŽŸ
	return LongInt();
}

LongInt LongInt::operator%(const LongInt& num)const&
{
	return LongInt();
}

std::ostream& operator<<(std::ostream& output, const LongInt& numout)
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