// CPathTabPane.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CPathTabPane.h"


// CPathTabPane

IMPLEMENT_DYNAMIC(CPathTabPane, CDockablePane)

CPathTabPane::CPathTabPane()
{

}

CPathTabPane::~CPathTabPane()
{
}


BEGIN_MESSAGE_MAP(CPathTabPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CPathTabPane 消息处理程序
int CPathTabPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_dlgPathTab.Create(IDD_PATHTAB_DIALOG, this))
	{
		TRACE0("未能创建输出选项卡窗口/n");
		return -1;      // 未能创建
	}
	m_dlgPathTab.ShowWindow(SW_SHOW);
	return 0;
}


void CPathTabPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_dlgPathTab.GetSafeHwnd())
	{	
		CRect rect;
		GetClientRect(rect);

		m_dlgPathTab.MoveWindow(rect);
	}
}


void CPathTabPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_dlgPathTab.DestroyWindow();
}


void CPathTabPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

