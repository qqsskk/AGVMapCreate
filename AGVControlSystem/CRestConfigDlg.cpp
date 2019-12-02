// CRestConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CRestConfigDlg.h"
#include "afxdialogex.h"


// CRestConfigDlg 对话框

IMPLEMENT_DYNAMIC(CRestConfigDlg, CDialogEx)

CRestConfigDlg::CRestConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESTCONFIG_DIALOG, pParent)
{

}

CRestConfigDlg::~CRestConfigDlg()
{
}

void CRestConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REST, m_listCtrlRest);
}


BEGIN_MESSAGE_MAP(CRestConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRestConfigDlg 消息处理程序
void CRestConfigDlg::InitListCtrl()
{
	m_listCtrlRest.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlRest.InsertColumn(LISTITEM_NO + 1, _T("待机位编号"), LVCFMT_CENTER);
	m_listCtrlRest.InsertColumn(LISTITEM_LOCATION + 1, _T("待机位坐标"), LVCFMT_CENTER);
	m_listCtrlRest.InsertColumn(LISTITEM_NEXT + 1, _T("下一待机位编号"), LVCFMT_CENTER);
	m_listCtrlRest.InsertColumn(LISTITEM_STOPLIST + 1, _T("禁行坐标组"), LVCFMT_CENTER);
	m_listCtrlRest.InsertColumn(LISTITEM_NAME + 1, _T("待机位名称"), LVCFMT_CENTER);
	m_listCtrlRest.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlRest.SetExtendedStyle(m_listCtrlRest.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlRest.EnableTips();

	return;
}


void CRestConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlRest.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlRest.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlRest.SetColumnWidth(LISTITEM_NO, 100);
	m_listCtrlRest.SetColumnWidth(LISTITEM_LOCATION, 100);
	m_listCtrlRest.SetColumnWidth(LISTITEM_NEXT, 130);
	m_listCtrlRest.SetColumnWidth(LISTITEM_STOPLIST, rcClient.Width() < 100 ? 100 : rcClient.Width());
	m_listCtrlRest.SetColumnWidth(LISTITEM_NAME, rcClient.Width() * 2 / 5 < 100 ? 100 : rcClient.Width() * 2 / 5);

	return;
}


void CRestConfigDlg::Sort()
{
	int nRow = m_listCtrlRest.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlRest.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlRest.GetItemText(i, j);
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
	m_listCtrlRest.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlRest.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlRest.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CRestConfigDlg::UpdateListCtrlItem(CMNavRestPosition * pRest)
{
	int nItemCount = m_listCtrlRest.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pRest->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlRest.GetItemText(i, LISTITEM_NO)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlRest.InsertItem(nItemCount, cstrNo);
	}

	CString cstrLoaction = _T("");		/*!< 坐标 */
	CString cstrNext = _T("");			/*!< 下一待机位 */
	CString cstrStop = _T("");			/*!< 禁行坐标列表 */
	CString cstrName = _T("");			/*!< 待机位名称 */

	pRest->GetLocation(cstrLoaction);
	pRest->GetNext(cstrNext);
	pRest->GetStopRFIDList(cstrStop);
	pRest->GetName(cstrName);

	m_listCtrlRest.SetRedraw(false);

	m_listCtrlRest.SetItemText(nRow, LISTITEM_LOCATION, cstrLoaction);
	m_listCtrlRest.SetItemText(nRow, LISTITEM_NEXT, cstrNext);
	m_listCtrlRest.SetItemText(nRow, LISTITEM_STOPLIST, cstrStop);
	m_listCtrlRest.SetItemText(nRow, LISTITEM_NAME, cstrName);

	m_listCtrlRest.SetRedraw(true);

	Sort();

	return;
}


void CRestConfigDlg::ClearListCtrlItem()
{
	m_listCtrlRest.DeleteAllItems();
}

BOOL CRestConfigDlg::OnInitDialog()
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


void CRestConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CRestConfigDlg::PreTranslateMessage(MSG* pMsg)
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