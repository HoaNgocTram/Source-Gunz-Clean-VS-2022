/*
 CNewAppPopupDlg Ŭ����(NewAppPopupDlg.cpp)

  desc : Popup Dialog ���� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "NewAppPopupDlg.h"


/************************************************************************************
  CNewAppPopupDlg dialog
*************************************************************************************/
// CNewAppPopupDlg
CNewAppPopupDlg::CNewAppPopupDlg(CWnd* pParent /*=NULL*/)
	: CDDBDialog(CNewAppPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAppPopupDlg)
	//}}AFX_DATA_INIT

	// Set dialog information : �̰����� ������ ���̾�α��� ������ �Է��Ѵ�.
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�.
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth = 306;
	DlgInfo.nHeight = 236;
	DlgInfo.IDDlgSkinBmpResource = IDB_SKIN_POPUP;
	DlgInfo.nTextColor = RGB( 170, 170, 170);
	DlgInfo.nTextBkColor = RGB( 0, 0, 0);
	SetDialogInfo( DlgInfo);


	// ���� �ʱ�ȭ
	m_szMessage[ 0] = 0;
	m_nIcon = 0;
}


// DoDataExchange
void CNewAppPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppPopupDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MESSAGE, m_cMessage);
	DDX_Control(pDX, IDOK, m_cOK);
}


// Message map
BEGIN_MESSAGE_MAP(CNewAppPopupDlg, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppPopupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  CNewAppPopupDlg message handlers
*************************************************************************************/
// OnInitDialog
BOOL CNewAppPopupDlg::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();

	
	// ��ư ����
	m_cOK.InitDDBButton( IDB_BTNSKIN_NORMAL, NULL);
	m_cOK.MoveWindow( 106, 186);


	// UI ����
	m_cMessage.SetWindowText( (LPCTSTR)m_szMessage);
	m_cMessage.MoveWindow( 63, 25, 233, 160);

	m_cDDB.PutBitmap( 8, 27, GetSkinBmp(), m_nIcon * 49, 237, 48, 73, SRCCOPY);

	
	// ���� ���
//	PlayEffectSound( IDR_WAVE_POPUP);

	return TRUE;
}


// DrawDialogWindow
void CNewAppPopupDlg::DrawDialogWindow()
{
	CRect rect;
	GetWindowRect( &rect);
	m_cDDB.PutBitmap( 0, 0, GetSkinBmp(), 0, 0, rect.Width(), rect.Height(), SRCCOPY);
}


// SetMessage
void CNewAppPopupDlg::SetMessage( const char* szMessage)
{
	strcpy( m_szMessage, szMessage);
}


// SetIcon
void CNewAppPopupDlg::SetIcon( int nIconNum)
{
	m_nIcon = nIconNum;
}
