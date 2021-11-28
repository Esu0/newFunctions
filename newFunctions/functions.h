#pragma once

#include<vector>
#include<iostream>
#include<string>
#include<random>

namespace MyFunctions1
{
	//多倍長整数
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//負のとき1
		std::size_t digit_num = 1;//桁数
		std::vector<byte> data;//表記と逆順に並んでいる

		void addabs(const LongInt&, LongInt&)const&;
		void subtractabs(const LongInt&, LongInt&)const&;//*this-num2>=0の前提
		bool cmpabs(const LongInt&)const&;

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
	public:
		void multiplyabs(long long, LongInt&)const&;
		LongInt()noexcept;//デフォルトコンストラクタ
		LongInt(int);//コンストラクタint
		LongInt(std::size_t, bool);
		LongInt(const std::string&);//コンストラクタstring
		LongInt(const char*);//コンストラクタchar*
		LongInt(const LongInt&) = default;//コピーコンストラクタ
		LongInt(LongInt&&) = default;//ムーブコンストラクタ

		virtual ~LongInt() = default;

		LongInt& operator=(const LongInt&)& = default;
		LongInt& operator=(LongInt&&)& = default;
		LongInt& operator=(int)&;

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

		LongInt operator+(const LongInt&)const&;//加算
		LongInt operator-(const LongInt&)const&;//減算
		LongInt operator*(const LongInt&)const&;//乗算
		LongInt operator/(const LongInt&)const&;//除算
		LongInt operator%(const LongInt&)const&;//モジュル演算

		LongInt& operator+=(const LongInt&)&;
		LongInt& operator-=(const LongInt&)&;
		LongInt& operator*=(const LongInt&)&;
		LongInt& operator/=(const LongInt&)&;
		LongInt& operator%=(const LongInt&)&;
		
		byte operator[](std::size_t)const& noexcept;

		std::size_t getdnum()const&
		{
			return digit_num;
		}

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
				if (data[i] != 0)digit_num = i + 1;
			}
			sign = rand(mt) <= 4;
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