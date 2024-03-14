/*
 CNewAppMainDlg Ŭ����(NewAppMainDlg.cpp)

  desc : Main Dialog ���� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "NewAppMainDlg.h"
#include "CGLEncription.h"


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
	DlgInfo.nWidth = 405;
	DlgInfo.nHeight = 349;
	DlgInfo.IDDlgSkinBmpResource = IDB_SKIN_MAINDLG;
	DlgInfo.IDDlgIconResource = IDR_ICON_MAINFRAME;
	DlgInfo.bEnableDlgMove = true;
	DlgInfo.nTextColor = RGB( 255, 255, 255);
	DlgInfo.nTextBkColor = RGB( 0, 0, 0);
	SetDialogInfo( DlgInfo);
}


// DoDataExchange
void CNewAppMainDlg::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppMainDlg)
	DDX_Control(pDX, IDC_STARTGAME,			m_cStartGame);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppMainDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppMainDlg)
	ON_BN_CLICKED(IDC_STARTGAME, OnBnClickedStartgame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  CNewAppMainDlg message handlers
*************************************************************************************/
// OnInitDialog
// ���̾�α� �ʱ�ȭ
BOOL CNewAppMainDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


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
	m_cStartGame.InitDDBButton( IDB_BTNSKIN_STARTGAME, NULL);
	m_cStartGame.MoveWindow( 331, 11);


	return true;
}


// OnBnClickedStartgame
void CNewAppMainDlg::OnBnClickedStartgame()
{
	CGLEncription cEncription;
	cEncription.CreateSerialKey();


	WinExec("Gunz.exe", SW_SHOW);

	CDialog::OnOK();
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
	m_cDDB.PutBitmap( 0, 0, GetSkinBmp(), 0, 0, 405, 349, SRCCOPY);
}
