// CCAConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCAConfigPane.h"


// CCAConfigPane

IMPLEMENT_DYNAMIC(CCAConfigPane, CDockablePane)

CCAConfigPane::CCAConfigPane()
{

}

CCAConfigPane::~CCAConfigPane()
{
}


BEGIN_MESSAGE_MAP(CCAConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CCAConfigPane 消息处理程序
int CCAConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgCAConfig.Create(IDD_CACONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgCAConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CCAConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgCAConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgCAConfig.MoveWindow(rect);
	}
}


void CCAConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgCAConfig.DestroyWindow();
}

