#pragma once

#include<vector>
#include<iostream>
#include<string>
#include<random>
#include<complex>
#include<cmath>
#include<sstream>
#include<iomanip>
#include"number_theoretic_transform.h"

#define VERSION 2
#define PI 3.141592653589793238462643383279502884197169399375
#define USE_NTT

//#define MULTI_REVISED

namespace MyFunctions1
{
	template<class T>
	void swap(T& a, T& b)
	{
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}

	//多倍長整数
#define LINT_BASE 100000
#define LINT_BASE_DNUM 5
	class LongInt
	{

	private:

		template<class T>
		void swap(T& a, T& b)
		{
			T tmp = std::move(a);
			a = std::move(b);
			b = std::move(tmp);
		}

		std::vector<int> data;
		bool sign = 0;//負のとき1

		void del_zero() & noexcept
		{
			if (data.empty())
			{
				data.push_back(0);
				sign = 0;
				return;
			}
			while (data.size() > 1 && !data.back())
			{
				data.pop_back();
			}
			if (data.size() == 1 && data[0] == 0)
			{
				sign = 0;
			}
		}

		void fix_carry()&
		{
			int carry = 0;
			for (std::size_t i = 0; i < data.size(); ++i)
			{
				data[i] += carry;
				carry = 0;
				if (data[i] > 0)
				{
					carry = data[i] / LINT_BASE;
					data[i] %= LINT_BASE;
				}
				else if (data[i] < 0)
				{
					carry = (data[i] + 1) / LINT_BASE - 1;
					data[i] -= carry * LINT_BASE;
				}
			}

			while (carry > 0)
			{
				data.push_back(carry % LINT_BASE);
				carry /= LINT_BASE;
			}
			while (carry <= -LINT_BASE)
			{
				data.push_back((carry + 1) % LINT_BASE + LINT_BASE - 1);
				carry = (carry + 1) / LINT_BASE - 1;
			}
			if (carry < 0)data.push_back(carry);
			if (data.back() < 0)
			{
				sign = !sign;
				carry = 1;
				for (std::size_t i = 0; i < data.size(); ++i)
				{
					data[i] = LINT_BASE - 1 - data[i] + carry;
					carry = data[i] / LINT_BASE;
					data[i] %= LINT_BASE;
				}
				data.back() %= LINT_BASE;
			}
			del_zero();
		}

	public:

		LongInt()noexcept;//デフォルトコンストラクタ
		LongInt(int);//コンストラクタint
		LongInt(std::size_t, bool);

		LongInt(const char*);//コンストラクタchar*
		LongInt(const LongInt&) = default;//コピーコンストラクタ
		LongInt(LongInt&&) = default;//ムーブコンストラクタ

		virtual ~LongInt()noexcept = default;

		LongInt& operator=(const LongInt&)& = default;
		LongInt& operator=(LongInt&&)& = default;

		bool operator<(const LongInt&)const& noexcept;
		bool operator>(const LongInt&)const& noexcept;
		bool operator<=(const LongInt&)const& noexcept;
		bool operator>=(const LongInt&)const& noexcept;
		bool operator==(const LongInt&)const& noexcept;
		bool operator!=(const LongInt&)const& noexcept;

		LongInt& operator++()&;
		LongInt operator++(int)&;
		LongInt& operator--()&;
		LongInt operator--(int)&;

		LongInt operator+()const&;
		LongInt operator-()const&;

		LongInt operator+(LongInt)const&;
		LongInt operator-(LongInt)const&;

		void multiply_nr(const std::vector<int>& num, std::vector<unsigned long long>& result)const&
		{
			for (unsigned long long i = 0; i < data.size(); ++i)
			{
				for (unsigned long long j = 0; j < num.size(); ++j)
				{
					result[i + j] += (unsigned long long)data[i] * (unsigned long long)num[j];
				}
			}
		}

		/*
		static LongInt mul(const LongInt& num1, const LongInt& num2)
		{
			LongInt result(num1.data.size() + num2.data.size(), true);
			num1.multiply_nr(num2, result);
			result.sign = num1.sign ^ num2.sign;
			result.fix_carry();
			return result;
		}
		*/
		LongInt operator*(const LongInt&)const&;//乗算
		LongInt operator/(const LongInt&)const&;//除算
		LongInt operator%(const LongInt&)const&;//モジュル演算

		LongInt& operator+=(const LongInt&)&;
		LongInt& operator-=(const LongInt&)&;
		LongInt& operator*=(const LongInt&)&;

		int operator[](std::size_t)const& noexcept;

		void random(std::size_t size)&
		{
			std::size_t i;
			static std::random_device seed;
			static std::mt19937_64 mt(seed());
			static std::uniform_int_distribution<> rand(0, LINT_BASE - 1);
			data.resize(size);
			for (i = 0; i < size; ++i)
			{
				data[i] = rand(mt);
			}
			sign = rand(mt) <= LINT_BASE / 2 - 1;
			del_zero();
		}

		static LongInt zero()
		{
			return LongInt();
		}

		void absolute()&
		{
			sign = 0;
		}

		void tostring(std::string&)const&;
	};
}

std::ostream& operator<<(std::ostream&, const MyFunctions1::LongInt&);//標準出力
std::istream& operator>>(std::istream&, MyFunctions1::LongInt&);//標準入力