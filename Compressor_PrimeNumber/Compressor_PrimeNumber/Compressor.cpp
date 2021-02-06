#include "Compressor.h"

#include <iostream>

bool KDI::CCompressor_PN::ReadFromFile( const char * _FileName, const char* _Mode, const size_t _StartIndex, const size_t _ReadCount )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, _FileName, _Mode );

	if ( NULL == pFile )
	{
		std::cout << "File Path error" << std::endl;
		fclose( pFile );
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	const uBig_t filelength = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );

	if ( filelength <= 0 )
	{
		fclose( pFile );
		return false;
	}

	uBig_t	primNumber = 0;
	if ( 0 == _ReadCount && filelength >= 1000000 )
		vecOriginalData.reserve( 1000000 );
	else
		vecOriginalData.reserve( _ReadCount );
	size_t  Count = 0;

	std::cout << "Read prime data start..." << "Mode : " << _Mode << std::endl;

	// _ReadCount == 0 일 시 끝까지 읽기.
	if ( 0 == _ReadCount )
	{
		while ( 1 == fscanf_s( pFile, cBig_t, &primNumber ) )
		{
			++Count;
			if ( Count > _StartIndex )
				vecOriginalData.push_back( primNumber );
		}
	}
	else
	{
		while ( 1 == fscanf_s( pFile, cBig_t, &primNumber ) && Count < _ReadCount )
		{
			++Count;
			if ( Count > _StartIndex )
				vecOriginalData.push_back( primNumber );
		}
	}
	std::cout << "Read prime data End" << std::endl;
	if ( !vecOriginalData.empty() )
		std::cout << "start prime number : " << *vecOriginalData.begin() << " end prime number : " << *(--vecOriginalData.end()) << " readCount : " << Count - _StartIndex << std::endl;
	else
		std::cout << "Prime Data Empty" << std::endl;

	fclose( pFile );
	return true;
}

bool KDI::CCompressor_PN::KDICompression()
{
	if ( vecOriginalData.empty() )
		return false;

	uBig_t num = vecOriginalData[0] - (vecOriginalData[0] % 10);
	std::vector<int> vecLastNum = { 1, 3, 7, 9 };
	struct tPrime8 primeData8;
	primeData8.Reset();

	bool bFlag = true;
	bool bPrevFlag = bFlag;

	size_t iCount = 0;
	size_t	iIndex = 0;

	// 10부터 패턴 적용.
	if ( num < 10 )
	{
		num = 10;
		while ( true )
		{
			if ( vecOriginalData.size() <= iIndex )
				return false;

			if ( vecOriginalData[iIndex] < num )
				++iIndex;
			else
				break;
		}
	}

	bool bPrime8Cycle = false;
	std::cout << "KDI Pattern Start" << std::endl;
	while ( num <= *(--vecOriginalData.end()) )
	{
		for ( size_t i = 0; i < 4; ++i )
		{
			if ( vecOriginalData.size() <= iIndex )
				break;

			uBig_t newNum = num + vecLastNum[i];

			if ( newNum > vecOriginalData[iIndex] )
			{
				std::cout << vecOriginalData[iIndex] << " error 1" << std::endl;
				return false;
			}

			if ( newNum == vecOriginalData[iIndex] )
			{
				if ( bPrime8Cycle == false )
					primeData8.prime8 |= (1 << i);
				else
					primeData8.prime8 |= (1 << (i + 4));

				strCompression1.append( "1" );
				++iIndex;
				bFlag = true;
			}
			else
			{
				strCompression1.append( "0" );
				bFlag = false;
			}

			++iCount;

			if ( bFlag != bPrevFlag )
			{
				--iCount;
				strCompression2.append( std::to_string( iCount ) );
				iCount = 1;
			}

			bPrevFlag = bFlag;
		}
		num += 10;
		bPrime8Cycle = !bPrime8Cycle;
		if ( false == bPrime8Cycle )
		{
			vecKDICompressionData.push_back( primeData8 );
			primeData8.Reset();
		}
	}
	if ( true == bPrime8Cycle )
	{
		vecKDICompressionData.push_back( primeData8 );
		primeData8.Reset();
	}
	std::cout << "KDI Pattern End" << std::endl;

	const char *pStr = strCompression1.c_str();

	size_t i = 0;

	//while ( pStr[i] != 0 )
	//{
	//	std::cout << pStr[i];
	//	++i;
	//}

	//pStr = strCompression2.c_str();
	//i = 0;
	//while ( pStr[i] != 0 )
	//{
	//	std::cout << pStr[i];
	//	++i;
	//}

	std::cout << std::endl;
	std::cout << "KDI Pattern 결과" << std::endl;
	std::cout << "bit로 압축시 : ";
	std::cout << vecKDICompressionData.size() * sizeof( tPrime8 ) << " Byte" << std::endl;
	std::cout << "문자로 압축시(1byte) : " << strCompression1.length() << " Byte" << std::endl;

	std::cout << std::endl;
	std::cout << "문자 1차 압축 결과" << std::endl;
	std::cout << strCompression2.length() << "Byte" << std::endl;
	std::cout << "문자 1차 압축된 Byte : " << strCompression1.length() - strCompression2.length() << " Byte" << "\t총 압축률 : " << (float)(strCompression1.length() - strCompression2.length()) / strCompression1.length() * 100 << " %" << std::endl;

	return true;
}

bool KDI::CCompressor_PN::WriteToFile( const char * _FileName, const char * _Mode, const size_t _StartIndex, const size_t _ReadCount )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, _FileName, _Mode );

	if ( NULL == pFile )
	{
		std::cout << "File Path error" << std::endl;
		fclose( pFile );
		return false;
	}

	if ( vecKDICompressionData.empty() )
	{
		fclose( pFile );
		return false;
	}


	std::cout << "Write prime data start..." << "Mode : " << _Mode << std::endl;
	for ( const auto& data : vecKDICompressionData )
	{
		data.Write( pFile );
	}
	std::cout << "Write prime data End" << std::endl;

	fclose( pFile );

	return true;
}

bool KDI::CCompressor_PN::WriteToFileA( const char * _FileName, const char * _Mode, const size_t _StartIndex, const size_t _ReadCount )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, _FileName, _Mode );

	if ( NULL == pFile )
	{
		std::cout << "File Path error" << std::endl;
		fclose( pFile );
		return false;
	}

	if ( vecKDICompressionData.empty() )
	{
		fclose( pFile );
		return false;
	}


	std::cout << "Write prime data start..." << "Mode : " << _Mode << std::endl;
	if ( _Mode == "w" )
		fwrite( strCompression1.c_str(), strCompression1.length() + 1, 1, pFile );
	else
	{
		size_t i = 0;
		const char* pa = strCompression1.c_str();

		while ( pa[i] != 0 )
		{
			fwrite( &pa[i], sizeof( char ), 1, pFile );
			++i;
		}
	}
	std::cout << "Write prime data End" << std::endl;

	fclose( pFile );

	return true;
}

bool KDI::CCompressor_PN::WriteToFileB( const char * _FileName, const char * _Mode, const size_t _StartIndex, const size_t _ReadCount )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, _FileName, _Mode );

	if ( NULL == pFile )
	{
		std::cout << "File Path error" << std::endl;
		fclose( pFile );
		return false;
	}

	if ( vecKDICompressionData.empty() )
	{
		fclose( pFile );
		return false;
	}


	std::cout << "Write prime data start..." << "Mode : " << _Mode << std::endl;
	if ( _Mode == "w" )
		fwrite( strCompression2.c_str(), strCompression2.length() + 1, 1, pFile );
	else
	{
		size_t i = 0;
		const char* pa = strCompression2.c_str();
		fwrite( pa, sizeof( char ), strCompression2.length(), pFile );
	}
	std::cout << "Write prime data End" << std::endl;

	fclose( pFile );

	return true;
}

KDI::CCompressor_PN::CCompressor_PN()
{
}

KDI::CCompressor_PN::~CCompressor_PN()
{
}

bool KDI::CPN_Viewer::ReadFromFile( const char * _FileName, const char * _Mode )
{
	FILE *pFile = NULL;
	fopen_s( &pFile, _FileName, _Mode );

	if ( NULL == pFile )
	{
		std::cout << "File Path error" << std::endl;
		fclose( pFile );
		return false;
	}

	size_t  Count = 0;
	size_t  iTotal = 0;

	while ( !feof( pFile ) )
	{
		tPrime8 tempPrime8;
		tempPrime8.Reset();
		Count = fread( &tempPrime8, sizeof( tPrime8 ), 1, pFile );
		if ( Count != 1 )
			break;

		vecKDICompressionData.push_back( tempPrime8 );

		iTotal += Count;
	}

	fclose( pFile );
	return false;
}
