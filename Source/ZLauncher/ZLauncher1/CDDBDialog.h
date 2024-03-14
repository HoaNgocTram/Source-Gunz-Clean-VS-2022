/*
 CDDBDialog Ŭ����(CDDBDialog.h)

  desc : Dialog�� �׷��ִ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_CDDBDLG_
#define _INCLUDED_CDDBDLG_


#include "CDDB.h"
#include "CDDBButton.h"
#include "CSound.h"


// Define button ID resource name
#define WM_MOUSEDRAG				(WM_USER + 100)			// Message of drag mouse
#define WM_TRAYICON_NOTIFY			(WM_USER + 101)			// Message of trayicon
#define IDC_ABOUTBOX				(WM_USER + 200)			// Control button Aboutbox
#define IDC_MINIMIZE				(WM_USER + 201)			// Control button Minimize
#define IDC_MAXIMIZE				(WM_USER + 202)			// Control button Maximize
#define IDC_QUIT					(WM_USER + 203)			// Control button Quit
#define IDC_MENUBUTTON				(WM_USER + 204)			// Control button Menu
#define IDC_MENUBUTTONMAX			(IDC_MENUBUTTON + 50)	// Control button Menu



/////////////////////////////////////////////////////////////////////////////
// CDDBDialog dialog
class CDDBDialog : public CDialog
{
public:
	// Structure of CDDBDIALOGINFO
	struct CDDBDLGINFO
	{
		// ���̾�α� ���� ����
		UINT nWidth;						// ������ ���̾�α��� ���̸� �����Ѵ�.
		UINT nHeight;						// ������ ���̾�α��� ���̸� �����Ѵ�.
		UINT IDDlgSkinBmpResource;			// (�ʼ�)���̾�α׿� ���� ��Ų ���ҽ��� ID�� �����Ѵ�.
		UINT IDDlgIconResource;				// ���̾�α׿� ����� �������� ���ҽ��� ID�� �����Ѵ�.
		COLORREF nTextColor;				// ���̾�α� �� ��Ʈ�ѿ� ����� �ؽ�Ʈ�� ������ �����Ѵ�.
		COLORREF nTextBkColor;				// ���̾�α� �� ��Ʈ�ѿ� ����� �ؽ�Ʈ ��� ������ �����Ѵ�.

		// Ÿ��Ʋ�� ���� ���� ����
		BOOL bUseTitlebar;					// �� �׸��� TRUE�� �����ϸ� ���̾�α� ���κп� Ÿ��Ʋ�ٸ� �����.
		BOOL bUseBmpTitle;					// �� �׸��� TRUE�� �����ϸ� Ÿ��Ʋ���� ������ ��Ʈ�� �̹����� ����ؼ� �׸���.
		BOOL bUseIconSystemMenu;			// �� �׸��� TRUE�� �����ϸ� Ÿ��Ʋ���� �ý��� �޴� ������ ������(IDDlgIconResource)�� �׸���.

		// �ý��۹�ư ��� �÷���
		BOOL bUseAboutboxButton;			// �� �׸��� TRUE�� �����ϸ� Aboutbox ��ư�� ����� �� �ִ�.
		BOOL bUseMinimizeButton;			// �� �׸��� TRUE�� �����ϸ� Minimize ��ư�� ����� �� �ִ�.
		BOOL bUseMaximizeButton;			// �� �׸��� TRUE�� �����ϸ� Maximize ��ư�� ����� �� �ִ�.
		BOOL bUseQuitButton;				// �� �׸��� TRUE�� �����ϸ� Quit ��ư�� ����� �� �ִ�.

		// ��Ÿ �ɼ�
		BOOL bEnableDlgMove;				// �� �׸��� TRUE�� �����ϸ� ���̾�α׸� �巡���ؼ� �̵���ų �� �ִ�.
		BOOL bEnableDragAndDrop;			// �� �׸��� TRUE�� �����ϸ� ���̾�α׿� ������ �巡�� �� ����� �� �ִ�.
		BOOL bEnableRenderingLayer;			// �� �׸��� TRUE�� �����ϸ� Rendering�� DDB�� �̿��� �� �ִ�.


		// �ʱ�ȭ
		CDDBDLGINFO()
		{
			nWidth					= 320;
			nHeight					= 240;
			IDDlgSkinBmpResource	= NULL;
			IDDlgIconResource		= NULL;
			nTextColor				= RGB(0,0,0);
			nTextBkColor			= RGB(255,255,255);
			bUseTitlebar			= false;
			bUseBmpTitle			= false;
			bUseIconSystemMenu		= false;
			bUseAboutboxButton		= false;
			bUseMinimizeButton		= false;
			bUseMaximizeButton		= false;
			bUseQuitButton			= false;
			bEnableDlgMove			= false;
			bEnableDragAndDrop		= false;
			bEnableRenderingLayer	= false;
		}
	};

	// Structure of CDDBDLGPANELMENU
	struct CDDBDLGPANELMENU : public CObject
	{
		CDDBButton button;
		UINT nIDMenuResource;
		UINT nSubMenu;
	};


	CDDBDialog( UINT nIDTemplate, CWnd* pParentWnd);
	virtual ~CDDBDialog();
	void SetWindowText( LPCTSTR lpszString);
	UINT GetWidth();
	UINT GetHeight();


	//{{AFX_VIRTUAL(CDDBDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void DrawDialogWindow();
	virtual void DrawPanel( int x, int y, int width, int height, LPCTSTR lpszTitle, UINT nIconNum, UINT nIDMenuResource, UINT 
nSubMenu);
	virtual void CreateAboutButton( int x, int y);
	virtual void CreateMaximizeButton( int x, int y);
	virtual void CreateMinimizeButton( int x, int y);
	virtual void CreateQuitButton( int x, int y);
	//}}AFX_VIRTUAL


protected:
	CDDB m_cDDB;						// DDB to drawing screen
	CSound m_sndSound;

	CDDBButton	m_cButtonAbout,			// About button
				m_cButtonMinimize,		// Minimize button
				m_cButtonMaximize,		// Maximize button
				m_cButtonQuit;			// Quit button


	void Rendering();
	void Rendering( int x, int y, int width, int height);
	void Rendering( CDDB *pDDB);
	void GetDialogInfo( CDDBDLGINFO* DlgInfo);
	void SetDialogInfo( CDDBDLGINFO DlgInfo);
	CDC*  GetSkinBmp()			{ return m_ddbSkin.GetDC(); }
	void CreateTrayicon( LPCTSTR lpszTooltip);
	void DeleteTrayicon();
	void PlayEffectSound( UINT nIDWavResource);


	//{{AFX_MSG(CDDBDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseDrag( WPARAM wParam, LPARAM lParam);
	afx_msg void OnAboutbox();
	afx_msg void OnMaximize();
	afx_msg void OnMinimize();
	afx_msg void OnQuit();
	afx_msg LRESULT OnTrayNotification( WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG


private:
	CDDBDLGINFO m_DlgInfo;				// Informations of this dialog
	HICON m_hIcon;						// Handle of icon
	CBrush m_brushCtlColor;				// Window color brush
	CDDB m_ddbRendering;				// DDB to rendering
	CDDB m_ddbSkin;						// DDB of Skin
	BOOL m_fShowWindow;					// Flag of show window
	BOOL m_fTrayicon;					// Flag of trayicon
	BOOL m_fMouseLButton;				// Mouse left button check flag
	CPoint m_nMouseLBtnDownPos;			// Mouse left button position
	NOTIFYICONDATA trayicon;			// Handle of trayicon
	CObList m_listPanelMenu;			// List of panel menu
	UINT m_nNumOfMenuButton;			// Number of menu button

	void OnMenuButton( UINT nID);

	DECLARE_MESSAGE_MAP()
};

#endif
