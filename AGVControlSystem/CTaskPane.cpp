// CTaskPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CTaskPane.h"


// CTaskPane

IMPLEMENT_DYNAMIC(CTaskPane, CDockablePane)

CTaskPane::CTaskPane()
{

}

CTaskPane::~CTaskPane()
{
}


BEGIN_MESSAGE_MAP(CTaskPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CTaskPane 消息处理程序
int CTaskPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgTask.Create(IDD_TASK_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgTask.ShowWindow(SW_SHOW);

	return 0;
}


void CTaskPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgTask.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgTask.MoveWindow(rect);
	}
}


void CTaskPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgTask.DestroyWindow();
}

