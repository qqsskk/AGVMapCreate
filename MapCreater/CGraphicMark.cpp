/*!
 * @file CGraphicMark.cpp
 * @brief CGraphicMark类执行文件
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#include "stdafx.h"
#include "CGraphicMark.h"

#ifdef _CGRAPHICMARK_H

unsigned int CGraphicMark::g_unWidth = 10;	/*!< 宽 */
unsigned int CGraphicMark::g_unHeight = 10;	/*!< 高 */

CGraphicMark::CGraphicMark(unsigned short usNo, CPoint ptCenter)
{
	m_usNo = usNo;

	m_ptCenter = ptCenter;
	m_ptPrepoint = CPoint(0, 0);
	m_ptLastpoint = CPoint(0, 0);
	m_ptLocal = CPoint(0, 0);
	m_bMove = false;
	m_bSelect = false;
}

CGraphicMark::~CGraphicMark()
{
}

unsigned short CGraphicMark::GetNo() const
{
	return m_usNo;
}

bool CGraphicMark::IsInside(CPoint ptWinpoint)
{
	CPoint ptWindow = VecToWin(m_ptCenter);
	CRect rect;
	rect.left = LONG(ptWindow.x - (g_unWidth / 2) * g_fScale);
	rect.top = LONG(ptWindow.y - (g_unHeight / 2) * g_fScale);
	rect.right = LONG(rect.left + g_unWidth * g_fScale);
	rect.bottom = LONG(rect.top + g_unHeight * g_fScale);

	return rect.PtInRect(ptWinpoint);
}

bool CGraphicMark::IsSelected()
{
	return m_bSelect;
}

void CGraphicMark::Select(CPoint ptWinpoint)
{
	m_ptPrepoint = ptWinpoint;
	m_ptLastpoint = ptWinpoint;

	m_bMove = true;
	m_bSelect = true;

	return;
}

HCURSOR CGraphicMark::Drag(CPoint ptWinPoint)
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

void CGraphicMark::Confirm()
{
	m_bMove = false;

	m_ptCenter = CGraph::Correct(m_ptCenter);

	return;
}

void CGraphicMark::Cancel()
{
	m_ptCenter = m_ptLocal;
	m_bMove = false;
	m_bSelect = false;

	return;
}

CPoint CGraphicMark::GetCenterPoint() const
{
	return m_ptCenter;
}

void CGraphicMark::SetCenterPoint(CPoint ptCenter)
{
	m_ptCenter = ptCenter;
	m_ptLocal = m_ptCenter;

	return;
}

void CGraphicMark::Draw(CDC * pDC)
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
		g.FillRectangle(pBrush,Rect(rect.left,rect.top,rect.Width(),rect.Height()));
	}

	/*!
	 * 绘制底圈
	*/

	// 设置为白色
	pBrush->SetColor(Color::White);

	g.FillEllipse(pBrush, Rect(rect.left, rect.top, rect.Width(), rect.Height()));

	/*!
	 * 绘制外环
	*/
	Pen pen(Color::Black, 2.0f);

	g.DrawEllipse(&pen, Rect(rect.left, rect.top, rect.Width(), rect.Height()));

	/*!
	 * 绘制编号
	*/
	Color gcrFont(Color::Black);

	FontFamily fontFamily(_T("宋体"));
	Gdiplus::Font font(&fontFamily, (g_unWidth / 3) * g_fScale, FontStyleRegular, UnitPoint);

	StringFormat gsFormat;
	gsFormat.SetAlignment(StringAlignmentCenter);
	gsFormat.SetLineAlignment(StringAlignmentCenter);

	CString cstrNo = _T("");
	cstrNo.Format(_T("%d"), m_usNo);

	pBrush->SetColor(gcrFont);
	g.DrawString(cstrNo, cstrNo.GetLength(), &font, RectF((REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);

	delete pBrush;

	return;
}

#endif //!_CGRIPHICMARK_H