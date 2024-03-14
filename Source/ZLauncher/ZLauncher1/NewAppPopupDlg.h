/*
 CNewAppPopupDlg Ŭ����(NewAppPopupDlg.h)

  desc : Popup Dialog ���� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_NEWAPPPOPUPDLG_
#define _INCLUDED_NEWAPPPOPUPDLG_


#include "resource.h"
#include "CDDBDialog.h"
#include "afxwin.h"
#include "cddbbutton.h"


const enum NOTIFYICON { NI_NOTIFY, NI_QUESTION, NI_ERROR, NI_ABOUT};



/////////////////////////////////////////////////////////////////////////////
// CNewAppPopupDlg dialog
class CNewAppPopupDlg : public CDDBDialog
{
public:

	CNewAppPopupDlg(CWnd* pParent = NULL);

	void SetMessage( const char* szMessage);
	void SetIcon( int nIconNum);

	//{{AFX_DATA(CNewAppPopupDlg)
	enum { IDD = IDD_DIALOG_POPUP };
	CStatic m_cMessage;
	CDDBButton m_cOK;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CNewAppPopupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void DrawDialogWindow();
	//}}AFX_VIRTUAL

protected:
	char m_szMessage[ 1024];
	int m_nIcon;

	//{{AFX_MSG(CNewAppPopupDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
