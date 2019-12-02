/*!
 * @file CGraphicBackground.cpp
 * @brief CGraphicBackground类执行文件
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#include "stdafx.h"
#include "CGraphicBackground.h"

#ifdef _CGRAPHICBACKGROUND_H

CGraphicBackground::CGraphicBackground(Color colorLine, float fWidth, CString cstrPath)
{
	m_colorLine = colorLine;
	m_fWidth = fWidth;
	m_cstrPath = cstrPath;
	m_bMove = false;
	m_ptPrepoint = CPoint(0,0);
	m_ptLastpoint = CPoint(0, 0);
	m_ptLocal = CPoint(0, 0);
	
	m_pImage = new Image(m_cstrPath);
	m_pPen = new Pen(m_colorLine, m_fWidth);
}


CGraphicBackground::~CGraphicBackground()
{
	delete m_pImage;
	delete m_pPen;
}

void CGraphicBackground::SetLine(Color colorLine, float fWidth)
{
	lock_guard<mutex> lock(m_mutexLock);

	m_colorLine = colorLine;
	m_fWidth = fWidth;

	delete m_pPen;

	m_pPen = new Pen(m_colorLine, m_fWidth);

	return;
}

void CGraphicBackground::GetLine(Color & colorLine, float & fWidth)
{
	colorLine = m_colorLine;
	fWidth = m_fWidth;

	return;
}

void CGraphicBackground::SetImage(CString cstrPath)
{
	lock_guard<mutex> lock(m_mutexLock);

	m_cstrPath = cstrPath;

	delete m_pImage;

	m_pImage = new Image(m_cstrPath);

	return;
}

CString CGraphicBackground::GetImage()
{
	return m_cstrPath;
}

void CGraphicBackground::Scale(bool bShrink)
{
	if (bShrink)
	{
		if (g_fScale >= MIN_SCALE)
		{
			g_fScale -= 0.1f;
		}
	}
	else
	{
		if (g_fScale <= MAX_SCALE)
		{
			g_fScale += 0.1f;
		}
	}

	if (g_fScale == 0)
	{
		if (bShrink)
		{
			g_fScale -= 0.1f;
		}
		else
		{
			g_fScale += 0.1f;
		}
	}

	return;
}

void CGraphicBackground::Reset()
{
	g_fScale = DEFALUT_SCALE;
	g_ptOffset = CPoint(0, 0);

	return;
}

HCURSOR CGraphicBackground::Drag(CPoint ptWinPoint)
{
	HCURSOR hCur = nullptr;

	m_ptLastpoint = ptWinPoint;

	if (m_bMove)
	{
		g_ptOffset = m_ptLastpoint - m_ptPrepoint + m_ptLocal;

		CPoint ptLeftTop = WinToVec(CPoint(g_rcClient.left, g_rcClient.top));
		CPoint ptRightBottom = WinToVec(CPoint(g_rcClient.right, g_rcClient.bottom));

		if (ptLeftTop.x < -g_sizeVector.cx / 2
			|| ptLeftTop.y > g_sizeVector.cy / 2
			|| ptRightBottom.x > g_sizeVector.cx / 2
			|| ptRightBottom.y < -g_sizeVector.cy / 2)
		{
			g_ptOffset = m_ptLocal;
		}

		hCur = LoadCursor(NULL, IDC_HAND);
	}
	else
	{
		m_ptLocal = g_ptOffset;
	}

	return hCur;
}

void CGraphicBackground::Select(CPoint ptWinPoint)
{
	m_ptPrepoint = ptWinPoint;
	m_ptLastpoint = ptWinPoint;
	
	m_bMove = true;

	return;
}

void CGraphicBackground::Cancel()
{
	g_ptOffset = m_ptLocal;
	m_bMove = false;

	return;
}

void CGraphicBackground::Confirm()
{
	m_bMove = false;

	return;
}

void CGraphicBackground::DrawBackground(CDC * pDC)
{
	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	Graphics g(pDC->GetSafeHdc());

	g.DrawImage(m_pImage, RectF((REAL)g_rcClient.left, (REAL)g_rcClient.top, (REAL)g_rcClient.Width(), (REAL)g_rcClient.Height()));

	m_mutexLock.unlock();

	return;
}

void CGraphicBackground::DrawLine(CDC * pDC)
{
	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	Graphics g(pDC->GetSafeHdc());

	// 设置实线
	m_pPen->SetDashStyle(DashStyleSolid);
	// 设置宽度
	m_pPen->SetWidth(m_fWidth);
	// 设置画笔颜色
	m_pPen->SetColor(m_colorLine);

	CPoint cptStart = CPoint(0,0);	/*!< 起点坐标 */
	CPoint cptEnd = CPoint(0,0);	/*!< 终点坐标 */

	Point ptStart = Point(0, 0);	/*!< 起点坐标 GDI+ */
	Point ptEnd = Point(0, 0);		/*!< 终点坐标 GDI+ */
	
	/*!
	 * 绘制实线
	*/
	
	// 从左向右绘制实线(竖线)
	for (int x = -g_sizeVector.cx / 2; x <= g_sizeVector.cx / 2; x += g_unDistance * 10)
	{
		cptStart = VecToWin(CPoint(x, g_sizeVector.cy / 2));
		cptEnd = VecToWin(CPoint(x, -g_sizeVector.cy / 2));
		ptStart.X = cptStart.x;
		ptStart.Y = cptStart.y;
		ptEnd.X = cptEnd.x;
		ptEnd.Y = cptEnd.y;

		ptStart.Y = ptStart.Y < g_rcClient.top ? g_rcClient.top : ptStart.Y;
		ptEnd.Y = ptEnd.Y > g_rcClient.bottom ? g_rcClient.bottom - 1 : ptEnd.Y;

		if (g_rcClient.PtInRect(CPoint(ptStart.X, ptStart.Y)) && g_rcClient.PtInRect(CPoint(ptEnd.X, ptEnd.Y)))
		{
			g.DrawLine(m_pPen, ptStart, ptEnd);
		}
	}

	// 由上至下绘制实线(横线)
	for (int y = g_sizeVector.cx; y >= -g_sizeVector.cy / 2; y -= g_unDistance*10)
	{
		cptStart = VecToWin(CPoint(-g_sizeVector.cx / 2, y));
		cptEnd = VecToWin(CPoint(g_sizeVector.cx / 2, y));
		ptStart.X = cptStart.x;
		ptStart.Y = cptStart.y;
		ptEnd.X = cptEnd.x;
		ptEnd.Y = cptEnd.y;

		ptStart.X = ptStart.X < g_rcClient.left ? g_rcClient.left : ptStart.X;
		ptEnd.X = ptEnd.X > g_rcClient.right ? g_rcClient.right - 1 : ptEnd.X;

		if (g_rcClient.PtInRect(CPoint(ptStart.X, ptStart.Y)) && g_rcClient.PtInRect(CPoint(ptEnd.X, ptEnd.Y)))
		{
			g.DrawLine(m_pPen, ptStart, ptEnd);
		}
	}

	/*!
	 * 绘制虚线
	*/
	if (g_fScale < 1)
	{
		m_mutexLock.unlock();

		return;
	}

	// 设置实线
	m_pPen->SetDashStyle(DashStyleDot);
	// 设置宽度
	m_pPen->SetWidth(1.0f);

	int nCount = 0;

	// 从左向右绘制虚线(竖线)
	for (int x = -g_sizeVector.cx / 2; x <= g_sizeVector.cx / 2; x += g_unDistance, nCount++)
	{
		if (nCount % 10 == 0)
		{
			continue;
		}

		cptStart = VecToWin(CPoint(x, g_sizeVector.cy / 2));
		cptEnd = VecToWin(CPoint(x, -g_sizeVector.cy / 2));
		ptStart.X = cptStart.x;
		ptStart.Y = cptStart.y;
		ptEnd.X = cptEnd.x;
		ptEnd.Y = cptEnd.y;

		ptStart.Y = ptStart.Y < g_rcClient.top ? g_rcClient.top : ptStart.Y;
		ptEnd.Y = ptEnd.Y > g_rcClient.bottom ? g_rcClient.bottom - 1 : ptEnd.Y;

		if (g_rcClient.PtInRect(CPoint(ptStart.X, ptStart.Y)) && g_rcClient.PtInRect(CPoint(ptEnd.X, ptEnd.Y)))
		{
			g.DrawLine(m_pPen, ptStart, ptEnd);
		}
	}

	nCount = 0;

	// 由上至下绘制虚线(横线)
	for (int y = g_sizeVector.cx; y >= -g_sizeVector.cy / 2; y -= g_unDistance, nCount++)
	{
		if (nCount % 10 == 0)
		{
			continue;
		}
		cptStart = VecToWin(CPoint(-g_sizeVector.cx / 2, y));
		cptEnd = VecToWin(CPoint(g_sizeVector.cx / 2, y));
		ptStart.X = cptStart.x;
		ptStart.Y = cptStart.y;
		ptEnd.X = cptEnd.x;
		ptEnd.Y = cptEnd.y;

		ptStart.X = ptStart.X < g_rcClient.left ? g_rcClient.left : ptStart.X;
		ptEnd.X = ptEnd.X > g_rcClient.right ? g_rcClient.right - 1 : ptEnd.X;

		if (g_rcClient.PtInRect(CPoint(ptStart.X, ptStart.Y)) && g_rcClient.PtInRect(CPoint(ptEnd.X, ptEnd.Y)))
		{
			g.DrawLine(m_pPen, ptStart, ptEnd);
		}
	}

	m_mutexLock.unlock();

	return;
}

void CGraphicBackground::DrawPoint(CDC * pDC)
{
	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	Graphics g(pDC->GetSafeHdc());

	CPoint cptVector = CPoint(0, 0);	/*!< 矢量坐标 */
	CString cstrText = _T("");

	Color gcrFont(Color::Red);			/*!< 文字颜色 */

	FontFamily fontFamily(_T("宋体"));
	Gdiplus::Font font(&fontFamily, 10.0f, FontStyle::FontStyleRegular, Gdiplus::Unit::UnitPoint);

	StringFormat gsFormat;
	gsFormat.SetAlignment(StringAlignmentNear);
	gsFormat.SetLineAlignment(StringAlignmentNear);

	SolidBrush* pSolidBrush = new SolidBrush(gcrFont);
	       
	/*!
	 * 绘制实线坐标
	*/

	// 绘制X坐标(实线)
	for (int x = g_rcClient.left; x <= g_rcClient.right; x++)
	{
		cptVector = WinToVec(CPoint(x,g_rcClient.top));

		if (abs(cptVector.x) % (g_unDistance * 10) == 0 && abs(cptVector.x) < g_sizeVector.cx / 2)
		{
			cstrText.Format(_T("%d"), cptVector.x);

			g.DrawString(cstrText, cstrText.GetLength(), &font, RectF((REAL)(x + m_fWidth), (REAL)g_rcClient.top, (REAL)50, (REAL)15), &gsFormat, pSolidBrush);
		}
	}

	// 绘制Y坐标(实线)
	for (int y = g_rcClient.top; y <= g_rcClient.bottom; y++)
	{
		cptVector = WinToVec(CPoint(g_rcClient.left, y));

		if (abs(cptVector.y) % (g_unDistance * 10) == 0 && abs(cptVector.y) < g_sizeVector.cy / 2)
		{
			cstrText.Format(_T("%d"), cptVector.y);

			g.DrawString(cstrText, cstrText.GetLength(), &font, RectF((REAL)g_rcClient.left, (REAL)(y + m_fWidth), (REAL)50, (REAL)15), &gsFormat, pSolidBrush);
		}
	}

	/*!
	 * 绘制虚线坐标
	*/

	if (g_fScale < 1)
	{
		delete pSolidBrush;

		m_mutexLock.unlock();

		return;
	}

	// 绘制X坐标(实线)
	for (int x = g_rcClient.left; x <= g_rcClient.right; x++)
	{
		cptVector = WinToVec(CPoint(x, g_rcClient.top));

		// 跳过实线
		if (abs(cptVector.x) % (g_unDistance * 10) == 0)
		{
			continue;
		}

		if (abs(cptVector.x) % (g_unDistance) == 0 && abs(cptVector.x) < g_sizeVector.cx / 2)
		{
			cstrText.Format(_T("%d"), cptVector.x);

			g.DrawString(cstrText, cstrText.GetLength(), &font, RectF((REAL)(x + m_fWidth), (REAL)g_rcClient.top, (REAL)50, (REAL)15), &gsFormat, pSolidBrush);
		}
	}

	// 绘制Y坐标(实线)
	for (int y = g_rcClient.top; y <= g_rcClient.bottom; y++)
	{
		cptVector = WinToVec(CPoint(g_rcClient.left, y));

		// 跳过实线
		if (abs(cptVector.y) % (g_unDistance * 10) == 0 && abs(cptVector.y) < g_sizeVector.cy / 2)
		{
			continue;
		}

		if (abs(cptVector.y) % (g_unDistance) == 0)
		{
			cstrText.Format(_T("%d"), cptVector.y);

			g.DrawString(cstrText, cstrText.GetLength(), &font, RectF((REAL)g_rcClient.left, (REAL)(y + m_fWidth), (REAL)50, (REAL)15), &gsFormat, pSolidBrush);
		}
	}

	delete pSolidBrush;

	m_mutexLock.unlock();

	return;
}

#endif // !_CGRAPHICBACKGROUND_H