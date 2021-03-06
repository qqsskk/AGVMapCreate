// CMapTabDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CMapTabDlg.h"
#include "afxdialogex.h"


// CMapTabDlg 对话框

IMPLEMENT_DYNAMIC(CMapTabDlg, CDialogEx)

CMapTabDlg::CMapTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAPTAB_DIALOG, pParent)
{

}

CMapTabDlg::~CMapTabDlg()
{
}

void CMapTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAP, m_tabMap);
}


BEGIN_MESSAGE_MAP(CMapTabDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAP, &CMapTabDlg::OnTcnSelchangeTabMap)
END_MESSAGE_MAP()


// CMapTabDlg 消息处理程序
void CMapTabDlg::InitTab()
{
	m_tabMap.InsertItem(TABDLG_MARK, _T("地标卡"));
	m_tabMap.InsertItem(TABDLG_STATION, _T("工位"));
	m_tabMap.InsertItem(TABDLG_REST, _T("待机位"));
	m_tabMap.InsertItem(TABDLG_CHARGER, _T("充电器"));

	m_dlgMarkList.Create(IDD_MARKLIST_DIALOG, &m_tabMap);
	m_dlgStationList.Create(IDD_STATIONLIST_DIALOG, &m_tabMap);
	m_dlgRestList.Create(IDD_RESTLIST_DIALOG, &m_tabMap);
	m_dlgChargerList.Create(IDD_CHARGERLIST_DIALOG, &m_tabMap);

	m_dlgMarkList.ShowWindow(SW_SHOW);
	m_dlgStationList.ShowWindow(SW_HIDE);
	m_dlgRestList.ShowWindow(SW_HIDE);
	m_dlgChargerList.ShowWindow(SW_HIDE);

	m_tabMap.SetCurFocus(TABDLG_MARK);
}


void CMapTabDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_tabMap.GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	CSize sizeTabHeader;	/*!< 页头 */
	CRect rcTabItem;
	CRect rcSpace;

	m_tabMap.MoveWindow(rcClient);

	m_tabMap.GetClientRect(rcSpace);

	sizeTabHeader.cx = (rcSpace.Width() - 4) / 4;
	sizeTabHeader.cy = 25;
	m_tabMap.SetItemSize(sizeTabHeader);

	m_tabMap.GetItemRect(0, rcTabItem);

	rcSpace.top += sizeTabHeader.cy + 4;
	rcSpace.left += 4;
	rcSpace.bottom -= 4;
	rcSpace.right -= 4;

	m_dlgMarkList.MoveWindow(rcSpace);
	m_dlgStationList.MoveWindow(rcSpace);
	m_dlgRestList.MoveWindow(rcSpace);
	m_dlgChargerList.MoveWindow(rcSpace);

	return;
}


BOOL CMapTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常:  OCX 属性页应返回 FALSE
}


BOOL CMapTabDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return TRUE;
	case VK_RETURN:
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMapTabDlg::OnTcnSelchangeTabMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_dlgMarkList.ShowWindow(SW_HIDE);
	m_dlgStationList.ShowWindow(SW_HIDE);
	m_dlgRestList.ShowWindow(SW_HIDE);
	m_dlgChargerList.ShowWindow(SW_HIDE);

	switch (m_tabMap.GetCurSel())
	{
	case TABDLG_MARK:
		m_dlgMarkList.ShowWindow(SW_SHOW);

		m_dlgMarkList.SetFocus();

		break;
	case TABDLG_STATION:
		m_dlgStationList.ShowWindow(SW_SHOW);

		m_dlgStationList.SetFocus();
		break;
	case TABDLG_REST:
		m_dlgRestList.ShowWindow(SW_SHOW);

		m_dlgRestList.SetFocus();
		break;
	case TABDLG_CHARGER:
		m_dlgChargerList.ShowWindow(SW_SHOW);

		m_dlgChargerList.SetFocus();
		break;
	default:
		m_dlgMarkList.ShowWindow(SW_SHOW);

		m_dlgMarkList.SetFocus();
		break;
	}

	return;
}
