#include "Compressor.h"
// ExtremeCode
int main()
{
	KDI::CCompressor_PN compn;

	// �Ҽ� ���� read
	compn.ReadFromFile( "./2T_part1.txt", "r", 0, 0 );
	// ���� ����
	compn.KDICompression();

	// ��Ʈ�������� ����
	compn.WriteToFile( "./�Ҽ� ��Ʈ.txt" );
	// ���� 1��
	compn.WriteToFileA( "./���ڿ� 1�� ����.txt", "w" );
	// ���� 2��
	compn.WriteToFileB( "./���ڿ� 2�� ����.txt", "w" );

	return 0;
}

