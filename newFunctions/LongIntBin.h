#pragma once

#include<vector>

class LongIntBin
{
private:
	std::vector<long long> data;
	
public:

	LongIntBin(): data(1, 0)
	{}
	LongIntBin(long long num): data(1, num)
	{}
	LongIntBin(unsigned long long num) : data()
	{
		data.push_back(0);
		data.push_back(num);
	}

	LongIntBin(const char* snum) : data()
	{
		std::vector<char> tmp;
		std::size_t i, j = 0;
		unsigned char sh = 0;
		std::size_t begin = 0;
		char r = 0;
		for (; *snum != '\0'; ++snum)
		{
			tmp.push_back(*snum - '0');
		}
		data.push_back(0);
		while (1)
		{
			data[j] |= (long long)(tmp[tmp.size() - 1] % 2) << sh;
			for (i = begin; i < tmp.size(); ++i)
			{
				tmp[i] += r * 10;
				r = tmp[i] % 2;
				tmp[i] /= 2;
			}
			--begin;
			while (tmp[++begin] == 0);
			if (begin >= tmp.size())break;
			j += sh / 64;
			sh /= 64;
		}
	}

	LongIntBin(const LongIntBin&) = default;
	LongIntBin(LongIntBin&&) = default;
};