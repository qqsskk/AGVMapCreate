// CCACallConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCACallConfigDlg.h"
#include "afxdialogex.h"


// CCACallConfigDlg 对话框

IMPLEMENT_DYNAMIC(CCACallConfigDlg, CDialogEx)

CCACallConfigDlg::CCACallConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CACALLCONFIG_DIALOG, pParent)
{

}

CCACallConfigDlg::~CCACallConfigDlg()
{
}

void CCACallConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CACALL, m_listCtrlCall);
}


BEGIN_MESSAGE_MAP(CCACallConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCACallConfigDlg 消息处理程序
void CCACallConfigDlg::InitListCtrl()
{
	m_listCtrlCall.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_NO + 1, _T("无线呼叫器编号"), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_ENDPOINT + 1, _T("终点坐标"), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_STARTPOINT + 1, _T("起点坐标"), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_AGV + 1, _T("指定AGV类型"), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_ACTION + 1, _T("任务动作"), LVCFMT_CENTER);
	m_listCtrlCall.InsertColumn(LISTITEM_MSG + 1, _T("附属信息"), LVCFMT_CENTER);
	m_listCtrlCall.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlCall.SetExtendedStyle(m_listCtrlCall.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlCall.EnableTips();

	return;
}


void CCACallConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlCall.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlCall.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlCall.SetColumnWidth(LISTITEM_NO, 150);
	m_listCtrlCall.SetColumnWidth(LISTITEM_ENDPOINT, 100);
	m_listCtrlCall.SetColumnWidth(LISTITEM_STARTPOINT, 200);
	m_listCtrlCall.SetColumnWidth(LISTITEM_AGV, (rcClient.Width() - 455) / 3 < 150 ? 150 : (rcClient.Width() - 455) / 3);
	m_listCtrlCall.SetColumnWidth(LISTITEM_ACTION, (rcClient.Width() - 455) / 3 < 150 ? 150 : (rcClient.Width() - 455) / 3);
	m_listCtrlCall.SetColumnWidth(LISTITEM_MSG, (rcClient.Width() - 455) / 3 < 100 ? 100 : (rcClient.Width() - 455) / 3);

	return;
}


void CCACallConfigDlg::Sort()
{
	int nRow = m_listCtrlCall.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlCall.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlCall.GetItemText(i, j);
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
	m_listCtrlCall.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlCall.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlCall.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CCACallConfigDlg::UpdateListCtrlItem(CACall* pCall)
{
	int nItemCount = m_listCtrlCall.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pCall->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlCall.GetItemText(i, LISTITEM_NO)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlCall.InsertItem(nItemCount, cstrNo);
	}

	CString cstrEnd = _T("");				/*!< 终点坐标 */
	CString cstrStart = _T("");				/*!< 起点坐标 */
	CString cstrType = _T("");				/*!< 类型 */
	CString cstrMessage = _T("");			/*!< 信息 */
	CString cstrAction = _T("");			/*!< 动作 */

	pCall->GetEndPoint(cstrEnd);
	pCall->GetStartPoint(cstrStart);
	pCall->GetType(cstrType);
	pCall->GetOverheadInfomation(cstrMessage);
	pCall->GetAction(cstrAction);

	m_listCtrlCall.SetRedraw(false);

	m_listCtrlCall.SetItemText(nRow, LISTITEM_ENDPOINT, cstrEnd);
	m_listCtrlCall.SetItemText(nRow, LISTITEM_STARTPOINT, cstrStart);
	m_listCtrlCall.SetItemText(nRow, LISTITEM_AGV, cstrType);
	m_listCtrlCall.SetItemText(nRow, LISTITEM_ACTION, cstrAction);
	m_listCtrlCall.SetItemText(nRow, LISTITEM_MSG, cstrMessage);

	m_listCtrlCall.SetRedraw(true);

	Sort();

	return;
}


void CCACallConfigDlg::ClearListCtrlItem()
{
	m_listCtrlCall.DeleteAllItems();
}


BOOL CCACallConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CACall(5, 5, 0, 0,"测试5"));
	UpdateListCtrlItem(&CACall(1, 1, 1, _AGVTYPE_ARM, "测试1"));
	UpdateListCtrlItem(&CACall(3, 3, 3, 0, "测试3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCACallConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CCACallConfigDlg::PreTranslateMessage(MSG* pMsg)
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