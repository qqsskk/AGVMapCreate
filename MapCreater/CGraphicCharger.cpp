/*!
 * @file CGraphicCharger.cpp
 * @brief CGraphicCharger类执行文件
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#include "stdafx.h"
#include "CGraphicCharger.h"

#ifdef _CGRAPHICCHARGER_H

unsigned int CGraphicCharger::g_unWidth = 20;
unsigned int CGraphicCharger::g_unHeight = 20;
CString CGraphicCharger::g_cstrPath = _T("../Image/charger.png");
CString CGraphicCharger::g_cstrPower = _T("../Image/power.png");

CGraphicCharger::CGraphicCharger(unsigned char byNo, unsigned short usMark, unsigned char byCtrl, CString cstrName)
{
	m_byNo = byNo;
	m_usMark = usMark;
	m_byCtrl = byCtrl;
	m_cstrName = cstrName;
#ifdef _WIN64
	m_ullChargeTime = 0;
#elif _WIN32
	m_ulChargeTime = 0;
#endif

	m_bPower = false;

	m_ptCenter = CPoint(0, 0);
	m_ptPrepoint = CPoint(0, 0);
	m_ptLastpoint = CPoint(0, 0);
	m_ptLocal = CPoint(0, 0);
	m_bMove = false;
	m_bSelect = false;
}


CGraphicCharger::~CGraphicCharger()
{
}

unsigned char CGraphicCharger::GetNo() const
{
	return m_byNo;
}

void CGraphicCharger::SetName(CString cstrName)
{
	m_cstrName = cstrName;

	return;
}

CString CGraphicCharger::GetName() const
{
	return m_cstrName;
}

#ifdef _WIN64
void CGraphicCharger::SetPower(bool bPower, unsigned long long ullChargeTime)
{
	m_bPower = bPower;

	if (m_bPower == false)
	{
		m_ullChargeTime = 0;
	}
	else
	{
		m_ullChargeTime = ullChargeTime;
	}

	return;
}
#elif _WIN32
void CGraphicCharger::SetPower(bool bPower, unsigned long ulChargeTime)
{
	m_bPower = bPower;

	if (m_bPower == false)
	{
		m_ulChargeTime = 0;
	}
	else
	{
		m_ulChargeTime = ulChargeTime;
	}

	return;
}
#endif

void CGraphicCharger::SetCtrl(unsigned char byCtrl)
{
	m_byCtrl = byCtrl;

	return;
}

unsigned char CGraphicCharger::GetCtrl() const
{
	return m_byCtrl;
}

void CGraphicCharger::SetMark(unsigned short usMark)
{
	m_usMark = usMark;

	return;
}

unsigned short CGraphicCharger::GetMark() const
{
	return m_usMark;
}

void CGraphicCharger::SetImage(CString cstrPath, CString cstrPower)
{
	g_cstrPath = cstrPath;
	g_cstrPower = cstrPower;

	return;
}

void CGraphicCharger::GetImage(CString & cstrPath, CString & cstrPower)
{
	cstrPath = g_cstrPath;
	cstrPower = g_cstrPower;

	return;
}

bool CGraphicCharger::IsInside(CPoint ptWinpoint)
{
	CPoint ptWindow = VecToWin(m_ptCenter);
	CRect rect;
	rect.left = LONG(ptWindow.x - (g_unWidth / 2) * g_fScale);
	rect.top = LONG(ptWindow.y - (g_unHeight / 2) * g_fScale);
	rect.right = LONG(rect.left + g_unWidth * g_fScale);
	rect.bottom = LONG(rect.top + g_unHeight * g_fScale);

	return rect.PtInRect(ptWinpoint);
}

bool CGraphicCharger::IsSelected()
{
	return m_bSelect;
}

void CGraphicCharger::Select(CPoint ptWinpoint)
{
	m_ptPrepoint = ptWinpoint;
	m_ptLastpoint = ptWinpoint;

	m_bMove = true;
	m_bSelect = true;

	return;
}

HCURSOR CGraphicCharger::Drag(CPoint ptWinPoint)
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

void CGraphicCharger::Confirm()
{
	m_bMove = false;

	m_ptCenter = CGraph::Correct(m_ptCenter);

	return;
}

void CGraphicCharger::Cancel()
{
	m_ptCenter = m_ptLocal;
	m_bMove = false;
	m_bSelect = false;

	return;
}

CPoint CGraphicCharger::GetCenterPoint()
{
	return m_ptCenter;
}

void CGraphicCharger::SetCenterPoint(CPoint ptCenter)
{
	m_ptCenter = ptCenter;
	m_ptLocal = m_ptCenter;

	return;
}

void CGraphicCharger::Draw(CDC * pDC)
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
	 * 绘制充电标识符
	*/
	if (m_bPower)
	{
		pImage = Image::FromFile(g_cstrPower);

#ifdef _WIN64
		if (g_ullFlashTime == 0)
		{
			g_bFlash = true;
			g_ullFlashTime = GetTickCount64();
		}
		else if (g_ullFlashTime > 0 && GetTickCount64() - g_ullFlashTime > 1000)
		{
			g_bFlash = !g_bFlash;
			g_ullFlashTime = GetTickCount64();
		}
#elif _WIN32
		if (g_ulFlashTime == 0)
		{
			g_bFlash = true;
			g_ulFlashTime = GetTickCount();
		}
		else if (g_ulFlashTime > 0 && GetTickCount() - g_ulFlashTime > 1000)
		{
			g_bFlash = !g_bFlash;
			g_ulFlashTime = GetTickCount();
		}
#endif

		if (g_bFlash)
		{
			g.DrawImage(pImage, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
		}

		delete pImage;
	}

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
	cstrNo.Format(_T("充电器：%d"), m_byNo);

	pBrush->SetColor(gcrFont);
	g.DrawString(cstrNo, cstrNo.GetLength(), &font, RectF((REAL)rect.left, (REAL)(rect.top - rect.Height()), (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);

	if (m_bPower == false || m_ullChargeTime == 0)
	{
		if (m_cstrName.IsEmpty() == false)
		{
			gsFormat.SetAlignment(StringAlignmentCenter);
			gsFormat.SetLineAlignment(StringAlignmentNear);

			g.DrawString(m_cstrName, m_cstrName.GetLength(), &font, RectF((REAL)rect.left, (REAL)rect.bottom, (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);
		}

		delete pBrush;
		return;
	}

	unsigned int unDay = 0;		/*!< 持续天数 */
	unsigned int unHour = 0;	/*!< 持续小时 */
	unsigned int unMin = 0;		/*!< 持续分钟 */
	unsigned int unSec = 0;		/*!< 持续秒 */
	unsigned int unMinSec = 0;	/*!< 持续毫秒 */

#ifdef _WIN64
	unsigned long long ullDurativeTime = 0;
	ullDurativeTime = GetTickCount64() - m_ullChargeTime;

	unDay = ullDurativeTime / 86400000L;
	unHour = (ullDurativeTime % 86400000L) / 3600000L;
	unMin = ((ullDurativeTime % 86400000L) % 3600000L) / 60000L;
	unSec = (((ullDurativeTime % 86400000L) % 3600000L) % 60000L) / 1000L;
	unMinSec = (((ullDurativeTime % 86400000L) % 3600000L) % 60000L) % 1000L;
#elif _WIN32
	unsigned long ulDurativeTime = 0;
	ulDurativeTime = GetTickCount() - m_ulChargeTime;

	unDay = ulDurativeTime / 86400000L;
	unHour = (ulDurativeTime % 86400000L) / 3600000L;
	unMin = ((ulDurativeTime % 86400000L) % 3600000L) / 60000L;
	unSec = (((ulDurativeTime % 86400000L) % 3600000L) % 60000L) / 1000L;
	unMinSec = (((ulDurativeTime % 86400000L) % 3600000L) % 60000L) % 1000L;
#endif //!_WIN64

	CString cstrDurativeTime = _T("");
	cstrDurativeTime.Format(_T("%d天:%d小时:%d分:%d秒"), unDay, unHour, unMin, unSec);

	gsFormat.SetAlignment(StringAlignmentCenter);
	gsFormat.SetLineAlignment(StringAlignmentNear);

	g.DrawString(cstrDurativeTime, cstrDurativeTime.GetLength(), &font, RectF((REAL)rect.left, (REAL)rect.bottom, (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pBrush);

	delete pBrush;

	return;
}

#endif //!_CGRAPHICCHARGER_H