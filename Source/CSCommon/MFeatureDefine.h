#pragma once

// ����� �������� ��õ� ��Ȳ�� �ٸ� ��� define���� ���� ������ ���ݴϴ�
// cscommon�� �� define�� ���� ������ ������ Ŭ�� ���ÿ� ����ǵ��� �ϱ� �����Դϴ�


#if defined(_DEBUG) || defined(_RELEASE) || defined(LOCALE_KOREA) || defined(LOCALE_NHNUSA) || defined(LOCALE_JAPAN) || defined(LOCALE_BRAZIL)
	#define _AVATAR_ENABLE	// �ƹ�Ÿ ���� ����
#endif

//#if defined(_DEBUG) || defined(_RELEASE) || defined(LOCALE_KOREA)
#if 0
#	define _SELL_CASHITEM	// ĳ�� ������ ���ȱ� ���
#endif

// Define 
/*
#define POINTER_64 __ptr64
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
*/