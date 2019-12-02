// CStationConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CStationConfigPane.h"


// CStationConfigPane

IMPLEMENT_DYNAMIC(CStationConfigPane, CDockablePane)

CStationConfigPane::CStationConfigPane()
{

}

CStationConfigPane::~CStationConfigPane()
{
}


BEGIN_MESSAGE_MAP(CStationConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CStationConfigPane 消息处理程序
int CStationConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgStationConfig.Create(IDD_STATIONCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgStationConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CStationConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgStationConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgStationConfig.MoveWindow(rect);
	}
}


void CStationConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgStationConfig.DestroyWindow();
}

