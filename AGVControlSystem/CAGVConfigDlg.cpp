// CAGVConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVConfigDlg.h"
#include "afxdialogex.h"


// CAGVConfigDlg 对话框

IMPLEMENT_DYNAMIC(CAGVConfigDlg, CDialogEx)

CAGVConfigDlg::CAGVConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGVCONFIG_DIALOG, pParent)
{

}

CAGVConfigDlg::~CAGVConfigDlg()
{
}

void CAGVConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AGV, m_listCtrlAGV);
}


BEGIN_MESSAGE_MAP(CAGVConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAGVConfigDlg 消息处理程序
void CAGVConfigDlg::InitListCtrl()
{
	m_listCtrlAGV.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_NO + 1, _T("AGV编号"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_TYPE + 1, _T("AGV类型"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_IP + 1, _T("AGVIP地址"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_PORT + 1, _T("AGV端口"), LVCFMT_CENTER);
	m_listCtrlAGV.InsertColumn(LISTITEM_MAXSPEED + 1, _T("AGV最大速度"), LVCFMT_CENTER);
	m_listCtrlAGV.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlAGV.SetExtendedStyle(m_listCtrlAGV.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlAGV.EnableTips();

	return;
}


void CAGVConfigDlg::ListCtrlSize(CRect rcClient)
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
	m_listCtrlAGV.SetColumnWidth(LISTITEM_TYPE, rcClient.Width() - 505 < 100 ? 100 : rcClient.Width() - 505);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_IP, 150);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_PORT, 100);
	m_listCtrlAGV.SetColumnWidth(LISTITEM_MAXSPEED, 150);
	return;
}


void CAGVConfigDlg::Sort()
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


void CAGVConfigDlg::UpdateListCtrlItem(CTCPMNavAGV* pAGV)
{
	int nItemCount = m_listCtrlAGV.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
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

	CString cstrType = _T("");			/*!< 类型 */
	CString cstrDrive = _T("");			/*!< 可运动方向 */
	CString cstrIP = _T("");			/*!< IP地址 */
	CString cstrPort = _T("");			/*!< 端口 */
	CString cstrMaxSpeed = _T("");		/*!< 最大速度 */

	pAGV->GetType(cstrType);
	pAGV->GetDriver(cstrDrive);
	pAGV->GetIP(cstrIP);
	pAGV->GetPort(cstrPort);
	pAGV->GetMaxSpeed(cstrMaxSpeed);

	m_listCtrlAGV.SetRedraw(false);

	m_listCtrlAGV.SetItemText(nRow, LISTITEM_TYPE, cstrDrive + cstrType);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_IP, cstrIP);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_PORT, cstrPort);
	m_listCtrlAGV.SetItemText(nRow, LISTITEM_MAXSPEED, cstrMaxSpeed);

	m_listCtrlAGV.SetRedraw(true);

	Sort();

	return;
}


void CAGVConfigDlg::ClearListCtrlItem()
{
	m_listCtrlAGV.DeleteAllItems();
}


BOOL CAGVConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CTCPMNavAGV(5, _AGVTYPE_PULL, _AGVDRIVER_SINGLE, 48.0f, "192.168.255.255"));
	UpdateListCtrlItem(&CTCPMNavAGV(1, _AGVTYPE_TRAVEL, _AGVDRIVER_DOUBLE, 48.0f, "192.168.1.1"));
	UpdateListCtrlItem(&CTCPMNavAGV(3, _AGVTYPE_SNEAK, _AGVDRIVER_SINGLE, 48.0f, "192.168.1.3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAGVConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CAGVConfigDlg::PreTranslateMessage(MSG* pMsg)
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