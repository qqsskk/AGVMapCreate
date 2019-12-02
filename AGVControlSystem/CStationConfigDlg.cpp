// CStationConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CStationConfigDlg.h"
#include "afxdialogex.h"


// CStationConfigDlg 对话框

IMPLEMENT_DYNAMIC(CStationConfigDlg, CDialogEx)

CStationConfigDlg::CStationConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STATIONCONFIG_DIALOG, pParent)
{

}

CStationConfigDlg::~CStationConfigDlg()
{
}

void CStationConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STATION, m_listCtrlStation);
}


BEGIN_MESSAGE_MAP(CStationConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CStationConfigDlg 消息处理程序
void CStationConfigDlg::InitListCtrl()
{
	m_listCtrlStation.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlStation.InsertColumn(1,_T("工位编号"), LVCFMT_CENTER);
	m_listCtrlStation.InsertColumn(2,_T("工位坐标"), LVCFMT_CENTER);
	m_listCtrlStation.InsertColumn(3,_T("工位名称"), LVCFMT_CENTER);
	m_listCtrlStation.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlStation.SetExtendedStyle(m_listCtrlStation.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlStation.EnableTips();

	return;
}


void CStationConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlStation.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlStation.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlStation.SetColumnWidth(0, 100);
	m_listCtrlStation.SetColumnWidth(1, 100);
	m_listCtrlStation.SetColumnWidth(2, rcClient.Width() - 205 < 100 ? 100 : rcClient.Width() - 205);

	return;
}


void CStationConfigDlg::Sort()
{
	int nRow = m_listCtrlStation.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlStation.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlStation.GetItemText(i, j);
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
	m_listCtrlStation.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlStation.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlStation.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CStationConfigDlg::UpdateListCtrlItem(CMNavStation * pStation)
{
	int nItemCount = m_listCtrlStation.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pStation->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlStation.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlStation.InsertItem(nItemCount, cstrNo);
	}

	CString cstrLoaction = _T("");		/*!< 坐标 */
	CString cstrName = _T("");			/*!< 待机位名称 */

	pStation->GetLocation(cstrLoaction);
	pStation->GetName(cstrName);

	m_listCtrlStation.SetRedraw(false);

	m_listCtrlStation.SetItemText(nRow, 1, cstrLoaction);
	m_listCtrlStation.SetItemText(nRow, 2, cstrName);

	m_listCtrlStation.SetRedraw(true);

	Sort();

	return;
}


void CStationConfigDlg::ClearListCtrlItem()
{
	m_listCtrlStation.DeleteAllItems();
}

BOOL CStationConfigDlg::OnInitDialog()
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


void CStationConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CStationConfigDlg::PreTranslateMessage(MSG* pMsg)
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