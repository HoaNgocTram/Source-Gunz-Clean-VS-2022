/*
 CNewApp Ŭ����(NewApp.h)

  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_CNEWAPP_
#define _INCLUDED_CNEWAPP_



/////////////////////////////////////////////////////////////////////////////
// CNewApp dialog
class CNewAppApp : public CWinApp
{
public:
	CNewAppApp();

	//{{AFX_VIRTUAL(CNewAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNewAppApp)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#define ZUPDATE_MUTEXNAME	"ZUpdateMutexObject"

#endif
