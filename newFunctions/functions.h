#pragma once

#include<vector>
#include<iostream>
#include<string>

namespace MyFunctions1
{
	//���{������
	class LongInt
	{
	private:
		
		using byte = unsigned char;

		bool sign = 0;//���̂Ƃ�1
		std::vector<byte> data;//�\�L�Ƌt���ɕ���ł���

	public:

		LongInt();//�f�t�H���g�R���X�g���N�^
		LongInt(int);//�R���X�g���N�^int
		LongInt(const std::string&);//�R���X�g���N�^string
		LongInt(const char*);//�R���X�g���N�^char*
		LongInt(const LongInt&) = default;//�R�s�[�R���X�g���N�^
		LongInt(LongInt&&) = default;//���[�u�R���X�g���N�^
		
		LongInt& operator=(const LongInt&)& = default;
		LongInt& operator=(LongInt&&)& = default;
		LongInt& operator=(int)&;
		
		void tostring(std::string&)const&;

	};
}

std::ostream& operator<<(std::ostream&, const MyFunctions1::LongInt&);//�W���o��
std::istream& operator>>(std::istream&, MyFunctions1::LongInt&);//�W������