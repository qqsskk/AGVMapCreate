/*!
 * @file CGraphicAGV.cpp
 * @brief CGraphicAGV类执行文件
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#include "stdafx.h"
#include "CGraphicAGV.h"

#ifdef _CGRAPHICAGV_H

unsigned int CGraphicAGV::g_unWidth = 20;
unsigned int CGraphicAGV::g_unHeight = 20;
CString CGraphicAGV::g_cstrAGV = _T("../Image/AGV.png");
CString CGraphicAGV::g_cstrRun = _T("../Image/run.png");
CString CGraphicAGV::g_cstrUnVol = _T("../Image/unvol.png");
CString CGraphicAGV::g_cstrError = _T("../Image/error.png");
CString CGraphicAGV::g_cstrObs = _T("../Image/obs.png");
CString CGraphicAGV::g_cstrLoad = _T("../Image/load.png");
CString CGraphicAGV::g_cstrUnload = _T("../Image/unload.png");
CString CGraphicAGV::g_cstrCargo = _T("../Image/cargo.png");

CGraphicAGV::CGraphicAGV(unsigned char byNo, CString cstrType, CString cstrMove)
{
	m_byNo = byNo;
	m_cstrType = cstrType;
	m_cstrMove = cstrMove;

	m_bRun = false;
	m_bUnVol = false;
	m_bError = false;
	m_bObs = false;
	m_bCargo = false;
	m_byAction = _GRAPHICAGVACTSTA_NONE;

	m_bShow = false;

	m_ptCenter = CPoint(0,0);
}


CGraphicAGV::~CGraphicAGV()
{
}

void CGraphicAGV::SetType(CString cstrType)
{
	m_cstrType = cstrType;
	return;
}

void CGraphicAGV::SetMove(CString cstrMove)
{
	m_cstrMove = cstrMove;
	return;
}

unsigned char CGraphicAGV::GetNo() const
{
	return m_byNo;
}

CString CGraphicAGV::GetType() const
{
	return m_cstrType;
}

CString CGraphicAGV::GetMove() const
{
	return m_cstrMove;
}

void CGraphicAGV::SetImage(CString cstrAGV, CString cstrRun, CString cstrUnVol, CString cstrError, CString cstrObs, CString cstrLoad, CString cstrUnload, CString cstrCargo)
{
	g_cstrAGV = cstrAGV;
	g_cstrRun = cstrRun;
	g_cstrUnVol = cstrUnVol;
	g_cstrError = cstrError;
	g_cstrObs = cstrObs;
	g_cstrLoad = cstrLoad;
	g_cstrUnload = cstrUnload;
	g_cstrCargo = cstrCargo;

	return;
}

void CGraphicAGV::SetAGVImage(CString cstrAGV)
{
	if (g_cstrAGV != cstrAGV)
	{
		g_cstrAGV = cstrAGV;
	}

	return;
}

void CGraphicAGV::SetRunImage(CString cstrRun)
{
	if (g_cstrRun != cstrRun)
	{
		g_cstrRun = cstrRun;
	}

	return;
}

void CGraphicAGV::SetUnVolImage(CString cstrUnVol)
{
	if (g_cstrUnVol != cstrUnVol)
	{
		g_cstrUnVol = cstrUnVol;
	}

	return;
}

void CGraphicAGV::SetErrorImage(CString cstrError)
{
	if (g_cstrError != cstrError)
	{
		g_cstrError = cstrError;
	}

	return;
}

void CGraphicAGV::SetObsImage(CString cstrObs)
{
	if (g_cstrObs != cstrObs)
	{
		g_cstrObs = cstrObs;
	}

	return;
}

void CGraphicAGV::SetLoadImage(CString cstrLoad)
{
	if (g_cstrLoad != cstrLoad)
	{
		g_cstrLoad = cstrLoad;
	}

	return;
}

void CGraphicAGV::SetUnloadImage(CString cstrUnload)
{
	if (g_cstrUnload != cstrUnload)
	{
		g_cstrUnload = cstrUnload;
	}

	return;
}

void CGraphicAGV::SetCargoImage(CString cstrCargo)
{
	if (g_cstrCargo != cstrCargo)
	{
		g_cstrCargo = cstrCargo;
	}

	return;
}

void CGraphicAGV::GetImage(CString & cstrAGV, CString & cstrRun, CString & cstrUnVol, CString & cstrError, CString & cstrObs, CString & cstrLoad, CString & cstrUnload, CString & cstrCargo)
{
	cstrAGV = g_cstrAGV;
	cstrRun = g_cstrRun;
	cstrUnVol = g_cstrUnVol;
	cstrError = g_cstrError;
	cstrObs = g_cstrObs;
	cstrLoad = g_cstrLoad;
	cstrUnload = g_cstrUnload;
	cstrCargo = g_cstrCargo;

	return;
}

CString CGraphicAGV::GetAGVImage()
{
	return g_cstrAGV;
}

CString CGraphicAGV::GetRunImage()
{
	return g_cstrRun;
}

CString CGraphicAGV::GetUnVolImage()
{
	return g_cstrUnVol;
}

CString CGraphicAGV::GetErrorImage()
{
	return g_cstrError;
}

CString CGraphicAGV::GetObsImage()
{
	return g_cstrObs;
}

CString CGraphicAGV::GetLoadImage()
{
	return g_cstrLoad;
}

CString CGraphicAGV::GetUnloadImage()
{
	return g_cstrUnload;
}

CString CGraphicAGV::GetCargoImage()
{
	return g_cstrCargo;
}

void CGraphicAGV::SetStatus(bool bRun, bool bUnVol, bool bError, bool bObs, bool bCargo, unsigned char byAction)
{
	m_bRun = bRun;
	m_bUnVol = bUnVol;
	m_bError = bError;
	m_bObs = bObs;
	m_bCargo = bCargo;
	m_byAction = byAction;

	return;
}

void CGraphicAGV::SetRunStatus(bool bRun)
{
	if (m_bRun != bRun)
	{
		m_bRun = bRun;
	}

	return;
}

void CGraphicAGV::SetUnVolStatus(bool bUnVol)
{
	if (m_bUnVol != bUnVol)
	{
		m_bUnVol = bUnVol;
	}

	return;
}

void CGraphicAGV::SetErrorStatus(bool bError)
{
	if (m_bError != bError)
	{
		m_bError = bError;
	}

	return;
}

void CGraphicAGV::SetObsStatus(bool bObs)
{
	if (m_bObs != bObs)
	{
		m_bObs = bObs;
	}

	return;
}

void CGraphicAGV::SetCargoStatus(bool bCargo)
{
	if (m_bCargo != bCargo)
	{
		m_bCargo = bCargo;
	}

	return;
}

void CGraphicAGV::SetActionStatus(unsigned char byAction)
{
	if (m_byAction != byAction)
	{
		m_byAction = byAction;
	}

	return;
}

void CGraphicAGV::IsShow(bool bShow)
{
	m_bShow = bShow;

	return;
}

void CGraphicAGV::SetCenterPoint(CPoint ptCenter)
{
	m_ptCenter = ptCenter;

	return;
}

CPoint CGraphicAGV::GetCenterPoint() const
{
	return m_ptCenter;
}

void CGraphicAGV::Draw(CDC * pDC)
{
	if (m_bShow == false)
	{
		return;
	}
	
	Graphics g(pDC->GetSafeHdc());

	CPoint ptWindow = VecToWin(m_ptCenter);
	CRect rect;
	rect.left = LONG(ptWindow.x - (g_unWidth / 2) * g_fScale);
	rect.top = LONG(ptWindow.y - (g_unHeight / 2) * g_fScale);
	rect.right = LONG(rect.left + g_unWidth * g_fScale);
	rect.bottom = LONG(rect.top + g_unHeight * g_fScale);

	/*!
	 * 绘制AGV型号
	*/
	CString cstrType = m_cstrType;	/*!< 类型 */
	CString cstrMove = m_cstrMove;	/*!< 可运动方向 */

	CString cstrText = _T("");	/*!< 文本 */
	cstrText.Format(_T("%s-%s：%d"), cstrType, cstrMove, m_byNo);

	CPoint ptWinCenter = VecToWin(m_ptCenter);

	Color gcrFont(Color::Red); /*!< 文字颜色 */

	FontFamily fontFamily(_T("宋体"));
	Gdiplus::Font font(&fontFamily, (g_unWidth / 10) * g_fScale, FontStyleRegular, Gdiplus::Unit::UnitPoint);

	StringFormat gsFormat;
	gsFormat.SetAlignment(StringAlignmentCenter);
	gsFormat.SetLineAlignment(StringAlignmentNear);

	SolidBrush* pSolidBrush = new SolidBrush(gcrFont);

	g.DrawString(cstrText, cstrText.GetLength(), &font, RectF((REAL)rect.left, (REAL)rect.bottom, (REAL)rect.Width(), (REAL)rect.Height()), &gsFormat, pSolidBrush);

	delete pSolidBrush;

	/*!
	 * 绘制AGV贴图
	*/
	Image *pImage = nullptr;
	pImage = Image::FromFile(g_cstrAGV);

	g.DrawImage(pImage, RectF((REAL)rect.left, (REAL)rect.top, (REAL)rect.Width(), (REAL)rect.Height()));

	/*!
	 * 绘制AGV状态
	*/
	RectF rcfDraw((REAL)(rect.left + rect.Width() / 4), (REAL)(rect.top + rect.Height() / 4), (REAL)(rect.Width() / 2), (REAL)(rect.Height() / 2));

	bool bFlash = false;

	// 移动
	if (m_bRun)
	{
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrRun);

		bFlash = true;
	}

	// 避障
	if (m_bObs)
	{
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrObs);

		bFlash = true;
	}

	// 动作
	switch (m_byAction)
	{
	case _GRAPHICAGVACTSTA_LOAD:
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrLoad);

		bFlash = true;
		break;
	case _GRAPHICAGVACTSTA_UNLOAD:
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrUnload);

		bFlash = true;
		break;
	}

	// 电量不足
	if (m_bUnVol)
	{
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrUnVol);
		
		if (m_bError == false)
		{
			rcfDraw.Y += (REAL)(rect.Height() / 8);
			rcfDraw.Height = (REAL)(rect.Height() /4);
		}

		bFlash = true;
	}

	// 异常
	if (m_bError)
	{
		if (pImage)
		{
			delete pImage;
		}
		pImage = Image::FromFile(g_cstrError);

		bFlash = true;
	}

	if (bFlash)
	{
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
			g.DrawImage(pImage, rcfDraw);
		}
	}

	// 载货
	if (m_byAction == _GRAPHICAGVACTSTA_NONE && m_bCargo)
	{
		if (pImage)
		{
			delete pImage;
		}

		pImage = Image::FromFile(g_cstrCargo);

		rcfDraw.X = (REAL)(rect.left + rect.Width() / 4);
		rcfDraw.Y = (REAL)(rect.top - rect.Height() /2);
		rcfDraw.Width = (REAL)(rect.Width() /2);
		rcfDraw.Height = (REAL)(rect.Height() /2);

		g.DrawImage(pImage, rcfDraw);
	}

	if (pImage)
	{
		delete pImage;
	}

	return;
}

#endif //!_CGRAPHICAGV_H