/*
 CDDBDialog Ŭ����(CDDBDialog.cpp)

  desc : Dialog�� �׷��ִ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "CDDBDialog.h"


// Functions
static int GetX(int x, int y) { return x;}
static int GetY(int x, int y) { return y;}



/********************************************************************
 Skin map
*********************************************************************/
// Dialog�� �̹��� ��ǥ �� ũ��
#define IMG_DIALOGDEFAULT						0,   0
#define SIZE_DIALOGDEFAULT						20,  20

// Dialog titlebar�� �̹��� ��ǥ �� ũ��
#define IMG_TITLEBARDEFAULT						126, 0
#define SIZE_TITLEBARDEFAULT					21,  21

// Dialog bitmap title�� �̹��� ��ǥ �� ũ��
#define IMG_BMPTITLEDEFAULT						126, 40
#define SIZE_BMPTITLEDEFAULT					290, 20

// Panel�� �̹��� ��ǥ �� ũ��
#define IMG_PANELDEFAULT						63,  0
#define SIZE_PANELDEFAULT						20,  20

// Panel title bar�� �̹��� ��ǥ �� ũ��
#define IMG_PANELTITLEBARDEFAULT				126, 22
#define SIZE_PANELTITLEBARDEFAULT				17,  17

// Panel icons�� �̹��� ��ǥ �� ũ��
#define IMG_PANELICONDEFAULT					0,   63
#define SIZE_PANELICONDEFAULT					84,  80

// About button�� �̹��� ��ǥ �� ũ��
#define IMG_ABOUTBUTTONDEFAULT					192, 0
#define SIZE_ABOUTBUTTONDEFAULT					17,  17

// Minimize button�� �̹��� ��ǥ �� ũ��
#define IMG_MINIMIZEBUTTONDEFAULT				264, 0
#define SIZE_MINIMIZEBUTTONDEFAULT				17,  17

// Maximize button�� �̹��� ��ǥ �� ũ��
#define IMG_MAXIMIZEBUTTONDEFAULT				192, 18
#define SIZE_MAXIMIZEBUTTONDEFAULT				17,  17

// Quit button�� �̹��� ��ǥ �� ũ��
#define IMG_QUITBUTTONDEFAULT					264, 18
#define SIZE_QUITBUTTONDEFAULT					17,  17

// Menu button�� �̹��� ��ǥ �� ũ��
#define IMG_MENUBUTTONDEFAULT					336, 0
#define SIZE_MENUBUTTONDEFAULT					26,  28


/************************************************************************************
  CDDBDialog
*************************************************************************************/
// CDDBDialog
CDDBDialog::CDDBDialog( UINT nIDTemplate, CWnd* pParentWnd)
{
	m_pParentWnd = pParentWnd;
	m_lpszTemplateName = MAKEINTRESOURCE( nIDTemplate);
	m_nIDHelp = nIDTemplate;

	m_listPanelMenu.RemoveAll();
	m_nNumOfMenuButton = 0;

	m_fMouseLButton = false;
	m_fShowWindow = true;
	m_fTrayicon = false;
}


// ~CDDBDialog
CDDBDialog::~CDDBDialog()
{
	// Delete DC
	m_cDDB.DeleteDDB();
	m_ddbRendering.DeleteDDB();
	m_ddbSkin.DeleteDDB();

	// Delete trayicon
	DeleteTrayicon();
}


// DoDataExchange
void CDDBDialog::DoDataExchange( CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDDBDialog)
	//}}AFX_DATA_MAP
}


// Message map
BEGIN_MESSAGE_MAP(CDDBDialog, CDialog)
	//{{AFX_MSG_MAP(CDDBDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_ABOUTBOX, OnAboutbox)
	ON_BN_CLICKED(IDC_MAXIMIZE, OnMaximize)
	ON_BN_CLICKED(IDC_MINIMIZE, OnMinimize)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_COMMAND_RANGE(IDC_MENUBUTTON, IDC_MENUBUTTONMAX, OnMenuButton)
	ON_MESSAGE(WM_MOUSEDRAG, OnMouseDrag)
	ON_MESSAGE(WM_TRAYICON_NOTIFY, OnTrayNotification)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  CDDBDialog message handlers
*************************************************************************************/
// OnInitDialog
BOOL CDDBDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if ( m_DlgInfo.IDDlgSkinBmpResource == NULL)
	{
		MessageBox( "���̾�α׿� ����� ��Ų�� �������� �ʾҽ��ϴ�.\n��Ų ���ҽ��� �߰��� �ֽñ� �ٶ��ϴ�.",
			        "ERROR", MB_OK);
		return false;
	}

	// Create skin m_cDDB
	if ( ! m_ddbSkin.CreateDDB( m_cDDB.GetDC(), m_DlgInfo.IDDlgSkinBmpResource))
	{
		MessageBox( "��Ų ��Ʈ���� �ε��� �� �����ϴ�. m_cDDB�� ������ �� �����ϴ�.", "ERROR", MB_OK);
		return false;
	}

	// Create rendering m_cDDB
	if ( m_DlgInfo.bEnableRenderingLayer)
	{
		if ( ! m_ddbRendering.CreateDDB( 0, 0, GetDC(), m_DlgInfo.nWidth, m_DlgInfo.nHeight, 0))
		{
			MessageBox( "�޸𸮰� �����մϴ�. m_cDDB�� ������ �� �����ϴ�.", "ERROR", MB_OK);
			return false;
		}
	}

	// Create drawing m_cDDB
	if ( ! m_cDDB.CreateDDB( 0, 0, GetDC(), m_DlgInfo.nWidth, m_DlgInfo.nHeight, 0))
	{
		MessageBox( "�޸𸮰� �����մϴ�. m_cDDB�� ������ �� �����ϴ�.", "ERROR", MB_OK);
		return false;
	}

	// ������ ��Ÿ�� ����
	ModifyStyle( WS_CAPTION, WS_MINIMIZEBOX | WS_POPUP | WS_CLIPCHILDREN | WS_SYSMENU);

	// ������ ��ġ �� ũ�� �ʱ�ȭ
	CRect rect;
	GetWindowRect( &rect);
	rect.right  = rect.left + m_DlgInfo.nWidth;
	rect.bottom = rect.top  + m_DlgInfo.nHeight;
	MoveWindow( rect, true);

	// ������ ������Ʈ ��� �� �۾� ���� ����
//	m_brushCtlColor = new CBrush();
//	m_brushCtlColor.CreateSolidBrush( m_DlgInfo.nTextBkColor);
	m_brushCtlColor.CreateSolidBrush(m_DlgInfo.nTextBkColor);

	// ���̾�α� ������ �׸���
	DrawDialogWindow();

	// ������ Ÿ��Ʋ �׸���
	CString strTitle;
	GetWindowText( strTitle);
	SetWindowText( strTitle);

	// ���̾�α� ������ ����
	if ( m_DlgInfo.IDDlgIconResource != NULL)
	{
		m_hIcon = AfxGetApp()->LoadIcon( m_DlgInfo.IDDlgIconResource);	// Load icon
		SetIcon( m_hIcon, true);
		SetIcon( m_hIcon, false);
	}

	// �ý��� ��ư ����
	int pos = m_DlgInfo.nWidth - 2;
	if ( m_DlgInfo.bUseQuitButton)										// Create quit button
	{
		pos -= GetX(SIZE_QUITBUTTONDEFAULT);
		CreateQuitButton( pos, 2);
	}
	if ( m_DlgInfo.bUseMaximizeButton)									// Create maximize button
	{
		pos -= GetX(SIZE_MAXIMIZEBUTTONDEFAULT);
		CreateMaximizeButton( pos, 2);
	}
	if ( m_DlgInfo.bUseMinimizeButton)									// Create minimize button
	{
		pos -= GetX(SIZE_MINIMIZEBUTTONDEFAULT);
		CreateMinimizeButton( pos, 2);
	}
	if ( m_DlgInfo.bUseAboutboxButton)									// Create about button
	{
		pos -= GetX(SIZE_ABOUTBUTTONDEFAULT);
		CreateAboutButton( pos, 2);
	}

	// ���� �巡�� �� ��� ��� ����
	DragAcceptFiles( m_DlgInfo.bEnableDragAndDrop);

	// Rendering
	Rendering();

	return true;
}


// CreateAboutButton
void CDDBDialog::CreateAboutButton( int x, int y)
{
	m_cButtonAbout.Create( _T( ""),
		                   WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
		                   CRect( x, y, x+GetX(SIZE_ABOUTBUTTONDEFAULT), y+GetY(SIZE_ABOUTBUTTONDEFAULT)),
						   this, IDC_ABOUTBOX);
	m_cButtonAbout.InitDDBButton( m_ddbSkin.GetDC(), IMG_ABOUTBUTTONDEFAULT, SIZE_ABOUTBUTTONDEFAULT, NULL);
}


// CreateMinimizeButton
void CDDBDialog::CreateMinimizeButton( int x, int y)
{
	m_cButtonMinimize.Create( _T( ""),
			                    WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                    CRect( x, y, x+GetX(SIZE_MINIMIZEBUTTONDEFAULT), y+GetY(SIZE_MINIMIZEBUTTONDEFAULT)),
								this, IDC_MINIMIZE);
	m_cButtonMinimize.InitDDBButton( m_ddbSkin.GetDC(), IMG_MINIMIZEBUTTONDEFAULT, SIZE_MINIMIZEBUTTONDEFAULT, NULL);
}


// CreateMaximizeButton
void CDDBDialog::CreateMaximizeButton( int x, int y)
{
	m_cButtonMaximize.Create( _T( ""),
			                    WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                    CRect( x, y, x+GetX(SIZE_MAXIMIZEBUTTONDEFAULT), y+GetY(SIZE_MAXIMIZEBUTTONDEFAULT)),
								this, IDC_MAXIMIZE);
	m_cButtonMaximize.InitDDBButton( m_ddbSkin.GetDC(), IMG_MAXIMIZEBUTTONDEFAULT, SIZE_MAXIMIZEBUTTONDEFAULT, NULL);
}


// CreateQuitButton
void CDDBDialog::CreateQuitButton( int x, int y)
{
	m_cButtonQuit.Create( _T( ""),
			                WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
			                CRect( x, y, x+GetX(SIZE_QUITBUTTONDEFAULT), y+GetY(SIZE_QUITBUTTONDEFAULT)),
							this, IDC_QUIT);
	m_cButtonQuit.InitDDBButton( m_ddbSkin.GetDC(), IMG_QUITBUTTONDEFAULT, SIZE_QUITBUTTONDEFAULT, NULL);
}


// DrawDialogWindow
void CDDBDialog::DrawDialogWindow()
{
	// �ʱ�ȭ
	m_listPanelMenu.RemoveAll();
	m_nNumOfMenuButton = 0;

	// ���̾�α� ��� �׸�
	int dy = 0;
	if ( m_DlgInfo.bUseTitlebar)
		dy = GetY( SIZE_TITLEBARDEFAULT);

	CRect rect;
	GetWindowRect( &rect);
	m_cDDB.FillTiledBitmap( 0, dy, rect.Width(), rect.Height()-dy, m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT)+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT);
	m_cDDB.FillTiledBitmap( 0, dy, GetX(SIZE_DIALOGDEFAULT), rect.Height()-dy, m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT), GetY(IMG_DIALOGDEFAULT)+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT);
	m_cDDB.FillTiledBitmap( rect.Width()-GetX(SIZE_DIALOGDEFAULT), dy, GetX(SIZE_DIALOGDEFAULT), rect.Height()-dy, m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT)+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT);
	m_cDDB.FillTiledBitmap( 0, dy, rect.Width(), GetY(SIZE_DIALOGDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT), SIZE_DIALOGDEFAULT);
	m_cDDB.FillTiledBitmap( 0, rect.Height()-GetY(SIZE_DIALOGDEFAULT), rect.Width(), GetY(SIZE_DIALOGDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT)+GetY(SIZE_DIALOGDEFAULT)+1+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT);
	m_cDDB.PutBitmap( 0, dy, m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT), GetY(IMG_DIALOGDEFAULT), SIZE_DIALOGDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( rect.Width()-GetX(SIZE_DIALOGDEFAULT), dy, m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT), SIZE_DIALOGDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( 0, rect.Height()-GetY(SIZE_DIALOGDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT), GetY(IMG_DIALOGDEFAULT)+GetY(SIZE_DIALOGDEFAULT)+1+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( rect.Width()-GetX(SIZE_DIALOGDEFAULT), rect.Height()-GetY(SIZE_DIALOGDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1+GetX(SIZE_DIALOGDEFAULT)+1, GetY(IMG_DIALOGDEFAULT)+GetX(SIZE_DIALOGDEFAULT)+1+GetY(SIZE_DIALOGDEFAULT)+1, SIZE_DIALOGDEFAULT, SRCCOPY);
}


// DrawPanel
void CDDBDialog::DrawPanel( int x, int y, int width, int height, LPCTSTR lpszTitle, UINT nIconNum, UINT nIDMenuResource, UINT nSubMenu)
{
	// Draw Panel
	m_cDDB.FillTiledBitmap( x, y, width, height, m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT);
	m_cDDB.FillTiledBitmap( x, y, GetX(SIZE_PANELDEFAULT), height, m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT), GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT);
	m_cDDB.FillTiledBitmap( x+width-GetX(SIZE_PANELDEFAULT), y, GetX(SIZE_PANELDEFAULT), height, m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT);
	m_cDDB.FillTiledBitmap( x, y, width, GetY(SIZE_PANELDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT), SIZE_PANELDEFAULT);
	m_cDDB.FillTiledBitmap( x, y+height-GetX(SIZE_PANELDEFAULT), width, GetY(SIZE_PANELDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT);
	m_cDDB.PutBitmap( x, y, m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT), GetY(IMG_PANELDEFAULT), SIZE_PANELDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( x+width-GetX(SIZE_PANELDEFAULT), y, m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT), SIZE_PANELDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( x, y+height-GetX(SIZE_PANELDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT), GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT, SRCCOPY);
	m_cDDB.PutBitmap( x+width-GetX(SIZE_PANELDEFAULT), y+height-GetY(SIZE_PANELDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELDEFAULT)+GetX(SIZE_PANELDEFAULT)+1+GetX(SIZE_PANELDEFAULT)+1, GetY(IMG_PANELDEFAULT)+GetY(SIZE_PANELDEFAULT)+1+GetY(SIZE_PANELDEFAULT)+1, SIZE_PANELDEFAULT, SRCCOPY);

	// Draw Icon
	if ( nIconNum > 0)
		m_cDDB.PutBitmap( x-10, y-21, m_ddbSkin.GetDC(), GetX(IMG_PANELICONDEFAULT)+GetX(SIZE_PANELICONDEFAULT)*(nIconNum-1)+nIconNum-1, GetY(IMG_PANELICONDEFAULT), SIZE_PANELICONDEFAULT, TRANSPARENT);

	// Draw panel title
	if ( lpszTitle != "")
	{
		if ( nIconNum > 0)
		{
			// Draw Panel titlebar
			m_cDDB.FillTiledBitmap( x+77, y+7, width-77-28, GetY(SIZE_PANELTITLEBARDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT)+GetX(SIZE_PANELTITLEBARDEFAULT)+1, GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT);
			m_cDDB.PutBitmap( x+77, y+7, m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT), GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT, SRCCOPY);
			m_cDDB.PutBitmap( x+width-28, y+7, m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT)+GetX(SIZE_PANELTITLEBARDEFAULT)+1+GetX(SIZE_PANELTITLEBARDEFAULT)+1, GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT, SRCCOPY);

			// Draw Panel Title
			m_cDDB.PutText( x+77+8, y+9+1, x+width-133, GetY(SIZE_PANELTITLEBARDEFAULT), 12, "����", lpszTitle, TSTYLE_BOLD, DT_LEFT, RGB(80,80,80));
		}
		else
		{
			// Draw Panel titlebar
			m_cDDB.FillTiledBitmap( x+6, y+6, width-50-GetX(SIZE_PANELTITLEBARDEFAULT), GetY(SIZE_PANELTITLEBARDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT)+GetX(SIZE_PANELTITLEBARDEFAULT)+1, GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT);
			m_cDDB.PutBitmap( x+6, y+6, m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT), GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT, SRCCOPY);
			m_cDDB.PutBitmap( x+width-50-GetX(SIZE_PANELTITLEBARDEFAULT), y+6, m_ddbSkin.GetDC(), GetX(IMG_PANELTITLEBARDEFAULT)+GetX(SIZE_PANELTITLEBARDEFAULT)+1+GetX(SIZE_PANELTITLEBARDEFAULT)+1, GetY(IMG_PANELTITLEBARDEFAULT), SIZE_PANELTITLEBARDEFAULT, SRCCOPY);

			// Draw Panel Title
			m_cDDB.PutText( x+6+7, y+9+1, x+width-100, GetY(SIZE_PANELTITLEBARDEFAULT), 12, "����", lpszTitle, TSTYLE_BOLD, DT_LEFT, RGB(80,80,80));
		}
	}

	/*

	// Draw menu button
	if ( nIDMenuResource != NULL)
	{
		CDDBDLGPANELMENU *ddbButton;
		ddbButton = new CDDBDLGPANELMENU;

		ddbButton->nIDMenuResource = nIDMenuResource;
		ddbButton->nSubMenu = nSubMenu;
		ddbButton->button.Create( _T( ""),
			                      WS_CHILD|WS_VISIBLE | BS_PUSHBUTTON,
						          CRect( x+10, y+height-39, x+10+GetX(SIZE_MENUBUTTONDEFAULT), y+height-39+GetY(SIZE_MENUBUTTONDEFAULT)),
					              this, IDC_MENUBUTTON+m_nNumOfMenuButton++);
		ddbButton->button.InitDDBButton( m_ddbSkin.GetDC(), IMG_MENUBUTTONDEFAULT, SIZE_MENUBUTTONDEFAULT, NULL);

		// Add node to list
		m_listPanelMenu.AddTail( ddbButton);
	}
	*/

	// Rendering
	if ( nIconNum > 0)
		Rendering( x-10, y-21, width+10, height+21);
	else
		Rendering( x, y, width, height);
}


// SetWindowText
void CDDBDialog::SetWindowText( LPCTSTR lpszString)
{
	// ���̾�α� Ÿ��Ʋ�� �׸�
	if ( m_DlgInfo.bUseTitlebar)
	{
		m_cDDB.FillTiledBitmap( 0, 0, m_DlgInfo.nWidth, GetY(SIZE_TITLEBARDEFAULT), m_ddbSkin.GetDC(), GetX(IMG_TITLEBARDEFAULT)+GetX(SIZE_TITLEBARDEFAULT)+1, GetY(IMG_TITLEBARDEFAULT), SIZE_TITLEBARDEFAULT);
		m_cDDB.PutBitmap( 0, 0, m_ddbSkin.GetDC(), IMG_TITLEBARDEFAULT, SIZE_TITLEBARDEFAULT, SRCCOPY);
		m_cDDB.PutBitmap( m_DlgInfo.nWidth-GetX(SIZE_TITLEBARDEFAULT), 0, m_ddbSkin.GetDC(), GetX(IMG_TITLEBARDEFAULT)+GetX(SIZE_TITLEBARDEFAULT)+1+GetX(SIZE_TITLEBARDEFAULT)+1, GetY(IMG_TITLEBARDEFAULT), SIZE_TITLEBARDEFAULT, SRCCOPY);

		if ( m_DlgInfo.bUseBmpTitle)
			m_cDDB.PutBitmap( 20, 1, m_ddbSkin.GetDC(), IMG_BMPTITLEDEFAULT, SIZE_BMPTITLEDEFAULT, SRCCOPY);
		else
		{
			m_cDDB.PutText( 21, 2, 18, "Arial Black", lpszString, TSTYLE_NORMAL, RGB(0,0,0));
			m_cDDB.PutText( 20, 1, 18, "Arial Black", lpszString, TSTYLE_NORMAL, RGB(235,235,235));
		}

		// Ÿ��Ʋ�ٿ� �ý��۸޴� �������� �׸�
		if ( m_DlgInfo.bUseIconSystemMenu && (m_DlgInfo.IDDlgIconResource != NULL))
		{
			HICON hIcon;
			hIcon = AfxGetApp()->LoadIcon( m_DlgInfo.IDDlgIconResource);
			DrawIconEx( m_cDDB.GetDC()->m_hDC, 3, 3, hIcon, 16, 16, 0, NULL, DI_NORMAL);
		}
	}

	// Rendering
	Rendering( 0, 0, m_DlgInfo.nWidth, GetY(SIZE_TITLEBARDEFAULT));

	// Set window text
	CDialog::SetWindowText( lpszString);
}


// OnPaint
void CDDBDialog::OnPaint() 
{
	CPaintDC dc(this);

	// Paint dialog screen
	if ( m_DlgInfo.bEnableRenderingLayer)
		m_ddbRendering.Rendering();
	else
		m_cDDB.Rendering();
}


// Rendering
// ���� �׸� �̹��� ���θ� �������Ѵ�.
void CDDBDialog::Rendering()
{
	// Paint bitmap to dialog screen
	m_cDDB.Rendering();

	// Paint bitmap to rendering screen
	if ( m_DlgInfo.bEnableRenderingLayer)
	{
		CRect rect;
		GetWindowRect( &rect);
		m_ddbRendering.PutBitmap( 0, 0, m_cDDB.GetDC(), 0, 0, rect.Width(), rect.Height(), SRCCOPY);
	}
}


// Rendering
// ���� �׸� �̹��� �Ϻθ� �������Ѵ�.
void CDDBDialog::Rendering( int x, int y, int width, int height)
{
	// Paint bitmap to dialog screen
	m_cDDB.Rendering( x, y, x, y, width, height);

	// Paint bitmap to rendering screen
	if ( m_DlgInfo.bEnableRenderingLayer)
		m_ddbRendering.PutBitmap( x, y, m_cDDB.GetDC(), x, y, width, height, SRCCOPY);
}


// Rendering
// �Է��� m_cDDB�� ȭ�鿡 ǥ���Ѵ�.
void CDDBDialog::Rendering( CDDB *pDDB)
{
	// Paint bitmap to dialog screen
	m_cDDB.PutBitmap( pDDB->m_nWndX, pDDB->m_nWndY, pDDB->GetDC(), 0, 0, pDDB->GetWidth(), pDDB->GetHeight(), SRCCOPY);

	// Paint bitmap to rendering screen
	if ( m_DlgInfo.bEnableRenderingLayer)
		m_ddbRendering.PutBitmap( pDDB->m_nWndX, pDDB->m_nWndY, pDDB->GetDC(), 0, 0, pDDB->GetWidth(), pDDB->GetHeight(), SRCCOPY);
}


// OnCtlColor
HBRUSH CDDBDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	pDC->SetTextColor( m_DlgInfo.nTextColor);
	pDC->SetBkColor( m_DlgInfo.nTextBkColor);

	return (HBRUSH)m_brushCtlColor.m_hObject;
}


// OnWindowPosChanging
void CDDBDialog::OnWindowPosChanging( WINDOWPOS FAR* lpwndpos) 
{
	// ������ ����Ī : ������ �̵��� ȭ�� �����ڸ��� ������ ���� �ڵ����� �ٴ´�.
	CRect rc;
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rc, NULL );
	int nScreenX = rc.Width();
	int nScreenY = rc.Height();
 
	CRect rcWin;
	rcWin.left = lpwndpos->x;
	rcWin.right = rcWin.left + m_DlgInfo.nWidth;
	rcWin.top = lpwndpos->y;
	rcWin.bottom = rcWin.top + m_DlgInfo.nHeight;

	// �����찡 ������ ��ġ�� �����ϰ� ���� ���� ���¿���
	if(  (lpwndpos->flags & SWP_NOMOVE) != SWP_NOMOVE)
	{
		if( (rcWin.left < 10) || (rcWin.left < 0))
			lpwndpos->x = 0;
		if( ((nScreenX - rcWin.right) < 10) || (rcWin.right > nScreenX))
			lpwndpos->x = nScreenX - lpwndpos->cx;
		if( (rcWin.top < 10) || (rcWin.top < 0))
			lpwndpos->y = 0;
		if( ((nScreenY - rcWin.bottom) <= 10) || (rcWin.bottom > nScreenY))
			lpwndpos->y = nScreenY - lpwndpos->cy;
	}
}


// OnNcHitTest
UINT CDDBDialog::OnNcHitTest(CPoint point) 
{
	UINT hit = CDialog::OnNcHitTest( point);

	if ( (hit == HTCLIENT) && m_DlgInfo.bEnableDlgMove)
	{
		CRect rect;
		GetWindowRect( rect);

		// ���� ���콺�� ��ǥ�� �����쿡 ���� �����ǥ�� ��ȯ�Ͽ� ���Ѵ�.
		CPoint pos;
		pos.x = point.x - rect.left;
		pos.y = point.y - rect.top;

		// Caption ���� �˻�
		if ( m_DlgInfo.bUseTitlebar)
		{
			if ( pos.y <= GetY(SIZE_TITLEBARDEFAULT))
			{
				if ( pos.x <= 20)
					return  HTSYSMENU;
				else
					return  HTCAPTION;
			}
		}
		else
			return  HTCAPTION;
	}
	
	return CDialog::OnNcHitTest(point);
}


// OnLButtonDown
void CDDBDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_fMouseLButton = true;					// ���� �÷��� ��Ʈ ��
	m_nMouseLBtnDownPos = point;			// ���� ���� ��ǥ ���
	SetCapture();
	
	CDialog::OnLButtonDown(nFlags, point);
}


// OnLButtonUp
void CDDBDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_fMouseLButton = false;				// ���� �÷��� ��Ʈ ����
	
	CDialog::OnLButtonUp(nFlags, point);
}


// OnMouseMove
void CDDBDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// ���콺 �巡�� ���� 
	if ( m_fMouseLButton)
	{
		if ( ( point.x != m_nMouseLBtnDownPos.x) || ( point.y != m_nMouseLBtnDownPos.y))
		{
			PostMessage( WM_MOUSEDRAG, (m_nMouseLBtnDownPos.x-point.x), (m_nMouseLBtnDownPos.y-point.y));
			m_nMouseLBtnDownPos = point;
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}


// OnMouseDrag
LRESULT CDDBDialog::OnMouseDrag( WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	return -1;
}


// GetDialogInfo
void CDDBDialog::GetDialogInfo( CDDBDLGINFO* DlgInfo)
{
	*DlgInfo = m_DlgInfo;
}


// SetDialogInfo
void CDDBDialog::SetDialogInfo( CDDBDLGINFO DlgInfo)
{
	m_DlgInfo = DlgInfo;
}


// GetWidth
UINT CDDBDialog::GetWidth()
{
	return m_DlgInfo.nWidth;
}


// GetHeight
UINT CDDBDialog::GetHeight()
{
	return m_DlgInfo.nHeight;
}


// OnMenuButton
void CDDBDialog::OnMenuButton( UINT nID)
{
	// ���ؽ�Ʈ �޴� ���
	CMenu menu, *pSubMenu;
	CDDBDLGPANELMENU *ddbButton;
	POSITION pNode = m_listPanelMenu.FindIndex( nID-IDC_MENUBUTTON);
	ddbButton = (CDDBDLGPANELMENU *)m_listPanelMenu.GetAt( pNode);
	
	if ( ! menu.LoadMenu( ddbButton->nIDMenuResource))
		return ;
	if ( ! (pSubMenu = menu.GetSubMenu( ddbButton->nSubMenu)))
		return ;
	
	CPoint pos;
	GetCursorPos( &pos);
	SetForegroundWindow();
	pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_BOTTOMALIGN, pos.x, pos.y, this);
	menu.DestroyMenu();
}


// OnAboutbox
void CDDBDialog::OnAboutbox() 
{
	// TODO: Add your command handler code here
}


// OnMinimize
void CDDBDialog::OnMinimize()
{
	// Minimize window
	ShowWindow( SW_MINIMIZE);
}

// OnMaximize
void CDDBDialog::OnMaximize()
{
	// TODO...
}


// OnQuit
void CDDBDialog::OnQuit()
{
	// Quit application
	PostMessage( WM_QUIT, 0, 0);
}



/************************************************************************************
  CDDBDialog trayicon
*************************************************************************************/
// CreateTrayicon
void CDDBDialog::CreateTrayicon( LPCTSTR lpszTooltip)
{
	if ( m_fTrayicon)
		return;

	if ( m_DlgInfo.IDDlgIconResource == NULL)
	{
		MessageBox( "Ʈ���� ������ ���ҽ��� �������� �ʾҽ��ϴ�. Ʈ���� �������� ������ �� �����ϴ�.", "ERROR", MB_OK);
		return ;
	}

	// Ʈ���̾����ܰ� �ڵ��� ���
	HICON hIcon;
	hIcon = (HICON)LoadIcon((HINSTANCE)GetWindowLong( GetSafeHwnd(), GWL_HINSTANCE), MAKEINTRESOURCE( m_DlgInfo.IDDlgIconResource));

	trayicon.cbSize = sizeof( NOTIFYICONDATA);
	trayicon.hWnd   = GetSafeHwnd();
	trayicon.uID    = m_DlgInfo.IDDlgIconResource;
	trayicon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayicon.hIcon  = hIcon;
	trayicon.uCallbackMessage = WM_TRAYICON_NOTIFY;
	strcpy( trayicon.szTip, lpszTooltip);
	Shell_NotifyIcon( NIM_ADD, &trayicon);

	m_fTrayicon = true;
}


// DeleteTrayicon
void CDDBDialog::DeleteTrayicon()
{
	if ( ! m_fTrayicon)
		return;

	trayicon.uFlags = 0;
	Shell_NotifyIcon( NIM_DELETE, &trayicon);

	m_fTrayicon = false;
}


// OnTrayNotification
LRESULT CDDBDialog::OnTrayNotification( WPARAM wParam, LPARAM lParam)
{
	if ( LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{
		if ( m_fShowWindow)
		{
			ShowWindow( SW_MINIMIZE);
			ShowWindow( SW_HIDE);
			m_fShowWindow = false;
		}
		else
		{
			ShowWindow( SW_SHOW);
			ShowWindow( SW_RESTORE);
			m_fShowWindow = true;
		}
	}

	return -1;
}



/************************************************************************************
  CDDBDialog Sound
*************************************************************************************/
// PlayEffectSound
void CDDBDialog::PlayEffectSound( UINT nIDWavResource)
{
	m_sndSound.PlayWavSound( nIDWavResource);
}
