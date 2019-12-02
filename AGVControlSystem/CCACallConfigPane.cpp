// CCACallConfigPane.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCACallConfigPane.h"


// CCACallConfigPane

IMPLEMENT_DYNAMIC(CCACallConfigPane, CDockablePane)

CCACallConfigPane::CCACallConfigPane()
{

}

CCACallConfigPane::~CCACallConfigPane()
{
}


BEGIN_MESSAGE_MAP(CCACallConfigPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CCACallConfigPane 消息处理程序
int CCACallConfigPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgCallConfig.Create(IDD_CACALLCONFIG_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}

	m_dlgCallConfig.ShowWindow(SW_SHOW);

	return 0;
}


void CCACallConfigPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgCallConfig.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(rect);

		m_dlgCallConfig.MoveWindow(rect);
	}
}


void CCACallConfigPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgCallConfig.DestroyWindow();
}

