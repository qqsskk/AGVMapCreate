// CChargerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerDlg.h"
#include "afxdialogex.h"


// CChargerDlg 对话框

IMPLEMENT_DYNAMIC(CChargerDlg, CDialogEx)

CChargerDlg::CChargerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARGER_DIALOG, pParent)
{

}

CChargerDlg::~CChargerDlg()
{
}

void CChargerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARGER, m_listCtrlCharger);
}


BEGIN_MESSAGE_MAP(CChargerDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CChargerDlg 消息处理程序
void CChargerDlg::InitListCtrl()
{
	m_listCtrlCharger.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_NO + 1, _T("充电器编号"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_STATUS + 1, _T("充电状态"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_DURATIVETIME + 1, _T("已充电时间"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_MINTIME + 1, _T("最短充电时长"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_POWER + 1, _T("当前电池电量"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(LISTITEM_NAME + 1, _T("充电器名称"), LVCFMT_CENTER);
	m_listCtrlCharger.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlCharger.SetExtendedStyle(m_listCtrlCharger.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlCharger.EnableTips();

	return;
}


void CChargerDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlCharger.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlCharger.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlCharger.SetColumnWidth(LISTITEM_NO, 100);
	m_listCtrlCharger.SetColumnWidth(LISTITEM_STATUS, 100);
	m_listCtrlCharger.SetColumnWidth(LISTITEM_DURATIVETIME, rcClient.Width() * 2 / 4 < 100 ? 100 : rcClient.Width() * 2 / 4);
	m_listCtrlCharger.SetColumnWidth(LISTITEM_MINTIME, rcClient.Width() * 2 / 4 < 100 ? 100 : rcClient.Width() * 2 / 4);
	m_listCtrlCharger.SetColumnWidth(LISTITEM_POWER, 150);
	m_listCtrlCharger.SetColumnWidth(LISTITEM_NAME, rcClient.Width() * 2 / 4 < 100 ? 100 : rcClient.Width() * 2 / 4);

	return;
}


void CChargerDlg::Sort()
{
	int nRow = m_listCtrlCharger.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlCharger.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlCharger.GetItemText(i, j);
		}
	}

	// 排序
	for (int i = 1; i < nRow; ++i)
	{
		for (int j = 0; j < nRow - 1; ++j)
		{
			// 依据编号排序
			if (_ttoi(ppCStr[j][LISTITEM_NO]) > _ttoi(ppCStr[j + 1][LISTITEM_NO]))
			{
				// 调位
				CString *pCStrTmp = ppCStr[j];
				ppCStr[j] = ppCStr[j + 1];
				ppCStr[j + 1] = pCStrTmp;
			}
		}
	}

	// 更新
	m_listCtrlCharger.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlCharger.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlCharger.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CChargerDlg::UpdateListCtrlItem(CMNavCharger * pCharger)
{
	int nItemCount = m_listCtrlCharger.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pCharger->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlCharger.GetItemText(i, LISTITEM_NO)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlCharger.InsertItem(nItemCount, cstrNo);
	}

	CString cstrStatus = _T("");		/*!< 充电状态 */
	CString cstrDurativeTime = _T("");	/*!< 充电时间 */
	CString cstrMinTime = _T("");		/*!< 最短充电时长 */
	CString cstrPower = _T("");			/*!< 当前电池电量 */
	CString cstrName = _T("");			/*!< 充电时间 */
	COLORREF color = m_listCtrlCharger.GetBkColor();

	pCharger->GetStatus(cstrStatus, color);
	pCharger->GetDurativeTime(cstrDurativeTime);
	pCharger->GetMinDurativeTime(cstrMinTime);
	pCharger->GetPower(cstrPower);
	pCharger->GetName(cstrName);

	m_listCtrlCharger.SetRedraw(false);

	m_listCtrlCharger.SetItemText(nRow, LISTITEM_STATUS, cstrStatus);
	m_listCtrlCharger.SetItemText(nRow, LISTITEM_DURATIVETIME, cstrDurativeTime);
	m_listCtrlCharger.SetItemText(nRow, LISTITEM_MINTIME, cstrMinTime);
	m_listCtrlCharger.SetItemText(nRow, LISTITEM_POWER, cstrPower);
	m_listCtrlCharger.SetItemText(nRow, LISTITEM_NAME, cstrName);

	m_listCtrlCharger.SetItemColor(nRow, COLOR_BLACK, color);

	m_listCtrlCharger.SetRedraw(true);

	Sort();

	return;
}


void CChargerDlg::ClearListCtrlItem()
{
	m_listCtrlCharger.DeleteAllItems();
}

BOOL CChargerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CMNavCharger(5, 5, 1,"测试5"));
	UpdateListCtrlItem(&CMNavCharger(1, 1, 1, "测试1"));
	UpdateListCtrlItem(&CMNavCharger(3, 3, 1, "测试3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChargerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CChargerDlg::PreTranslateMessage(MSG* pMsg)
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