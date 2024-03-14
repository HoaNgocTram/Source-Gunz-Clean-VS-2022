/*
 CDDBDialog Ŭ����(CDDB.cpp)

  desc : DDB�� �����ϰ� ó���ϴ� Ŭ����.
  date : 2004�� 5�� 30��
  comp : ������б� �����������ȸ(KUCIRA) - �ӵ�ȯ
         http://www.kucira.com
  text : MFC�� �����ϴ� ��� ������ ���� ���� �� ������ �����մϴ�. ��, Ư�� �����̳�
         ��ü�� ������ ���� �������� ���Ǵ� ���� ���մϴ�.

  ������ ������ ��쿣 ������Ʈ�� msimg32.lib�� �߰������ �մϴ�.
*/


#include "stdafx.h"
#include "CDDB.h"



/*************************************************************
 CDDB

  desc : ������
**************************************************************/
CDDB::CDDB()
{
	m_pParentDC			= NULL;						// Set parent window DC

	m_nWidth			= 0;						// Set width
	m_nHeight			= 0;						// Set height
	m_nColor			= RGB( 255, 255, 255);		// Set color
	m_nTransColor		= RGB( 255, 0,   255);		// Set transparent color
	m_CoordinateMode	= CM_RELATIVE;				// Set coordinate mode
}


/*************************************************************
 ~CDDB

  desc : �Ҹ���
**************************************************************/
CDDB::~CDDB()
{
	m_DC.DeleteDC();								// Delete memory DC
	m_pParentDC			= NULL;						// Reset parent window DC pointer

	m_nWidth			= 0;						// Reset width
	m_nHeight			= 0;						// Reset height
	m_nColor			= RGB( 255, 255, 255);		// Reset color
	m_nTransColor		= RGB( 255, 0,   255);		// Reset transparent color
	m_CoordinateMode	= CM_RELATIVE;				// Set coordinate mode
}


/*************************************************************
 CreateDDB

  desc : ������ ũ���� DDB�� �����Ѵ�.
  arg  : pDC = �������� DC ������ �Է�
         width, height = ������ DDB�� ũ��
		 color = DDB�� ä�� ����(RGB) �Է�
  ret  : true(=����) or false(=����)
  ex   : CreateDDB( GetDC(), 200, 100, RGB(255,255,255));
**************************************************************/
bool CDDB::CreateDDB( CDC* pDC, int width, int height, COLORREF color)
{
	if ( !m_DC.CreateCompatibleDC( pDC))			// Create memory DC
		return false;								// Fail


	m_pParentDC = pDC;								// Set parent window DC pointer

	// Create bitmap
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( pDC, width, height);
	m_DC.SelectObject( &bitmap);

	// Initialize
	m_nWidth  = width;								// Set width
	m_nHeight = height;								// Set height
	m_DC.SetBkMode( TRANSPARENT);					// Set text background color

	// Fill color
	m_DC.FillSolidRect( 0, 0, width, height, color);
	m_nColor = color;


	return true;									// Success
}


/*************************************************************
 CreateDDB

  desc : ������ ��Ʈ�� ���ҽ��� ���� DDB�� �����Ѵ�.
  arg  : pDC = �������� DC ������ �Է�
		 nIDResource = �̹��� ���ҽ� ID
  ret  : true(=����) or false(=����)
  ex   : CreateDDB( GetDC(), IDB_BITMAP);
**************************************************************/
bool CDDB::CreateDDB( CDC* pDC, UINT nIDResource)
{
	if ( !m_DC.CreateCompatibleDC( pDC))			// Create memory DC
		return false;								// Fail


	m_pParentDC = pDC;								// Set parent window DC pointer

	// Create bitmap
	CBitmap bitmap;
	if ( !bitmap.LoadBitmap( nIDResource))
		return false;								// Load bitmap fail


	m_DC.SelectObject( &bitmap);					// Create bitmap

	// Initialize
	BITMAP bmpInfo;
	bitmap.GetBitmap( &bmpInfo);					// Get bitmap infomation
	m_nWidth  = bmpInfo.bmWidth;					// Get bitmap width size
	m_nHeight = bmpInfo.bmHeight;					// Get bitmap height size
	m_DC.SetBkMode( TRANSPARENT);					// Set text background color


	return true;									// Success
}


/*************************************************************
 CreateDDB

  desc : ������ ���Ϸκ��� ��Ʈ���� �����ͼ� DDB�� �����Ѵ�.
  arg  : pDC = �������� DC ������ �Է�
		 szBmpFileName = ��Ʈ�� ���� �̸�
  ret  : true(=����) or false(=����)
  ex   : CreateDDB( GetDC(), "..\bitmap.bmp");
**************************************************************/
bool CDDB::CreateDDB( CDC* pDC, LPCTSTR szBmpFileName)
{
	// ���� ���� ������

	return false;
}


/*************************************************************
 DeleteDDB

  desc : DDB�� �����Ѵ�.
  arg  : none
  ret  : true(=����) or false(=����)
  exp  : DeleteDDB();
**************************************************************/
bool CDDB::DeleteDDB()
{
	// Delete DC
	if ( m_DC.DeleteDC() == TRUE)
	{
		m_DC.DeleteDC();							// Delete memory DC
		m_pParentDC			= NULL;					// Reset parent window DC pointer

		m_nWidth			= 0;					// Reset width
		m_nHeight			= 0;					// Reset height
		m_nColor			= RGB( 255, 255, 255);	// Reset color
		m_nTransColor		= RGB( 255, 0, 255);	// Reset transparent color
		m_CoordinateMode	= CM_RELATIVE;			// Set coordinate mode

		return true;								// Success
	}


	return  false;									// Fail
}


/*************************************************************
 Rendering

  desc : DDB �̹����� ��ü�� parent window�� DC�� ����Ѵ�.
  arg  : none
  ret  : none
  ex   : Rendering();
**************************************************************/
void CDDB::Rendering( CDC* pDC)
{
	// Rendering
	if ( m_pParentDC)
	{
		// Copy memory bitmap
		pDC->BitBlt( 0, 0, m_nWidth, m_nHeight, &m_DC, 0, 0, SRCCOPY);
	}
}


/*************************************************************
 Rendering

  desc : DDB �̹����� �Ϻ� ������ parent window�� DC�� ����Ѵ�.
  arg  : x, y = �̹����� ��µ� parent window DC�� ���� ��ǥ
         xSrc, ySrc = ����� �̹����� ���� ��ǥ �Է�
         xSrc, ySrc = ����� �̹����� ũ�� Ȥ�� �� ��ǥ
  ret  : none
  ex   : Rendering( 200, 100, 300, 400, 100, 100);
**************************************************************/
void CDDB::Rendering( CDC* pDC, int x, int y, int xSrc, int ySrc, int dxSrc, int dySrc)
{
	// Coordinate mode is not relative
	if ( m_CoordinateMode != CM_RELATIVE)
		GetRelativeCoordinate( &xSrc, &ySrc, &dxSrc, &dySrc);


	// Rendering
	if ( m_pParentDC)
	{
		// Copy memory bitmap
		pDC->BitBlt( x, y, dxSrc, dySrc, &m_DC, xSrc, ySrc, SRCCOPY);
	}
}


/*************************************************************
 PutBitmap

  desc : DDB�� ������ ��ġ�� �ٸ� DDB�κ��� �̹��� �Ϻ� ������ �����Ѵ�.
  arg  : x, y = ����� ������ ��ġ ��ǥ �Է�
		 *pDC = ����� �̹����� ������ DC ������ �Է�
         xSrc, ySrc = Source DC���� �̹����� ������ ���� ��ǥ �Է�
		 dxSrc, dySrc = Source DC���� �̹����� ������ ũ�� Ȥ�� �� ��ǥ
 		 width, height = ������ �̹����� ����ϱ� ���� ũ��
		 dwRop = ������ ���۷��̼�(TRANSPARENT=����� ó��, SRCCOPY, SRCAND, SRCOR...)
		 alpha = ����
  ret  : true(=����) or false(=����)
  ex   : PutBitmap( 100, 100, GetDC(), 200, 200, 300, 300, SRCCOPY, 255);
**************************************************************/
bool CDDB::PutBitmap( int x, int y, CDC *pDC, int xSrc, int ySrc, int dxSrc, int dySrc, DWORD dwRop, int alpha)
{
	return PutBitmap( x, y, pDC, xSrc, ySrc, dxSrc, dySrc, dxSrc, dySrc, dwRop, alpha);
}

bool CDDB::PutBitmap( int x, int y, CDC *pDC, int xSrc, int ySrc, int dxSrc, int dySrc, int width, int height, DWORD dwRop, int alpha)
{
	// Coordinate mode is not relative
	if ( m_CoordinateMode != CM_RELATIVE)
		GetRelativeCoordinate( &xSrc, &ySrc, &dxSrc, &dySrc);


	// Copy alphablended memory DC
	if ( alpha < 255)
	{
		BLENDFUNCTION blendFunc = { AC_SRC_OVER, 0, alpha, 0 };

		// Copy memory bitmap
		AlphaBlend( m_DC, x, y, width, height, pDC->m_hDC, xSrc, ySrc, dxSrc, dySrc, blendFunc);
	}

	// Copy transparent memory DC
	else if ( dwRop == TRANSPARENT)
	{
		// Copy memory bitmap
		TransparentBlt( m_DC, x, y, width, height, pDC->m_hDC, xSrc, ySrc, dxSrc, dySrc, m_nTransColor);
	}

	// Copy memory DC
	else
	{
		// Copy memory bitmap
		m_DC.StretchBlt( x, y, width, height, pDC, xSrc, ySrc, dxSrc, dySrc, dwRop);
	}


	return true;
}


/*************************************************************
 PutBitmap

  desc : DDB�� ������ ��Ʈ�� ���ҽ��� �Ϻ� ������ ����Ѵ�.
  arg  : x, y = DDB�� �̹����� ����� ���� ��ǥ
		 nIDResource = ��Ʈ�� ���ҽ� ID
         xSrc, ySrc = Source DC���� �̹����� ������ ���� ��ǥ �Է�
		 dxSrc, dySrc = Source DC���� �̹����� ������ ũ�� Ȥ�� �� ��ǥ
		 width, height = ������ �̹����� ����ϱ� ���� ũ��
		 dwRop = ������ ���۷��̼�(TRANSPARENT=����� ó��, SRCCOPY, SRCAND, SRCOR...)
		 alpha = ����
  ret  : true(=����) or false(=����)
  ex   : PutBitmap( 200, 100, IDB_BITMAP, 300, 400, 100, 100, 200, 200, SRCCOPY, 255);
**************************************************************/
bool CDDB::PutBitmap( int x, int y, UINT nIDResource, int xSrc, int ySrc, int dxSrc, int dySrc, DWORD dwRop, int alpha)
{
	return PutBitmap( x, y, nIDResource, xSrc, ySrc, dxSrc, dySrc, dxSrc, dySrc, dwRop, alpha);
}

bool CDDB::PutBitmap( int x, int y, UINT nIDResource, int xSrc, int ySrc, int dxSrc, int dySrc, int width, int height, DWORD dwRop, int alpha)
{
	// Get bitmap resource
	CDC NewDC;
	NewDC.CreateCompatibleDC( &m_DC);
	
	CBitmap NewBmp;
	if ( !NewBmp.LoadBitmap( nIDResource))
		return false;								// Fail


	CBitmap *pOldBmp = (CBitmap *)NewDC.SelectObject( &NewBmp);
	BITMAP bmpInfo;
	NewBmp.GetBitmap(&bmpInfo);


	// Put bitmap
	bool bRetVal = PutBitmap( x, y, &NewDC, xSrc, ySrc, dxSrc, dySrc, width, height, dwRop, alpha);

	NewDC.SelectObject( pOldBmp);


	return bRetVal;
}


/*************************************************************
 FillColor

  desc : DDB ��ü�� ������ ������ ä���.
  arg  : color = ä�� ����(RGB)
  ret  : none
  ex   : FillColor( RGB(255,255,255));
**************************************************************/
void CDDB::FillColor( COLORREF color)
{
	m_DC.FillSolidRect( 0, 0, m_nWidth, m_nHeight, color);
}


/*************************************************************
 FillTiledBitmap

  desc : ������ ��Ʈ�� ���ҽ��� Ÿ�� ���·� ������ ������ ä���.
  arg  : x, y = ��Ʈ������ ä���� ���� ���� ��ǥ
         dx, dy = ��Ʈ������ ä���� ���� �� ��ǥ Ȥ�� ũ��
		 pDC = �̹����� ������ source DC�� ������ �Է�
		 xSrc, ySrc = Source DC���� �̹����� ������ ���� ��ǥ
		 dxSrc, dySrc = Source DC���� �̹����� ������ ũ�� Ȥ�� �� ��ǥ
  ret  : true(=����) or false(=����)
  ex   : FillTiledBitmap( 100, 100, 200, 200, GetDC(), 50, 50, 100, 100);
**************************************************************/
bool CDDB::FillTiledBitmap( int x, int y, int dx, int dy, CDC *pDC, int xSrc, int ySrc, int dxSrc, int dySrc)
{
	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);

	// Coordinate mode is not relative
	if ( m_CoordinateMode != CM_RELATIVE)
		GetRelativeCoordinate( &xSrc, &ySrc, &dxSrc, &dySrc);


	// Fill with bitmap
	int px, py;
	int width, height;
	for ( py = y;  py < dy;  py += dySrc)			// Get size y
	{
		height = dy-py;
		if ( height > dySrc)
			height = dySrc;

		for ( px = x;  px < dx;  px += dxSrc)		// Get size x
		{
			width = dx-px;
			if ( width > dxSrc)
				width = dxSrc;

			// Copy memory bitmap
			m_DC.BitBlt( px, py, width, height, pDC, xSrc, ySrc, SRCCOPY);
		}
	}


	return true;
}


/*************************************************************
 FillTiledBitmap

  desc : DDB ��ü�� ������ ��Ʈ�� ���ҽ��� Ÿ�� ���·� ä���.
  arg  : x, y = ��Ʈ������ ä���� ���� ���� ��ǥ
         dx, dy = ��Ʈ������ ä���� ���� �� ��ǥ Ȥ�� ũ��
		 nIDResource = ä�� ��Ʈ�� ���ҽ� ID
  		 xSrc, ySrc = Source DC���� �̹����� ������ ���� ��ǥ
		 dxSrc, dySrc = Source DC���� �̹����� ������ ũ�� Ȥ�� �� ��ǥ
  ret  : true(=����) or false(=����)
  ex   : FillTiledBitmap(  100, 100, 200, 200, IDB_BITMAP, 50, 50, 100, 100);
**************************************************************/
bool CDDB::FillTiledBitmap( int x, int y, int dx, int dy, UINT nIDResource, int xSrc, int ySrc, int dxSrc, int dySrc)
{
	// Get bitmap resource
	CDC NewDC;
	NewDC.CreateCompatibleDC( &m_DC);
	
	CBitmap NewBmp;
	if ( !NewBmp.LoadBitmap( nIDResource))
		return false;								// Fail


	CBitmap *pOldBmp = (CBitmap *)NewDC.SelectObject( &NewBmp);
	BITMAP bmpInfo;
	NewBmp.GetBitmap( &bmpInfo);


	// Fill with bitmap resource
	BOOL bRetVal = FillTiledBitmap( x, y, dx, dy, &NewDC, xSrc, ySrc, dxSrc, dySrc);

	NewDC.SelectObject( pOldBmp);


	return ( (bRetVal > 0) ? true : false);
}


/*************************************************************
 PutPixel

  desc : DDB�� ���� ����Ѵ�.
  arg  : x, y = DDB�� ���� ����� ��ǥ
         color = �� ����
  ret  : none
  ex   : PutPixel( 200, 100, RGB(255,255,255));
**************************************************************/
void CDDB::PutPixel( int x, int y, COLORREF color)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Draw a pixel
	m_DC.SetPixel( x, y, m_nColor);					// Put pixel
}


/*************************************************************
 GetPixel

  desc : DDB���� ������ ��ǥ�� ����(RGB)�� ���Ѵ�.
  arg  : x, y = ������ ���� ��ǥ(pixel)
  ret  : pixel�� ����
  ex   : COLORREF Color = GetPixel( 200, 100);
**************************************************************/
COLORREF CDDB::GetPixel( int x, int y)
{
	return m_DC.GetPixel( x, y);					// Get pixel
}


/*************************************************************
 PutLine

  desc : DDB�� ���� ����Ѵ�.
  arg  : x, y = DDB�� ���� ����� ���� ��ǥ
         dx, dy = DDB�� ���� ����� �� ��ǥ Ȥ�� ũ��
         color = �� ����(RGB)
		 width = �� �β�
  ret  : none
  ex   : PutLine( 100, 100, 200, 200, RGB(255,255,255), 1);
**************************************************************/
void CDDB::PutLine( int x, int y, int dx, int dy, COLORREF color, int width)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color

	
	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);


	// Draw a line
	CPen pen;
	pen.CreatePen( PS_SOLID, width, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	m_DC.MoveTo( x,	 y);
	m_DC.LineTo( dx, dy);
	m_DC.SelectObject( pOldPen);
}


/*************************************************************
 PutBox

  desc : DDB�� �簢���� ����Ѵ�.
  arg  : x, y = DDB�� �簢���� ����� ���� ��ǥ
         dx, dy = DDB�� �簢���� ����� �� ��ǥ Ȥ�� ũ��
         color = �簢�� ����(RGB)
		 width = �� �β�
  ret  : none
  ex   : PutBox( 100, 100, 200, 200, RGB(255,255,255), 1);
**************************************************************/
void CDDB::PutBox( int x, int y, int dx, int dy, COLORREF color, int width)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);


	// Draw a box
	CPen pen;
	pen.CreatePen( PS_SOLID, width, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	m_DC.MoveTo( x,		y);
	m_DC.LineTo( dx-1,	y);
	m_DC.LineTo( dx-1,	dy-1);
	m_DC.LineTo( x,		dy-1);
	m_DC.LineTo( x,		y-1);
	m_DC.SelectObject( pOldPen);
}


/*************************************************************
 PutFillBox

  desc : DDB�� ���θ� ä�� �簢���� ����Ѵ�.
  arg  : x, y = DDB�� �簢���� ����� ���� ��ǥ
         dx, dy = DDB�� �簢���� ����� �� ��ǥ
         color = �簢�� ����(RGB)
  ret  : none
  ex   : PutFillBox( 200, 100, 100, 100, RGB(255,255,255));
**************************************************************/
void CDDB::PutFillBox( int x, int y, int dx, int dy, COLORREF color)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color

	
	// Coordinate mode is not relative
	if ( m_CoordinateMode != CM_RELATIVE)
		GetRelativeCoordinate( &x, &y, &dx, &dy);


	// Draw a rectangle
	m_DC.FillSolidRect( x, y, dx, dy, m_nColor);
}


/*************************************************************
 PutRoundBox

  desc : DDB�� �ձ� �簢���� ����Ѵ�.
  arg  : x, y = DDB�� �ձ� �簢���� ����� ���� ��ǥ
         dx, dy = DDB�� �簢���� ����� �� ��ǥ Ȥ�� ũ��
		 radius = �𼭸� ���� ����
         color = �簢�� ����(RGB)
		 width = �� �β�
  ret  : none
  ex   : PutRoundBox( 100, 100, 200, 200, 5, RGB(255,255,255), 1);
**************************************************************/
void CDDB::PutRoundBox( int x, int y, int dx, int dy, int radius, COLORREF color, int width)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color

	
	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);


	// Draw a round box
	CPen pen;
	pen.CreatePen( PS_SOLID, width, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	m_DC.MoveTo( x+radius, y); 
	m_DC.AngleArc( x+radius, y+radius, radius, 90, 90); 
	m_DC.MoveTo( dx-1, y+radius); 
	m_DC.AngleArc( dx-radius-1, y+radius, radius, 0, 90); 
	m_DC.MoveTo( x, dy-radius-1); 
	m_DC.AngleArc( x+radius, dy-radius-1, radius, 180, 90); 
	m_DC.MoveTo( dx-radius-1, dy-1); 
	m_DC.AngleArc( dx-radius-1, dy-radius-1, radius, 270, 90); 

	m_DC.MoveTo( x+radius,		y); 
	m_DC.LineTo( dx-radius+1,	y); 
	m_DC.MoveTo( x+radius,		dy-1); 
	m_DC.LineTo( dx-radius+1,	dy-1); 
	m_DC.MoveTo( x,				y+radius); 
	m_DC.LineTo( x,				dy-radius+1); 
	m_DC.MoveTo( dx-1,			y+radius); 
	m_DC.LineTo( dx-1,			dy-radius+1); 

	m_DC.SelectObject( pOldPen);
}


/*************************************************************
 PutFillRoundBox

  desc : DDB�� �簢���� ����Ѵ�.
  arg  : x1, y1 = DDB�� �簢���� ����� ���� ��ǥ
         x2_or_width, y2_or_height = DDB�� �簢���� ����� �� ��ǥ Ȥ�� ũ��
		 width = �� �β�
         color = �簢�� ����(RGB)
  ret  : none
  ex   : PutFillRoundBox( 100, 100, 200, 200, 5, RGB(255,255,255));
**************************************************************/
void CDDB::PutFillRoundBox( int x, int y, int dx, int dy, int radius, COLORREF color)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);


	// Draw a round box
	CPen pen;
	pen.CreatePen( PS_SOLID, 1, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	CBrush brush;
	brush.CreateSolidBrush( m_nColor);
	CBrush *pOldBrush = (CBrush *)m_DC.SelectObject( &brush);


	m_DC.RoundRect( x, y, dx, dy, radius, radius);

	m_DC.SelectObject( pOldPen);
	m_DC.SelectObject( &pOldBrush);
}


/*************************************************************
 PutCircle

  desc : DDB�� ���� ����Ѵ�.
  arg  : x, y = ���� ����� �߽���
         radius = ���� ������
         color = ���� ����(RGB)
		 width = �� �β�
  ret  : none
  ex   : PutCircle( 100, 100, 50, RGB(0,0,0), 1);
  err  : ���� ������� ���μ��� 1pixel�� ũ�� ������.
**************************************************************/
void CDDB::PutCircle( int x, int y, int radius, COLORREF color, int width)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Draw a circle
	CPen pen;
	pen.CreatePen( PS_SOLID, width, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	m_DC.MoveTo( x+radius, y); 
	m_DC.AngleArc( x, y, radius, 0, 360); 

	m_DC.SelectObject( pOldPen);
}


/*************************************************************
 PutFillCircle

  desc : DDB�� ���θ� ä�� ���� ����Ѵ�.
  arg  : x, y = ���� ����� �߽���
         radius = ���� ������
         color = ���� ����(RGB)
  ret  : none
  ex   : PutFillCircle( 100, 100, 50, RGB(0,0,0));
**************************************************************/
void CDDB::PutFillCircle( int x, int y, int radius, COLORREF color)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Draw a circle
	CPen pen;
	pen.CreatePen( PS_SOLID, 1, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	CBrush brush;
	brush.CreateSolidBrush( m_nColor);
	CBrush *pOldBrush = (CBrush *)m_DC.SelectObject( &brush);

	m_DC.Ellipse( x-radius, y-radius, x+radius, y+radius);

	m_DC.SelectObject( pOldPen);
	m_DC.SelectObject( pOldBrush);
}


/*************************************************************
 PutFillEllips

  desc : DDB�� ���θ� ä�� Ÿ���� ����Ѵ�.
  arg  : x, y = Ÿ���� ����� �߽���
         radiusX, radiusY = ���� x,y�� ������
         color = ���� ����(RGB)
  ret  : none
  ex   : PutFillEllips( 100, 100, 50, 30, RGB(0,0,0));
**************************************************************/
void CDDB::PutFillEllips( int x, int y, int rx, int ry, COLORREF color)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Draw a ellips
	CPen pen;
	pen.CreatePen( PS_SOLID, 1, m_nColor);
	CPen *pOldPen = (CPen *)m_DC.SelectObject( &pen);

	CBrush brush;
	brush.CreateSolidBrush( m_nColor);
	CBrush *pOldBrush = (CBrush *)m_DC.SelectObject( &brush);

	m_DC.Ellipse( x-rx, y-ry, x+rx, y+ry);

	m_DC.SelectObject( pOldPen);
	m_DC.SelectObject( pOldBrush);
}


/*************************************************************
 PutText

  desc : DDB�� �۾��� ����Ѵ�.
  arg  : x, y = DDB�� �۾��� ����� ���� ��ǥ
         nSize = �۾� ũ��
		 lpszFont = �۾� ��Ʈ
		 lpszMsg = ��� ����
         color = ����
		 nStyle = �۾� ��Ÿ�� ( TS_BOLD | TS_ITALIC | TS_UDLINE)
  ret  : none
  ex   : PutText( 200, 100, 14, "����", "PutText ����", RGB(255,255,255), TS_BOLD);
**************************************************************/
void CDDB::PutText( int x, int y, int nSize, LPCTSTR lpszFont, LPCTSTR lpszMsg, COLORREF color, UINT nStyle)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color

	
	// Put text
	UINT nBold		= (nStyle & TS_BOLD)   ? FW_BOLD : FW_NORMAL;
	BYTE bItalic	= (nStyle & TS_ITALIC) ? true : false;
	BYTE bUnderLine	= (nStyle & TS_UDLINE) ? true : false;

	CFont font;
	font.CreateFont( nSize, 0, 0, 0, nBold, bItalic, bUnderLine, 0, ANSI_CHARSET,
	                 OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                 DEFAULT_PITCH | FF_SWISS, lpszFont);

	CFont *pOldFont = (CFont *)m_DC.SelectObject( &font);

	m_DC.SetTextColor( m_nColor);
	m_DC.TextOut( x, y, lpszMsg);
	m_DC.SelectObject( pOldFont);
}


/*************************************************************
 PutText

  desc : DDB�� ������ ���� �ȿ��� �۾��� ����Ѵ�.
  arg  : x, y = �۾��� ��µ� ������ ���� ��ǥ
         dx, dy = �۾��� ����� ������ �� ��ǥ Ȥ�� ũ��
         nSize = �۾� ũ��
		 lpszFont = �۾� ��Ʈ
		 lpszMsg = ��� ����
         color = ����
		 nStyle = �۾� ��Ÿ�� ( TS_BOLD | TS_ITALIC | TS_UDLINE)
		 nAlign = �۾� ����( DT_LEFT | DT_CENTER | DT_RIGHT | DT_TOP | DT_VCENTER | DT_BOTTOM)
  ret  : none
  ex   : PutText( 200, 100, 300, 200, 14, "����", "PutText ����", TS_BOLD, DT_CENTER|DT_VCENTER, RGB(255,255,255), TS_BOLD,  DT_CENTER);
**************************************************************/
void CDDB::PutText( int x, int y, int dx, int dy, int nSize, LPCTSTR lpszFont, LPCTSTR lpszMsg, COLORREF color, UINT nStyle, UINT nAlign)
{
	if ( color != NOCOLOR)
		m_nColor = color;							// Set color


	// Coordinate mode is not absolute
	if ( m_CoordinateMode != CM_ABSOLUTE)
		GetAbsoluteCoordinate( &x, &y, &dx, &dy);


	// Put text
	UINT nBold		= (nStyle & TS_BOLD)   ? FW_BOLD : FW_NORMAL;
	BYTE bItalic	= (nStyle & TS_ITALIC) ? true : false;
	BYTE bUnderLine	= (nStyle & TS_UDLINE) ? true : false;

	CFont font;
	font.CreateFont( nSize, 0, 0, 0, nBold, bItalic, bUnderLine, 0, ANSI_CHARSET,
	                 OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                 DEFAULT_PITCH | FF_SWISS, lpszFont);

	CFont *pOldFont = (CFont *)m_DC.SelectObject( &font);
	
	CRect rect;
	rect.top		= y;
	rect.left		= x;
	rect.right		= dx;
	rect.bottom		= dy;

	m_DC.SetTextColor( m_nColor);
	m_DC.DrawText( lpszMsg, (LPRECT)rect, nAlign);
	m_DC.SelectObject( pOldFont);
}



/*************************************************************
 GetRelativeCoordinate

  desc : ���� ��ǥ ���� ��� ��ǥ ������ ��ȯ�Ѵ�
  arg  : *x1, *y1 = ���� ��ǥ
         *x2, *y2 = �� ��ǥ
  ret  : none
**************************************************************/
void CDDB::GetRelativeCoordinate( int *x1, int *y1, int *x2, int *y2)
{
	*x2 = *x2 - *x1;							// Get width
	*y2 = *y2 - *y1;							// Get height
}


/*************************************************************
 GetAbsoluteCoordinate

  desc : ��� ��ǥ ���� ���� ��ǥ ������ ��ȯ�Ѵ�
  arg  : *x1, *y1 = ���� ��ǥ
         *width, *height = ����/����
  ret  : none
**************************************************************/
void CDDB::GetAbsoluteCoordinate( int *x1, int *y1, int *width, int *height)
{
	*width  = *x1 + *width;						// Get end x
	*height = *y1 + *height;					// Get end y
}
