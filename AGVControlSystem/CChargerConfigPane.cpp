// CChargerConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerConfigPane.h"


// CChargerConfigPane

IMPLEMENT_DYNAMIC(CChargerConfigPane, CDockablePane)

CChargerConfigPane::CChargerConfigPane()
{

}

CChargerConfigPane::~CChargerConfigPane()
{
}


BEGIN_MESSAGE_MAP(CChargerConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChargerConfigPane 消息处理程序
int CChargerConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgChargerConfig.Create(IDD_CHARGERCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgChargerConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CChargerConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgChargerConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgChargerConfig.MoveWindow(rect);
	}
}


void CChargerConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgChargerConfig.DestroyWindow();
}

