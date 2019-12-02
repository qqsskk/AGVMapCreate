#include "stdafx.h"
#include "CGraphicTrack.h"

#ifdef _CGRAPHICTRACK_H

float CGraphicTrack::g_fWidth = 5.0f;

CGraphicTrack::CGraphicTrack(bool bArc)
{
	m_ptStart = CPoint(0,0);
	m_ptEnd = CPoint(0,0);
	m_bArc = bArc;

	m_ptPrepoint = CPoint(0, 0);
	m_ptLastpoint = CPoint(0, 0);
	m_ptLocal = CPoint(0, 0);
	m_bMove = false;
	m_bSelect = false;
	m_bShow = false;

	m_ptDistance = m_ptStart - m_ptEnd;
 }


CGraphicTrack::~CGraphicTrack()
{
}

void CGraphicTrack::SetStartPoint(CPoint ptStart)
{
	m_ptStart = ptStart;
	m_bShow = true;

	m_ptDistance = m_ptStart - m_ptEnd;
	
	return;
}

void CGraphicTrack::SetEndPoint(CPoint ptEnd)
{
	m_ptEnd = ptEnd;

	m_ptDistance = m_ptStart - m_ptEnd;

	return;
}

void CGraphicTrack::SetType(bool bArc)
{
	m_bArc = bArc;

	return;
}

CPoint CGraphicTrack::GetStartPoint()
{
	return m_ptStart;
}

CPoint CGraphicTrack::GetEndPoint()
{
	return m_ptEnd;
}

bool CGraphicTrack::GetType()
{
	return m_bArc;
}

bool CGraphicTrack::IsInside(CPoint ptWinpoint)
{
	CPoint ptStartWin, ptEndWin;
	ptStartWin = VecToWin(m_ptStart);
	ptEndWin = VecToWin(m_ptEnd);

	double dbW = g_fWidth * g_fScale; /*!< ¼ì²â°ë¾¶:¿í */
	double dbH = g_fWidth * g_fScale; /*!< ¼ì²â°ë¾¶:¸ß */

	if (m_bArc)
	{
		RectF rcfTmp;
		rcfTmp.Width = (REAL)abs(ptStartWin.x - ptEndWin.x) * 2;
		rcfTmp.Height = (REAL)abs(ptStartWin.y - ptEndWin.y) * 2;

		double nLr = (rcfTmp.Width >= rcfTmp.Height ? rcfTmp.Width : rcfTmp.Height) / 2; /*!< ³¤°ëÖá*/
		double nSr = (rcfTmp.Width <= rcfTmp.Height ? rcfTmp.Width : rcfTmp.Height) / 2; /*!< ¶Ì°ëÖá */

		CPoint ptMid = 0; /*!< ÖÐÐÄ×ø±ê */

		double x = 0;
		double y = 0;
		double a = 0; /*!< ½Ç¶ÈÖµ */

		 // 1
		if (ptEndWin.x < ptStartWin.x && ptEndWin.y < ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptEndWin.x;
			rcfTmp.Y = (REAL)ptEndWin.y - rcfTmp.Height / 2;

			ptMid.x = LONG(rcfTmp.X + rcfTmp.Width / 2);
			ptMid.y = LONG(rcfTmp.Y + rcfTmp.Height / 2);

			for (float i = 0; i <= 90; i += 0.1f)
			{
				a = tan(AngleToRadian(i));

				// ÍÖÔ²½¹µãÔÚXÖá x^2/a^2+y^2/b^2=1£¬(a>b>0)£»
				if (ptMid.x + nSr < rcfTmp.X + rcfTmp.Width)
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / ((nSr*nSr) + ((nLr*nLr) * (a * a))));
				}
				// ÍÖÔ²½¹µãÔÚYÖá y^2/a^2+x^2/b^2=1£¬(a>b>0)£»
				else
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / (((nSr*nSr) * (a * a)) + (nLr*nLr)));
				}

				y = a * x;

				x = ptMid.x - x;
				y = ptMid.y + y;

				RectF rcf((REAL)(x - dbW / 2), (REAL)(y - dbH / 2), (REAL)dbW, (REAL)dbH);

				if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
				{
					return true;
				}
			}

		}
		// 2
		else if (ptEndWin.x > ptStartWin.x && ptEndWin.y < ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptStartWin.x - rcfTmp.Width / 2;
			rcfTmp.Y = (REAL)ptEndWin.y - rcfTmp.Height / 2;

			ptMid.x = (LONG)(rcfTmp.X + rcfTmp.Width / 2);
			ptMid.y = (LONG)(rcfTmp.Y + rcfTmp.Height / 2);

			for (float i = 0; i <= 90; i += 0.1f)
			{
				a = tan(AngleToRadian(i));

				// ÍÖÔ²½¹µãÔÚXÖá x^2/a^2+y^2/b^2=1£¬(a>b>0)£»
				if (ptMid.x + nSr < rcfTmp.X + rcfTmp.Width)
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / ((nSr*nSr) + ((nLr*nLr) * (a * a))));
				}
				// ÍÖÔ²½¹µãÔÚYÖá y^2/a^2+x^2/b^2=1£¬(a>b>0)£»
				else
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / (((nSr*nSr) * (a * a)) + (nLr*nLr)));
				}

				y = a * x;

				x = ptMid.x + x;
				y = ptMid.y + y;

				RectF rcf((REAL)(x - dbW / 2), (REAL)(y - dbH / 2), (REAL)dbW, (REAL)dbH);

				if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
				{
					return true;
				}
			}

		}
		// 3
		else if (ptEndWin.x < ptStartWin.x && ptEndWin.y > ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptEndWin.x;
			rcfTmp.Y = (REAL)ptStartWin.y;

			ptMid.x = (LONG)(rcfTmp.X + rcfTmp.Width / 2);
			ptMid.y = (LONG)(rcfTmp.Y + rcfTmp.Height / 2);

			for (float i = 0; i <= 90; i += 0.1f)
			{
				a = tan(AngleToRadian(i));

				// ÍÖÔ²½¹µãÔÚXÖá x^2/a^2+y^2/b^2=1£¬(a>b>0)£»
				if (ptMid.x + nSr < rcfTmp.X + rcfTmp.Width)
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / ((nSr*nSr) + ((nLr*nLr) * (a * a))));
				}
				// ÍÖÔ²½¹µãÔÚYÖá y^2/a^2+x^2/b^2=1£¬(a>b>0)£»
				else
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / (((nSr*nSr) * (a * a)) + (nLr*nLr)));
				}

				y = a * x;

				x = ptMid.x - x;
				y = ptMid.y - y;

				RectF rcf((REAL)(x - dbW / 2), (REAL)(y - dbH / 2), (REAL)dbW, (REAL)dbH);

				if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
				{
					return true;
				}
			}

		}
		// 4
		else if (ptEndWin.x > ptStartWin.x && ptEndWin.y > ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptStartWin.x - rcfTmp.Width / 2;
			rcfTmp.Y = (REAL)ptStartWin.y;

			ptMid.x = (LONG)(rcfTmp.X + rcfTmp.Width / 2);
			ptMid.y = (LONG)(rcfTmp.Y + rcfTmp.Height / 2);

			for (float i = 0; i <= 90; i += 0.1f)
			{
				a = tan(AngleToRadian(i));

				// ÍÖÔ²½¹µãÔÚXÖá x^2/a^2+y^2/b^2=1£¬(a>b>0)£»
				if (ptMid.x + nSr < rcfTmp.X + rcfTmp.Width)
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / ((nSr*nSr) + ((nLr*nLr) * (a * a))));
				}
				// ÍÖÔ²½¹µãÔÚYÖá y^2/a^2+x^2/b^2=1£¬(a>b>0)£»
				else
				{
					x = sqrt((nLr*nLr) * (nSr*nSr) / (((nSr*nSr) * (a * a)) + (nLr*nLr)));
				}

				y = a * x;

				x = ptMid.x + x;
				y = ptMid.y - y;

				RectF rcf((REAL)(x - dbW / 2), (REAL)(y - dbH / 2), (REAL)dbW, (REAL)dbH);

				if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
				{
					return true;
				}
			}

		}
		else
		{
			x = ptStartWin.x < ptEndWin.x ? ptStartWin.x : ptEndWin.x;
			y = ptStartWin.y < ptEndWin.y ? ptStartWin.y : ptEndWin.y;

			x -= (int)(dbW / 2.0);
			y -= (int)(dbH / 2.0);

			int w = (int)(abs(ptStartWin.x - ptEndWin.x) > 0 ? abs(ptStartWin.x - ptEndWin.x) : dbW);
			int h = (int)(abs(ptStartWin.y - ptEndWin.y) > 0 ? abs(ptStartWin.y - ptEndWin.y) : dbW);

			RectF rcf((REAL)x, (REAL)y, (REAL)w, (REAL)h);

			if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
			{
				return true;
			}

		}
	}
	else
	{
		double dx = 0, dy = 0;
		dx = ptStartWin.x - ptEndWin.x;
		dy = ptStartWin.y - ptEndWin.y;

		// Ö±Ïß
		if (dx == 0 || dy == 0)
		{
			int x = ptStartWin.x < ptEndWin.x ? ptStartWin.x : ptEndWin.x;
			int y = ptStartWin.y < ptEndWin.y ? ptStartWin.y : ptEndWin.y;

			x -= (int)(dbW / 2.0);
			y -= (int)(dbH / 2.0);

			int w = (int)(abs(ptStartWin.x - ptEndWin.x) > 0 ? abs(ptStartWin.x - ptEndWin.x) : dbW);
			int h = (int)(abs(ptStartWin.y - ptEndWin.y) > 0 ? abs(ptStartWin.y - ptEndWin.y) : dbW);

			RectF rcf((REAL)x, (REAL)y, (REAL)w, (REAL)h);

			if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
			{
				return true;
			}
		}
		// Ð±Ïß
		else
		{
			double s = abs(dy / dx);

			int x = 0, y = 0;
			for (int i = 0; i < abs(dx); i += (int)(dbW / 2.0f))
			{
				if (dx > 0)
				{
					x = ptStartWin.x - i;
				}
				else
				{
					x = ptStartWin.x + i;
				}

				if (dy > 0)
				{
					y = (int)(ptStartWin.y - i * s);
				}
				else
				{
					y = (int)(ptStartWin.y + i * s);
				}

				RectF rcf((REAL)(x - dbW / 2), (REAL)(y - dbH / 2), (REAL)dbW, (REAL)dbH);

				if (rcf.Contains(PointF((REAL)ptWinpoint.x, (REAL)ptWinpoint.y)))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool CGraphicTrack::IsSelected()
{
	return m_bSelect;
}

void CGraphicTrack::Select(CPoint ptWinpoint)
{
	m_ptPrepoint = ptWinpoint;
	m_ptLastpoint = ptWinpoint;

	m_bMove = true;
	m_bSelect = true;

	return;
}

HCURSOR CGraphicTrack::Drag(CPoint ptWinPoint)
{
	HCURSOR hCur = nullptr;

	m_ptLastpoint = ptWinPoint;

	if (m_bMove)
	{
		m_ptStart.x = (LONG)((m_ptLastpoint.x - m_ptPrepoint.x) / g_fScale + m_ptLocal.x);
		m_ptStart.y = (LONG)(-(m_ptLastpoint.y - m_ptPrepoint.y) / g_fScale + m_ptLocal.y);

		if (m_ptStart.x < -g_sizeVector.cx / 2
			|| m_ptStart.y > g_sizeVector.cy / 2
			|| m_ptStart.x > g_sizeVector.cx / 2
			|| m_ptStart.y < -g_sizeVector.cy / 2)
		{
			m_ptStart = m_ptLocal;
		}

		if (m_ptEnd.x < -g_sizeVector.cx / 2
			|| m_ptEnd.y > g_sizeVector.cy / 2
			|| m_ptEnd.x > g_sizeVector.cx / 2
			|| m_ptEnd.y < -g_sizeVector.cy / 2)
		{
			m_ptStart = m_ptLocal;
		}

		hCur = LoadCursor(NULL, IDC_HAND);
	}
	else
	{
		m_ptLocal = m_ptStart;
	}

	m_ptEnd = m_ptStart - m_ptDistance;

	return 	hCur;
}

void CGraphicTrack::Confirm()
{
	m_bMove = false;

	m_ptStart = CGraph::Correct(m_ptStart);
	m_ptEnd = m_ptStart - m_ptDistance;

	return;
}

void CGraphicTrack::Cancel()
{
	m_ptStart = m_ptLocal;
	m_ptEnd = m_ptStart - m_ptDistance;

	m_bMove = false;
	m_bSelect = false;

	return;
}

void CGraphicTrack::Draw(CDC * pDC)
{
	if (m_bShow == false)
	{
		return;
	}

	Graphics g(pDC->GetSafeHdc());

	CPoint ptStartWin = VecToWin(m_ptStart);
	CPoint ptEndWin = VecToWin(m_ptEnd);

	Gdiplus::Pen p(Color::Black, g_fWidth * g_fScale);
	p.SetDashStyle(DashStyle::DashStyleSolid);

	if (m_bArc)
	{
		RectF rcfTmp;
		rcfTmp.Width = (REAL)abs(ptStartWin.x - ptEndWin.x) * 2;
		rcfTmp.Height = (REAL)abs(ptStartWin.y - ptEndWin.y) * 2;

		// 1
		if (ptEndWin.x < ptStartWin.x && ptEndWin.y < ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptEndWin.x;
			rcfTmp.Y = (REAL)ptEndWin.y - rcfTmp.Height / 2;

			if (m_bSelect)
			{
				p.SetColor(Color::Red);
				g.DrawArc(&p, rcfTmp, 180, -90);
			}
			else
			{
				g.DrawArc(&p, rcfTmp, 180, -90);
				p.SetWidth((g_fWidth / 2) * g_fScale);
				p.SetColor(Color::Yellow);
				p.SetDashStyle(DashStyle::DashStyleDash);
				g.DrawArc(&p, rcfTmp, 180, -90);
			}
		
		}
		// 2
		else if (ptEndWin.x > ptStartWin.x && ptEndWin.y < ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptStartWin.x - rcfTmp.Width / 2;
			rcfTmp.Y = (REAL)ptEndWin.y - rcfTmp.Height / 2;

			if (m_bSelect)
			{
				p.SetColor(Color::Red);
				g.DrawArc(&p, rcfTmp, 0, 90);
			}
			else
			{
				g.DrawArc(&p, rcfTmp, 0, 90);
				p.SetWidth((g_fWidth / 2) * g_fScale);
				p.SetColor(Color::Yellow);
				p.SetDashStyle(DashStyle::DashStyleDash);
				g.DrawArc(&p, rcfTmp, 0, 90);
			}
		}
		// 3
		else if (ptEndWin.x < ptStartWin.x && ptEndWin.y > ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptEndWin.x;
			rcfTmp.Y = (REAL)ptStartWin.y;

			if (m_bSelect)
			{
				p.SetColor(Color::Red);
				g.DrawArc(&p, rcfTmp, 180, 90);
			}
			else
			{
				g.DrawArc(&p, rcfTmp, 180, 90);
				p.SetWidth((g_fWidth / 2) * g_fScale);
				p.SetColor(Color::Yellow);
				p.SetDashStyle(DashStyle::DashStyleDash);
				g.DrawArc(&p, rcfTmp, 180, 90);
			}
		}
		// 4
		else if (ptEndWin.x > ptStartWin.x && ptEndWin.y > ptStartWin.y)
		{
			rcfTmp.X = (REAL)ptStartWin.x - rcfTmp.Width / 2;
			rcfTmp.Y = (REAL)ptStartWin.y;

			if (m_bSelect)
			{
				p.SetColor(Color::Red);

				g.DrawArc(&p, rcfTmp, 0, -90);
			}
			else
			{
				g.DrawArc(&p, rcfTmp, 0, -90);
				p.SetWidth((g_fWidth / 2) * g_fScale);
				p.SetColor(Color::Yellow);
				p.SetDashStyle(DashStyle::DashStyleDash);
				g.DrawArc(&p, rcfTmp, 0, -90);
			}
		}
		else
		{
			if (m_bSelect)
			{
				p.SetColor(Color::Red);

				g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
			}
			else
			{
				g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
				p.SetWidth((g_fWidth / 2) * g_fScale);
				p.SetColor(Color::Yellow);
				p.SetDashStyle(DashStyle::DashStyleDash);
				g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
			}
		}
	}
	else
	{
		if (m_bSelect)
		{
			p.SetColor(Color::Red);

			g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
		}
		else
		{
			g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
			p.SetWidth((g_fWidth / 2) * g_fScale);
			p.SetColor(Color::Yellow);
			p.SetDashStyle(DashStyle::DashStyleDash);
			g.DrawLine(&p, ptStartWin.x, ptStartWin.y, ptEndWin.x, ptEndWin.y);
		}
	}

	return;
}

void CGraphicTrack::SetWidth(float fWidth)
{
	g_fWidth = fWidth;

	return;
}

float CGraphicTrack::GetWidth()
{
	return g_fWidth;
}

double CGraphicTrack::RadianToAngle(double dbRadian)
{
	double PI = 3.14159265;
	return dbRadian * 180 / PI;
}


double CGraphicTrack::AngleToRadian(double dbAngle)
{
	double PI = 3.14159265;
	return dbAngle * PI / 180;
}

#endif //!_CGRAPHICTRACK_H