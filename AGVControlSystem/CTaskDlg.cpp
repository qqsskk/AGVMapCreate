// CTaskDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CTaskDlg.h"
#include "afxdialogex.h"


// CTaskDlg 对话框

IMPLEMENT_DYNAMIC(CTaskDlg, CDialogEx)

CTaskDlg::CTaskDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASK_DIALOG, pParent)
{

}

CTaskDlg::~CTaskDlg()
{
}

void CTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK, m_listCtrlTask);
}


BEGIN_MESSAGE_MAP(CTaskDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTaskDlg 消息处理程序
void CTaskDlg::InitListCtrl()
{
	m_listCtrlTask.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_NO + 1, _T("任务号"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_LEVLE + 1, _T("阶段号"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_TARGET + 1, _T("任务坐标"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_BEGIN + 1, _T("起始坐标"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_ACTION + 1, _T("任务动作"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_EXECUTER + 1, _T("执行者"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_STATUS + 1, _T("执行状态"), LVCFMT_CENTER);
	m_listCtrlTask.InsertColumn(LISTITEM_ERROR + 1, _T("异常信息"), LVCFMT_CENTER);
	m_listCtrlTask.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlTask.SetExtendedStyle(m_listCtrlTask.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlTask.EnableTips();

	return;
}


void CTaskDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlTask.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlTask.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlTask.SetColumnWidth(LISTITEM_NO, 100);
	m_listCtrlTask.SetColumnWidth(LISTITEM_LEVLE, 100);
	m_listCtrlTask.SetColumnWidth(LISTITEM_TARGET, 100);
	m_listCtrlTask.SetColumnWidth(LISTITEM_BEGIN, 200);
	m_listCtrlTask.SetColumnWidth(LISTITEM_ACTION, (rcClient.Width() - 605) / 2 < 100 ? 100 : (rcClient.Width() - 605) / 2);
	m_listCtrlTask.SetColumnWidth(LISTITEM_EXECUTER, 100);
	m_listCtrlTask.SetColumnWidth(LISTITEM_STATUS, (rcClient.Width() - 605) / 2 < 100 ? 100 : (rcClient.Width() - 605) / 2);
	m_listCtrlTask.SetColumnWidth(LISTITEM_ERROR, rcClient.Width() < 100 ? 100 : rcClient.Width());

	return;
}

void CTaskDlg::Sort()
{
	int nRow = m_listCtrlTask.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlTask.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlTask.GetItemText(i, j);
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
			else if (_ttoi(ppCStr[j][LISTITEM_NO]) == _ttoi(ppCStr[j + 1][LISTITEM_NO]))
			{
				if (_ttoi(ppCStr[j][LISTITEM_LEVLE]) > _ttoi(ppCStr[j + 1][LISTITEM_LEVLE]))
				{
					// 调位
					CString *pCStrTmp = ppCStr[j];
					ppCStr[j] = ppCStr[j + 1];
					ppCStr[j + 1] = pCStrTmp;
				}
			}
		}
	}

	// 更新
	m_listCtrlTask.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlTask.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlTask.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}

void CTaskDlg::UpdateListCtrlItem(AGVTask* pTask)
{
	int nItemCount = m_listCtrlTask.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 任务号 */
	CString cstrLevel = _T("");	/*!< 阶段号 */
	pTask->GetNo(cstrNo);
	pTask->GetLevel(cstrLevel);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlTask.GetItemText(i, LISTITEM_NO)) == cstrNo && _ttoi(m_listCtrlTask.GetItemText(i, LISTITEM_LEVLE)) == cstrLevel)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlTask.InsertItem(nItemCount, cstrNo);
	}

	CString cstrTarget = _T("");		/*!< 任务坐标 */
	CString cstrBegin = _T("");			/*!< 起始坐标 */
	CString cstrAction = _T("");		/*!< 任务动作 */
	CString cstrExecuter = _T("");		/*!< 执行者 */
	CString cstrStatus = _T("");		/*!< 执行状态 */
	CString cstrError = _T("");			/*!< 异常信息 */
	COLORREF color = m_listCtrlTask.GetBkColor();

	pTask->GetTarget(cstrTarget);
	pTask->GetBegin(cstrBegin);
	pTask->GetAction(cstrAction);
	pTask->GetExecuter(cstrExecuter);
	pTask->GetStatus(cstrStatus);
	pTask->GetError(cstrError, color);

	m_listCtrlTask.SetRedraw(false);

	m_listCtrlTask.SetItemText(nRow, LISTITEM_LEVLE, cstrLevel);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_TARGET, cstrTarget);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_BEGIN, cstrBegin);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_ACTION, cstrAction);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_EXECUTER, cstrExecuter);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_STATUS, cstrStatus);
	m_listCtrlTask.SetItemText(nRow, LISTITEM_ERROR, cstrError);

	m_listCtrlTask.SetItemColor(nRow, COLOR_BLACK, color);

	m_listCtrlTask.SetRedraw(true);

	Sort();

	return;
}

void CTaskDlg::ClearListCtrlItem()
{
	m_listCtrlTask.DeleteAllItems();
}

BOOL CTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_menu.LoadMenu(IDR_TASKPOP_MENU);

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&AGVTask(5, 5, _TASKACTION_A_EMPTYMOVE, 5,0, Task::_TASKSTA_NONE,5,_TASKERROR_A_ACTOVERTIME));
	UpdateListCtrlItem(&AGVTask(1, 1, _TASKACTION_A_END, 1, 3, Task::_TASKSTA_PAUSE,1));
	UpdateListCtrlItem(&AGVTask(3, 3, _TASKACTION_A_FULLMOVE, 3, 7, Task::_TASKSTA_CANCEL));
	UpdateListCtrlItem(&AGVTask(3, 2, _TASKACTION_A_LOAD, 2));
	UpdateListCtrlItem(&AGVTask(5, 1, _TASKACTION_A_MOVE, 3, 5, Task::_TASKSTA_FINISH));
	UpdateListCtrlItem(&AGVTask(1, 4, _TASKACTION_A_UNLOAD, 4, 2, Task::_TASKSTA_EXECUTE));
	UpdateListCtrlItem(&AGVTask(1, 2, _TASKACTION_A_END, 1, 1, Task::_TASKSTA_FINISH));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTaskDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CTaskDlg::PreTranslateMessage(MSG* pMsg)
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

void CTaskDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	// 右键弹出菜单
	CMenu *subMenu = nullptr;

	for (int i = 0; i < m_listCtrlTask.GetItemCount(); ++i)
	{
		if (m_listCtrlTask.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
			&& m_listCtrlTask.GetItemState(i, LVIS_FOCUSED) == LVIS_FOCUSED)
		{
			// 右键弹出菜单
			subMenu = m_menu.GetSubMenu(0);
			subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

			return;
		}
	}

	// 右键弹出菜单
	subMenu = m_menu.GetSubMenu(1);
	subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	return;
}


void CTaskDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_menu.DestroyMenu();
}


BOOL CTaskDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd* pMain = AfxGetMainWnd();

	for (int i = 0; i < m_listCtrlTask.GetItemCount(); ++i)
	{
		if (m_listCtrlTask.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED
			&& m_listCtrlTask.GetItemState(i, LVIS_FOCUSED) == LVIS_FOCUSED)
		{
			CPoint *pPtTask = new CPoint(_ttoi(m_listCtrlTask.GetItemText(i, LISTITEM_NO)), _ttoi(m_listCtrlTask.GetItemText(i, LISTITEM_LEVLE)));

			if (::PostMessage(pMain->m_hWnd, WM_COMMAND_TASK, wParam, (LPARAM)pPtTask) == false)
			{
				delete pPtTask;
				pPtTask = nullptr;
			}

			return CDialogEx::OnCommand(wParam, lParam);
		}
	}

	::PostMessage(pMain->m_hWnd, WM_COMMAND_TASK, wParam, NULL);

	return CDialogEx::OnCommand(wParam, lParam);
}