/*!
 * @file CGraph.cpp
 * @brief CGraph类执行文件
 * @author FanKaiyu
 * @date 2018-10-16
 * @version 2.0
*/

#include "stdafx.h"
#include "CGraph.h"

#ifdef _CGRAPH_H

bool CGraph::g_bFlash = false;
#ifdef _WIN64
unsigned long long CGraph::g_ullFlashTime = 0;
#elif _WIN32
unsigned long CGraph::g_ulFlashTime = 0;
#endif 
CPoint CGraph::g_ptOffset = CPoint(0,0);	
float CGraph::g_fScale = DEFALUT_SCALE;
unsigned int CGraph::g_unDistance = 10;
CSize CGraph::g_sizeVector = CSize(4800, 4800);
CRect CGraph::g_rcClient = CRect(0,0,0,0);

CGraph::CGraph()
{
	// 初始化GDI+
	GdiplusStartup(&m_ulGdiplusToken, &m_gdiplusInput, &m_gdiplusOutput);
}


CGraph::~CGraph()
{
	// 关闭GDI+
	GdiplusShutdown(m_ulGdiplusToken);
}

void CGraph::SetClientRect(CRect rcClient)
{
	g_rcClient = rcClient;
	return;
}

CPoint CGraph::GetOffset()
{
	return g_ptOffset;
}

void CGraph::SetOffset(CPoint ptOffset)
{
	g_ptOffset = ptOffset;
	return;
}

float CGraph::GetScale()
{
	return g_fScale;
}

void CGraph::SetScale(float fScale)
{
	g_fScale = fScale;
	return;
}

CPoint CGraph::VecToWin(CPoint ptVector)
{
	int nX = (int)(ptVector.x * g_fScale + g_rcClient.right / 2 + g_ptOffset.x);
	int nY = (int)(-ptVector.y * g_fScale + g_rcClient.bottom / 2 + g_ptOffset.y);

	return CPoint(nX,nY);
}

CPoint CGraph::WinToVec(CPoint ptWindow)
{
	double fX0 = g_rcClient.right / 2.0f;	/*!< 客户区中心X坐标 */
	double fY0 = g_rcClient.bottom / 2.0f;	/*!< 客户区中心Y坐标 */

	int nX = ptWindow.x - (int)fX0;
	int nY = (int)fY0 - ptWindow.y;

	nX = (int)((nX - g_ptOffset.x) / g_fScale);
	nY = (int)((nY + g_ptOffset.y) / g_fScale);

	return CPoint(nX,nY);
}

CPoint CGraph::Correct(CPoint ptVector)
{
	int nDis = g_unDistance;
	int nDx = abs(ptVector.x) % nDis; /*!< 距离最近方格的X距离 */
	int nDy = abs(ptVector.y) % nDis; /*!< 距离最近方格的Y距离*/

	if (nDx > nDis / 2)
	{
		if (ptVector.x > 0)
		{
			ptVector.x += nDis;
		}
		else
		{
			ptVector.x -= nDis;
		}
	}

	if (nDy > nDis / 2)
	{
		if (ptVector.y > 0)
		{
			ptVector.y += nDis;
		}
		else
		{
			ptVector.y -= nDis;
		}
	}

	if (ptVector.x > 0)
	{
		ptVector.x -= nDx;
	}
	else
	{
		ptVector.x += nDx;
	}

	if (ptVector.y > 0)
	{
		ptVector.y -= nDy;
	}
	else
	{
		ptVector.y += nDy;
	}

	return ptVector;
}

void CGraph::Locate(CPoint ptLocation)
{
	CPoint ptWin = VecToWin(ptLocation);

	CPoint ptDis = CPoint(g_rcClient.right / 2, g_rcClient.bottom / 2) - ptWin;
	SetOffset(ptDis + g_ptOffset);

	return;
}

#endif // !_CGRAPH_H