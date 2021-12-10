#include"LongIntBin.h"

#ifndef NUSE_LONGINTBIN
LongIntBin LongIntBin::operator~()const&
{
	LongIntBin result(data.size(), true);
	for (std::size_t i = 0; i < data.size(); ++i)
	{
		result.data[i] = ~data[i];
	}
	return result;
}

LongIntBin LongIntBin::operator-()const&
{
	LongIntBin result = ~(*this);
	std::size_t i = 0;
	while (result.data[i] == ULLONG_MAX)
	{
		result.data[i] = 0;
		++i;
		if (i >= result.data.size())return result;
	}
	++(result.data[i]);
	return result;
}

std::ostream& operator<<(std::ostream& output, const LongIntBin& numout)
{
	LongIntBin tmp = (numout.data[numout.data.size()] & 0x8000000000000000) ? -numout : numout;

}

std::istream& operator>>(std::istream& input, LongIntBin& numin)
{
	std::string tmp;
	input >> tmp;
	numin = tmp.c_str();
}
#endif