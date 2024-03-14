/*
 CDDBAniBmp Ŭ����(CDDBAniBmp.h)

  desc : DDB Animation Bitmap�� �����ϰ� ó���ϴ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#include <stdafx.h>
#include "CDDBAniBmp.h"


#define TIMER_ANIMATE		1


/************************************************************************************
  CDDBAniBmp window
*************************************************************************************/
// CDDBAniBmp
CDDBAniBmp::CDDBAniBmp()
{
	m_nNumOfScene		= 0;
	m_nCurrentScene		= 0;
	m_fAnimate			= false;
	m_fLoop				= true;
}


//~CDDBAniBmp
CDDBAniBmp::~CDDBAniBmp()
{
	ddbSkin.DeleteDDB();
	ddbRendering.DeleteDDB();
}


// Message map
BEGIN_MESSAGE_MAP(CDDBAniBmp, CStatic)
	//{{AFX_MSG_MAP(CDDBAniBmp)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/************************************************************************************
  CDDBAniBmp message handlers
*************************************************************************************/

/*************************************************************
 InitDDBAniBmp

  desc : Animation bitmap�� �����Ѵ�.
  arg  : nIDResource = �̹��� ���ҽ� ID
         nNumOfScene = �̹��� ���(scene) ��
  ret  : true(=����) or false(=����)
  ex   : InitDDBAniBmp( IDR_ANIBMP, 5);
**************************************************************/
BOOL CDDBAniBmp::InitDDBAniBmp( UINT nIDResource, UINT nNumOfScene)
{
	if ( nNumOfScene < 1)
		return false;

	// KillTimer
	KillTimer( TIMER_ANIMATE);

	// Delete DDB
	ddbSkin.DeleteDDB();
	ddbRendering.DeleteDDB();
	
	// CreateDDB
	if ( ! ddbSkin.CreateDDB( GetDC(), nIDResource))
		return false;
	m_nWidth  = (ddbSkin.GetWidth() - nNumOfScene + 1) / nNumOfScene;
	m_nHeight = ddbSkin.GetHeight();
	if ( ! ddbRendering.CreateDDB( 0, 0, GetDC(), m_nWidth, m_nHeight, RGB(255,255,255)))
		return false;

	// Set varialbes
	for ( UINT i = 0;  i < 50;  i++)
		m_nTimerValue[ i] = 1000;
	m_nNumOfScene    = nNumOfScene;
	m_nStartSceneNum = 0;
	m_nEndSceneNum   = nNumOfScene - 1;

	// Set dialog rectangle
	CRect parent_rect, rect;
	GetParent()->GetWindowRect( &parent_rect);
	GetWindowRect( &rect);
	rect.left   = rect.left - parent_rect.left;
	rect.top    = rect.top  - parent_rect.top;
	rect.right  = rect.left + m_nWidth;
	rect.bottom = rect.top  + m_nHeight;
	MoveWindow( rect);

	// Show scene
	ShowScene( 0);

	return true;
}


/*************************************************************
 EnableAnimate

  desc : ���ϸ��̼��� ������
  arg  : fAnimate
  ret  : true(=����) or false(=����)
  ex   : EnableAnimate();
**************************************************************/
void CDDBAniBmp::EnableAnimate( BOOL fAnimate)
{
	// Set timer
	if ( fAnimate)
	{
		ShowScene( m_nStartSceneNum);
		SetTimer( TIMER_ANIMATE, m_nTimerValue[ m_nStartSceneNum], NULL);
	}
	// Reset timer
	else
		KillTimer( TIMER_ANIMATE);

	m_fAnimate = fAnimate;
}


/*************************************************************
 SetAnimateScene()

  desc : ���ϸ��̼��� �̹����� ����
  arg  : nStartSceneNum = ���� �̹��� ��� ��ȣ
         nEndtSceneNum = ���� �̹��� ��� ��ȣ
  ret  : true(=����) or false(=����)
  ex   : SetAnimateScene( 0, 5);
**************************************************************/
BOOL CDDBAniBmp::SetAnimateScene(UINT nStartSceneNum, UINT nEndtSceneNum)
{
	if ( (nStartSceneNum >= m_nNumOfScene) || (nEndtSceneNum >= m_nNumOfScene))
		return false;
	if ( nStartSceneNum > m_nNumOfScene)
		return false;

	// Kill timer
	KillTimer( TIMER_ANIMATE);

	// Set start/end scene number
	m_nStartSceneNum = nStartSceneNum;
	m_nEndSceneNum   = nEndtSceneNum;

	// Show scene
	if ( m_fAnimate)
	{
		ShowScene( m_nStartSceneNum);
		// Set timer
		SetTimer( TIMER_ANIMATE, m_nTimerValue[ m_nStartSceneNum], NULL);
	}

	return true;
}


/*************************************************************
 ShowScene

  desc : ������ ��ȣ�� ����� ���δ�.
  arg  : nSceneNum = ��� ��ȣ
  ret  : true(=����) or false(=����)
  ex   : ShowScene( 1);
**************************************************************/
BOOL CDDBAniBmp::ShowScene( UINT nSceneNum)
{
	// Check scene number
	if ( nSceneNum > m_nNumOfScene)
		return false;

	// Draw scene
	UINT pos;
	if ( nSceneNum == 0)
		pos = 0;
	else
		pos = m_nWidth * nSceneNum + nSceneNum;
	ddbRendering.PutBitmap( 0, 0, ddbSkin.GetDC(), pos, 0, m_nWidth, m_nHeight, SRCCOPY);

	// Paint dialog screen
	ddbRendering.Rendering();
	m_nCurrentScene = nSceneNum;

	// Set timer
	if ( m_fAnimate)
		SetTimer( TIMER_ANIMATE, m_nTimerValue[ nSceneNum], NULL);

	return true;
}


/*************************************************************
 GetCurrentScene

  desc : ���� ���̴� ����� �̹��� ��ȣ�� ���Ѵ�.
  arg  : none
  ret  : ��� ��ȣ
  ex   : GetCurrentScene();
**************************************************************/
UINT CDDBAniBmp::GetCurrentScene()
{
	return m_nCurrentScene;
}


/*************************************************************
 SetLoop

  desc : �ݺ�(loop) ���ϸ��̼��� ��
  arg  : fLoop = true(�ݺ���) or false(�ݺ����� ����)
  ret  : none
  ex   : SetLoop( true);
**************************************************************/
void CDDBAniBmp::SetLoop( BOOL fLoop)
{
	m_fLoop = fLoop;
}


/*************************************************************
 SetTimerValue

  desc : �� ����� �����ð�(mili second)�� �����Ѵ�.
  arg  : ...
  ret  : none
  ex   : SetTimerValue( 200);
**************************************************************/
void CDDBAniBmp::SetTimerValue( UINT nTimerValue)
{
	for ( UINT i = 0;  i < m_nNumOfScene;  i++)
		m_nTimerValue[ i] = nTimerValue;
}


void CDDBAniBmp::SetTimerValue( UINT nSceneNum, UINT nTimerValue)
{
	m_nTimerValue[ nSceneNum] = nTimerValue;
}


void CDDBAniBmp::SetTimerValue( UINT nStartSceneNum, UINT nEndSceneNum, UINT nTimerValue)
{
	for ( UINT i = nStartSceneNum;  i <= nEndSceneNum;  i++)
		m_nTimerValue[ i] = nTimerValue;
}


void CDDBAniBmp::SetTimerValue( UINT *nTimerValue)
{
	for ( UINT i = 0;  i < m_nNumOfScene;  i++)
		m_nTimerValue[ i] = *( nTimerValue + i);
}


/*************************************************************
 PreSubclassWindow (protected)

  desc : �׸��� ����
**************************************************************/
void CDDBAniBmp::PreSubclassWindow() 
{
	ModifyStyle( 0, BS_OWNERDRAW);
	
	CStatic::PreSubclassWindow();
}


/*************************************************************
 OnPaint (protected)

  desc : ȭ�� ����
**************************************************************/
void CDDBAniBmp::OnPaint() 
{
	CPaintDC dc(this);

	// Paint dialog screen
	ddbRendering.Rendering();
}


/*************************************************************
 OnTimer (Protected)

  desc : Ÿ�̸� �ڵ鷯
**************************************************************/
void CDDBAniBmp::OnTimer( UINT nIDEvent) 
{
	if ( nIDEvent == TIMER_ANIMATE)
	{
		// Kill timer
		KillTimer( TIMER_ANIMATE);

		// Show next scene
		BOOL bLoop = true;
		m_nCurrentScene++;
		if ( (m_nCurrentScene > m_nEndSceneNum) || (m_nCurrentScene == m_nNumOfScene))
		{
			m_nCurrentScene = m_nStartSceneNum;

			if ( ! m_fLoop)
				bLoop = false;
		}

		ShowScene( m_nCurrentScene);

		// Set timer
		if ( bLoop)
			SetTimer( TIMER_ANIMATE, m_nTimerValue[ m_nCurrentScene], NULL);
	}

	CStatic::OnTimer(nIDEvent);
}
