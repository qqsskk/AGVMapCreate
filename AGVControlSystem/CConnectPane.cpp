// CConnectPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CConnectPane.h"


// CConnectPane

IMPLEMENT_DYNAMIC(CConnectPane, CDockablePane)

CConnectPane::CConnectPane()
{

}

CConnectPane::~CConnectPane()
{
}


BEGIN_MESSAGE_MAP(CConnectPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CConnectPane 消息处理程序
int CConnectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgConnect.Create(IDD_CONNECT_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgConnect.ShowWindow(SW_SHOW);

	return 0;
}


void CConnectPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgConnect.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgConnect.MoveWindow(rect);
	}
}


void CConnectPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgConnect.DestroyWindow();
}

