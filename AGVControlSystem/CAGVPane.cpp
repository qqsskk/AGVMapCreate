// CAGVPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVPane.h"


// CAGVPane

IMPLEMENT_DYNAMIC(CAGVPane, CDockablePane)

CAGVPane::CAGVPane()
{

}

CAGVPane::~CAGVPane()
{
}


BEGIN_MESSAGE_MAP(CAGVPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CAGVPane 消息处理程序
int CAGVPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgAGV.Create(IDD_AGV_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgAGV.ShowWindow(SW_SHOW);

	return 0;
}


void CAGVPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgAGV.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgAGV.MoveWindow(rect);
	}
}


void CAGVPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgAGV.DestroyWindow();
}