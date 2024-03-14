/////////////////////////////////////////////////////////////
// cxr_MMatchBuff.h
//
// This file was generated by CXR, the literal string encryptor.
// CXR, Copyright 2002, Smaller Animals Software, Inc., all rights reserved.
//
// Please do not edit this file. Any changes here will be overwritten on the next compile.
// If you wish to make changes to a string, please edit:
//     MMatchBuff.cxr
//

/////////////////////////////////////////////////////////////

#pragma once
#include "cxr_inc.h"

/*
	== XML ��� ���� ==
	
	+ zBuff.xml ����	
	+ attribute ����
		BuffID			: ���� ���̵�						(int)
		BuffName		: ���� �̸�							(string[128])
		BuffIconName	: ���� ������ �̸�					(string[128])
		
		BuffType		: ���� Ÿ��			(MMatchBuffDesc.h���� MMatchBuffType ����)
		BuffPeriodType	: ���� �Ⱓ Ÿ��	(MMatchBuffDesc.h���� MMatchBuffPeriodType ����)
		BuffPeirod		: ���� �Ⱓ			(�� ����)
		
		HP	:
		AP	:
		XP	:		
*/

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_BUFF	_CXR("\x86\x82\x88\x85\x83\x86\x84\x85\x87\x80")
#else
#define MICTOK_BUFF														_CXR("BUFF")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_ID	_CXR("\x8a\x8e\x89\x8e\x8b\x85")
#else
#define MICTOK_ATTRNAME_BUFF_ID											_CXR("ID")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_ICON_NAME	_CXR("\x8b\x83\x8b\x81\x86\x86\x8f\x8e\x80\x8e\x88\x82\x8c\x84\x81\x84\x81\x8d")
#else
#define MICTOK_ATTRNAME_BUFF_ICON_NAME									_CXR("IconName")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_PERIOD_TYPE	_CXR("\x87\x88\x8e\x89\x81\x84\x81\x8b\x8a\x8c\x84\x8e\x8f\x87\x85\x88\x8b\x86\x81\x87\x81\x81")
#else
#define MICTOK_ATTRNAME_BUFF_PERIOD_TYPE								_CXR("PeriodType")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_PERIOD	_CXR("\x80\x81\x8f\x8e\x85\x82\x80\x85\x81\x8f\x80\x8e\x82\x86")
#else
#define MICTOK_ATTRNAME_BUFF_PERIOD										_CXR("Period")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_EFFECT_TYPE	_CXR("\x8e\x8c\x8f\x8b\x85\x82\x8a\x80\x84\x89\x8e\x81\x82\x84\x80\x81\x86\x83\x83\x8c\x8b\x87")
#else
#define MICTOK_ATTRNAME_BUFF_EFFECT_TYPE								_CXR("EffectType")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_HP_VALUE	_CXR("\x86\x8b\x82\x8b\x86\x85")
#else
#define MICTOK_ATTRNAME_BUFF_HP_VALUE									_CXR("HP")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_AP_VALUE	_CXR("\x8b\x89\x81\x8d\x86\x8d")
#else
#define MICTOK_ATTRNAME_BUFF_AP_VALUE									_CXR("AP")
#endif


///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRNAME_BUFF_RESPAWN_DECREASE_TIME	_CXR("\x8b\x8a\x89\x81\x86\x85\x87\x89\x8e\x8b\x82\x86\x89\x85\x8a\x83")
#else
#define MICTOK_ATTRNAME_BUFF_RESPAWN_DECREASE_TIME						_CXR("Respawn")
#endif


///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRVAL_PERIOD_TYPE_SHORT	_CXR("\x80\x8a\x8b\x80\x8b\x8f\x85\x89\x8e\x85\x81\x8e")
#else
#define MICTOK_ATTRVAL_PERIOD_TYPE_SHORT								_CXR("short")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRVAL_PERIOD_TYPE_LONG	_CXR("\x8f\x84\x87\x85\x86\x8c\x80\x8d\x88\x88")
#else
#define MICTOK_ATTRVAL_PERIOD_TYPE_LONG									_CXR("long")
#endif


///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRVAL_BUFF_EFFECT_TYPE_NORMAL	_CXR("\x83\x82\x80\x8f\x84\x81\x8e\x8b\x81\x8b\x88\x82\x84\x8b")
#else
#define MICTOK_ATTRVAL_BUFF_EFFECT_TYPE_NORMAL							_CXR("normal")
#endif

///////////////////////////
#ifdef _USING_CXR
#define MICTOK_ATTRVAL_BUFF_EFFECT_TYPE_DOTE	_CXR("\x8d\x88\x82\x83\x87\x8a\x8a\x87\x8f\x8e")
#else
#define MICTOK_ATTRVAL_BUFF_EFFECT_TYPE_DOTE							_CXR("dote")
#endif
