#pragma once

#include<vector>
#include<iostream>
#include<string>
#include<random>
#include<complex>
#include<cmath>

#define VERSION 2

namespace MyFunctions1
{
	class mycomplex
	{

	};
	//���{������
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//���̂Ƃ�1
#if VERSION == 1
		std::size_t digit_num = 1;//����
#endif
#if VERSION == 1
		std::vector<byte> data;//�\�L�Ƌt���ɕ���ł���
#elif VERSION == 2
		std::vector<int> data;
#endif
		void addabs(const LongInt&, LongInt&)const&;
		void subtractabs(const LongInt&, LongInt&)const&;//*this-num2>=0�̑O��
		bool cmpabs(const LongInt&)const&;
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
		void del_zero()&
		{
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
		static void fft(std::vector<std::complex<double>>& vec, int inv)
		{
			std::size_t size = vec.size();
			if (size == 1)return;
			std::vector<std::complex<double>> func0, func1;
			for (unsigned long long i = 0; i < size; i += 2)
			{
				func0.push_back(vec[i]);
				func1.push_back(vec[i + 1]);
			}
			fft(func0, inv);
			fft(func1, inv);
			std::complex<double> k = 1.0, zeta = std::exp(std::complex<double>(0.0, inv * 2.0 * std::acos(-1) / size));
			for (unsigned long long i = 0; i < size; ++i)
			{
				vec[i] = func0[i % (size / 2)] + k * func1[i % (size / 2)];
				k *= zeta;
			}
		}
#endif
		LongInt()noexcept;//�f�t�H���g�R���X�g���N�^
		LongInt(int);//�R���X�g���N�^int
		LongInt(std::size_t, bool);
		LongInt(const std::string&);//�R���X�g���N�^string
		LongInt(const char*);//�R���X�g���N�^char*
		LongInt(const LongInt&) = default;//�R�s�[�R���X�g���N�^
		LongInt(LongInt&&) = default;//���[�u�R���X�g���N�^

		virtual ~LongInt() = default;

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
		LongInt operator+(const LongInt&)const&;//���Z
		LongInt operator-(const LongInt&)const&;//���Z
#elif VERSION == 2
		LongInt operator+(LongInt)const&;
		LongInt operator-(LongInt)const&;
#endif
		
		LongInt operator*(const LongInt&)const&;//��Z
		LongInt operator/(const LongInt&)const&;//���Z
		LongInt operator%(const LongInt&)const&;//���W�������Z

#if VERSION == 1
		LongInt& operator+=(const LongInt&)&;
		LongInt& operator-=(const LongInt&)&;
		LongInt& operator*=(const LongInt&)&;
		LongInt& operator/=(const LongInt&)&;
		LongInt& operator%=(const LongInt&)&;
#elif VERSION == 2
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

std::ostream& operator<<(std::ostream&, const MyFunctions1::LongInt&);//�W���o��
std::istream& operator>>(std::istream&, MyFunctions1::LongInt&);//�W������