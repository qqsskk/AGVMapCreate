// CTrafficConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CTrafficConfigPane.h"


// CTrafficConfigPane

IMPLEMENT_DYNAMIC(CTrafficConfigPane, CDockablePane)

CTrafficConfigPane::CTrafficConfigPane()
{

}

CTrafficConfigPane::~CTrafficConfigPane()
{
}


BEGIN_MESSAGE_MAP(CTrafficConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTrafficConfigPane 消息处理程序
int CTrafficConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgTrafficConfig.Create(IDD_TRAFFICCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgTrafficConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CTrafficConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgTrafficConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgTrafficConfig.MoveWindow(rect);
	}
}


void CTrafficConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgTrafficConfig.DestroyWindow();
}

