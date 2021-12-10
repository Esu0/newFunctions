#pragma once
#define NUSE_LONGINTBIN

#ifndef NUSE_LONGINTBIN
#include<vector>
#include<iostream>
#include<iomanip>
#include<string>

class LongIntBin
{
private:
	std::vector<unsigned long long> data;
	
public:

	LongIntBin(): data(1, 0)
	{}
	LongIntBin(long long num): data(1, (unsigned long long)num)
	{}
	LongIntBin(std::size_t size, bool): data(size)
	{}
	LongIntBin(const char* snum) : data()
	{
		std::vector<char> tmp;
		std::size_t i, j = 0;
		unsigned char sh = 0;
		std::size_t begin = 0;
		char r = 0;
		bool sign = false;

		if (*snum == '-')
		{
			++snum;
			sign = true;
		}
		else if (*snum == '+')
		{
			++snum;
		}

		for (; *snum != '\0'; ++snum)
		{
			tmp.push_back(*snum - '0');
		}
		data.push_back(0);
		while (1)
		{
			data[j] |= (unsigned long long)(tmp[tmp.size() - 1] % 2) << sh;
			r = 0;
			for (i = begin; i < tmp.size(); ++i)
			{
				tmp[i] += r * 10;
				r = tmp[i] % 2;
				tmp[i] /= 2;
			}

			while (tmp[begin] == 0)
			{
				++begin;
				if (begin >= tmp.size())break;
			}
			if (begin >= tmp.size())break;
			if (sh == 63)
			{
				++j;
				sh = 0;
				data.push_back(0);
			}
			else
			{
				++sh;
			}
		}
		if (data[data.size() - 1] & 0x8000000000000000)data.push_back(0);
		if (sign)
		{
			for (auto& tmp : data)
			{
				tmp = ~tmp;
			}
			i = 0;
			while (data[i] == ULLONG_MAX)
			{
				data[i] = 0;
				++i;
				if (i >= data.size())return;
			}
			++(data[i]);
		}
	}

	LongIntBin(const LongIntBin&) = default;
	LongIntBin(LongIntBin&&) = default;
	~LongIntBin()noexcept = default;

	LongIntBin& operator=(const LongIntBin&)& = default;
	LongIntBin& operator=(LongIntBin&&)& = default;

	LongIntBin operator~()const&;
	LongIntBin operator-()const&;

	void outhex()
	{
		for (std::vector<unsigned long long>::iterator itr = data.end(); itr != data.begin();)
		{
			--itr;
			std::cout << std::hex << std::setw(16) << std::setfill('0') << *itr;
		}
		std::cout << std::endl;
	}

	void tostring(std::string& str)
	{
		LongIntBin tmp;
		if (data[data.size() - 1] & 0x8000000000000000)tmp = -*this;
		else tmp = *this;
		
	}
	friend std::ostream& operator<<(std::ostream&, const LongIntBin&);
	friend std::istream& operator>>(std::istream&, LongIntBin&);
};
#endif