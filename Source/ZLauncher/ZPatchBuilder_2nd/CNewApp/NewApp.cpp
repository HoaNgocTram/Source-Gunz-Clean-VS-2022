/*
 CNewApp Ŭ����(NewApp.cpp)

  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "NewApp.h"
#include "NewAppDlg.h"


// Variables
CNewApp theApp;



/************************************************************************************
  CNewApp
*************************************************************************************/
// CNewApp
CNewApp::CNewApp()
{
}


// Message map
BEGIN_MESSAGE_MAP(CNewApp, CWinApp)
	//{{AFX_MSG_MAP(CNewApp)
	//}}AFX_MSG
END_MESSAGE_MAP()



/************************************************************************************
  CNewApp initialization
*************************************************************************************/
BOOL CNewApp::InitInstance()
{
	AfxEnableControlContainer();

	// Open main dialog box
	CNewAppDlg dlg;
	dlg.DoModal();

	
	return false;
}
