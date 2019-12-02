#include "stdafx.h"
#include "CGraphicRest.h"

#ifdef _CGRAPHICREST_H

unsigned int CGraphicRest::g_unWidth = 20;
unsigned int CGraphicRest::g_unHeight = 20;
CString CGraphicRest::g_cstrPath = _T("../Image/rest.png");

CGraphicRest::CGraphicRest(unsigned char byNo, unsigned short usMark, CString cstrName)
{
	m_byNo = byNo;
	m_usMark = usMark;
	m_cstrName = cstrName;

	m_ptCenter = CPoint(0, 0);
	m_ptPrepoint = CPoint(0, 0);
	m_ptLastpoint = CPoint(0, 0);
	m_ptLocal = CPoint(0, 0);
	m_bMove = false;
	m_bSelect = false;
}


CGraphicRest::~CGraphicRest()
{
}

unsigned char CGraphicRest::GetNo() const
{
	return m_byNo;
}

void CGraphicRest::SetName(CString cstrName)
{
	m_cstrName = cstrName;

	return;
}

CString CGraphicRest::GetName() const
{
	return m_cstrName;
}

void CGraphicRest::SetMark(unsigned short usMark)
{
	m_usMark = usMark;

	return;
}

unsigned short CGraphicRest::GetMark() const
{
	return m_usMark;
}

void CGraphicRest::SetImage(CString cstrPath)
{
	g_cstrPath = cstrPath;

	return;
}

CString CGraphicRest::GetImage()
{
	return g_cstrPath;
}

bool CGraphicRest::IsInside(CPoint ptWinpoint)
{
	CPoint ptWindow = VecToWin(m_ptCenter);
	CRect rect;
	rect.left = LONG(ptWindow.x - (g_unWidth / 2) * g_fScale);
	rect.top = LONG(ptWindow.y - (g_unHeight / 2) * g_fScale);
	rect.right = LONG(rect.left + g_unWidth * g_fScale);
	rect.bottom = LONG(rect.top + g_unHeight * g_fScale);

	return rect.PtInRect(ptWinpoint);
}

bool CGraphicRest::IsSelected()
{
	return m_bSelect;
}

void CGraphicRest::Select(CPoint ptWinpoint)
{
	m_ptPrepoint = ptWinpoint;
	m_ptLastpoint = ptWinpoint;

	m_bMove = true;
	m_bSelect = true;

	return;
}

HCURSOR CGraphicRest::Drag(CPoint ptWinPoint)
{
	HCURSOR hCur = nullptr;

	m_ptLastpoint = ptWinPoint;

	if (m_bMove)
	{
		m_ptCenter.x = LONG((m_ptLastpoint.x - m_ptPrepoint.x) / g_fScale + m_ptLocal.x);
		m_ptCenter.y = LONG(-(m_ptLastpoint.y - m_ptPrepoint.y) / g_fScale + m_ptLocal.y);

		if (m_ptCenter.x < -g_sizeVector.cx / 2
			|| m_ptCenter.y > g_sizeVector.cy / 2
			|| m_ptCenter.x > g_sizeVector.cx / 2
			|| m_ptCenter.y < -g_sizeVector.cy / 2)
		{
			m_ptCenter = m_ptLocal;
		}

		hCur = LoadCursor(NULL, IDC_HAND);
	}
	else
	{
		m_ptLocal = m_ptCenter;
	}

	return 	hCur;
}

void CGraphicRest::Confirm()
{
	m_bMove = false;

	m_ptCenter = CGraph::Correct(m_ptCenter);

	return;
}

void CGraphicRest::Cancel()
{
	m_ptCenter = m_ptLocal;
	m_bMove = false;
	m_bSelect = false;

	return;
}

CPoint CGraphicRest::GetCenterPoint()
{
	return m_ptCenter;
}

void CGraphicRest::SetCenterPoint(CPoint ptCenter)
{
	m_ptCenter = ptCenter;
	m_ptLocal = m_ptCenter;

	return;
}

void CGraphicRest::Draw(CDC * pDC)
{
	Graphics g(pDC->GetSafeHdc());

	CPoint ptWindow = VecToWin(m_ptCenter);
	CRect rect;
	rect.left = LONG(ptWindow.x - (g_unWidth / 2) * g_fScale);
	rect.top = LONG(ptWindow.y - (g_unHeight / 2) * g_fScale);
	rect.right = LONG(rect.left + g_unWidth * g_fScale);
	rect.bottom = LONG(rect.top + g_unHeight * g_fScale);

	/*!
	 * 绘制选中背景
	*/
	SolidBrush *pBrush = new SolidBrush(Color::Red);

	if (m_bSelect)
	{
		g.FillRectangle(pBrush, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	}

	Image *pImage = Image::FromFile(g_cstrPath);

	/*!
	 * 绘制底圈
	*/
	g.DrawImage(pImage, Rect(rect.left, rect.top, rect.Width(), rect.Height()));

	delete pImage;

	/*!
	 * 绘制编号
	*/
	Color gcrFont(Color::Red);

	FontFamily fontFamily(_T("宋体"));
	Gdiplus::Font font(&fontFamily, (g_unWidth / 10) * g_fScale, FontStyleRegular, UnitPoint);

	StringFormat gsFormat;
	gsFormat.SetAlignment(StringAlignmentCenter);
	gsFormat.SetLineAlignment(StringAlignmentFar);

	CString cstrNo = _T("");
	cstrNo.Format(_T("待机位：%d"), m_byNo);

	pBrush->SetColor(gcrFont);
	g.DrawString(cstrNo, cstrNo.GetLength(), &font, RectF((REAL)rect.left, (REAL)(rect.top - rect.Height()), (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);

	/*!
	 * 绘制名称
	*/
	gsFormat.SetAlignment(StringAlignmentCenter);
	gsFormat.SetLineAlignment(StringAlignmentNear);

	pBrush->SetColor(gcrFont);
	g.DrawString(m_cstrName, m_cstrName.GetLength(), &font, RectF((REAL)rect.left, (REAL)rect.bottom, (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);

	delete pBrush;

	return;
}

#endif //!_CGRAPHICREST_H