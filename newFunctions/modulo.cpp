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

//a‚Æmod‚ÍŒÝ‚¢‚É‘f
long long minv(long long a, long long mod)
{
	long long b = mod, u = 1, v = 0;
	while (b)
	{
		long long tmp;

		tmp = u;
		u = v;
		v = tmp - (a / b) * v;

		tmp = a;
		a = b;
		b = tmp % b;
	}
	u %= mod;
	if (u < 0)u += mod;
	return u;
}

//m[i]‚Í‚Ç‚Ì2‚Â‚ðŽæ‚Á‚Ä‚àŒÝ‚¢‚É‘f
long long garner(const std::vector<long long>& r, const std::vector<long long>& m)
{
	long long m_prod = 1;
	long long x = r[0] % m[0];
	for (unsigned int i = 1; i < (unsigned int)r.size(); ++i)
	{
		m_prod *= m[(std::size_t)i - 1];
		long long t = ((r[i] - x) * minv(m_prod, m[i])) % m[i];
		if (t < 0)t += m[i];
		x += t * m_prod;
	}
	return x;
}

long long garner(long long r1, long long r2, long long m1, long long m2)
{
	r1 %= m1;
	r2 = ((r2 - r1) * minv(m1, m2)) % m2;
	if (r2 < 0)r2 += m2;
	r1 += r2 * m1;
	return r1;
}