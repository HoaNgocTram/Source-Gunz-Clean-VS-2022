
#include "stdafx.h"
#include "NewAppWarnVista.h"



/************************************************************************************
  Dialog
*************************************************************************************/
// Ŭ���� ������
CNewAppWarnVista::CNewAppWarnVista( CWnd* pParent) : CDDBDialog( IDD_WARNVISTA, pParent)
{
	//{{AFX_DATA_INIT( CNewAppWarnVista)
	//}}AFX_DATA_INIT

	// �̰����� ������ ���̾�α��� ������ �Է��Ѵ�
	// �� �� �ڼ��� ������ ���ϸ� CDDBDialog.h ȭ���� Structure of CDDBDIALOGINFO ������ �����Ѵ�
	CDDBDLGINFO DlgInfo;
	DlgInfo.nWidth = 380;
	DlgInfo.nHeight = 230;
	DlgInfo.IDSkinResource = IDR_BMP_POPUPDLG;
	DlgInfo.nTextColor = RGB( 120, 30, 30);
	DlgInfo.nTextBkColor = RGB( 166, 156, 153);
#ifdef LOCALE_NHNUSA
	DlgInfo.nTextColor = RGB( 150, 150, 150);
	DlgInfo.nTextBkColor = RGB( 46, 47, 48);
#endif
	SetDialogInfo( DlgInfo);


	// ���� �ʱ�ȭ
	m_bCheck = false;
}


// DoDataExchange
void CNewAppWarnVista::DoDataExchange( CDataExchange* pDX)
{
	CDDBDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAppWarnVista)
	DDX_Control(pDX, IDC_MESSAGE, m_cMessage);
	DDX_Control(pDX, IDC_CHECK,	  m_cCheck);
	DDX_Control(pDX, IDOK,        m_cOK);
	DDX_Control(pDX, IDCANCEL,    m_cCancel);
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP( CNewAppWarnVista, CDDBDialog)
	//{{AFX_MSG_MAP(CNewAppWarnVista)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  Ŭ���� �޼��� �ڵ鷯
*************************************************************************************/
// OnInitDialog : ���̾�α׸� �ʱ�ȭ�� �Լ�
BOOL CNewAppWarnVista::OnInitDialog() 
{
	CDDBDialog::OnInitDialog();


	// ������ �̵�
	CRect rect;
	GetWindowRect( &rect);
	int x = rect.left + rect.Width() / 2 - GetWidth() / 2;
	int y = rect.top + rect.Height() / 2 - GetHeight() / 2;
	MoveWindow( x, y, GetWidth(), GetHeight());
	

	// �г� �׸���
	DrawPanel( 6, 25, GetWidth()-12, GetHeight()-31);


	// ��ư �ʱ�ȭ
	m_cOK.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);
	m_cCancel.InitDDBButton( IDR_BMP_BUTTON, IDR_WAV_PUSHBUTTON);


	// ĸ�� ǥ��
	SetWindowText( m_strCaption);
	GetScreen()->PutText(  9, 5, 17, "Arial", m_strCaption, RGB( 0, 0, 0), TS_BOLD);
	GetScreen()->PutText( 10, 6, 17, "Arial", m_strCaption, RGB( 238, 156, 0), TS_BOLD);


	// �޽��� ǥ��
	m_cMessage.SetWindowText( m_strMessage);


	// üũ�ڽ� ����
	m_cCheck.MoveWindow( 25, GetHeight() - 70, 15, 15);
	GetScreen()->PutText( 42, GetHeight() - 70, 15, "Arial", "Do not display this message again.", GetTextColor());


	// ��ư �ʱ�ȭ
	if ( m_nButtonType == MB_OKCANCEL)
	{
		m_cOK.SetWindowText( "OK");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() - 5, GetHeight() - 35);

		m_cCancel.SetWindowText( "Cancel");
		m_cCancel.ShowWindow( SW_SHOW);
		m_cCancel.EnableWindow( TRUE);
		m_cCancel.MoveWindow( GetWidth() / 2 + 5, GetHeight() - 35);
	}
	else if ( m_nButtonType == MB_YESNO)
	{
		m_cOK.SetWindowText( "Yes");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() - 5, GetHeight() - 35);

		m_cCancel.SetWindowText( "No");
		m_cCancel.ShowWindow( SW_SHOW);
		m_cCancel.EnableWindow( TRUE);
		m_cCancel.MoveWindow( GetWidth() / 2 + 5, GetHeight() - 35);
	}
	else if ( m_nButtonType == MB_OK)
	{
		m_cOK.SetWindowText( "OK");
		m_cOK.ShowWindow( SW_SHOW);
		m_cOK.EnableWindow( TRUE);
		m_cOK.MoveWindow( GetWidth() / 2 - m_cOK.GetWidth() / 2, GetHeight() - 35);

		m_cCancel.ShowWindow( SW_HIDE);
	}


	return true;
}


// SetDialog
void CNewAppWarnVista::SetDialog( CString strCaption, CString strMessage, UINT nButtonType)
{
	m_strCaption = strCaption;
	m_strMessage = strMessage;
	m_nButtonType = nButtonType;
}


// OK
void CNewAppWarnVista::OnOK()
{
	if ( m_cCheck.GetCheck() > 0)
		m_bCheck = true;
	else
		m_bCheck = false;

	CDDBDialog::OnOK();
}
