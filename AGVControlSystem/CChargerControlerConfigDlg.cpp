// CChargerControlerConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CChargerControlerConfigDlg.h"
#include "afxdialogex.h"


// CChargerControlerConfigDlg 对话框

IMPLEMENT_DYNAMIC(CChargerControlerConfigDlg, CDialogEx)

CChargerControlerConfigDlg::CChargerControlerConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARGERCONTROLERCONFIG_DIALOG, pParent)
{

}

CChargerControlerConfigDlg::~CChargerControlerConfigDlg()
{
}

void CChargerControlerConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARGERCONTROLER, m_listCtrlChargerCtrler);
}


BEGIN_MESSAGE_MAP(CChargerControlerConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CChargerControlerConfigDlg 消息处理程序
void CChargerControlerConfigDlg::InitListCtrl()
{
	m_listCtrlChargerCtrler.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlChargerCtrler.InsertColumn(1, _T("充电控制器编号"), LVCFMT_CENTER);
	m_listCtrlChargerCtrler.InsertColumn(2, _T("充电控制器IP地址"), LVCFMT_CENTER);
	m_listCtrlChargerCtrler.InsertColumn(3, _T("充电控制器端口"), LVCFMT_CENTER);
	m_listCtrlChargerCtrler.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlChargerCtrler.SetExtendedStyle(m_listCtrlChargerCtrler.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlChargerCtrler.EnableTips();

	return;
}


void CChargerControlerConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlChargerCtrler.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlChargerCtrler.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlChargerCtrler.SetColumnWidth(0, (rcClient.Width() - 5) / 3 < 150 ? 150 : (rcClient.Width() - 5) / 3);
	m_listCtrlChargerCtrler.SetColumnWidth(1, (rcClient.Width() - 5) / 3 < 150 ? 150 : (rcClient.Width() - 5) / 3);
	m_listCtrlChargerCtrler.SetColumnWidth(2, (rcClient.Width() - 5) / 3 < 150 ? 150 : (rcClient.Width() - 5) / 3);

	return;
}


void CChargerControlerConfigDlg::Sort()
{
	int nRow = m_listCtrlChargerCtrler.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlChargerCtrler.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlChargerCtrler.GetItemText(i, j);
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
	m_listCtrlChargerCtrler.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlChargerCtrler.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlChargerCtrler.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CChargerControlerConfigDlg::UpdateListCtrlItem(CChargerControler* pChargerCtrler)
{
	int nItemCount = m_listCtrlChargerCtrler.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pChargerCtrler->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlChargerCtrler.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlChargerCtrler.InsertItem(nItemCount, cstrNo);
	}

	CString cstrIP = _T("");		/*!< IP地址 */
	CString cstrPort = _T("");		/*!< 端口 */

	pChargerCtrler->GetIP(cstrIP);
	pChargerCtrler->GetPort(cstrPort);

	m_listCtrlChargerCtrler.SetRedraw(false);

	m_listCtrlChargerCtrler.SetItemText(nRow, 1, cstrIP);
	m_listCtrlChargerCtrler.SetItemText(nRow, 2, cstrPort);

	m_listCtrlChargerCtrler.SetRedraw(true);

	Sort();

	return;
}


void CChargerControlerConfigDlg::ClearListCtrlItem()
{
	m_listCtrlChargerCtrler.DeleteAllItems();
}

BOOL CChargerControlerConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CChargerControler(5, "192.168.1.5"));
	UpdateListCtrlItem(&CChargerControler(1, "192.168.1.1"));
	UpdateListCtrlItem(&CChargerControler(3, "192.168.1.3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChargerControlerConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CChargerControlerConfigDlg::PreTranslateMessage(MSG* pMsg)
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