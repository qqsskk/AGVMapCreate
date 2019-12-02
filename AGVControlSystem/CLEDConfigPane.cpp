// CLEDConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CLEDConfigPane.h"


// CLEDConfigPane

IMPLEMENT_DYNAMIC(CLEDConfigPane, CDockablePane)

CLEDConfigPane::CLEDConfigPane()
{

}

CLEDConfigPane::~CLEDConfigPane()
{
}


BEGIN_MESSAGE_MAP(CLEDConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CLEDConfigPane 消息处理程序
int CLEDConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgLEDConfig.Create(IDD_LEDCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgLEDConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CLEDConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgLEDConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgLEDConfig.MoveWindow(rect);
	}
}


void CLEDConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgLEDConfig.DestroyWindow();
}


