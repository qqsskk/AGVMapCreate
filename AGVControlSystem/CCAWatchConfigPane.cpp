// CCAWatchConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCAWatchConfigPane.h"


// CCAWatchConfigPane

IMPLEMENT_DYNAMIC(CCAWatchConfigPane, CDockablePane)

CCAWatchConfigPane::CCAWatchConfigPane()
{

}

CCAWatchConfigPane::~CCAWatchConfigPane()
{
}


BEGIN_MESSAGE_MAP(CCAWatchConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CCAWatchConfigPane 消息处理程序
int CCAWatchConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgWatchConfig.Create(IDD_CAWATCHCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgWatchConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CCAWatchConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgWatchConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgWatchConfig.MoveWindow(rect);
	}
}


void CCAWatchConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgWatchConfig.DestroyWindow();
}

