// CCAConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CCAConfigDlg.h"
#include "afxdialogex.h"


// CCAConfigDlg 对话框

IMPLEMENT_DYNAMIC(CCAConfigDlg, CDialogEx)

CCAConfigDlg::CCAConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CACONFIG_DIALOG, pParent)
{

}

CCAConfigDlg::~CCAConfigDlg()
{
}

void CCAConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CA, m_listCtrlCA);
}


BEGIN_MESSAGE_MAP(CCAConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCAConfigDlg 消息处理程序
void CCAConfigDlg::InitListCtrl()
{
	m_listCtrlCA.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlCA.InsertColumn(1, _T("无线主机编号"), LVCFMT_CENTER);
	m_listCtrlCA.InsertColumn(2, _T("无线主机IP地址"), LVCFMT_CENTER);
	m_listCtrlCA.InsertColumn(3, _T("无线主机端口"), LVCFMT_CENTER);
	m_listCtrlCA.InsertColumn(4, _T("无线备注信息"), LVCFMT_CENTER);
	m_listCtrlCA.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlCA.SetExtendedStyle(m_listCtrlCA.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlCA.EnableTips();

	return;
}


void CCAConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlCA.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlCA.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlCA.SetColumnWidth(0, 150);
	m_listCtrlCA.SetColumnWidth(1, 150);
	m_listCtrlCA.SetColumnWidth(2, 150);
	m_listCtrlCA.SetColumnWidth(3, rcClient.Width() - 455 < 150 ? 150 : rcClient.Width() - 455);

	return;
}


void CCAConfigDlg::Sort()
{
	int nRow = m_listCtrlCA.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlCA.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlCA.GetItemText(i, j);
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
	m_listCtrlCA.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlCA.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlCA.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CCAConfigDlg::UpdateListCtrlItem(CCA* pCA)
{
	int nItemCount = m_listCtrlCA.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pCA->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlCA.GetItemText(i, 0)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlCA.InsertItem(nItemCount, cstrNo);
	}

	CString cstrIP = _T("");			/*!< IP地址 */
	CString cstrPort = _T("");			/*!< 端口 */
	CString cstrText = _T("");			/*!< 备注 */

	pCA->GetIP(cstrIP);
	pCA->GetPort(cstrPort);
	pCA->GetText(cstrText);

	m_listCtrlCA.SetRedraw(false);

	m_listCtrlCA.SetItemText(nRow, 1, cstrIP);
	m_listCtrlCA.SetItemText(nRow, 2, cstrPort);
	m_listCtrlCA.SetItemText(nRow, 3, cstrText);

	m_listCtrlCA.SetRedraw(true);

	Sort();

	return;
}


void CCAConfigDlg::ClearListCtrlItem()
{
	m_listCtrlCA.DeleteAllItems();
}


BOOL CCAConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CCA(5, "192.168.255.255"));
	UpdateListCtrlItem(&CCA(1, "192.168.1.1"));
	UpdateListCtrlItem(&CCA(3, "192.168.1.3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCAConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CCAConfigDlg::PreTranslateMessage(MSG* pMsg)
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