// CChargerConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerConfigDlg.h"
#include "afxdialogex.h"


// CChargerConfigDlg 对话框

IMPLEMENT_DYNAMIC(CChargerConfigDlg, CDialogEx)

CChargerConfigDlg::CChargerConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARGERCONFIG_DIALOG, pParent)
{

}

CChargerConfigDlg::~CChargerConfigDlg()
{
}

void CChargerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARGER, m_listCtrlCharger);
}


BEGIN_MESSAGE_MAP(CChargerConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CChargerConfigDlg 消息处理程序
void CChargerConfigDlg::InitListCtrl()
{
	m_listCtrlCharger.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(1, _T("充电器编号"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(2, _T("充电器坐标"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(3, _T("充电控制器编号"), LVCFMT_CENTER);
	m_listCtrlCharger.InsertColumn(4, _T("充电器名称"), LVCFMT_CENTER);
	m_listCtrlCharger.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlCharger.SetExtendedStyle(m_listCtrlCharger.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlCharger.EnableTips();

	return;
}


void CChargerConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlCharger.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlCharger.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlCharger.SetColumnWidth(0, 100);
	m_listCtrlCharger.SetColumnWidth(1, 100);
	m_listCtrlCharger.SetColumnWidth(2, 150);
	m_listCtrlCharger.SetColumnWidth(3, rcClient.Width() - 355 < 100 ? 100: rcClient.Width() - 355);

	return;
}


void CChargerConfigDlg::Sort()
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


void CChargerConfigDlg::UpdateListCtrlItem(CMNavCharger * pCharger)
{
	int nItemCount = m_listCtrlCharger.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pCharger->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlCharger.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlCharger.InsertItem(nItemCount, cstrNo);
	}

	CString cstrLocation = _T("");		/*!< 坐标 */
	CString cstrController = _T("");	/*!< 控制器 */
	CString cstrName = _T("");			/*!< 名称 */

	pCharger->GetLocation(cstrLocation);
	pCharger->GetController(cstrController);
	pCharger->GetName(cstrName);

	m_listCtrlCharger.SetRedraw(false);

	m_listCtrlCharger.SetItemText(nRow, 1, cstrLocation);
	m_listCtrlCharger.SetItemText(nRow, 2, cstrController);
	m_listCtrlCharger.SetItemText(nRow, 3, cstrName);

	m_listCtrlCharger.SetRedraw(true);

	Sort();

	return;
}


void CChargerConfigDlg::ClearListCtrlItem()
{
	m_listCtrlCharger.DeleteAllItems();
}

BOOL CChargerConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CMNavCharger(5, 5, 1, "测试5"));
	UpdateListCtrlItem(&CMNavCharger(1, 1, 1, "测试1"));
	UpdateListCtrlItem(&CMNavCharger(3, 3, 1, "测试3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChargerConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CChargerConfigDlg::PreTranslateMessage(MSG* pMsg)
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