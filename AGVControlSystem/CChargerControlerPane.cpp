// CChargerControlerPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerControlerPane.h"


// CChargerControlerPane

IMPLEMENT_DYNAMIC(CChargerControlerPane, CDockablePane)

CChargerControlerPane::CChargerControlerPane()
{

}

CChargerControlerPane::~CChargerControlerPane()
{
}


BEGIN_MESSAGE_MAP(CChargerControlerPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChargerControlerPane 消息处理程序
int CChargerControlerPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgChargerCtrlerConfig.Create(IDD_CHARGERCONTROLERCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgChargerCtrlerConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CChargerControlerPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgChargerCtrlerConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgChargerCtrlerConfig.MoveWindow(rect);
	}
}


void CChargerControlerPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgChargerCtrlerConfig.DestroyWindow();
}


