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
HANDLE g_hMutex = NULL;
BOOL CNewApp::InitInstance()
{
	// Mutex ����
	BOOL bFound = FALSE; 
	g_hMutex = ::CreateMutex( NULL, TRUE, "ZGunzLauncherMutex");
    
	// ���� �̹� ������� �ִٸ� Instance�� �̹� ������
	if( ::GetLastError() == ERROR_ALREADY_EXISTS) 
		bFound = TRUE;

	if( g_hMutex)
		::ReleaseMutex( g_hMutex);

	// �̹� �ϳ��� Instance�� �����ϸ� ���α׷� ����
	if( bFound)
		return FALSE;


	// ��Ʈ�� �ʱ�ȭ
	AfxEnableControlContainer();
	AfxInitRichEdit();


	// Open main dialog box
	CNewAppDlg dlg;
	dlg.DoModal();

	
	return false;
}
