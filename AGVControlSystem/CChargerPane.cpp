// CChargerPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerPane.h"


// CChargerPane

IMPLEMENT_DYNAMIC(CChargerPane, CDockablePane)

CChargerPane::CChargerPane()
{

}

CChargerPane::~CChargerPane()
{
}


BEGIN_MESSAGE_MAP(CChargerPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChargerPane 消息处理程序
int CChargerPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgCharger.Create(IDD_CHARGER_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgCharger.ShowWindow(SW_SHOW);

	return 0;
}


void CChargerPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgCharger.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgCharger.MoveWindow(rect);
	}
}


void CChargerPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgCharger.DestroyWindow();
}

