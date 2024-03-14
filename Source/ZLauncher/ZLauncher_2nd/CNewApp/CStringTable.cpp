
#include "stdafx.h"
#include "CStringTable.h"
#include <map>
#include <string>
using namespace std;



//#define _OUTPUT_STRING


// List of string
map<string,string>		g_MapList;



// Destroy
void STDestroy()
{
	// �޸� ����
	if ( !g_MapList.empty())
		g_MapList.clear();
}


// Load string from a file
bool STLoadFromFile( const char* szFileName)
{
	// �޸� �ʱ�ȭ
	if ( !g_MapList.empty())
		g_MapList.clear();


	// ���� ����
	CFile file;
	CFileException ex;
	if ( file.Open( szFileName, CFile::modeRead, &ex) == FALSE)
		return false;


	// ���� �б�
	char* szBuff = new char[ (unsigned int)file.GetLength()];
	file.Read( szBuff, (UINT)file.GetLength());


	// Load string from memory string
	bool bRetVal = STLoadFromMemory( szBuff);


	// �޸�, ���� ����
	delete [] szBuff;
	file.Close();

	return bRetVal;
}


// Load string from memory string
bool STLoadFromMemory( const char* szMemString)
{
	// ��Ʈ�� ��� �ʱ�ȭ
#ifdef _OUTPUT_STRING
	DeleteFile( "str_out.txt");
#endif

	// ��Ʈ�� �б�
	char ch;
	while ( 1)
	{
		// Skip block
		while ( 1)
		{
			ch = *szMemString++;
			if ( ch == 0)
				return true;

			if ( (ch == '\n') || (ch == '\r') || (ch == ' '))
				continue;

			break;
		}


		// Get name
		char szName[ 64];
		int nNameCount = 0;
		while ( 1)
		{
			szName[ nNameCount++] = ch;

			ch = *szMemString++;
			if ( (ch == 0) || (nNameCount >= 64))
				return true;

			if ( ch == ',')
				break;
		}
		szName[ nNameCount] = 0;


		// Get string
		char szString[ 512];
		int nStringCount = 0;
		while ( 1)
		{
			ch = *szMemString++;
			if ( (ch == 0) || ( nStringCount >= 512))
				return true;

			if ( (ch == '\n') || (ch == '\r'))
				break;

			szString[ nStringCount++] = ch;
		}
		szString[ nStringCount] = 0;



		// �ش� ��Ʈ���� �����ϸ� �׳� ���
		if ( g_MapList.find( szName) != g_MapList.end())
		{
			ASSERT( 0);

			continue;
		}


		// ��Ʈ�� �߰�
		g_MapList.insert( map<string,string>::value_type( szName, szString));


		// ��Ʈ�� ���
#ifdef _OUTPUT_STRING
		FILE* pFile = fopen( "str_out.txt", "a");
		if ( fopen)
		{
			fprintf( pFile, "%s , %s\n",szName, szString);
			fclose( pFile);
		}
#endif
	}
}


// Get string with name
const char* STGetString( const char* szName)
{
	// ��Ʈ�� ã��
	map<string,string>::iterator itr = g_MapList.find( szName);
	if ( itr == g_MapList.end())
		// ã�� ����
		return NULL;


	// ��Ʈ�� ����
	return (char*)(*itr).second.data();
}


// Get string with name
void STGetString( const char* szName, char* szString)
{
	// �ʱ�ȭ
	strcpy( szString, "NO_MSG");


	// ��Ʈ�� ã��
	map<string,string>::iterator itr = g_MapList.find( szName);
	if ( itr == g_MapList.end())
		// ã�� ����
		return;


	// ��Ʈ�� ����
	strcpy( szString, (char*)(*itr).second.data());
}
