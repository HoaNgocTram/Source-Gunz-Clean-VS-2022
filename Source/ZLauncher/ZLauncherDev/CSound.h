/*
 CSound Ŭ����(CSound.h)

  desc : Wave sound�� ����ϴ� Ŭ����
  date : 2004�� 5�� 30��
  comp : �ӵ�ȯ
*/


#ifndef _INCLUDED_CSOUND_
#define _INCLUDED_CSOUND_


// Functions
class CSound
{
public:
	void PlayWavSound( UINT nIDResource);
	inline void PlayWavSound( LPCTSTR lpszSound);

protected:
};

#endif