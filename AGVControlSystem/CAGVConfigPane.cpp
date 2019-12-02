// CAGVConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVConfigPane.h"


// CAGVConfigPane

IMPLEMENT_DYNAMIC(CAGVConfigPane, CDockablePane)

CAGVConfigPane::CAGVConfigPane()
{

}

CAGVConfigPane::~CAGVConfigPane()
{
}


BEGIN_MESSAGE_MAP(CAGVConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CAGVConfigPane 消息处理程序
int CAGVConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgAGVConfig.Create(IDD_AGVCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgAGVConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CAGVConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgAGVConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgAGVConfig.MoveWindow(rect);
	}
}


void CAGVConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgAGVConfig.DestroyWindow();
}

