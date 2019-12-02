// CConnectDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CConnectDlg.h"
#include "afxdialogex.h"


// CConnectDlg 对话框

IMPLEMENT_DYNAMIC(CConnectDlg, CDialogEx)

CConnectDlg::CConnectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONNECT_DIALOG, pParent)
{

}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_listCtrlDevice);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CConnectDlg 消息处理程序


void CConnectDlg::InitListCtrl()
{
	m_listCtrlDevice.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlDevice.InsertColumn(1, _T("设备名称"), LVCFMT_CENTER);
	m_listCtrlDevice.InsertColumn(2, _T("连接状态"), LVCFMT_CENTER);
	m_listCtrlDevice.InsertColumn(3, _T("连接时间"), LVCFMT_CENTER);
	m_listCtrlDevice.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlDevice.SetExtendedStyle(m_listCtrlDevice.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlDevice.EnableTips();

	return;
}

void CConnectDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if(m_listCtrlDevice.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlDevice.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlDevice.SetColumnWidth(0, (rcClient.Width() - 105) / 2 < 100 ? 100 : (rcClient.Width() - 105) / 2);
	m_listCtrlDevice.SetColumnWidth(1, 100);
	m_listCtrlDevice.SetColumnWidth(2, (rcClient.Width() - 105) / 2 < 100 ? 100 : (rcClient.Width() - 105) / 2);

	return;
}

void CConnectDlg::UpdateListCtrlItem(CString cstrName, CString cstrStatus, CString cstrTime)
{
	UpdateListCtrlItem(cstrName, cstrStatus, cstrTime, m_listCtrlDevice.GetBkColor());
}

void CConnectDlg::UpdateListCtrlItem(CString cstrName, CString cstrStatus, CString cstrTime, COLORREF color)
{
	int nItemCount = m_listCtrlDevice.GetItemCount();	/*!< 控件总行数 */

	// 查找更新成员
	for (int i = 0; i < nItemCount; ++i)
	{
		// 设备名称相同
		if (cstrName == m_listCtrlDevice.GetItemText(i, 0))
		{
			// 更新
			m_listCtrlDevice.SetRedraw(false);

			m_listCtrlDevice.SetItemText(i, 1, cstrStatus);
			m_listCtrlDevice.SetItemText(i, 2, cstrTime);

			m_listCtrlDevice.SetItemColor(i,COLOR_BLACK,color);

			m_listCtrlDevice.SetRedraw(true);

			return;
		}
	}

	// 创建
	m_listCtrlDevice.SetRedraw(false);

	int nRow = m_listCtrlDevice.InsertItem(nItemCount, cstrName);
	m_listCtrlDevice.SetItemText(nRow, 1, cstrStatus);
	m_listCtrlDevice.SetItemText(nRow, 2, cstrTime);

	m_listCtrlDevice.SetItemColor(nRow, COLOR_BLACK, color);

	m_listCtrlDevice.SetRedraw(true);

	return;
}

void CConnectDlg::ClearListCtrlItem()
{
	m_listCtrlDevice.DeleteAllItems();
}

BOOL CConnectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(_T("1号无线主机"), _T("未连接"), _T("0000-00-00 0:0:0"), COLOR_GRAY);
	UpdateListCtrlItem(_T("1号AGV"), _T("未连接"), _T("0000-00-00 0:0:0"), COLOR_GRAY);
	UpdateListCtrlItem(_T("服务端:192.168.1.1:8000"), _T("已监听"), _T("2019-02-11 11:52:00"), COLOR_GREEN);
	UpdateListCtrlItem(_T("2号AGV"), _T("连接中断"), _T("0000-00-00 0:0:0"), COLOR_RED);
	UpdateListCtrlItem(_T("2号AGV"), _T("已连接"), _T("2019-02-11 12:01:00"), COLOR_GREEN);
	UpdateListCtrlItem(_T("1号AGV"), _T("连接中断"), _T("0000-00-00 0:0:0"), COLOR_RED);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CConnectDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CConnectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return TRUE;
	case VK_RETURN:
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
