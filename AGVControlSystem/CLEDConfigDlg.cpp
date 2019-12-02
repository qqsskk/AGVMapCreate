//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CLEDConfigDlg.h"
#include "afxdialogex.h"


// CLEDConfigDlg 对话框

IMPLEMENT_DYNAMIC(CLEDConfigDlg, CDialogEx)

CLEDConfigDlg::CLEDConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEDCONFIG_DIALOG, pParent)
{

}

CLEDConfigDlg::~CLEDConfigDlg()
{
}

void CLEDConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LED, m_listCtrlLED);
}


BEGIN_MESSAGE_MAP(CLEDConfigDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CLEDConfigDlg 消息处理程序
void CLEDConfigDlg::InitListCtrl()
{
	m_listCtrlLED.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_NO + 1, _T("LED编号"), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_IP + 1, _T("LEDIP地址"), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_PORT + 1, _T("LED端口"), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_WIDTH + 1, _T("LED宽"), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_HEIGHT + 1, _T("LED高"), LVCFMT_CENTER);
	m_listCtrlLED.InsertColumn(LISTITEM_TEXT + 1, _T("LED备注信息"), LVCFMT_CENTER);
	m_listCtrlLED.DeleteColumn(0);
	// 绘制网格 | 双重缓存 | 单行选中 | 选中一整行
	m_listCtrlLED.SetExtendedStyle(m_listCtrlLED.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER | LVS_EX_SINGLEROW | LVS_EX_FULLROWSELECT);
	m_listCtrlLED.EnableTips();

	return;
}


void CLEDConfigDlg::ListCtrlSize(CRect rcClient)
{
	// 控件有效性
	if (m_listCtrlLED.GetSafeHwnd() == false)
	{
		return;
	}

	// 调整控件位置以及尺寸
	m_listCtrlLED.MoveWindow(rcClient);

	// 调整列宽
	m_listCtrlLED.SetColumnWidth(LISTITEM_NO,100);
	m_listCtrlLED.SetColumnWidth(LISTITEM_IP, 150);
	m_listCtrlLED.SetColumnWidth(LISTITEM_PORT, 100);
	m_listCtrlLED.SetColumnWidth(LISTITEM_WIDTH, 100);
	m_listCtrlLED.SetColumnWidth(LISTITEM_HEIGHT, 100);
	m_listCtrlLED.SetColumnWidth(LISTITEM_TEXT, rcClient.Width() - 555 < 150 ? 150 : rcClient.Width() - 555);
	return;
}


void CLEDConfigDlg::Sort()
{
	int nRow = m_listCtrlLED.GetItemCount();	/*!< 行 */
	int nCol = m_listCtrlLED.GetHeaderCtrl()->GetItemCount();	/*!< 列 */

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
			ppCStr[i][j] = m_listCtrlLED.GetItemText(i, j);
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
	m_listCtrlLED.SetRedraw(false);

	for (int i = 0; i < nRow; ++i)
	{
		for (int j = 0; j < nCol; ++j)
		{
			m_listCtrlLED.SetItemText(i, j, ppCStr[i][j]);
		}
	}

	m_listCtrlLED.SetRedraw(true);

	// 释放
	for (int i = 0; i < nRow; ++i)
	{
		delete[] ppCStr[i];
	}

	delete[] ppCStr;

	return;
}


void CLEDConfigDlg::UpdateListCtrlItem(CLED* pLED)
{
	int nItemCount = m_listCtrlLED.GetItemCount();	/*!< 行 */

	int nRow = -1;	/*!< 行 */

	CString cstrNo = _T("");	/*!< 充电器编号 */
	pLED->GetNo(cstrNo);

	for (int i = 0; i < nItemCount; ++i)
	{
		if (_ttoi(m_listCtrlLED.GetItemText(i, LISTITEM_NO)) == cstrNo)
		{
			nRow = i;

			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCtrlLED.InsertItem(nItemCount, cstrNo);
	}

	CString cstrWidth = _T("");		/*!< 宽 */
	CString cstrHeight = _T("");	/*!< 高 */
	CString cstrIP = _T("");		/*!< IP地址 */
	CString cstrPort = _T("");		/*!< 端口 */
	CString cstrText = _T("");		/*!< 备注 */

	pLED->GetWidth(cstrWidth);
	pLED->GetHeight(cstrHeight);
	pLED->GetIP(cstrIP);
	pLED->GetPort(cstrPort);
	pLED->GetText(cstrText);

	m_listCtrlLED.SetRedraw(false);

	m_listCtrlLED.SetItemText(nRow, LISTITEM_IP, cstrIP);
	m_listCtrlLED.SetItemText(nRow, LISTITEM_PORT, cstrPort);
	m_listCtrlLED.SetItemText(nRow, LISTITEM_WIDTH, cstrWidth);
	m_listCtrlLED.SetItemText(nRow, LISTITEM_HEIGHT, cstrHeight);
	m_listCtrlLED.SetItemText(nRow, LISTITEM_TEXT, cstrText);

	m_listCtrlLED.SetRedraw(true);

	Sort();

	return;
}


void CLEDConfigDlg::ClearListCtrlItem()
{
	m_listCtrlLED.DeleteAllItems();
}


BOOL CLEDConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitListCtrl();

//#define _TEST
#ifdef _TEST
	UpdateListCtrlItem(&CLED(5, 196, 128, "192.168.255.255",5,"测试5"));
	UpdateListCtrlItem(&CLED(1, 196, 128, "192.168.1.3", 1, "测试1"));
	UpdateListCtrlItem(&CLED(3, 196, 128, "192.168.1.3", 3, "测试3"));
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLEDConfigDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);

	ListCtrlSize(rcClient);
}


BOOL CLEDConfigDlg::PreTranslateMessage(MSG* pMsg)
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