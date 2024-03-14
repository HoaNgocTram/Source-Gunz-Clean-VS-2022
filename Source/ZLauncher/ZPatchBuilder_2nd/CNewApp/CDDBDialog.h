/*
 CDDBDialog Ŭ����(CDDBDialog.h)

  desc : CDialog�� ��ӹ޾Ƽ� GUI ���̾�α׸� ����� �� �ְ� ���ִ� Ŭ����.
  date : 2004�� 5�� 30��
  comp : ������б� �����������ȸ(KUCIRA) - �ӵ�ȯ
         http://www.kucira.com
  text : MFC�� �����ϴ� ��� ������ ���� ���� �� ������ �����մϴ�. ��, Ư�� �����̳�
         ��ü�� ������ ���� �������� ���Ǵ� ���� ���մϴ�.
*/


#ifndef _INCLUDED_CDDBDLG_
#define _INCLUDED_CDDBDLG_


#include "CDDB.h"								// Include CDDB
#include "CDDBButton.h"							// Include CDDB Button
#include "CSound.h"								// Include CSound
#include <afxcmn.h>								// Include afxcmn.h


// Define button ID resource name
#define WM_MOUSEDRAG		(WM_USER + 100)		// Message of drag mouse
#define WM_TRAYICON_NOTIFY	(WM_USER + 101)		// Message of m_TrayIcon
#define IDC_ABOUTBOX		(WM_USER + 200)		// Control button Aboutbox
#define IDC_MINIMIZE		(WM_USER + 201)		// Control button Minimize
#define IDC_MAXIMIZE		(WM_USER + 202)		// Control button Maximize
#define IDC_QUIT			(WM_USER + 203)		// Control button Quit



/////////////////////////////////////////////////////////////////////////////
// CDDBDialog dialog
class CDDBDialog : public CDialog
{
// Structure of CDDBDIALOGINFO
public:
	struct CDDBDLGINFO
	{
		// ���̾�α� ���� ����
		UINT		nWidth;						// ������ ���̾�α��� ���̸� �����Ѵ�.
		UINT		nHeight;					// ������ ���̾�α��� ���̸� �����Ѵ�.
		UINT		IDSkinResource;				// ���̾�α׿� ���� ��Ų ���ҽ��� ID�� �����Ѵ�.
		UINT		IDIconResource;				// ���̾�α׿� ����� �������� ���ҽ��� ID�� �����Ѵ�.
		UINT		IDWaveResource;				// ���̾�α׿� ���Ǵ� ��ư���� ���� ���� ���ҽ� ID�� �����Ѵ�.
		COLORREF	nTextColor;					// ���̾�α� �� ��Ʈ�ѿ� ����� �ؽ�Ʈ�� ������ �����Ѵ�.
		COLORREF	nTextBkColor;				// ���̾�α� �� ��Ʈ�ѿ� ����� �ؽ�Ʈ ��� ������ �����Ѵ�.

		// Ÿ��Ʋ�� ���� ���� ����
		BOOL		bUseTitlebar;				// �� �׸��� TRUE�� �����ϸ� ���̾�α� ���κп� Ÿ��Ʋ�ٸ� �����.
		BOOL		bUseBmpTitle;				// �� �׸��� TRUE�� �����ϸ� Ÿ��Ʋ���� ������ ��Ʈ�� �̹����� ����ؼ� �׸���.
		BOOL		bUseIconSystemMenu;			// �� �׸��� TRUE�� �����ϸ� Ÿ��Ʋ���� �ý��� �޴� ������ ������(IDIconResource)�� �׸���.

		// �ý��۹�ư ��� �÷���
		BOOL		bUseAboutboxButton;			// �� �׸��� TRUE�� �����ϸ� Aboutbox ��ư�� ����� �� �ִ�.
		BOOL		bUseMinimizeButton;			// �� �׸��� TRUE�� �����ϸ� Minimize ��ư�� ����� �� �ִ�.
		BOOL		bUseMaximizeButton;			// �� �׸��� TRUE�� �����ϸ� Maximize ��ư�� ����� �� �ִ�.
		BOOL		bUseQuitButton;				// �� �׸��� TRUE�� �����ϸ� Quit ��ư�� ����� �� �ִ�.

		// ��Ÿ �ɼ�
		BOOL		bEnableMoveDlg;				// �� �׸��� TRUE�� �����ϸ� ���̾�α׸� �巡���ؼ� �̵���ų �� �ִ�.
		BOOL		bEnableDragAndDrop;			// �� �׸��� TRUE�� �����ϸ� ���̾�α׿� ������ �巡�� �� ����� �� �ִ�.
		BOOL		bEnableRenderingLayer;		// �� �׸��� TRUE�� �����ϸ� Rendering�� DDB�� �̿��� �� �ִ�.


		// �ʱ�ȭ
		CDDBDLGINFO()
		{
			nWidth					= 320;
			nHeight					= 240;
			IDSkinResource			= NULL;
			IDIconResource			= NULL;
			IDWaveResource			= NULL;
			nTextColor				= RGB( 61,  99,  184);
			nTextBkColor			= RGB( 255, 255, 255);
			bUseTitlebar			= false;
			bUseBmpTitle			= false;
			bUseIconSystemMenu		= false;
			bUseAboutboxButton		= false;
			bUseMinimizeButton		= false;
			bUseMaximizeButton		= false;
			bUseQuitButton			= false;
			bEnableMoveDlg			= false;
			bEnableDragAndDrop		= false;
			bEnableRenderingLayer	= false;
		}
	};


// Variables
private:
	CDDB			m_Screen,					// DDB of drawing screen
					m_Skin,						// DDB of skin
					m_Render;					// DDB to rendering

	CSound			m_Sound;					// Play sound

	CDDBButton		m_cButtonAbout,				// About button
					m_cButtonMinimize,			// Minimize button
					m_cButtonMaximize,			// Maximize button
					m_cButtonQuit;				// Quit button

	CDDBDLGINFO		m_DlgInfo;					// Informations of this dialog

	HICON			m_hIcon;					// Handle of icon
	CBrush			m_brushCtlColor;			// Window color brush
	CRgn			m_Rgn;						// Window Region
	CToolTipCtrl	m_Tooltip;					// Tooltip

	bool			m_bInitialized;				// Is Initialized

	BOOL			m_bMouseLButton;			// Mouse left button check flag
	CPoint			m_nMouseLBtnDownPos;		// Mouse left button position
	NOTIFYICONDATA	m_TrayIcon;					// Handle of trayIcon


// Functions
public:
	CDDBDialog( UINT nIDTemplate, CWnd* pParentWnd);
	virtual ~CDDBDialog();

	void SetWindowText( LPCTSTR lpszString);
	
	UINT GetWidth()									{ return m_DlgInfo.nWidth;  }
	UINT GetHeight()								{ return m_DlgInfo.nHeight; }

	COLORREF GetTextColor()							{ return m_DlgInfo.nTextColor;   }
	COLORREF GetTextBkColor()						{ return m_DlgInfo.nTextBkColor; }


	//{{AFX_VIRTUAL(CDDBDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void DrawDialogWindow();
	virtual void DrawPanel( int x, int y, int width, int height, LPCTSTR lpszTitle=NULL);
	virtual void CreateAboutButton( int x, int y);
	virtual void CreateMaximizeButton( int x, int y);
	virtual void CreateMinimizeButton( int x, int y);
	virtual void CreateQuitButton( int x, int y);
	virtual void DropFiles( LPCTSTR szFileName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


protected:
	void Rendering();
	void Rendering( int x, int y, int width, int height);
	
	void GetDialogInfo( CDDBDLGINFO* DlgInfo)		{ *DlgInfo = m_DlgInfo; }
	void SetDialogInfo( CDDBDLGINFO DlgInfo)		{ if ( !m_bInitialized)  m_DlgInfo = DlgInfo;  }

	CDDB* GetScreen()								{ return &m_Screen; }
	CDDB* GetSkin()									{ return &m_Skin;   }
	CDDB* GetRender()								{ return &m_Render; }

	CDC* GetScreenDC()								{ return m_Screen.GetDC(); }
	CDC* GetSkinDC()								{ return m_Skin.GetDC();   }
	CDC* GetRenderDC()								{ return m_Render.GetDC(); }

	void AddTooltip( CWnd* pWnd, LPCTSTR lpszMsg)	{ if ( !m_bInitialized)  m_Tooltip.AddTool( pWnd, _T( lpszMsg)); }

	void CreateTrayicon( LPCTSTR lpszTooltip);
	void DeleteTrayicon();

	void PlayEffectSound( UINT IDWaveResource)		{ m_Sound.PlayWavSound( IDWaveResource); }


protected:
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
	afx_msg void OnDropFiles( HDROP hDropInfo);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

#endif
