
// AGVControlSystemView.cpp: CAGVControlSystemView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "AGVControlSystem.h"
#endif

#include "AGVControlSystemDoc.h"
#include "AGVControlSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAGVControlSystemView

IMPLEMENT_DYNCREATE(CAGVControlSystemView, CView)

BEGIN_MESSAGE_MAP(CAGVControlSystemView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CAGVControlSystemView 构造/析构

CAGVControlSystemView::CAGVControlSystemView() noexcept
{
	EnableActiveAccessibility();
	// TODO: 在此处添加构造代码
	m_pBackground = new CGraphicBackground();
}

CAGVControlSystemView::~CAGVControlSystemView()
{
	ClearAll();
}

BOOL CAGVControlSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAGVControlSystemView 绘图

void CAGVControlSystemView::OnDraw(CDC* pDC)
{
	CAGVControlSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (m_mutexLock.try_lock() == false)
	{
		return;
	}

	//设置兼容区域，一般为当前窗口大小
	CRect RectCompatible;

	//建立兼容DC，即后面那块画布
	CDC Memdc;
	CBitmap Bitmap;

	//得到当前窗口的大小
	GetClientRect(&RectCompatible);
	Memdc.CreateCompatibleDC(pDC);

	//创建兼容位图
	Bitmap.CreateCompatibleBitmap(pDC, RectCompatible.Width(), RectCompatible.Height());

	//把位图选到兼容DC中，这样才可以开始画
	Memdc.SelectObject(Bitmap);
	Memdc.FillSolidRect(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), RGB(255, 255, 255));//用白色先刷新整个兼容DC，这相当于原来的刷新背景

	if (m_pBackground)
	{
		m_pBackground->DrawBackground(&Memdc);
		m_pBackground->DrawLine(&Memdc);
		// m_pBackground->DrawPoint(&Memdc);
	}

	for (list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end(); ++it)
	{
		(*it)->Draw(&Memdc);
	}

	for (map<unsigned short, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); ++it)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned char, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); ++it)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned char, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); ++it)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned char, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); ++it)
	{
		it->second->Draw(&Memdc);
	}

	for (map<unsigned char, CGraphicAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); ++it)
	{
		it->second->Draw(&Memdc);
	}

	//位图中图片加载到画布上
	//pDC->BitBlt(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), &Memdc, RectCompatible.left, RectCompatible.top, SRCCOPY);//最后把兼容DC拷贝到pDC
	pDC->BitBlt(RectCompatible.left, RectCompatible.top, RectCompatible.Width(), RectCompatible.Height(), &Memdc, RectCompatible.left, RectCompatible.top, SRCCOPY);//最后把兼容DC拷贝到pDC
																																									//实现前后交替，便不会闪动
	Memdc.DeleteDC();
	Bitmap.DeleteObject();

	m_mutexLock.unlock();

	return;
}

void CAGVControlSystemView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	if (m_pBackground)
	{
		m_pBackground->Cancel();
	}

	ClientToScreen(&point);
	// OnContextMenu(this, point);
}

void CAGVControlSystemView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CAGVControlSystemView::CreateBackground(float fScale, CPoint ptOffset)
{
	if (m_pBackground == nullptr)
	{
		m_pBackground = new CGraphicBackground();
	}

	CGraph::SetScale(fScale);
	CGraph::SetOffset(ptOffset);

	return;
}

void CAGVControlSystemView::DeleteBackground()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_pBackground)
	{
		delete m_pBackground;
	}

	return;
}

void CAGVControlSystemView::CreateAGV(unsigned char byNo, CString cstrType, CString cstrMove)
{
	DeleteAGV(byNo);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	m_mapAGVs[byNo] = new CGraphicAGV(byNo, cstrType, cstrMove);

	return;
}

void CAGVControlSystemView::DeleteAGV(unsigned char byNo)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);
	
	if (m_mapAGVs.find(byNo) != m_mapAGVs.end())
	{
		delete m_mapAGVs[byNo];
		m_mapAGVs.erase(m_mapAGVs.find(byNo));
	}

	return;
}

void CAGVControlSystemView::DeleteAllAGV()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (map<unsigned char, CGraphicAGV*>::iterator it = m_mapAGVs.begin(); it != m_mapAGVs.end(); )
	{
		delete it->second;
		it = m_mapAGVs.erase(it);
	}

	return;
}

void CAGVControlSystemView::SetAGVLocation(unsigned char byNo, unsigned short usMark)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	if (m_mapMarks.find(usMark) == m_mapMarks.end())
	{
		m_mapAGVs[byNo]->IsShow(false);
	}
	else
	{
		m_mapAGVs[byNo]->SetCenterPoint(m_mapMarks[usMark]->GetCenterPoint());
		m_mapAGVs[byNo]->IsShow(true);
	}

	return;
}

void CAGVControlSystemView::IsAGVShow(unsigned char byNo, bool bShow)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	m_mapAGVs[byNo]->IsShow(bShow);

	return;
}

void CAGVControlSystemView::SetAGVStatus(unsigned char byNo, bool bRun, bool bUnVol, bool bError, bool bObs, bool bCargo, unsigned char byAction)
{
	if (m_mapAGVs.find(byNo) == m_mapAGVs.end())
	{
		return;
	}

	m_mapAGVs[byNo]->SetStatus(bRun, bUnVol, bError, bObs, bCargo, byAction);

	return;
}

void CAGVControlSystemView::CreateMark(unsigned short usNo, CPoint ptCenter)
{
	DeleteMark(usNo);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	m_mapMarks[usNo] = new CGraphicMark(usNo, ptCenter);

	return;
}

void CAGVControlSystemView::DeleteMark(unsigned short usNo)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_mapMarks.find(usNo) != m_mapMarks.end())
	{
		delete m_mapMarks[usNo];
		m_mapMarks.erase(m_mapMarks.find(usNo));
	}

	return;
}

void CAGVControlSystemView::DeleteAllMark()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (map<unsigned short, CGraphicMark*>::iterator it = m_mapMarks.begin(); it != m_mapMarks.end(); )
	{
		delete it->second;
		it = m_mapMarks.erase(it);
	}

	return;
}

void CAGVControlSystemView::CreateStation(unsigned char byNo, unsigned short usMark, CPoint ptCenter, CString cstrName)
{
	DeleteStation(byNo);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	m_mapStations[byNo] = new CGraphicStation(byNo, usMark, cstrName);
	m_mapStations[byNo]->SetCenterPoint(ptCenter);

	return;
}

void CAGVControlSystemView::DeleteStation(unsigned char byNo)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_mapStations.find(byNo) != m_mapStations.end())
	{
		delete m_mapStations[byNo];
		m_mapStations.erase(m_mapStations.find(byNo));
	}

	return;
}

void CAGVControlSystemView::DeleteAllStation()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (map<unsigned char, CGraphicStation*>::iterator it = m_mapStations.begin(); it != m_mapStations.end(); )
	{
		delete it->second;
		it = m_mapStations.erase(it);
	}

	return;
}

void CAGVControlSystemView::CreateRestPosition(unsigned char byNo, unsigned short usMark, CPoint ptCenter, CString cstrName)
{
	DeleteRestPosition(byNo);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	m_mapRests[byNo] = new CGraphicRest(byNo, usMark, cstrName);
	m_mapRests[byNo]->SetCenterPoint(ptCenter);

	return;
}

void CAGVControlSystemView::DeleteRestPosition(unsigned char byNo)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_mapRests.find(byNo) != m_mapRests.end())
	{
		delete m_mapRests[byNo];
		m_mapRests.erase(m_mapRests.find(byNo));
	}

	return;
}

void CAGVControlSystemView::DeleteAllRestPosition()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (map<unsigned char, CGraphicRest*>::iterator it = m_mapRests.begin(); it != m_mapRests.end(); )
	{
		delete it->second;
		it = m_mapRests.erase(it);
	}

	return;
}

void CAGVControlSystemView::CreateCharger(unsigned char byNo, unsigned short usMark, unsigned char byCtrl, CPoint ptCenter, CString cstrName)
{
	DeleteCharger(byNo);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	m_mapChargers[byNo] = new CGraphicCharger(byNo, usMark, byCtrl,cstrName);
	m_mapChargers[byNo]->SetCenterPoint(ptCenter);

	return;
}

void CAGVControlSystemView::DeleteCharger(unsigned char byNo)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_mapChargers.find(byNo) != m_mapChargers.end())
	{
		delete m_mapChargers[byNo];
		m_mapChargers.erase(m_mapChargers.find(byNo));
	}

	return;
}

void CAGVControlSystemView::DeleteAllCharger()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (map<unsigned char, CGraphicCharger*>::iterator it = m_mapChargers.begin(); it != m_mapChargers.end(); )
	{
		delete it->second;
		it = m_mapChargers.erase(it);
	}

	return;
}

#ifdef _WIN64
void CAGVControlSystemView::SetPower(unsigned char byNo, bool bPower, unsigned long long ullChargerTime)
{
	if (m_mapChargers.find(byNo) == m_mapChargers.end())
	{
		return;
	}

	m_mapChargers[byNo]->SetPower(bPower, ullChargerTime);

	return;
}
#elif _WIN32
void CAGVControlSystemView::SetPower(unsigned char byNo, bool bPower, unsigned long ulChargerTime)
{
	if (m_mapChargers.find(byNo) == m_mapChargers.end())
	{
		return;
	}

	m_mapChargers[byNo]->SetPower(bPower, ulChargerTime);

	return;
}
#endif

void CAGVControlSystemView::CreateTrack(CPoint ptStart, CPoint ptEnd, bool bArc)
{
	DeleteTrack(ptStart, ptEnd, bArc);

	std::lock_guard<std::mutex> lock(m_mutexLock);

	CGraphicTrack *pTrack = new CGraphicTrack(bArc);
	pTrack->SetStartPoint(ptStart);
	pTrack->SetStartPoint(ptEnd);
	m_listTracks.push_back(pTrack);

	return;
}

void CAGVControlSystemView::DeleteTrack(CPoint ptStart, CPoint ptEnd, bool bArc)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (std::list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end();)
	{
		CGraphicTrack *pTrack = *it;
		if (ptStart == pTrack->GetStartPoint() && ptEnd == pTrack->GetEndPoint() && bArc == pTrack->GetType())
		{
			delete *it;
			it = m_listTracks.erase(it);
		}
		else
		{
			++it;
		}
	}

	return;
}

void CAGVControlSystemView::DeleteAllTrack()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (std::list<CGraphicTrack*>::iterator it = m_listTracks.begin(); it != m_listTracks.end();)
	{
		delete *it;
		it = m_listTracks.erase(it);
	}
	
	return;
}

void CAGVControlSystemView::ClearAll()
{
	DeleteAllAGV();
	DeleteAllTrack();
	DeleteAllMark();
	DeleteAllCharger();
	DeleteAllRestPosition();
	DeleteAllStation();
	DeleteBackground();

	return;
}


// CAGVControlSystemView 诊断

#ifdef _DEBUG
void CAGVControlSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CAGVControlSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAGVControlSystemDoc* CAGVControlSystemView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAGVControlSystemDoc)));
	return (CAGVControlSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CAGVControlSystemView 消息处理程序
void CAGVControlSystemView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcWnd;	/*!< 客户区尺寸 */
	GetClientRect(rcWnd);

	CGraph::SetClientRect(rcWnd);

	return;
}


BOOL CAGVControlSystemView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		if (zDelta > 0)
		{
			m_pBackground->Scale();
		}
		else
		{
			m_pBackground->Scale(true);
		}

	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CAGVControlSystemView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);

	InvalidateRect(rcClient);
	UpdateWindow();

	CView::OnTimer(nIDEvent);
}


void CAGVControlSystemView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SetTimer(0, 1000 / 30, NULL);

//#define _TEST
#ifdef _TEST
	/*!
	 * 测试用例
	*/
	CreateMark(1, CPoint(-60, 0));
	CreateMark(2, CPoint(-50, 0));
	CreateMark(3, CPoint(-40, 0));
	CreateMark(4, CPoint(-30, 0));
	CreateMark(5, CPoint(-20, 0));
	CreateMark(6, CPoint(-10, 0));
	CreateMark(7, CPoint(0, 0));
	CreateMark(8, CPoint(10, 0));
	CreateMark(9, CPoint(20, 0));
	CreateMark(10, CPoint(30, 0));
	CreateMark(11, CPoint(40, 0));
	CreateMark(12, CPoint(50, 0));
	CreateMark(13, CPoint(60, 0));
	CreateAGV(1, _T("T"), _T("S"));
	CreateAGV(2, _T("A"), _T("D"));
	CreateAGV(3, _T("P"), _T("S"));
	CreateAGV(4, _T("S"), _T("S"));
	CreateAGV(5, _T("F"), _T("D"));
	CreateAGV(6, _T("L"), _T("F"));
	CreateAGV(7, _T("S"), _T("D"));
	SetAGVLocation(1, 1);
	SetAGVLocation(2, 3);
	SetAGVLocation(3, 5);
	SetAGVLocation(4, 7);
	SetAGVLocation(5, 9);
	SetAGVLocation(6, 11);
	SetAGVLocation(7, 13);
	SetAGVStatus(1, true, false, false, false, false, _GRAPHICAGVACTSTA_NONE);
	SetAGVStatus(2, false, true, false, false, false, _GRAPHICAGVACTSTA_NONE);
	SetAGVStatus(3, false, false, true, false, false, _GRAPHICAGVACTSTA_NONE);
	SetAGVStatus(4, false, false, false, true, false, _GRAPHICAGVACTSTA_NONE);
	SetAGVStatus(5, false, false, false, false, true, _GRAPHICAGVACTSTA_NONE);
	SetAGVStatus(6, false, false, false, false, false, _GRAPHICAGVACTSTA_LOAD);
	SetAGVStatus(7, false, false, false, false, false, _GRAPHICAGVACTSTA_UNLOAD);
	CreateStation(1, 1, CPoint(-60, -30));
	CreateStation(2, 3, CPoint(-40, -30),_T("测试名称:工位2"));
	CreateRestPosition(1, 5, CPoint(-20, -30));
	CreateRestPosition(2, 7, CPoint(0, -30),_T("测试名称:待机位2"));
	CreateCharger(1, 9, 0, CPoint(20, -30));
	CreateCharger(2, 11, 1, CPoint(40, -30),_T("测试名称:充电器2"));
	CreateCharger(3, 13, 1, CPoint(60, -30), _T("测试名称:充电器3"));
	SetPower(3, true, GetTickCount64());
#endif
	return;
}


BOOL CAGVControlSystemView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;

	//return CView::OnEraseBkgnd(pDC);
}


void CAGVControlSystemView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_pBackground)
	{
		m_pBackground->Reset();
	}

	CView::OnMButtonDblClk(nFlags, point);

	return;
}


void CAGVControlSystemView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		m_pBackground->Select(point);
	}

	CView::OnMButtonDown(nFlags, point);

	return;
}


void CAGVControlSystemView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pBackground)
	{
		m_pBackground->Confirm();
	}

	CView::OnMButtonUp(nFlags, point);

	return;
}


void CAGVControlSystemView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnMouseMove(nFlags, point);

	CPoint ptVector = CGraph::WinToVec(point);

	HCURSOR hCur = nullptr;

	if (m_pBackground)
	{
		HCURSOR hRecv = nullptr;

		hRecv = m_pBackground->Drag(point);

		if (hRecv)
		{
			hCur = hRecv;
		}
	}

	if (hCur)
	{
		hCur = SetCursor(hCur);
	}
	else
	{
		hCur = LoadCursor(NULL, IDC_ARROW);
		hCur = SetCursor(hCur);
	}

	return;
}