// CTrafficConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CTrafficConfigDlg.h"
#include "afxdialogex.h"


// CTrafficConfigDlg 对话框

IMPLEMENT_DYNAMIC(CTrafficConfigDlg, CDialogEx)

CTrafficConfigDlg::CTrafficConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAFFICCONFIG_DIALOG, pParent)
{

}

CTrafficConfigDlg::~CTrafficConfigDlg()
{
}

void CTrafficConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRAFFIC, m_listCtrlTraffic);
}


BEGIN_MESSAGE_MAP(CTrafficConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTrafficConfigDlg 消息处理程序
void CTrafficConfigDlg::InitListCtrl()
{
	m_listCtrlTraffic.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlTraffic.InsertColumn(1, _T("管制坐标"), LVCFMT_CENTER);
	m_listCtrlTraffic.InsertColumn(2, _T("禁行坐标组"), LVCFMT_CENTER);
	m_listCtrlTraffic.InsertColumn(3, _T("比对管制域组"), LVCFMT_CENTER);
	m_listCtrlTraffic.InsertColumn(4, _T("上一坐标"), LVCFMT_CENTER);
	m_listCtrlTraffic.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlTraffic.SetExtendedStyle(m_listCtrlTraffic.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlTraffic.EnableTips();

	return;
}


void CTrafficConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlTraffic.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlTraffic.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlTraffic.SetColumnWidth(0, 100);
	m_listCtrlTraffic.SetColumnWidth(3, 100);
	m_listCtrlTraffic.SetColumnWidth(1, (rcClient.Width() - 205) / 2 < 100 ? 100 : (rcClient.Width() - 205) / 2);
	m_listCtrlTraffic.SetColumnWidth(2, (rcClient.Width() - 205) / 2 < 150 ? 150 : (rcClient.Width() - 205) / 2);

	return;
}


void CTrafficConfigDlg::Sort()
{
	int nRow = m_listCtrlTraffic.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlTraffic.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

	// 初始化
	CString **ppCStr = new CString*[nRow];

	for (int i = 0; i < nRow; ++i)
	{
		ppCStr[i] = new CString[nCol];
	}

	// 赋值
	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			ppCStr[i][j] = m_listCtrlTraffic.GetItemText(i, j);
		}
	}

	// 排序
	for (int i = 1; i < nRow; ++i)
	{
		for (int j = 0; j < nRow - 1; ++j)
		{
			// 依据编号排序
			if (_ttoi(ppCStr[j][0]) > _ttoi(ppCStr[j + 1][0]))
			{
				// 调位
				CString *pCStrTmp = ppCStr[j];
				ppCStr[j] = ppCStr[j + 1];
				ppCStr[j + 1] = pCStrTmp;
			}
		}
	}

	// 更新
	m_listCtrlTraffic.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlTraffic.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlTraffic.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CTrafficConfigDlg::UpdateListCtrlItem(CMTTrafficCtrlArea* pTraffic)
{
	int nItemCount = m_listCtrlTraffic.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pTraffic->GetCtrlRFID(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlTraffic.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlTraffic.InsertItem(nItemCount, cstrNo);
	}

	CString cstrStop = _T("");			/*!< 禁行坐标组 */
	CString cstrCompare = _T("");		/*!< 比对管制域组 */
	CString cstrBefore = _T("");		/*!< 上一坐标 */

	pTraffic->GetStopRFIDList(cstrStop);
	pTraffic->GetCompareAreaList(cstrCompare);
	pTraffic->GetBeforeRFID(cstrBefore);

	m_listCtrlTraffic.SetRedraw(false);

	m_listCtrlTraffic.SetItemText(nRow, 1, cstrStop);
	m_listCtrlTraffic.SetItemText(nRow, 2, cstrCompare);
	m_listCtrlTraffic.SetItemText(nRow, 2, cstrBefore);

	m_listCtrlTraffic.SetRedraw(true);

	Sort();

	return;
}


void CTrafficConfigDlg::ClearListCtrlItem()
{
	m_listCtrlTraffic.DeleteAllItems();
}

BOOL CTrafficConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

#define _TEST
#ifdef _TEST

#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTrafficConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CTrafficConfigDlg::PreTranslateMessage(MSG* pMsg)
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