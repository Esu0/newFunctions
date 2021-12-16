#include"modulo.h"

unsigned int mpow(unsigned long long base, unsigned int exp, unsigned int div)
{
	unsigned long long tmp;
	if (!exp)return 1;
	if (exp % 2)
	{
		tmp = mpow(base, exp / 2, div);
		return (((tmp * tmp) % div) * base) % div;
	}
	else
	{
		tmp = mpow(base, exp / 2, div);
		return (tmp * tmp) % div;
	}
}

//a‚Æb‚ÍŒÝ‚¢‚É‘f
void gcd_extend(long long a, long long b, long long& x, long long& y)
{
	if (b == 1)
	{
		x = 1;
		y = 1 - a;
		return;
	}
	long long x2, y2;
	gcd_extend(b, a % b, x2, y2);
	x = y2;
	y = x2 - (a / b) * y2;
}

long long garner(std::vector<long long>& r, std::vector<long long>& m)
{
	long long m_prod = 1;
	long long x = r[0] % m[0];
	for (unsigned int i = 1; i < (unsigned int)r.size(); ++i)
	{
		m_prod *= m[(std::size_t)i - 1];
		
	}
	return 0;
}