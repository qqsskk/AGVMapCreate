// CAGVDlg.cpp: 实现文件
// CAGVDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVDlg.h"
#include "afxdialogex.h"


// CAGVDlg 对话框

IMPLEMENT_DYNAMIC(CAGVDlg, CDialogEx)

CAGVDlg::CAGVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGV_DIALOG, pParent)
{

}

CAGVDlg::~CAGVDlg()
{

}

void CAGVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AGV, m_listCtrlAGV);
}

BEGIN_MESSAGE_MAP(CAGVDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAGVDlg 消息处理程序
void CAGVDlg::InitListCtrl()
{
	m_listCtrlAGV.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_NO + 1, _T("AGV编号"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_TYPE + 1, _T("AGV类型"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_STATUS + 1, _T("AGV状态"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_POWER + 1, _T("AGV电量"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_SPEED + 1, _T("AGV速度"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_CURMARK + 1, _T("AGV当前坐标"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_ENDMARK + 1, _T("AGV终点坐标"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_ACTION + 1, _T("AGV动作"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_ACTSTA + 1, _T("AGV动作状态"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_CARGO + 1, _T("AGV载货数量"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_ERROR + 1, _T("AGV异常信息"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_OLDMARK + 1, _T("AGV上一坐标"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_NEXTMARK + 1, _T("AGV下一坐标"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_HISMARK + 1, _T("AGV历史坐标"), LVCFMT_CENTER);
	m_listCtrlAGV.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlAGV.SetExtendedStyle(m_listCtrlAGV.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlAGV.EnableTips();

	return;
}


void CAGVDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlAGV.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlAGV.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlAGV.SetColumnWidth(LISTITEM_NO, 100);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_TYPE, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_STATUS, rcClient.Width() * 2 / 12 < 100 ? 100: rcClient.Width() * 2 / 12);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_POWER, 100);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_CURMARK, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_ENDMARK, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_ACTION, rcClient.Width() * 2 / 12 < 100 ? 100 : rcClient.Width() * 2 / 12);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_ACTSTA, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_CARGO, 100);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_ERROR, rcClient.Width() * 2 / 12 < 150 ? 150 : rcClient.Width() * 2 / 12);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_SPEED, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_OLDMARK, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_NEXTMARK,150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_HISMARK, rcClient.Width() < 100 ? 100 : rcClient.Width());

	return;
}

void CAGVDlg::Sort()
{
	int nRow = m_listCtrlAGV.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlAGV.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlAGV.GetItemText(i, j);
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
	m_listCtrlAGV.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlAGV.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlAGV.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CAGVDlg::UpdateListCtrlItem(CTCPMNavAGV * pAGV)
{
	int nItemCount = m_listCtrlAGV.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< AGV编号 */
	pAGV->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlAGV.GetItemText(i, LISTITEM_NO)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlAGV.InsertItem(nItemCount, cstrNo);
	}

	CString cstrType = _T("");			/*!< AGV类型 */
	CString cstrDrive = _T("");			/*!< AGV可运行方向 */
	CString cstrStatus = _T("");		/*!< AGV当前状态 */
	CString cstrPower = _T("");			/*!< AGV电量 */
	CString cstrCurRFID = _T("");		/*!< AGV当前坐标 */
	CString cstrEndRFID = _T("");		/*!< AGV终点坐标 */
	CString cstrAction = _T("");		/*!< AGV动作 */
	CString cstrActionStatus = _T("");	/*!< AGV动作状态*/
	CString cstrCargo = _T("");			/*!< AGV载货数量*/
	CString cstrError = _T("");			/*!< AGV异常信息 */
	CString cstrSpeed = _T("");			/*!< AGV速度 */
	CString cstrConnect = _T("");		/*!< AGV连接状态 */
	CString cstrOldRFID = _T("");		/*!< AGV上一坐标 */
	CString cstrNextRFID = _T("");		/*!< AGV下一坐标 */
	CString cstrHisRFID = _T("");		/*!< AGV历史坐标 */
	COLORREF color = m_listCtrlAGV.GetBkColor();

	pAGV->GetType(cstrType);
	pAGV->GetDriver(cstrDrive);
	pAGV->GetRealitySpeed(cstrSpeed, color);
	pAGV->GetStatus(cstrStatus, color);
	pAGV->GetAction(cstrAction, cstrActionStatus, color);
	pAGV->GetPower(cstrPower, color);
	pAGV->GetCurRFID(cstrCurRFID);
	pAGV->GetEndRFID(cstrEndRFID);
	pAGV->GetCargo(cstrCargo);
	pAGV->GetError(cstrError, color);
	pAGV->GetOldRFID(cstrOldRFID);
	pAGV->GetNextRFID(cstrNextRFID);
	pAGV->GetOldRFIDList(cstrHisRFID);
	pAGV->IsConnected(cstrConnect, color);

	m_listCtrlAGV.SetRedraw(false);

	m_listCtrlAGV.SetItemText(nRow, LISTITEM_TYPE, cstrDrive + cstrType);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_STATUS, cstrStatus);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_POWER, cstrPower);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_CURMARK, cstrCurRFID);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_ENDMARK, cstrEndRFID);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_ACTION, cstrAction);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_ACTSTA, cstrActionStatus);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_CARGO, cstrCargo);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_ERROR, cstrError);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_SPEED, cstrSpeed);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_OLDMARK, cstrOldRFID);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_NEXTMARK, cstrNextRFID);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_HISMARK, cstrHisRFID);

	m_listCtrlAGV.SetItemColor(nRow, COLOR_BLACK, color);

	m_listCtrlAGV.SetRedraw(true);

	Sort();

	return;
}


void CAGVDlg::ClearListCtrlItem()
{
	m_listCtrlAGV.DeleteAllItems();
}

BOOL CAGVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_menu.LoadMenu(IDR_AGVPOP_MENU);

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CTCPMNavAGV(5, _AGVTYPE_PULL, _AGVDRIVER_SINGLE, 48.0f, "192.168.1.5"));
	UpdateListCtrlItem(&CTCPMNavAGV(1, _AGVTYPE_TRAVEL, _AGVDRIVER_DOUBLE, 48.0f, "192.168.1.1"));
	UpdateListCtrlItem(&CTCPMNavAGV(3, _AGVTYPE_SNEAK, _AGVDRIVER_SINGLE, 48.0f, "192.168.1.3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAGVDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CAGVDlg::PreTranslateMessage(MSG* pMsg)
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


void CAGVDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	for (int i = 0; i < m_listCtrlAGV.GetItemCount(); ++i)
	{
		if (m_listCtrlAGV.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
			&& m_listCtrlAGV.GetItemState(i, LVIS_FOCUSED) == LVIS_FOCUSED)
		{
			// 右键弹出菜单
			CMenu *subMenu = m_menu.GetSubMenu(0);
			subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	
			break;
		}
	}

	return;
}


void CAGVDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_menu.DestroyMenu();
}


BOOL CAGVDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	for (int i = 0; i < m_listCtrlAGV.GetItemCount(); ++i)
	{
		if (m_listCtrlAGV.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
			&& m_listCtrlAGV.GetItemState(i, LVIS_FOCUSED) == LVIS_FOCUSED)
		{
			CWnd* pMain = AfxGetMainWnd();
			::PostMessage(pMain->m_hWnd, WM_COMMAND_AGV, wParam, _ttoi(m_listCtrlAGV.GetItemText(i, LISTITEM_NO)));
			break;
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}