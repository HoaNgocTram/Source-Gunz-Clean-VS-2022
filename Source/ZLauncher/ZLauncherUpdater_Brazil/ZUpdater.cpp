/******************************************************************
   
   ZUpdater.cpp

     Corperation : MAIET entertainment
     Programmer  : Lim Dong Hwan
	 Date        : 4.July.2005

*******************************************************************/


#include "stdafx.h"												// Include stdafx.h
#include <windows.h>											// Include windows.h


// Defines
#define LAUNCHER_NAME				"Iniciando The Duel"		// ���� ���� App �̸�
#define LAUNCHER_FILENAME_PATCH		"GunzLauncher.ex_"			// ���� ���� ��ġ ���� �̸�
#define LAUNCHER_FILENAME_EXE		"GunzLauncher.exe"			// ���� ���� ���� ���� �̸�



// main
int _tmain(int argc, _TCHAR* argv[])
{
	// ȭ�鿡 �ؽ�Ʈ ���
	printf( "Gunz Launcher Updating...\n");


	// �������� ���� ���α׷��� ã�´�
	HWND hWnd = FindWindow( NULL, LAUNCHER_NAME);
	if ( hWnd != NULL)		// �������� ���İ� ������...
	{
		// �������� ���Ŀ� ���� �޽����� ������
		PostMessage( hWnd, WM_CLOSE, 0, 0);


		// �������� ���İ� ����� ������ ����Ѵ�.
		while ( hWnd != NULL)
		{
			hWnd = FindWindow( NULL, LAUNCHER_NAME);

			Sleep( 50);
		}
	}


	Sleep( 1000);


	// ���� ������Ʈ ����(GunzLauncher.ex_)�� ã�´�
	WIN32_FIND_DATA FindData;
	if ( FindFirstFile( _T( LAUNCHER_FILENAME_PATCH), &FindData) != INVALID_HANDLE_VALUE)		// ������ ������...
	{
		while ( 1)
		{
			// ������ �ִ� ���� ���� ����(GunzLauncher.exe)�� �����Ѵ�.
			if ( DeleteFile( _T( LAUNCHER_FILENAME_EXE)) == TRUE)		// ���� �Ǿ�����...
			{
				while ( 1)
				{
					// ���� ������Ʈ ����(GunzLauncher.ex_)�� ���� ��������(GunzLauncher.exe)�� �����Ѵ�.
					if ( CopyFile( _T( LAUNCHER_FILENAME_PATCH), _T( LAUNCHER_FILENAME_EXE), false) == TRUE)	// ���� ������...
					{
						// ���� ������Ʈ ����(GunzLauncher.ex_)�� �����Ѵ�.
						DeleteFile( _T( LAUNCHER_FILENAME_PATCH));

					
						break;
					}
					// ���翡 �����ߴٸ�...
					else
					{
						// ���
						Sleep( 50);
					}
				}

				break;
			}
			// ���� ���� ����(GunzLauncher.exe)�� ������ �� �� ���ٸ�...
			else
			{
				// ���
				Sleep( 50);
			}
		}
	}


	// ���ĸ� �ٽ� �����Ѵ�.
	WinExec( "GunzLauncher.exe", SW_SHOW);


	return 0;
}
