/*
 CNewAppMainDlg Ŭ����(NewAppMainDlg.cpp)

  desc : Main Dialog ���� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "NewAppMainDlg.h"
#include ".\newappmaindlg.h"
#include "MXML.h"
#include <Mmsystem.h>
#include "winver.h"
#include "Log.h"
#include "NewAppPopupDlg.h"
#include "MZIp.h"
#include "CGLEncription.h"


// Functions
UINT ThreadUpdate( LPVOID pParam);


#define TIMER_STARTUPDATE		0
#define TIMER_TIMEOUT			1
#define TIMER_UPDATEUI			2
#define TIMER_OPENWEB			3
#define TIMER_QUIT				4

#define WM_DESTROYTHREAD		(WM_USER + 50)			// Message of destroy thread

#define SKIP_COUNT				3

#define PROGRESSBAR1_POSX	50
#define PROGRESSBAR1_POSY	429
#define PROGRESSBAR2_POSX	39
#define PROGRESSBAR2_POSY	424
#define PROGRESSBAR_WIDTH	202
#define PROGRESSBAR_HEIGHT	13


/************************************************************************************
  CNewAppMainDlg dialog
*************************************************************************************/
// CNewAppMainDlg
CNewAppMainDlg::CNewAppMainDlg(CWnd* pParent /*=NULL*/)
	: CDDBDialog(CNewAppMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAppMainDlg)
	//}}AFX_DATA_INIT

	// Set dialog information : �̰����� ������ ���̾�α��� ������ �Է��Ѵ�.
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�.
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth = 600;
	DlgInfo.nHeight = 460;
	DlgInfo.IDDlgSkinBmpResource = IDB_SKIN_MAINDLG;
	DlgInfo.IDDlgIconResource = IDR_ICON_MAINFRAME;
	DlgInfo.bEnableDlgMove = true;
	DlgInfo.nTextColor = RGB( 182, 182, 182);
	DlgInfo.nTextBkColor = RGB( 38, 38, 38);
	SetDialogInfo( DlgInfo);

	// ���� �ʱ�ȭ
	m_pThreadUpdate = NULL;
	m_bThreadContinue = false;
	m_szServerIP[0] = 0;
	m_nPort = 0;
	strcpy( m_szClientVersion, "Unknown");
	m_bQuit = false;
	m_bUpdating = false;
	m_nSkip = 1;
	m_bTimeOut = false;
	m_bBlank = false;
	m_nLaunchMode = 0;
}


// DoDataExchange
void CNewAppMainDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppMainDlg)
	DDX_Control(pDX, IDC_EXPLORER,			m_cExplorer);
	DDX_Control(pDX, IDC_STARTGAME,			m_cStartGame);
	DDX_Control(pDX, IDC_UPDATEINFO,		m_cUpdateInfo);
	DDX_Control(pDX, IDC_PERCENT1,			m_cPercent1);
	DDX_Control(pDX, IDC_PERCENT2,			m_cPercent2);
	DDX_Control(pDX, IDC_CANCELDOWNLOAD,	m_cCancelDownload);
	DDX_Control(pDX, IDC_ANIBMP_RUN,		m_cAnibmpRun);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppMainDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppMainDlg)
	ON_WM_DESTROY()
	ON_COMMAND(IDM_QUIT, OnOK)
	ON_BN_CLICKED(IDC_STARTGAME, OnBnClickedStartgame)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CANCELDOWNLOAD, OnBnClickedCanceldownload)
	ON_MESSAGE(WM_DESTROYTHREAD, OnCompleteUpdate)
	ON_BN_CLICKED(IDC_ABOUTBOX, OnAbout)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  CNewAppMainDlg message handlers
*************************************************************************************/

// GetEncodedVersion
char* GetEncodedVersion( const char* szVersion)
{
	static char szEncVersion[ 256];
	szEncVersion[ 0] = 0;
	int nCount = 0;

	for ( int i = 0;  i <= (int)strlen( szVersion);  i++)
	{
		char chChar = *(szVersion + i);

		if ( (chChar != (char)',') && (chChar != (char)' '))
			szEncVersion[ nCount++] = chChar;
	}

	return szEncVersion;
}


// OnInitDialog
// ���̾�α� �ʱ�ȭ
BOOL CNewAppMainDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


	// �α� �޽��� �ʱ�ȭ
	OpenLog( "patchlog", "GunzLauncher Log", LOGTYPE_HTML);


	// ���̾�α� ��ġ �̵� �� ũ�� ����
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.left = AfxGetApp()->GetProfileInt( "Window Position", "x", 50);
	rect.top  = AfxGetApp()->GetProfileInt( "Window Position", "y", 50);
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);

	// �г� �׸���
	DrawPanel(1, 0,   598, 50, "", false, NULL, 0);
	DrawPanel(1, 376, 598, 50,  "", false, NULL, 0);
	m_cDDB.FillTiledBitmap( 1, 424, 597, 4, GetSkinBmp(), 154, 13, 17, 4);
	m_cDDB.PutBitmap( 35, 392, GetSkinBmp(), 0, 63, 284, 60, SRCCOPY);
	m_cDDB.PutBitmap( 50, 429, GetSkinBmp(), 42, 21, 206, 13, SRCCOPY);
	m_cDDB.PutText(0, 180, 600, 15, 13, "Arial", "Loading...", TSTYLE_BOLD, DT_CENTER | DT_VCENTER, RGB( 200,200,200));
	m_cDDB.PutBitmap(7, 402, GetSkinBmp(), 250, 21, 16, 16, SRCCOPY);
	m_cDDB.PutBitmap(578, 402, GetSkinBmp(), 250, 21, 16, 16, SRCCOPY);
	m_cDDB.PutBitmap(7, 434, GetSkinBmp(), 267, 21, 16, 16, SRCCOPY);
	m_cDDB.PutBitmap(578, 434, GetSkinBmp(), 267, 21, 16, 16, SRCCOPY);


	// ��ư �ʱ�ȭ
	CreateQuitButton( rect.Width() - 18, 3);			// Quit ��ư ����
	CreateAboutButton( rect.Width() - 35, 3);			// About ��ư ����
	(CButton*)GetDlgItem( IDC_ABOUTBOX)->ShowWindow( false);
	m_cStartGame.InitDDBButton( IDB_BTNSKIN_STARTGAME, NULL);
	m_cCancelDownload.InitDDBButton( IDB_BTNSKIN_STOP, NULL);

	
	// Ani bitmap �ʱ�ȭ
	m_cAnibmpRun.InitDDBAniBmp(IDB_ANIBMP_RUN, 30);
	m_cAnibmpRun.SetTimerValue(100);
	m_cAnibmpRun.EnableAnimate(TRUE);
	m_cAnibmpRun.ShowWindow(SW_HIDE);
	m_cAnibmpRun.MoveWindow(50, 409, 11, 11);


	// GunZ client�� ������ ���Ѵ�.
	GetGunzExeVersion();


	// mrs1 -> mrs2 üũ.. ������ 2-3�� ��ġ�Ŀ��� �����ص���...
//	PutLog( "[APP] Convert MRS1 to MRS2");

	char temp_path[ 1024];
	sprintf( temp_path,"*");

	FFileList file_list;
	GetFindFileListWin(temp_path,".mrs",file_list);
	file_list.UpgradeMrs();


	// ���ͳ� Ž����â �ʱ�ȭ
	m_cExplorer.MoveWindow( 1, 16, 0, 0, false);

	char szClientVersion[ 25];
	strcpy( szClientVersion, GetEncodedVersion( m_szClientVersion));
	char szLauncherVersion[ 25];
	strcpy( szLauncherVersion, GetEncodedVersion( GUNZLAUNCHER_VERSION));

	CString strURL;
//	strURL.Format( "http://www.gunzonline.com/launcher/start.asp?lver=%s&cver=%s",
//						szLauncherVersion,
//						szClientVersion);

	strURL.Format( "http://202.92.10.151/gunzweb/launcher/start.html");
	m_cExplorer.Navigate( _T( strURL), NULL, NULL, NULL, NULL);
#ifdef _DEBUG
	strURL = "[APP] Open URL : " + strURL;
#else
	strURL = "[APP] Open URL";
#endif
	PutLog( strURL);

	SetTimer( TIMER_OPENWEB, 100, NULL);


	// UI �ʱ�ȭ
	SetWindowText( "Gunz Launcher");

	m_cUpdateInfo.SetWindowText( "Connecting to server...");
	m_cUpdateInfo.MoveWindow( 66, 404, 230, 18);

	m_cPercent1.SetWindowText( "");
	m_cPercent1.MoveWindow( 260, 404, 30, 16);
	m_cPercent1.ShowWindow( SW_HIDE);

	m_cPercent2.SetWindowText( "");
	m_cPercent2.MoveWindow( 260, 421, 30, 16);
	m_cPercent2.ShowWindow( SW_HIDE);

	m_cStartGame.EnableWindow(FALSE);
	m_cStartGame.MoveWindow(371, 405);

	m_cCancelDownload.ShowWindow( SW_SHOW);
	m_cCancelDownload.EnableWindow( false);
	m_cCancelDownload.MoveWindow( 268, 428);


	Rendering();

	return true;
}


// OnDestroy
// ���̾�α� ����
void CNewAppMainDlg::OnDestroy() 
{
	// ���̾�α� ��ġ�� ����
	CRect rect;
	GetWindowRect( rect);
	AfxGetApp()->WriteProfileInt( "Window Position", "x", rect.left);
	AfxGetApp()->WriteProfileInt( "Window Position", "y", rect.top);
}


// OnBnClickedRefresh
void CNewAppMainDlg::OnBnClickedRefresh()
{
	m_cExplorer.Refresh();
}


// OnBnClickedUpdate
void CNewAppMainDlg::OnBnClickedUpdate()
{
	OnUpdateStart();
}


// OnBnClickedStartgame
void CNewAppMainDlg::OnBnClickedStartgame()
{
	m_cStartGame.EnableWindow( false);

	CGLEncription cEncription;
	cEncription.CreateSerialKey( m_nLaunchMode);

	WinExec( GUNZEXE_FILENAME, SW_SHOW);


	SetTimer( TIMER_QUIT, 3000, NULL);
}


// OnUpdateStart
bool CNewAppMainDlg::OnUpdateStart()
{
	// Updater.exe ������ �����.
	BOOL bRet = FALSE;
	int nTryCount = 0;
	WIN32_FIND_DATA FindData;
	while ( !bRet)
	{
		if ( FindFirstFile( _T( "Updater.exe"), &FindData) != INVALID_HANDLE_VALUE)
		{
			bRet = DeleteFile( _T( "Updater.exe"));
			Sleep( 50);
		}
		else
			bRet = TRUE;

		
		if ( nTryCount++ > 40)
			break;
	}

/*
	// Read GunzLauncher.xml
	MXmlDocument	xmlConfig;
	xmlConfig.Create();
	if ( !xmlConfig.LoadFromFile( "gunzlauncher.xml")) 
	{
		PutLog( "[APP] ERROR : Cannot open 'GunzLauncher.xml' file.", LOG_ERROR);

		OnUpdateStop();

		return false;
	}

	MXmlElement		parentElement = xmlConfig.GetDocumentElement();
	MXmlElement		serverElement;
	if ( !parentElement.IsEmpty())
	{
		if (parentElement.FindChildNode( "UPDATESERVER", &serverElement))
		{
			serverElement.GetChildContents( m_szServerIP, "IP");
			serverElement.GetChildContents( &m_nPort, "PORT");
		}
	}
	xmlConfig.Destroy();


	// ZUpdate create
	m_Update.Create( m_szServerIP, m_nPort, "", "");
*/

	// ZUpdate create
	char szUpdateFolderName[ 256];
	strcpy( szUpdateFolderName, "gunzweb/GunzUpdate");

	if ( m_nLaunchMode == 2)
		strcat( szUpdateFolderName, "Test");

	m_Update.Create( _T( "patch.gunzonline.co.in"), 80, szUpdateFolderName, "", "");


	// ����� ������ ����
	m_bThreadContinue = true;
	m_pThreadUpdate = AfxBeginThread( ThreadUpdate, (LPVOID*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	m_pThreadUpdate->ResumeThread();				// Run thread


	// Ÿ�̸� ����
	SetTimer( TIMER_UPDATEUI, 50, NULL);
	SetTimer( TIMER_TIMEOUT, 25000, NULL);


	// UI ����
	m_cCancelDownload.EnableWindow( true);
	m_cCancelDownload.ShowWindow( SW_SHOW);
	m_cStartGame.EnableWindow( false);
	m_cAnibmpRun.ShowWindow( SW_SHOW);


	m_bUpdating = true;

	return true;
}


// OnUpdateStop
void CNewAppMainDlg::OnUpdateStop()
{
	// ZUpdate destroy
	m_Update.Destroy();


	// Ÿ�̸� ����
	KillTimer( TIMER_UPDATEUI);
	KillTimer( TIMER_TIMEOUT);


	// Set variables
	m_bUpdating = false;
}


// OnCompleteUpdate
LRESULT CNewAppMainDlg::OnCompleteUpdate( WPARAM wParam, LPARAM lParam)
{
	// Stop update
	OnUpdateStop();


	// Delete patch file
	DeleteFile( "patch.xml");

	
	// UI ����
	UpdateProgressUI( true);
	m_cStartGame.EnableWindow( false);
	m_cCancelDownload.EnableWindow( false);
	m_cAnibmpRun.ShowWindow( SW_HIDE);


	// Check quit
	if ( m_bQuit)
	{
		CDDBDialog::OnOK();

		return 0;
	}


	// Check skip
	if ( m_nSkip == SKIP_COUNT)
	{
		m_cStartGame.EnableWindow( true);
		m_cUpdateInfo.SetWindowText( "Update canceled.");

		return 0;
	}


	// Complete fail
	if ( !m_Update.IsPatchComplete() || m_Update.GetUpdateInfo().GetPatchFailedCount())
	{
		m_cUpdateInfo.SetWindowText( "Update failed.");

		CNewAppPopupDlg dlgPopup;

		CString str;
//		str = "Update failed. You cannot start GunZ right now.\n";

		if ( (int)strlen( m_Update.GetFileTranferError()))
			str.Format( str+"[Error]\n%s\n", m_Update.GetFileTranferError());
		
		if ( (int)strlen( m_Update.GetLastError()))
			str.Format( str+"[TIP]\n%s", m_Update.GetLastError());

		dlgPopup.SetMessage( str);
		dlgPopup.SetIcon( 1);
		dlgPopup.DoModal();

		// Write to INI info
		AfxGetApp()->WriteProfileInt( "Update", "Count", 0);

		return 0;
	}

	m_cUpdateInfo.SetWindowText( "Update completed.");
	m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 42, 35, 206, 13, SRCCOPY);
	Rendering( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT);

	// Exist updated file
	if ( m_Update.GetUpdateInfo().GetTotalPatchFileCount() > 0)
	{
		// Write to INI info
		AfxGetApp()->WriteProfileInt( "Update", "Count", m_Update.GetUpdateInfo().GetTotalPatchFileCount());


		HRSRC hRes = ::FindResource( AfxGetResourceHandle(), MAKEINTRESOURCE( IDR_UPDATER), _T( "EXE"));
		if ( hRes == NULL)
			return 0;

		HGLOBAL hData = ::LoadResource( AfxGetResourceHandle(), hRes);
		if ( hData == NULL)
			return 0;

		LPVOID lpData= LockResource( hData);

		DWORD dwSize = SizeofResource( AfxGetInstanceHandle(), hRes);
		DWORD dwWriteSize = 0;
		HANDLE hFile = ::CreateFile( "Updater.exe", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if ( hFile != INVALID_HANDLE_VALUE)
		{
			// Make 'Updater.exe' file
			::WriteFile( hFile, lpData, dwSize, &dwWriteSize, NULL);
			::CloseHandle( hFile);


			// Run updater
			if ( dwSize == dwWriteSize)
			{
				PutLog( "[APP] Run updater.");

				WinExec("Updater.exe", SW_HIDE);
			}
		}

		FreeResource( hData);
	}

	// Not exist...
	else
	{
		// Set UI
		m_cStartGame.EnableWindow( true);


		// ������Ʈ�� ���� ��� ����â ǥ��
		if ( AfxGetApp()->GetProfileInt( "Update", "Count", 0) > 0)
		{
			CNewAppPopupDlg dlgPopup;
			CString strMsg;
			strMsg.Format( "Update complete.\n\nClient Version : %s\nLauncher Version : %s",
																m_szClientVersion, GUNZLAUNCHER_VERSION);
			dlgPopup.SetMessage( strMsg);
			dlgPopup.SetIcon( 0);
			dlgPopup.DoModal();
		}

		// Write to INI info
		AfxGetApp()->WriteProfileInt( "Update", "Count", 0);
	}

	return 0;
}


// ThreadUpdate
UINT ThreadUpdate( LPVOID pParam)
{
	// pDlg�� ���ؼ� NewAppDlg Ŭ������ ��� ����/�Լ�(public�� ��쿡��)��
	// ������ �� �ִ�.
	CNewAppMainDlg *pDlg = (CNewAppMainDlg*)pParam;


	// Start update
	pDlg->m_Update.StartUpdate( "./patch.xml");


	// Send message
	pDlg->PostMessage( WM_DESTROYTHREAD, 0, 0);

	return true;
}


// OnTimer
void CNewAppMainDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent == TIMER_STARTUPDATE)
	{
		KillTimer( TIMER_STARTUPDATE);

		PutLog( "[APP] Start update.");

		OnUpdateStart();
	}

	else if ( nIDEvent == TIMER_TIMEOUT)
	{
		KillTimer( TIMER_TIMEOUT);

		PutLog( "[APP] Timeout.");


		m_bTimeOut = true;

		OnBnClickedCanceldownload();
	}

	else if ( nIDEvent == TIMER_UPDATEUI)
	{
		UpdateProgressUI( false);
	}

	else if ( nIDEvent == TIMER_OPENWEB)
	{
		if ( !m_cExplorer.get_Busy())
		{
			KillTimer( TIMER_OPENWEB);

			m_cExplorer.MoveWindow( 1, 16, 598, 360, false);
		}
	}

	else if ( nIDEvent == TIMER_QUIT)
	{
		KillTimer( TIMER_QUIT);

		PutLog( "[APP] Quit app.");

		CDialog::OnOK();
	}

	CDDBDialog::OnTimer(nIDEvent);
}


// OnShowWindow
void CNewAppMainDlg::UpdateProgressUI( bool bClear)
{
	// Clear mode
	if ( bClear)
	{
//		m_cUpdateInfo.ShowWindow( SW_HIDE);
		m_cPercent1.ShowWindow( SW_HIDE);
		m_cPercent2.ShowWindow( SW_HIDE);

		m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 42, 21, 206, 13, SRCCOPY);
		Rendering( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT);

		return;
	}


    // Update screen
	ZUpdateUIInfo UIInfo = m_Update.GetUpdateInfo();
	CString strInfo;

	
	// Show message
	if ( !stricmp( UIInfo.GetCurrPatchFileName(), ""))
	{
		strInfo.Format( "Checking update file info...");
	}
	else if ( !stricmp( UIInfo.GetCurrPatchFileName(), "patch.xml"))
	{
		strInfo.Format( "Downloading patch file...");

		KillTimer( TIMER_TIMEOUT);
	}
	else
	{
		char szFileName[ 128];
		strcpy( szFileName, UIInfo.GetCurrPatchFileName());
		for ( int i = (int)strlen( UIInfo.GetCurrPatchFileName());  i >= 0;  i--)
		{
			char chChar = *(UIInfo.GetCurrPatchFileName() + i);

			if ( chChar == (char)'/')
			{
				strncpy( szFileName, UIInfo.GetCurrPatchFileName() + i + 1, (int)strlen( UIInfo.GetCurrPatchFileName()) - i);
				break;
			}
		}
		strInfo.Format( "Updating (%s , %u/%u)...",
									szFileName,
//									UIInfo.GetTotalDownloadSize() / 1024,
									UIInfo.GetCurrPatchedFileCount() + 1,
									UIInfo.GetTotalPatchFileCount());

		KillTimer( TIMER_TIMEOUT);
	}
	CString strText;
	m_cUpdateInfo.GetWindowText( strText);
	if ( strcmp( strText, strInfo) != 0)
		m_cUpdateInfo.SetWindowText( strInfo);



	// Draw progress bar
	m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 42, 21, 206, 13, SRCCOPY);

	// Progress info
//	int nPercentCurr   = (int)( (float)UIInfo.GetCurrDownloadSize() / (float)UIInfo.GetTotalDownloadSize() * 100.0f + 0.5f);
//	int nPercentTotal1 = (int)( (float)UIInfo.GetCurrPatchedFileSize() / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);
//	int nPercentTotal2 = (int)( (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetCurrDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);
//	int nPercentTotal3 = (int)( (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetTotalDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);
	float fValue = (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetTotalDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f;
	int nWidth = (int)( (float)PROGRESSBAR_WIDTH * fValue / 100.0f + 0.5f);
	if ( m_bBlank == true)
	{
		m_bBlank = false;
		m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 42, 49, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);
	}
	else
		m_bBlank = true;

	fValue = (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetCurrDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f;
	nWidth = (int)( (float)PROGRESSBAR_WIDTH * fValue / 100.0f + 0.5f);
	m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 42, 35, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);

	// Rendering
	Rendering( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, PROGRESSBAR_WIDTH, PROGRESSBAR_HEIGHT);
}


// GetGunzExeVersion
bool CNewAppMainDlg::GetGunzExeVersion()
{
	DWORD dwHandle;
	DWORD dwLength;

	dwLength = GetFileVersionInfoSize( GUNZEXE_FILENAME, &dwHandle);

	if ( !dwLength)
	{
		PutLog( "[APP] ERROR : Cannot get file version info.", LOG_ERROR);
		return false;
	}

	std::auto_ptr<BYTE> lpBlock(new BYTE[dwLength]);
	
	if ( !GetFileVersionInfo( GUNZEXE_FILENAME, dwHandle, dwLength, lpBlock.get()))
	{
		PutLog( "[APP] ERROR : Cannot get file version info.", LOG_ERROR);
		return false;
	}


	LPVOID pVersion = NULL;
	DWORD uLength;
	if ( !VerQueryValue( lpBlock.get(), "\\VarFileInfo\\Translation", (LPVOID*)&pVersion, (UINT*)&uLength))
	{
		PutLog( "[APP] ERROR : Cannot get file version info.", LOG_ERROR);
		return false;
	}


	CString rVersion;
	if ( uLength == 4)
	{
		DWORD langD;
		memcpy( &langD, pVersion, 4);            

		rVersion.Format( "\\StringFileInfo\\%02X%02X%02X%02X\\ProductVersion",
							( langD & 0xff00) >> 8,
							( langD & 0xff),
							( langD & 0xff000000) >> 24,
							( langD & 0xff0000) >> 16);
	}
	else
	{
		rVersion.Format( "\\StringFileInfo\\%04X04B0\\ProductVersion", GetUserDefaultLangID());
	}


	if( VerQueryValue( lpBlock.get(), (LPSTR)(LPCTSTR)rVersion, (LPVOID*)&pVersion, (UINT *)&uLength) == 0 )
	{
		PutLog( "[APP] ERROR : Cannot get file version info.", LOG_ERROR);
		return false;
	}


	// Success
	strcpy( m_szClientVersion, (char*)pVersion);

	return true;
}


// OnShowWindow
void CNewAppMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDDBDialog::OnShowWindow(bShow, nStatus);


	bool bNoUpdate = false;
	char szBuff[ 256], szCommand[ 256], szValue[ 256]; 
	FILE* pFile = fopen( "GunzLauncher.ini", "r");
	if ( pFile)
	{
		while ( 1)
		{
			if ( fgets( szBuff, 256, pFile) == NULL)
				break;

			szBuff[ strlen( szBuff) - 1] = 0;

			if ( strcmp( szBuff, "NO_UPDATE") == 0)											// No update
			{
				bNoUpdate = true;
			}
			else if ( strcmp( szBuff, "ADMIN_LAUNCH") == 0)									// Admin launch
			{
//				m_nLaunchMode = 1;
			}
			else if ( strcmp( szBuff, "TEST_LAUNCH") == 0)									// Admin launch
			{
				m_nLaunchMode = 2;
			}
			else if ( strncmp( szBuff, "SERVER_IP", (int)strlen( "SERVER_IP")) == 0)		// Server IP
			{
				sscanf( szBuff, "%s%s", szCommand, szValue);
			}
			else if ( strncmp( szBuff, "SERVER_PORT", (int)strlen( "SERVER_PORT")) == 0)	// Server port
			{
				sscanf( szBuff, "%s%s", szCommand, szValue);
			}
		}
		fclose( pFile);
	}


	if ( bNoUpdate)
	{
		AfxMessageBox( "Update Cancled.");
		OnUpdateStop();
		m_cStartGame.EnableWindow( true);
	}
	else
		// Ÿ�̸� ����
		SetTimer( TIMER_STARTUPDATE, 500, NULL);
}


// OnBnClickedCanceldownload
void CNewAppMainDlg::OnBnClickedCanceldownload()
{
	m_cCancelDownload.EnableWindow( false);

	AfxGetApp()->WriteProfileInt( "Update", "Count", 0);

	m_Update.StopUpdate();
}


// OnAbout
void CNewAppMainDlg::OnAbout()
{
	CNewAppPopupDlg dlgPopup;
	CString strMsg;
	strMsg.Format( "Gunz the Duel\n\nClient Version : %s\nLauncher Version : %s",
						m_szClientVersion, GUNZLAUNCHER_VERSION);
	dlgPopup.SetMessage( strMsg);
	dlgPopup.SetIcon( 0);
	dlgPopup.DoModal();
}


// OnQuit
void CNewAppMainDlg::OnQuit()
{
	if ( m_bUpdating)
	{
		m_bQuit = true;

		OnBnClickedCanceldownload();
	}
	else
	{
		CloseLog();

		Sleep( 1000);

		CDDBDialog::OnQuit();
	}
}


// OnOK
void CNewAppMainDlg::OnOK()
{
//	CDDBDialog::OnOK();
}


// OnCancel
void CNewAppMainDlg::OnCancel()
{
	OnQuit();
}


// OnNcHitTest
UINT CNewAppMainDlg::OnNcHitTest(CPoint point) 
{
	UINT hit = CDialog::OnNcHitTest( point);

	if ( hit == HTCLIENT)
	{
		CRect rect;
		GetWindowRect( rect);

		// ���� ���콺�� ��ǥ�� �����쿡 ���� �����ǥ�� ��ȯ�Ͽ� ���Ѵ�.
		CPoint pos;
		pos.x = point.x - rect.left;
		pos.y = point.y - rect.top;

		// Skip ���� �˻�
		if ( (pos.x > 48) && (pos.x < 188) && (pos.y > 65) && (pos.y < 390))
			return  hit;
		else
			return  HTCAPTION;
	}
	
	return CDDBDialog::OnNcHitTest(point);
}


// OnLButtonDown
void CNewAppMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( m_bUpdating && ( m_nSkip == SKIP_COUNT))
	{
		OnBnClickedCanceldownload();
		return;
	}

	m_nSkip++;

	CDDBDialog::OnLButtonDown(nFlags, point);
}



/***********************************************************************
  UI ����
************************************************************************/
static int GetX(int x, int y) { return x;}
static int GetY(int x, int y) { return y;}

// DrawDialogWindow
#define IMG_DIALOG			0,   0			// Dialog�� �̹��� ��ǥ �� ũ��
#define SIZE_DIALOG			13,  13
void CNewAppMainDlg::DrawDialogWindow()
{
	CRect rect;
	GetWindowRect( &rect);
	m_cDDB.FillTiledBitmap( 0, 0, rect.Width(), rect.Height(), GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG)+GetY(SIZE_DIALOG)+1, SIZE_DIALOG);
	m_cDDB.FillTiledBitmap( 0, 0, GetX(SIZE_DIALOG), rect.Height(), GetSkinBmp(), GetX(IMG_DIALOG), GetY(IMG_DIALOG)+GetY(SIZE_DIALOG)+1, SIZE_DIALOG);
	m_cDDB.FillTiledBitmap( rect.Width()-GetX(SIZE_DIALOG), 0, GetX(SIZE_DIALOG), rect.Height(), GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG)+GetY(SIZE_DIALOG)+1, SIZE_DIALOG);
	m_cDDB.FillTiledBitmap( 0, 0, rect.Width(), GetY(SIZE_DIALOG), GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG), SIZE_DIALOG);
	m_cDDB.FillTiledBitmap( 0, rect.Height()-GetY(SIZE_DIALOG), rect.Width(), GetY(SIZE_DIALOG), GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG)+GetY(SIZE_DIALOG)+1+GetY(SIZE_DIALOG)+1, SIZE_DIALOG);
	m_cDDB.PutBitmap( 0, 0, GetSkinBmp(), GetX(IMG_DIALOG), GetY(IMG_DIALOG), SIZE_DIALOG, SRCCOPY);
	m_cDDB.PutBitmap( rect.Width()-GetX(SIZE_DIALOG), 0, GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG), SIZE_DIALOG, SRCCOPY);
	m_cDDB.PutBitmap( 0, rect.Height()-GetY(SIZE_DIALOG), GetSkinBmp(), GetX(IMG_DIALOG), GetY(IMG_DIALOG)+GetY(SIZE_DIALOG)+1+GetY(SIZE_DIALOG)+1, SIZE_DIALOG, SRCCOPY);
	m_cDDB.PutBitmap( rect.Width()-GetX(SIZE_DIALOG), rect.Height()-GetY(SIZE_DIALOG), GetSkinBmp(), GetX(IMG_DIALOG)+GetX(SIZE_DIALOG)+1+GetX(SIZE_DIALOG)+1, GetY(IMG_DIALOG)+GetX(SIZE_DIALOG)+1+GetY(SIZE_DIALOG)+1, SIZE_DIALOG, SRCCOPY);
}


// DrawPanel
#define IMG_PANEL			42,  0			// Panel�� �̹��� ��ǥ �� ũ��
#define SIZE_PANEL			47,  20
void CNewAppMainDlg::DrawPanel( int x, int y, int width, int height, LPCTSTR lpszTitle, UINT nIconNum, UINT nIDMenuResource, UINT nSubMenu)
{
	m_cDDB.FillTiledBitmap( x, y, width, GetY(SIZE_PANEL), GetSkinBmp(), GetX(IMG_PANEL)+GetX(SIZE_PANEL)+1, GetY(IMG_PANEL), SIZE_PANEL);

	Rendering( x, y, width, height);
}


// CreateQuitButton
#define IMG_QUITBUTTONMAIN		154, 0			// Quit Button�� �̹��� ��ǥ �� ũ��
#define SIZE_QUITBUTTONMAIN		13,  12
void CNewAppMainDlg::CreateQuitButton( int x, int y)
{
	m_cButtonQuit.Create( _T( ""),
			                WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                CRect( x, y, x+GetX(SIZE_QUITBUTTONMAIN), y+GetY(SIZE_QUITBUTTONMAIN)),
							this, IDC_QUIT);
	m_cButtonQuit.InitDDBButton( GetSkinBmp(), IMG_QUITBUTTONMAIN, SIZE_QUITBUTTONMAIN, NULL);
}


// CreateAboutButton
#define IMG_ABOUTBUTTONMAIN		210, 0			// About Button�� �̹��� ��ǥ �� ũ��
#define SIZE_ABOUTBUTTONMAIN	13,  12
void CNewAppMainDlg::CreateAboutButton( int x, int y)
{
	m_cButtonAbout.Create( _T( ""),
			                WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                CRect( x, y, x+GetX(SIZE_ABOUTBUTTONMAIN), y+GetY(SIZE_ABOUTBUTTONMAIN)),
							this, IDC_ABOUTBOX);
	m_cButtonAbout.InitDDBButton( GetSkinBmp(), IMG_ABOUTBUTTONMAIN, SIZE_ABOUTBUTTONMAIN, NULL);
}
