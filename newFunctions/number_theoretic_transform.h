#pragma once
#include<vector>
#include"modulo.h"
template<unsigned int Div = 998244353, unsigned int Primitive_root = 3>
class Number_Theoretic_Transform
{
private:
	using ull = unsigned long long;

	std::vector<unsigned int> Root;//2^(m + 1)æª = Root[m]
	std::vector<unsigned int> inv_Root;//“¯—l
	unsigned int Times = 1;
public:
	Number_Theoretic_Transform(): Root(), inv_Root()
	{
		unsigned int root_n = Div - 1, inv_root_n;
		while (!(root_n % 2))
		{
			root_n >>= 1;
			Times <<= 1;
			Root.push_back(0);
			inv_Root.push_back(0);
		}
		root_n = mpow(Primitive_root, root_n, Div);
		inv_root_n = mpow(root_n, Times - 1, Div);
		for (unsigned int i = 0; i < Root.size(); ++i)
		{
			Root[i] = mpow(root_n, Times >> (i + 1), Div);
			inv_Root[i] = mpow(inv_root_n, Times >> (i + 1), Div);
		}
	}

	Number_Theoretic_Transform(unsigned int root_n) : Root(), inv_Root()
	{
		unsigned int inv_root_n = Div - 1;
		while (!(inv_root_n % 2))
		{
			inv_root_n >>= 1;
			Times <<= 1;
			Root.push_back(0);
			inv_Root.push_back(0);
		}
		inv_root_n = (unsigned int)minv(root_n, Div);
		for (unsigned int i = 0; i < Root.size(); ++i)
		{
			Root[i] = mpow(root_n, Times >> (i + 1), Div);
			inv_Root[i] = mpow(inv_root_n, Times >> (i + 1), Div);
		}
	}
	//vec‚Ì—v‘f”‚ª2^n‚Ì‚Æ‚«m = n - 1
	void operator()(std::vector<ull>& vec, int m, bool inv, ull* copy, ull begin = 0, ull e = 1)
	{
		static ull i, j, k;
		if (m < 0)return;
		operator()(vec, m - 1, inv, copy, begin, e << 1);
		operator()(vec, m - 1, inv, copy, begin + e, e << 1);
		ull l = 1;
		for (i = begin; i < vec.size(); i += e)
		{
			copy[i] = vec[i];
		}
		j = begin;
		for (i = begin; j < vec.size(); i += e)
		{
			k = j + e;
			vec[i] = (copy[j] + (l * copy[k])) % Div;
			if (inv)l *= inv_Root[m];
			else l *= Root[m];
			l %= Div;
			j += e << 1;
		}
		for (j = begin; i < vec.size(); i += e)
		{
			k = j + e;
			vec[i] = (copy[j] + (l * copy[k])) % Div;
			if (inv)l *= inv_Root[m];
			else l *= Root[m];
			l %= Div;
			j += e << 1;
		}
	}
};

using NTT = Number_Theoretic_Transform<>;