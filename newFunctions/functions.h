#pragma once

#include<vector>
#include<iostream>
#include<string>

namespace MyFunctions1
{
	//多倍長整数
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//負のとき1
		std::vector<byte> data;//表記と逆順に並んでいる

	public:

		LongInt();//デフォルトコンストラクタ
		LongInt(int);//コンストラクタint
		LongInt(const std::string&);//コンストラクタstring
		LongInt(const char*);//コンストラクタchar*
		LongInt(const LongInt&) = default;//コピーコンストラクタ
		LongInt(LongInt&&) = default;//ムーブコンストラクタ
		
		LongInt& operator=(const LongInt&)& = default;
		LongInt& operator=(LongInt&&)& = default;
		LongInt& operator=(int)&;
		
		void tostring(std::string&)const&;

	};
}

std::ostream& operator<<(std::ostream&, const MyFunctions1::LongInt&);//標準出力
std::istream& operator>>(std::istream&, MyFunctions1::LongInt&);//標準入力