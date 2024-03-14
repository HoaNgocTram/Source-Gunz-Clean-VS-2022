
#include "stdafx.h"
#include "NewAppDlg.h"
#include <Mmsystem.h>
#include <Tlhelp32.h>



// Defines
#define LAUNCHER_FILENAME			"GunzLauncher.exe"					// ���� ���� ���� ���� �̸�



// DoDataExchange
void CNewAppDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppDlg)
	DDX_Control(pDX, IDOK, m_cOK);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/************************************************************************************
  Dialog
*************************************************************************************/
// Ŭ���� ������
CNewAppDlg::CNewAppDlg( CWnd* pParent) : CDDBDialog( IDD_DIALOG_MAINDLG, pParent)
{
	// �̰����� ������ ���̾�α��� ������ �Է��Ѵ�
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth  = 300;
	DlgInfo.nHeight = 200;
	DlgInfo.IDSkinResource = IDR_BMP_MAINDLG;
	DlgInfo.IDIconResource = IDR_ICO_MAINFRAME;
	DlgInfo.nTextColor = RGB( 38, 38, 38);
	DlgInfo.nTextBkColor = RGB( 182, 182, 182);

#ifdef LOCALE_NHNUSA
	DlgInfo.nTextColor = RGB( 150, 150, 150);
	DlgInfo.nTextBkColor = RGB( 38, 38, 38);
#else
	DlgInfo.bUseTitlebar = true;
#endif

	SetDialogInfo( DlgInfo);
}


CNewAppDlg::~CNewAppDlg()
{
}


/************************************************************************************
  Ŭ���� �޼��� �ڵ鷯
*************************************************************************************/
// OnInitDialog : ���̾�α׸� �ʱ�ȭ�� �Լ�
BOOL CNewAppDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();
	

	// ���̾�α� Ÿ��Ʋ ����
	SetWindowText( "GunZ Updater");


#ifdef LOCALE_NHNUSA
	GetScreen()->PutText( 1, 3, GetWidth(), 20, 16, "Arial", "GunZ Updater", RGB( 20, 20, 20), TS_BOLD, DT_CENTER);
	GetScreen()->PutText( 0, 2, GetWidth(), 20, 16, "Arial", "GunZ Updater", RGB( 200, 200, 200), TS_BOLD, DT_CENTER);
	GetScreen()->PutBox( 12, 24, GetWidth() - 24, GetHeight() - 68, RGB( 80, 80, 80));
#endif


	// ���̾�α� ��ġ �̵� �� ũ�� ����
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);


	// �޽��� ǥ��
	((CListBox*)GetDlgItem( IDC_OUTPUT))->ResetContent();
	((CListBox*)GetDlgItem( IDC_OUTPUT))->MoveWindow( 13, 25, GetWidth() - 26, GetHeight() - 70);
	OutputMsg( "Ready...");


	// ��ư �ʱ�ȭ
	m_cOK.InitDDBButton( IDR_BMP_BUTTON, NULL);
	m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() / 2, GetHeight() - 35);
	m_cOK.EnableWindow( FALSE);


	// Ÿ�̸� ����
	SetTimer( 0, 200, NULL);


	return true;
}


/************************************************************************************
  Ÿ�̸�
*************************************************************************************/
void CNewAppDlg::OnTimer(UINT nIDEvent)
{
	// ������Ʈ Ÿ�̸�
	if ( nIDEvent == 0)
	{
		static DWORD tStartTime = timeGetTime();
		DWORD currTime = timeGetTime() - tStartTime;


		// ���� ���μ����� ����Ǵ� ���� Ȯ���Ѵ�
		static bool bClosedLauncher = false;
		if ( !bClosedLauncher && (currTime > 500))
		{
			bClosedLauncher = FindLauncherProcess();
		}


		// ���� ��ġ
		if ( bClosedLauncher)
		{
			static int nPatchCount = 0;
			static int nErrorCount = 0;
			switch ( nPatchCount)
			{
				case 0 :
					nPatchCount++;
					if ( PatchFile( ".\\PATCH\\GunzLauncher.exe_", ".\\GunzLauncher.exe") == false)
						nErrorCount++;
					break;

				case 1 :
					nPatchCount++;
					if ( PatchFile( ".\\PATCH\\HanAuthForClient.dll_", ".\\HanAuthForClient.dll") == false)
						nErrorCount++;
					break;

				case 2 :
					nPatchCount++;
					if ( PatchFile( ".\\PATCH\\HanReportForClient.dll_", ".\\HanReportForClient.dll") == false)
						nErrorCount++;
					break;

				case 3 :
					nPatchCount++;

					if ( nErrorCount)
					{
						((CListBox*)GetDlgItem( IDC_OUTPUT))->ShowScrollBar( SB_HORZ);
						((CListBox*)GetDlgItem( IDC_OUTPUT))->SetHorizontalExtent( 1000);
						
						char szMsg[ 512];
						sprintf( szMsg, "%d error has been found.", nErrorCount);
						OutputMsg( szMsg);
					}
					else
						OutputMsg( "Patch successfully completed.");
					
					tStartTime = timeGetTime();
					m_cOK.EnableWindow( TRUE);
					break;

				// ������ ����
				case 4 :
					if ( (nErrorCount == 0) && (currTime > 2000))
						OnOK();
					break;

				default :
					break;
			}
		}
	}


	CDDBDialog::OnTimer(nIDEvent);
}


/************************************************************************************
  �޽��� ����ϴ� �Լ�
*************************************************************************************/
void CNewAppDlg::OutputMsg( const char* pszMsg)
{
	((CListBox*)GetDlgItem( IDC_OUTPUT))->AddString( pszMsg);
	((CListBox*)GetDlgItem( IDC_OUTPUT))->SetCaretIndex( ((CListBox*)GetDlgItem( IDC_OUTPUT))->GetCount());
}


/************************************************************************************
  ���� ó��
*************************************************************************************/
void CNewAppDlg::OnOK()
{
	// ���ĸ� �ٽ� �����Ѵ�.
	char szCmdLine[ 4096] = { 0,};
	sprintf( szCmdLine, ".\\GunzLauncher.exe \"%s\"", GetCommandLine());
	WinExec( szCmdLine, SW_SHOW);


	// ������ ����
	CDialog::OnOK();
}

void CNewAppDlg::OnCancel()
{
}


/************************************************************************************
  ��ġ ����
*************************************************************************************/
bool CNewAppDlg::FindLauncherProcess()
{
	// ���İ� ���� ���������� Ȯ���Ѵ�
	if ( FindProcess( LAUNCHER_FILENAME) != NULL)
		return false;


	// ������ ���ؼ� 1�� ���� ��ٸ���
	Sleep( 1000);

	return true;
}


/************************************************************************************
  ���� ��ġ
*************************************************************************************/
bool CNewAppDlg::PatchFile( const char* szPatchFileName, const char* szFileName)
{
	// ������Ʈ ������ ã�´�
	WIN32_FIND_DATA FindData;
	HANDLE hFind = FindFirstFile( _T( szPatchFileName), &FindData);
	if ( hFind != INVALID_HANDLE_VALUE)		// ������ ������...
	{
		FindClose( hFind);


		// ������Ʈ ������ �����Ѵ�.
		if ( CopyFile( _T( szPatchFileName), _T( szFileName), false) == TRUE)	// ���� ������...
		{
			// ������Ʈ ������ �����Ѵ�.
			DeleteFile( _T( szPatchFileName));


			// ��� �޽��� �ۼ�
			char szMsg[ 512];
			sprintf( szMsg, "%s : Patched", szFileName);
			OutputMsg( szMsg);

			return true;
		}

		// ���� �����̸�...
		else
		{
			// ���� �ڵ�
			LPVOID lpMsgBuf;
			DWORD error = GetLastError();
			FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
							NULL, error, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR)&lpMsgBuf, 0, NULL);

			// ��� �޽��� �ۼ�
			char szMsg[ 512];
			sprintf( szMsg, "%s : Failed (%s)\n", szFileName, lpMsgBuf);
			OutputMsg( szMsg);

			LocalFree( lpMsgBuf);

			return false;
		}
	}


	return true;
}


/************************************************************************************
  �ش� ���μ����� �ڵ��� ���Ѵ�
*************************************************************************************/
/* 98���� ���� ����... =_=
HANDLE CNewAppDlg::FindProcess( const char* pszFileName)
{
	DWORD ProcessIDList[ 1024], cbNeeded;

	if ( !EnumProcesses( ProcessIDList, sizeof( ProcessIDList), &cbNeeded))
		return NULL;

	// Calculate how many process identifiers were returned.
	int nProcessCount = cbNeeded / sizeof( DWORD);

	for ( int i = 0;  i < nProcessCount;  i++)
	{
		DWORD nProcessID = ProcessIDList[ i];

		char szProcessName[ MAX_PATH] = "unknown";
		char szProcessPath[ MAX_PATH] = "unknown";

		// Get a handle to the process.
		HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, TRUE, nProcessID);
		if ( NULL != hProcess)		// Get the process name.
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if ( EnumProcessModules( hProcess, &hMod, sizeof( hMod), &cbNeeded))
			{
				GetModuleBaseName( hProcess, hMod, szProcessName, sizeof( szProcessName));
				GetModuleFileNameEx( hProcess, hMod, szProcessPath, sizeof( szProcessPath));

				// ���� �̸��� ��
				if ( stricmp( pszFileName, szProcessName) == 0)
					return hProcess;

				// ��ü ��α��� ��
				else if ( stricmp( pszFileName, szProcessPath) == 0)
					return hProcess;
			}

			CloseHandle( hProcess);
		}
	}

	return NULL;
}
*/

HANDLE CNewAppDlg::FindProcess( const char* pszFileName)
{
	// Take a snapshot of all processes in the system.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);
	if ( hProcessSnap == INVALID_HANDLE_VALUE)
		return NULL;


	// Set the size of the structure before using it.
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32);


	// Retrieve information about the first process, and exit if unsuccessful
	if ( !Process32First( hProcessSnap, &pe32))
	{
		CloseHandle( hProcessSnap);
		return NULL;
	}


	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		// Retrieve the priority class.
		DWORD dwPriorityClass = 0;
		HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

		if ( hProcess == NULL)
			continue;


		// Get module name
		char szModuleName[ 256];
		strcpy( szModuleName, (LPCTSTR)pe32.szExeFile);

		if ( stricmp( pszFileName, szModuleName) == 0)
		{
			// Find module
			CloseHandle( hProcess);
			CloseHandle( hProcessSnap);

			return hProcess;
		}

		CloseHandle( hProcess);

	} while( Process32Next( hProcessSnap, &pe32));


	CloseHandle( hProcessSnap);

	return NULL;
}
