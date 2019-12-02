// CRestConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CRestConfigPane.h"


// CRestConfigPane

IMPLEMENT_DYNAMIC(CRestConfigPane, CDockablePane)

CRestConfigPane::CRestConfigPane()
{

}

CRestConfigPane::~CRestConfigPane()
{
}


BEGIN_MESSAGE_MAP(CRestConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CRestConfigPane 消息处理程序
int CRestConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgRestConfig.Create(IDD_RESTCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgRestConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CRestConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgRestConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgRestConfig.MoveWindow(rect);
	}
}


void CRestConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgRestConfig.DestroyWindow();
}

