#pragma once

// 김동일 소수 압축.
// 소수들을 읽어 들이고 압축한다. 숫자 하나 당 8바이트일 때, 압축율 비교.
// 메모리 기준.
// 문자열로 압축시 : 1 바이트로 압축되며, 8배의 압축 효율이 나온다.
// 비트로 압축시   : 1 bit로 압축되며,    64배의 압축 효율이 나온다.
// 즉, 1기가 용량에 134,217,728개의 소수를 기억 하고 있었지만, (대략 1.3억개, max prime: )
// 비트로 압축시  8,589,934,592개의 소수를 기억 할 수 있다.    (대략 85.8억개, max prime: )
// text파일 기준. 문자 하나당 1byte 씩 먹는다. 예를 들어, 123123이 저장되어 있다면, 6byte+@의 공간을 차지한다.
// text파일: 90.1MB에 저장된 10,000,000개 (천만개)소수, max prime: 179,424,673 (1.7억).
// 소수 하나당 자리수 만큼 byte 먹기 때문에 효율적이지 못하다.
// 문자열로 압축시 : 9.53674 MB 로 압축가능하다. 만약 숫자가 커지면, 더 압축율이 좋다.
// bit로 압축시 천만개 소수는 천만 비트로 압축 할 수 있다. 1.19209 MB
// 617자리수의 소수는 3.2317 ... E616 bit 보다 작은 값으로 압축된다는 소리. tb는 우습게 넘어간다.

// RSA키는 2048bit로 암호화되는데, 2048bit로 표현 가능한 수는 3.2317 ... E616 값을 넣을 수 있다.
// 2^2048 = 10^n -> 2048 * log(2) = n * log(10) -> n = 2048 * log(2) = 616.5  => 총 617자리 숫자.
// 64bit에서는 1.8E19. 즉, 20자리의 숫자를 표현할 수있다. mpir 라이브러리 사용할 것.
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

	// 소수 압축기
	class CCompressor_PN
	{
	public:
		std::vector<uBig_t> vecOriginalData;
		std::vector<tPrime8> vecKDICompressionData; // KDI bit로 1차 압축
		std::string			strCompression1;		// 문자 1차 압축
		std::string			strCompression2;		// 문자 2차 압축
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
