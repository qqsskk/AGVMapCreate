// CCAWatchConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCAWatchConfigDlg.h"
#include "afxdialogex.h"


// CCAWatchConfigDlg 对话框

IMPLEMENT_DYNAMIC(CCAWatchConfigDlg, CDialogEx)

CCAWatchConfigDlg::CCAWatchConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAWATCHCONFIG_DIALOG, pParent)
{

}

CCAWatchConfigDlg::~CCAWatchConfigDlg()
{
}

void CCAWatchConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WATCH, m_listCtrlWatch);
}


BEGIN_MESSAGE_MAP(CCAWatchConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCAWatchConfigDlg 消息处理程序
void CCAWatchConfigDlg::InitListCtrl()
{
	m_listCtrlWatch.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlWatch.InsertColumn(1, _T("无线手表编号"), LVCFMT_CENTER);
	m_listCtrlWatch.InsertColumn(2, _T("受控主机"), LVCFMT_CENTER);
	m_listCtrlWatch.InsertColumn(3, _T("响应工位"), LVCFMT_CENTER);
	m_listCtrlWatch.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlWatch.SetExtendedStyle(m_listCtrlWatch.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlWatch.EnableTips();

	return;
}


void CCAWatchConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlWatch.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlWatch.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlWatch.SetColumnWidth(0, (rcClient.Width() - 5) / 3 < 150 ? 150 : (rcClient.Width() - 5) / 3);
	m_listCtrlWatch.SetColumnWidth(1, (rcClient.Width() - 5) / 3 < 100 ? 100 : (rcClient.Width() - 5) / 3);
	m_listCtrlWatch.SetColumnWidth(2, (rcClient.Width() - 5) / 3 < 100 ? 100 : (rcClient.Width() - 5) / 3);

	return;
}


void CCAWatchConfigDlg::Sort()
{
	int nRow = m_listCtrlWatch.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlWatch.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlWatch.GetItemText(i, j);
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
	m_listCtrlWatch.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlWatch.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlWatch.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CCAWatchConfigDlg::UpdateListCtrlItem(CAWatch* pWatch)
{
	int nItemCount = m_listCtrlWatch.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pWatch->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlWatch.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlWatch.InsertItem(nItemCount, cstrNo);
	}

	CString cstrCA = _T("");			/*!< 受控主机 */
	CString cstrStation = _T("");		/*!< 工位 */

	pWatch->GetControlCA(cstrCA);
	pWatch->GetStation(cstrStation);

	m_listCtrlWatch.SetRedraw(false);

	m_listCtrlWatch.SetItemText(nRow, 1, cstrCA);
	m_listCtrlWatch.SetItemText(nRow, 2, cstrStation);

	m_listCtrlWatch.SetRedraw(true);

	Sort();

	return;
}


void CCAWatchConfigDlg::ClearListCtrlItem()
{
	m_listCtrlWatch.DeleteAllItems();
}


BOOL CCAWatchConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CAWatch(5));
	UpdateListCtrlItem(&CAWatch(1, 1));
	UpdateListCtrlItem(&CAWatch(3,3,1));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCAWatchConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CCAWatchConfigDlg::PreTranslateMessage(MSG* pMsg)
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