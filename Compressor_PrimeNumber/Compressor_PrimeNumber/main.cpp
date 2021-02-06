#include "Compressor.h"
// ExtremeCode
int main()
{
	KDI::CCompressor_PN compn;

	// 소수 문서 read
	compn.ReadFromFile( "./2T_part1.txt", "r", 0, 0 );
	// 압축 시작
	compn.KDICompression();

	// 비트형식으로 압축
	compn.WriteToFile( "./소수 비트.txt" );
	// 압축 1차
	compn.WriteToFileA( "./문자열 1차 압축.txt", "w" );
	// 압축 2차
	compn.WriteToFileB( "./문자열 2차 압축.txt", "w" );

	return 0;
}

