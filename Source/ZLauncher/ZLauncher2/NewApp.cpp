/*
 CNewApp Ŭ����(NewApp.cpp)

  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "NewApp.h"
#include "NewAppMainDlg.h"


// Variables
CNewAppApp theApp;



/************************************************************************************
  CNewApp
*************************************************************************************/
// CNewAppApp
CNewAppApp::CNewAppApp()
{
}


// Message map
BEGIN_MESSAGE_MAP(CNewAppApp, CWinApp)
	//{{AFX_MSG_MAP(CNewAppApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()



/************************************************************************************
  CNewAppApp initialization
*************************************************************************************/
HANDLE g_hMutex = NULL;
BOOL CNewAppApp::InitInstance()
{
	// Mutex ����
	BOOL bFound = FALSE; 
	g_hMutex = ::CreateMutex( NULL, TRUE, ZUPDATE_MUTEXNAME);
    
	// ���� �̹� ������� �ִٸ� Instance�� �̹� ������
	if( ::GetLastError() == ERROR_ALREADY_EXISTS) 
		bFound = TRUE;

	if( g_hMutex)
		::ReleaseMutex( g_hMutex);

	// �̹� �ϳ��� Instance�� �����ϸ� ���α׷� ����
	if( bFound)
		return FALSE;
	
	
	AfxEnableControlContainer();

	// Open main dialog box
	CNewAppMainDlg dlg;
	dlg.DoModal();

	
	return false;
}
