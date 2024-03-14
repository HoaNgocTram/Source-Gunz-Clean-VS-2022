/******************************************************************
   
   Log.h

     Corperation : MAIET entertainment
     Programmer  : Lim Dong Hwan
	 Date        : 22.June.2005

*******************************************************************/


#ifndef _LOG
#define _LOG


// Defines
//#ifdef _DEBUG
#define LOG_ENABLE							// ���� �α� ����� ������ ���� ��� �ּ�ó�� �Ұ�.
//#endif

#define LOG_DEFFONT			"Arial"			// �⺻ ��Ʈ
#define LOG_SHOWFILE		false			// �� ���� true�� : Text ������ ������ �� ������ ȭ�鿡 ����.
											//                  HTML ������ ������ �� �������� ȭ�鿡 ����.

enum LOG_TYPE								// �α� ��� ���
{
	LOGTYPE_OUTPUT,								// ��� â
	LOGTYPE_TEXT,								// �ؽ�Ʈ ����
	LOGTYPE_HTML,								// HTML ����
	LOGTYPE_EXEL,								// ���� ����
};

enum LOG_MSG								// �α� �޽��� ����
{
	LOG_NORMAL,									// �Ϲ�
	LOG_HIGHLIGHT,								// ����
	LOG_WARNNING,								// ���
	LOG_ERROR,									// ����
};


// Functions
bool OpenLog( const char* pszFileName, const char* pszTitle, LOG_TYPE nFileType);
bool CloseLog( void);
bool PutLog( const char* pszLog);
bool PutLogH( const char* pszLog);
bool PutLogW( const char* pszLog);
bool PutLogE( const char* pszLog);
bool PutLog( const char* pszLog, LOG_MSG nMsgType);

#endif
