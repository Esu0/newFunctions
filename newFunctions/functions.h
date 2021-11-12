#pragma once

#include<vector>
#include<iostream>

namespace MyFunctions1
{
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//•‰‚Ì‚Æ‚«1
		std::vector<byte> data;

	public:
		LongInt();
		LongInt(int num);
		LongInt(const LongInt&) = default;
		LongInt(LongInt&&) = default;
	};
}