/*
 CNewApp Ŭ����(NewApp.h)

  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_CNEWAPP_
#define _INCLUDED_CNEWAPP_



/////////////////////////////////////////////////////////////////////////////
// CNewApp dialog
class CNewApp : public CWinApp
{
public:
	CNewApp();

	//{{AFX_VIRTUAL(CNewAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNewApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
