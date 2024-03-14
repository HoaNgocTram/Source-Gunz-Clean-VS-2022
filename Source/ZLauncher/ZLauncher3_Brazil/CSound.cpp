/*
 CSound Ŭ����(CSound.cpp)

  desc : Wave sound�� ����ϴ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ

  �ݵ�� Project Settings���� Link->Object/library modules��
  winmm.lib�� �߰������ ��.
*/

#include <stdafx.h>
#include "CSound.h"
#include <MMSystem.h>



// PlayWavSound
void CSound::PlayWavSound( UINT nIDResource)
{
	// Play sound
	PlayWavSound( MAKEINTRESOURCE( nIDResource));
}


// PlayWavSound
inline void CSound::PlayWavSound( LPCTSTR lpszSound)
{
	HRSRC hRes;
	HGLOBAL hData;
	BOOL bOk = false;
	if (( hRes  = ::FindResource( AfxGetResourceHandle(), lpszSound, _T( "WAVE"))) != NULL &&
	    ( hData = ::LoadResource( AfxGetResourceHandle(), hRes)) != NULL)
	{
		bOk = sndPlaySound( (LPCTSTR)::LockResource( hData), SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		FreeResource( hData);
	}
}
