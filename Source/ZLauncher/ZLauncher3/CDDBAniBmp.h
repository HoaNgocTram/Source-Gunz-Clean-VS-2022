/*
 CDDBAniBmp Ŭ����(CDDBAniBmp.h)

  desc : DDB Animation Bitmap�� �����ϰ� ó���ϴ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_CDDBANIBMP_
#define _INCLUDED_CDDBANIBMP_


// Include DDB libraries
#include "CDDB.h"



/////////////////////////////////////////////////////////////////////////////
// CDDBAniBmp window
class CDDBAniBmp : public CStatic
{
public:
	// Functions
	CDDBAniBmp();
	virtual ~CDDBAniBmp();

	BOOL InitDDBAniBmp( UINT nIDResource, UINT nNumOfScene);

	void EnableAnimate( BOOL fAnimate);
	BOOL SetAnimateScene( UINT nStartSceneNum, UINT nEndtSceneNum);

	BOOL ShowScene( UINT nSceneNum);
	UINT GetCurrentScene();
	void SetLoop( BOOL fLoop);

	void SetTimerValue( UINT nTimerValue);
	void SetTimerValue( UINT nSceneNum, UINT nTimerValue);
	void SetTimerValue( UINT nStartSceneNum, UINT nEndSceneNum, UINT nTimerValue);
	void SetTimerValue( UINT *nTimerValue);
	
	//{{AFX_VIRTUAL(CDDBAniBmp)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


protected:
	// Variables
	CDDB ddbSkin;
	CDDB ddbRendering;
	
	UINT m_nWidth;
	UINT m_nHeight;

	UINT m_nNumOfScene;
	UINT m_nCurrentScene;
	UINT m_nStartSceneNum;
	UINT m_nEndSceneNum;

	UINT m_nTimerValue[ 50];		// 50���� �����Ѵ�

	BOOL m_fAnimate;
	BOOL m_fLoop;

	//{{AFX_MSG(CDDBAniBmp)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif
