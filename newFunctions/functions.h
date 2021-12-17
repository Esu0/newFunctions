#pragma once

#include<vector>
#include<iostream>
#include<string>
#include<random>
#include<complex>
#include<cmath>
#include"time_debug.h"

#define VERSION 2
#define PI 3.141592653589793238462643383279502884197169399375
#define USE_NTT

//#define MULTI_REVISED

namespace MyFunctions1
{
	//多倍長整数
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//負のとき1
#if VERSION == 1
		std::size_t digit_num = 1;//桁数
#endif
#if VERSION == 1
		std::vector<byte> data;//表記と逆順に並んでいる
#elif VERSION == 2
		std::vector<int> data;
#endif
		void addabs(const LongInt&, LongInt&)const&;
		void subtractabs(const LongInt&, LongInt&)const&;//*this-num2>=0の前提
		bool cmpabs(const LongInt&)const&;

#if VERSION == 2
	public:
		class mycomplex
		{
		public:
			double real = 0.0;
			double imag = 0.0;

			mycomplex()noexcept
			{}

			mycomplex(double r, double i)noexcept :real(r), imag(i)
			{}

			mycomplex(double theta):real(std::cos(theta)), imag(std::sin(theta))
			{}
			
			mycomplex(const mycomplex&) = default;
			mycomplex(mycomplex&&) = default;
			virtual ~mycomplex()noexcept = default;

			mycomplex& operator=(const mycomplex& z)& noexcept
			{
				real = z.real;
				imag = z.imag;
				return *this;
			}
			mycomplex& operator=(mycomplex&&)& = default;

			void times(const mycomplex& z)&
			{
				double tmp = real;
				real *= z.real;
				real -= imag * z.imag;
				imag *= z.real;
				imag += z.imag * tmp;
			}
			
			mycomplex multiply(const mycomplex& z)const&
			{
				return mycomplex(real * z.real - imag * z.imag, real * z.imag + z.real * imag);
			}

			void add(const mycomplex& z1, const mycomplex& z2)&
			{
				real = z1.real + z2.real;
				imag = z1.imag + z2.imag;
			}
			void add(const mycomplex& z)&
			{
				real += z.real;
				imag += z.imag;
			}
		};
#endif
	private:
#if VERSION == 1
		void check_dnum(std::size_t init)
		{
			for (std::size_t i = init; i > 0; --i)
			{
				if (data[i] != 0)
				{
					digit_num = i + 1ull;
					return;
				}
			}
			digit_num = 1ull;
		}
#endif

#if VERSION == 2
		void del_zero()& noexcept
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
					carry = data[i] / 10;
					data[i] %= 10;
				}
				else if (data[i] < 0)
				{
					carry = (data[i] + 1) / 10 - 1;
					data[i] -= carry * 10;
				}
			}

			while (carry > 0)
			{
				data.push_back(carry % 10);
				carry /= 10;
			}
			while (carry < -9)
			{
				data.push_back((carry + 1) % 10 + 9);
				carry = (carry + 1) / 10 - 1;
			}
			if (carry < 0)data.push_back(carry);
			if (data.back() < 0)
			{
				sign = !sign;
				carry = 1;
				for (std::size_t i = 0; i < data.size(); ++i)
				{
					data[i] = 9 - data[i] + carry;
					carry = data[i] / 10;
					data[i] %= 10;
				}
				data.back() %= 10;
			}
			del_zero();
		}
#endif
	public:
#if VERSION == 1
		void multiplyabs(long long, LongInt&)const&;
#elif VERSION == 2
		void multiply_nr(const LongInt& num, LongInt& result)const&
		{
			for (unsigned long long i = 0; i < data.size(); ++i)
			{
				for (unsigned long long j = 0; j < num.data.size(); ++j)
				{
					result.data[i + j] += data[i] * num.data[j];
				}
			}
		}

		static void fft(std::vector<mycomplex>& vec, int inv, mycomplex* copy, unsigned long long begin = 0, unsigned long long n = 1)
		{
			static unsigned long long i, j, l;
			if (vec.size() == n)return;
			fft(vec, inv, copy, begin, n << 1);
			fft(vec, inv, copy, begin + n, n << 1);
			mycomplex k(1.0, 0.0), zeta(inv * 2 * PI / (vec.size() / n));
			for (i = begin; i < vec.size(); i += n)
			{
				copy[i].real = vec[i].real;
				copy[i].imag = vec[i].imag;
			}
			j = begin;
			for (i = begin; j < vec.size(); i += n)
			{
				l = j + n;
				vec[i].real = copy[j].real + k.real * copy[l].real - k.imag * copy[l].imag;
				vec[i].imag = copy[j].imag + k.real * copy[l].imag + k.imag * copy[l].real;
				k.times(zeta);
				j += n << 1;
			}
			for (j = begin; i < vec.size(); i += n)
			{
				l = j + n;
				vec[i].real = copy[j].real + k.real * copy[l].real - k.imag * copy[l].imag;
				vec[i].imag = copy[j].imag + k.real * copy[l].imag + k.imag * copy[l].real;
				k.times(zeta);
				j += n << 1;
			}
		}
		static void fft_nr(std::vector<mycomplex>& vec, int inv, mycomplex* copy, unsigned long long begin = 0, unsigned long long n = 1)
		{
			for (unsigned long long i = begin; i < vec.size(); i += n)
			{
				copy[i] = vec[i];
			}
			mycomplex zetaij(1.0, 0.0), zetai(1.0, 0.0), zeta(inv * 2.0 * PI / (vec.size() / n));
			for (unsigned long long i = begin; i < vec.size(); i += n)
			{
				vec[i].real = 0;
				vec[i].imag = 0;
				zetaij.real = 1.0;
				zetaij.imag = 0.0;
				for (unsigned long long j = begin; j < vec.size(); j += n)
				{
					vec[i].add(zetaij.multiply(copy[j]));
					zetaij.times(zetai);
				}
				zetai.times(zeta);
			}
		}
		/*
		static void ntt(
			std::vector<unsigned long long>& vec, 
			int inv, 
			unsigned long long* copy, 
			unsigned long long begin = 0, 
			unsigned long long n = 1
		)
		{
			static unsigned long long i, j, k;
			if (vec.size() == n)return;
			ntt(vec, inv, copy, begin, n << 1);
			ntt(vec, inv, copy, begin + n, n << 1);

		}
		*/
#endif
		LongInt()noexcept;//デフォルトコンストラクタ
		LongInt(int);//コンストラクタint
		LongInt(std::size_t, bool);
#if VERSION == 1
		LongInt(const std::string&);//コンストラクタstring
#endif
		LongInt(const char*);//コンストラクタchar*
		LongInt(const LongInt&) = default;//コピーコンストラクタ
		LongInt(LongInt&&) = default;//ムーブコンストラクタ

		virtual ~LongInt()noexcept = default;

		LongInt& operator=(const LongInt&)& = default;
		LongInt& operator=(LongInt&&)& = default;
#if VERSION == 1
		LongInt& operator=(int)&;
#endif
		bool operator<(const LongInt&)const& noexcept;
		bool operator>(const LongInt&)const& noexcept;
		bool operator<=(const LongInt&)const& noexcept;
		bool operator>=(const LongInt&)const& noexcept;

		LongInt& operator++()&;
		LongInt operator++(int)&;
		LongInt& operator--()&;
		LongInt operator--(int)&;

		LongInt operator+()const&;
		LongInt operator-()const&;

#if VERSION == 1
		LongInt operator+(const LongInt&)const&;//加算
		LongInt operator-(const LongInt&)const&;//減算
#elif VERSION == 2
		LongInt operator+(LongInt)const&;
		LongInt operator-(LongInt)const&;
#endif
		
		LongInt operator*(const LongInt&)const&;//乗算
		LongInt operator/(const LongInt&)const&;//除算
		LongInt operator%(const LongInt&)const&;//モジュル演算

#if VERSION == 1
		LongInt& operator+=(const LongInt&)&;
		LongInt& operator-=(const LongInt&)&;
		LongInt& operator*=(const LongInt&)&;
#elif VERSION == 2
		LongInt& operator+=(const LongInt&)&;
		LongInt& operator-=(const LongInt&)&;
		LongInt& operator*=(const LongInt&)&;
#endif
#if VERSION == 1
		byte operator[](std::size_t)const& noexcept;
#elif VERSION == 2
		int operator[](std::size_t)const& noexcept;
#endif

#if VERSION == 1
		std::size_t getdnum()const&
		{
			return digit_num;
		}
#endif

		void random(std::size_t size)&
		{
			std::size_t i;
			static std::random_device seed;
			static std::mt19937_64 mt(seed());
			static std::uniform_int_distribution<> rand(0, 9);
			data.resize(size);
			for (i = 0; i < size; ++i)
			{
				data[i] = rand(mt);
			}
			sign = rand(mt) <= 4;
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