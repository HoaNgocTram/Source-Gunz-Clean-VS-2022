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
#include "Global.h"
#include "NewAppPopupDlg.h"


// Functions
UINT ThreadUpdate( LPVOID pParam);


#define TIMER_STARTUPDATE		0
#define TIMER_TIMEOUT			1
#define TIMER_UPDATEUI			2
#define TIMER_OPENWEB			3
#define TIMER_QUIT				4

#define WM_DESTROYTHREAD		(WM_USER + 50)			// Message of destroy thread

#define SKIP_COUNT				3


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
	DlgInfo.nWidth = 602;
	DlgInfo.nHeight = 462;
	DlgInfo.IDDlgSkinBmpResource = IDB_SKIN_MAINDLG;
	DlgInfo.IDDlgIconResource = IDR_ICON_MAINFRAME;
	DlgInfo.bEnableDlgMove = true;
	DlgInfo.nTextColor = RGB( 120, 120, 120);
	DlgInfo.nTextBkColor = RGB( 238, 235, 229);
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

		if ( chChar != (char)',')
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
	ClearDebugMSG();


	// ���̾�α� ��ġ �̵� �� ũ�� ����
	CRect rect; 
	GetWindowRect( &rect);
	int nWidth = rect.Width(), nHeight = rect.Height();
	rect.left = AfxGetApp()->GetProfileInt( "Window Position", "x", 50);
	rect.top  = AfxGetApp()->GetProfileInt( "Window Position", "y", 50);
	rect.right  = rect.left + nWidth;
	rect.bottom = rect.top  + nHeight;
	MoveWindow( rect, true);

	
	// ��ư �ʱ�ȭ
	CreateQuitButton( rect.Width() - 18, 3);			// Quit ��ư ����
	CreateAboutButton( rect.Width() - 35, 3);			// About ��ư ����
	m_cStartGame.InitDDBButton( IDB_BTNSKIN_STARTGAME, IDR_WAVE_BUTTON);
	m_cCancelDownload.InitDDBButton( IDB_BTNSKIN_STOP, NULL);

	
	// Ani bitmap �ʱ�ȭ
	m_cAnibmpRun.InitDDBAniBmp( IDB_ANIBMP_RUN, 30);
	m_cAnibmpRun.SetTimerValue( 50);
	m_cAnibmpRun.EnableAnimate( true);
	m_cAnibmpRun.ShowWindow( SW_HIDE);

	
	// ���ͳ� Ž����â �ʱ�ȭ
	m_cExplorer.MoveWindow( 335, 58, 0, 0, false);

	GetGunzExeVersion();
	char szClientVersion[ 25];
	strcpy( szClientVersion, GetEncodedVersion( m_szClientVersion));
	char szLauncherVersion[ 25];
	strcpy( szLauncherVersion, GetEncodedVersion( GUNZLAUNCHER_VERSION));

	CString strURL;
	strURL.Format( "http://www.gunzonline.com/launcher/start.asp?lver=%s&cver=%s",
						szLauncherVersion,
						szClientVersion);

	m_cExplorer.Navigate( _T( strURL), NULL, NULL, NULL, NULL);
	strURL = "[APP] Open URL : " + strURL;
	PutDebugMSG( strURL);

	SetTimer( TIMER_OPENWEB, 100, NULL);


	// UI �ʱ�ȭ
	SetWindowText( "Gunz Launcher");
	m_cUpdateInfo.SetWindowText( "");
	m_cPercent1.SetWindowText( "");
	m_cPercent2.SetWindowText( "");
	m_cStartGame.ShowWindow( SW_HIDE);
	m_cCancelDownload.ShowWindow( SW_HIDE);
	m_cCancelDownload.EnableWindow( false);


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
	AfxGetApp()->WriteProfileString( "Version", "Client", m_szClientVersion);
	AfxGetApp()->WriteProfileString( "Version", "Launcher", GUNZLAUNCHER_VERSION);
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
	WinExec("Gunz.exe /internationaledt", SW_SHOW);

	m_cStartGame.EnableWindow( false);

	SetTimer( TIMER_QUIT, 3000, NULL);
}


// OnUpdateStart
bool CNewAppMainDlg::OnUpdateStart()
{
/*
	// Read GunzLauncher.xml
	MXmlDocument	xmlConfig;
	xmlConfig.Create();
	if ( !xmlConfig.LoadFromFile( "gunzlauncher.xml")) 
	{
		PutDebugMSG( "[APP] ERROR : Cannot open 'GunzLauncher.xml' file.");

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
	m_Update.Create( _T( "maiet.nefficient.com"), 21, "maiet/gunzupdate", "", "");


	// ����� ������ ����
	m_bThreadContinue = true;
	m_pThreadUpdate = AfxBeginThread( ThreadUpdate, (LPVOID*)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	m_pThreadUpdate->ResumeThread();				// Run thread


	// Ÿ�̸� ����
	SetTimer( TIMER_UPDATEUI, 250, NULL);
	SetTimer( TIMER_TIMEOUT, 15000, NULL);


	// UI ����
	m_cCancelDownload.EnableWindow( true);
	m_cCancelDownload.ShowWindow( SW_SHOW);
	m_cStartGame.ShowWindow( SW_HIDE);
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


	// UI ����
	UpdateProgressUI( true);

	m_cStartGame.ShowWindow( true);
	m_cCancelDownload.ShowWindow( SW_HIDE);
	m_cAnibmpRun.ShowWindow( SW_HIDE);

	bool bUpdateSuccess = (bool)( m_Update.IsPatchComplete() && !m_Update.GetUpdateInfo().GetPatchFailedCount());

	if ( m_nSkip == SKIP_COUNT)
		bUpdateSuccess = true;

	if ( bUpdateSuccess)
	{
		GetGunzExeVersion();

		m_cStartGame.EnableWindow( true);

		// Ŭ���̾�Ʈ�� ������ �ٲ���� ���...
		if ( strcmp( AfxGetApp()->GetProfileString( "Version", "Client", ""), m_szClientVersion) != 0)
		{
			CNewAppPopupDlg dlgPopup;
			CString strMsg;
			strMsg.Format( "Client update complete.\n\nClient Version : %s\nLauncher Version : %s",
																m_szClientVersion,GUNZLAUNCHER_VERSION);
			dlgPopup.SetMessage( strMsg);
			dlgPopup.SetIcon( 0);
			dlgPopup.DoModal();
		}
		else
		{
			PlayEffectSound( IDR_WAVE_OK);
		}
	}
	else
		m_cStartGame.EnableWindow( false);
}


// OnCompleteUpdate
LRESULT CNewAppMainDlg::OnCompleteUpdate( WPARAM wParam, LPARAM lParam)
{
	OnUpdateStop();

	// Check skip
	if ( m_nSkip == SKIP_COUNT)
	{
		// ���� ���
		PlayEffectSound( IDR_WAVE_OK);
		
		return 0;
	}


	// Check quit
	if ( m_bQuit)
		CDDBDialog::OnOK();
	
	else
	{
		// ���� �˾�â ����
		bool bUpdateSuccess = (bool)( m_Update.IsPatchComplete() && !m_Update.GetUpdateInfo().GetPatchFailedCount());
		if ( !bUpdateSuccess)
		{
			CNewAppPopupDlg dlgPopup;
			dlgPopup.SetMessage( "WARNNING!\n\nYou cannot run the GunZ.");
			dlgPopup.SetIcon( 1);
			dlgPopup.DoModal();
		}
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
	pDlg->m_Update.StartUpdate( "patch.xml");


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

		PutDebugMSG( "[APP] Start update.");

		OnUpdateStart();
	}

	else if ( nIDEvent == TIMER_TIMEOUT)
	{
		KillTimer( TIMER_TIMEOUT);

		PutDebugMSG( "[APP] Timeout.");

		OnBnClickedCanceldownload();
	}

	else if ( nIDEvent == TIMER_UPDATEUI)
	{
		UpdateProgressUI( false);
	}

	else if ( nIDEvent == TIMER_OPENWEB)
	{
		if ( !m_cExplorer.get_Busy())
			m_cExplorer.MoveWindow( 335, 58, 240, 293, false);
		else
			m_cExplorer.MoveWindow( 335, 58, 0, 0, false);
	}

	else if ( nIDEvent == TIMER_QUIT)
	{
		KillTimer( TIMER_QUIT);

		PutDebugMSG( "[APP] Quit app.");

		CDialog::OnOK();
	}

	CDDBDialog::OnTimer(nIDEvent);
}


// OnShowWindow
#define PROGRESSBAR1_POSX	266
#define PROGRESSBAR1_POSY	382
#define PROGRESSBAR2_POSX	266
#define PROGRESSBAR2_POSY	403
#define PROGRESSBAR_WIDTH	173
#define PROGRESSBAR_HEIGHT	10
void CNewAppMainDlg::UpdateProgressUI( bool bClear)
{
	// Clear mode
	if ( bClear)
	{
		m_cUpdateInfo.ShowWindow( SW_HIDE);
		m_cPercent1.ShowWindow( SW_HIDE);
		m_cPercent2.ShowWindow( SW_HIDE);
	
		m_cCancelDownload.ShowWindow( SW_HIDE);

		m_cDDB.PutFillBox( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY-8, PROGRESSBAR_WIDTH, 39, RGB(238, 235, 229));
		Rendering( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY-8, PROGRESSBAR_WIDTH, 39);

		if ( m_Update.IsPatchComplete() && !m_Update.GetUpdateInfo().GetPatchFailedCount())
			m_cDDB.PutBitmap( 325, 376, GetSkinBmp(), 404, 463, 175, 23, SRCCOPY);
		else
			m_cDDB.PutFillBox( 325, 376, 175, 23, RGB( 238, 235, 229));

		Rendering( 325, 376, 175, 23);
	
		return;
	}


    // Update screen
	ZUpdateUIInfo UIInfo = m_Update.GetUpdateInfo();
	CString strInfo;

	int nPercentCurr   = (int)( (float)UIInfo.GetCurrDownloadSize() / (float)UIInfo.GetTotalDownloadSize() * 100.0f + 0.5f);
	int nPercentTotal1 = (int)( (float)UIInfo.GetCurrPatchedFileSize() / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);
	int nPercentTotal2 = (int)( (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetCurrDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);
	int nPercentTotal3 = (int)( (float)(UIInfo.GetCurrPatchedFileSize() + UIInfo.GetTotalDownloadSize()) / (float)UIInfo.GetTotalPatchFileSize() * 100.0f + 0.5f);

	
	// Show message
	if ( !stricmp( UIInfo.GetCurrPatchFileName(), ""))
	{
		strInfo.Format( "Checking update file info...");
	}
	else if ( !stricmp( UIInfo.GetCurrPatchFileName(), "patch.xml"))
	{
		strInfo.Format( "Downloading patch file...");

		nPercentCurr   = 0;
		nPercentTotal1 = 0;
		nPercentTotal2 = 0;
		nPercentTotal3 = 0;

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
	m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY-8, GetSkinBmp(), 56, 463, PROGRESSBAR_WIDTH, 39, SRCCOPY);

	int nWidth = (int)( (float)PROGRESSBAR_WIDTH * (float)nPercentCurr / 100.0f + 0.5);
	m_cDDB.PutBitmap( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY, GetSkinBmp(), 230, 485, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);

//	nWidth = (int)( (float)PROGRESSBAR_WIDTH * (float)nPercentTotal3 / 100.0f + 0.5);
//	m_cDDB.PutBitmap( PROGRESSBAR2_POSX, PROGRESSBAR2_POSY, GetSkinBmp(), 230, 463, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);

	nWidth = (int)( (float)PROGRESSBAR_WIDTH * (float)nPercentTotal2 / 100.0f + 0.5);
	m_cDDB.PutBitmap( PROGRESSBAR2_POSX, PROGRESSBAR2_POSY, GetSkinBmp(), 230, 474, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);

	nWidth = (int)( (float)PROGRESSBAR_WIDTH * (float)nPercentTotal1 / 100.0f + 0.5);
	m_cDDB.PutBitmap( PROGRESSBAR2_POSX, PROGRESSBAR2_POSY, GetSkinBmp(), 230, 485, nWidth, PROGRESSBAR_HEIGHT, SRCCOPY);


	// Show percentage
	strInfo.Format( "%d%%", nPercentCurr);
	m_cPercent1.SetWindowText( strInfo);

	strInfo.Format( "%d%%", nPercentTotal2);
	m_cPercent2.SetWindowText( strInfo);


	// Rendering
	Rendering( PROGRESSBAR1_POSX, PROGRESSBAR1_POSY-8, PROGRESSBAR_WIDTH, 39);
}


// GetGunzExeVersion
#define GUNZEXE_FILENAME	"Gunz.exe"
bool CNewAppMainDlg::GetGunzExeVersion()
{
	DWORD dwHandle;
	DWORD dwLength;

	dwLength = GetFileVersionInfoSize( GUNZEXE_FILENAME, &dwHandle);

	if ( !dwLength)
	{
		PutDebugMSG( "[APP] ERROR : Cannot get file version into size.");
		return false;
	}

	LPBYTE lpBlock = new BYTE[ dwLength];
	if ( !GetFileVersionInfo( GUNZEXE_FILENAME, dwHandle, dwLength, lpBlock))
	{
		PutDebugMSG( "[APP] ERROR : Cannot get file version info.");
		return false;
	}


	LPVOID pVersion = NULL;
	DWORD uLength;
	if ( !VerQueryValue( lpBlock, "\\VarFileInfo\\Translation", (LPVOID*)&pVersion, (UINT*)&uLength))
	{
		PutDebugMSG( "[APP] ERROR : Cannot get file version info.");
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


	if( VerQueryValue( lpBlock, (LPSTR)(LPCTSTR)rVersion, (LPVOID*)&pVersion, (UINT *)&uLength) == 0 )
	{
		PutDebugMSG( "[APP] ERROR : Cannot get file version info.");
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


	// ���� ���
	PlayEffectSound( IDR_WAVE_POPUP);


	// Ÿ�̸� ����
	SetTimer( TIMER_STARTUPDATE, 100, NULL);
}


// OnBnClickedCanceldownload
void CNewAppMainDlg::OnBnClickedCanceldownload()
{
	m_cCancelDownload.EnableWindow( false);

	m_Update.StopUpdate();
}


// OnAbout
void CNewAppMainDlg::OnAbout()
{
	CNewAppPopupDlg dlgPopup;
	CString strMsg;
	strMsg.Format( "GunZ the Dual - International Edition\nMAIET Entertainment, Inc.\n\nClient Version : %s\nLauncher Version : %s",
						m_szClientVersion, GUNZLAUNCHER_VERSION);
	dlgPopup.SetMessage( strMsg);
	dlgPopup.SetIcon( 0);
	dlgPopup.DoModal();
}


// OnQuit
void CNewAppMainDlg::OnQuit()
{
	// ���� ���
	PlayEffectSound( IDR_WAVE_BUTTON);


	if ( m_bUpdating)
	{
		m_bQuit = true;

		OnBnClickedCanceldownload();
	}
	else
	{
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
		if ( (pos.x > 184) && (pos.x < 195) && (pos.y > 224) && (pos.y < 233))
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

// DrawDialogWindow
void CNewAppMainDlg::DrawDialogWindow()
{
	CRect rect;
	GetWindowRect( &rect);
	m_cDDB.PutBitmap( 0, 0, GetSkinBmp(), 0, 0, rect.Width(), rect.Height(), SRCCOPY);
}


// CreateQuitButton
static int GetX(int x, int y) { return x;}
static int GetY(int x, int y) { return y;}
#define IMG_QUITBUTTONMAIN		0,  463			// Quit Button�� �̹��� ��ǥ �� ũ��
#define SIZE_QUITBUTTONMAIN		13, 12
void CNewAppMainDlg::CreateQuitButton( int x, int y)
{
	m_cButtonQuit.Create( _T( ""),
			                WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                CRect( x, y, x+GetX(SIZE_QUITBUTTONMAIN), y+GetY(SIZE_QUITBUTTONMAIN)),
							this, IDC_QUIT);
	m_cButtonQuit.InitDDBButton( GetSkinBmp(), IMG_QUITBUTTONMAIN, SIZE_QUITBUTTONMAIN, NULL);
}


// CreateAboutButton
#define IMG_ABOUTBUTTONMAIN		0,  476			// About Button�� �̹��� ��ǥ �� ũ��
#define SIZE_ABOUTBUTTONMAIN	13, 12
void CNewAppMainDlg::CreateAboutButton( int x, int y)
{
	m_cButtonAbout.Create( _T( ""),
			                WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                CRect( x, y, x+GetX(SIZE_ABOUTBUTTONMAIN), y+GetY(SIZE_ABOUTBUTTONMAIN)),
							this, IDC_ABOUTBOX);
	m_cButtonAbout.InitDDBButton( GetSkinBmp(), IMG_ABOUTBUTTONMAIN, SIZE_ABOUTBUTTONMAIN, NULL);
}
