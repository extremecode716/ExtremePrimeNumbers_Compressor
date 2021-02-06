#pragma once

// �赿�� �Ҽ� ����.
// �Ҽ����� �о� ���̰� �����Ѵ�. ���� �ϳ� �� 8����Ʈ�� ��, ������ ��.
// �޸� ����.
// ���ڿ��� ����� : 1 ����Ʈ�� ����Ǹ�, 8���� ���� ȿ���� ���´�.
// ��Ʈ�� �����   : 1 bit�� ����Ǹ�,    64���� ���� ȿ���� ���´�.
// ��, 1�Ⱑ �뷮�� 134,217,728���� �Ҽ��� ��� �ϰ� �־�����, (�뷫 1.3�ﰳ, max prime: )
// ��Ʈ�� �����  8,589,934,592���� �Ҽ��� ��� �� �� �ִ�.    (�뷫 85.8�ﰳ, max prime: )
// text���� ����. ���� �ϳ��� 1byte �� �Դ´�. ���� ���, 123123�� ����Ǿ� �ִٸ�, 6byte+@�� ������ �����Ѵ�.
// text����: 90.1MB�� ����� 10,000,000�� (õ����)�Ҽ�, max prime: 179,424,673 (1.7��).
// �Ҽ� �ϳ��� �ڸ��� ��ŭ byte �Ա� ������ ȿ�������� ���ϴ�.
// ���ڿ��� ����� : 9.53674 MB �� ���డ���ϴ�. ���� ���ڰ� Ŀ����, �� �������� ����.
// bit�� ����� õ���� �Ҽ��� õ�� ��Ʈ�� ���� �� �� �ִ�. 1.19209 MB
// 617�ڸ����� �Ҽ��� 3.2317 ... E616 bit ���� ���� ������ ����ȴٴ� �Ҹ�. tb�� ����� �Ѿ��.

// RSAŰ�� 2048bit�� ��ȣȭ�Ǵµ�, 2048bit�� ǥ�� ������ ���� 3.2317 ... E616 ���� ���� �� �ִ�.
// 2^2048 = 10^n -> 2048 * log(2) = n * log(10) -> n = 2048 * log(2) = 616.5  => �� 617�ڸ� ����.
// 64bit������ 1.8E19. ��, 20�ڸ��� ���ڸ� ǥ���� ���ִ�. mpir ���̺귯�� ����� ��.
// https://namu.wiki/w/RSA%20%EC%95%94%ED%98%B8%ED%99%94
// https://www.dcode.fr/prime-number-pi-count   / https://ko.calcuworld.com/%EB%B9%84%EC%A6%88%EB%8B%88%EC%8A%A4/%EB%B0%94%EC%9D%B4%ED%8A%B8-%EA%B3%84%EC%82%B0%EA%B8%B0/

#include <vector>
#include <string>
namespace KDI
{
	typedef unsigned long long uBig_t;
	constexpr char*	cBig_t = "%llu";

	struct tPrime8 {
		union {
			struct {
				bool a : 1;
				bool b : 1;
				bool c : 1;
				bool d : 1;

				bool e : 1;
				bool f : 1;
				bool g : 1;
				bool h : 1;
			};
			char prime8;
		};
		void Reset()
		{
			prime8 = 0;
		}
		void Write( FILE * _pFile ) const
		{
			fwrite( this, sizeof( tPrime8 ), 1, _pFile );
		}
	};

	// �Ҽ� �����
	class CCompressor_PN
	{
	public:
		std::vector<uBig_t> vecOriginalData;
		std::vector<tPrime8> vecKDICompressionData; // KDI bit�� 1�� ����
		std::string			strCompression1;		// ���� 1�� ����
		std::string			strCompression2;		// ���� 2�� ����
													// .. more Compression
	public:
		bool ReadFromFile( const char* _FileName, const char* _Mode = "r", const size_t _StartIndex = 0, const size_t _ReadCount = 0 );
		bool KDICompression();
		bool WriteToFile( const char* _FileName, const char* _Mode = "wb", const size_t _StartIndex = 0, const size_t _ReadCount = 0 );
		bool WriteToFileA( const char* _FileName, const char* _Mode = "wb", const size_t _StartIndex = 0, const size_t _ReadCount = 0 );
		bool WriteToFileB( const char* _FileName, const char* _Mode = "wb", const size_t _StartIndex = 0, const size_t _ReadCount = 0 );
	public:
		CCompressor_PN();
		~CCompressor_PN();
	};

	class CPN_Viewer
	{
	public:
		std::vector<tPrime8> vecKDICompressionData; // KDI bit 

	public:
		bool ReadFromFile( const char* _FileName, const char* _Mode = "rb" );
	};
}
