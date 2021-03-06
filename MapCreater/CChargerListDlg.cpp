// CChargerListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CChargerListDlg.h"
#include "afxdialogex.h"
#include "CSetChargerDlg.h"
#include "MainFrm.h"
#include "MapCreaterView.h"


// CChargerListDlg 对话框

IMPLEMENT_DYNAMIC(CChargerListDlg, CDialogEx)

CChargerListDlg::CChargerListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHARGERLIST_DIALOG, pParent)
{

}

CChargerListDlg::~CChargerListDlg()
{
}

void CChargerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHARGER, m_listCharger);
}


BEGIN_MESSAGE_MAP(CChargerListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOCATION, &CChargerListDlg::OnBnClickedButtonLocation)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CChargerListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CChargerListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CChargerListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CChargerListDlg 消息处理程序
void CChargerListDlg::SortList()
{
	int nRow = m_listCharger.GetItemCount();
	int nCol = 4;

	if (nRow < 2)
	{
		return;
	}

	/*!
	 * 将列表成员放入数组集合
	 * 列表成员为字符串数组
	*/
	CString **ppCStrList = new CString*[nRow];

	for (int i = 0; i < nRow; i++)
	{
		ppCStrList[i] = new CString[nCol];
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int l = 0; l < nCol; l++)
		{
			ppCStrList[i][l] = m_listCharger.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 充电器的排序条件为充电器号
	 * 冒泡排序
	*/
	for (int i = 1; i < nRow; i++)
	{
		for (int j = 0; j < nRow - i; j++)
		{
			if (_ttoi(ppCStrList[j][0]) > _ttoi(ppCStrList[j + 1][0]))
			{
				CString *pCStrItem = ppCStrList[j];
				ppCStrList[j] = ppCStrList[j + 1];
				ppCStrList[j + 1] = pCStrItem;
			}
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			m_listCharger.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

void CChargerListDlg::Delete(unsigned int unCharger)
{
	/*!
	 * 通知界面删除工位图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteCharger(unCharger);

	return;
}

void CChargerListDlg::DeleteAll()
{
	/*!
	 * 通知界面删除所有工位图形
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->DeleteAllCharger();

	return;
}

void CChargerListDlg::Locate(unsigned int unCharger)
{
	/*!
	 * 通知界面定位选中的工位
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->LocateCharger(unCharger);

	return;
}

void CChargerListDlg::Update(unsigned int unCharger, unsigned int unCtrl, unsigned int unMark)
{
	/*!
	 * 通知界面更新工位属性
	*/
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CMapCreaterView* pView = (CMapCreaterView*)pMainFrame->GetActiveView();
	pView->UpdateCharger(unCharger, unCtrl, unMark);

	return;
}

void CChargerListDlg::Add(unsigned int unCharger, CPoint ptCenter, unsigned int unCtrl, unsigned int unMark)
{
	CString cstrNo = _T("");
	CString cstrPoint = _T("");
	CString cstrCtrl = _T("");
	CString cstrMark = _T("");
	cstrNo.Format(_T("%d"), unCharger);
	cstrPoint.Format(_T("%d,%d"), ptCenter.x, ptCenter.y);
	cstrCtrl.Format(_T("%d"), unCtrl);
	cstrMark.Format(_T("%d"), unMark);

	int nRow = -1;

	for (int i = 0; i < m_listCharger.GetItemCount(); i++)
	{
		if (unCharger == _ttoi(m_listCharger.GetItemText(i, 0)))
		{
			nRow = i;
			break;
		}
	}

	if (nRow == -1)
	{
		nRow = m_listCharger.InsertItem(m_listCharger.GetItemCount(), cstrNo);
		m_listCharger.SetItemText(nRow, 1, cstrPoint);
		m_listCharger.SetItemText(nRow, 2, cstrMark);
		m_listCharger.SetItemText(nRow, 3, cstrCtrl);

		SortList();
	}
	else
	{
		m_listCharger.SetItemText(nRow, 1, cstrPoint);
		m_listCharger.SetItemText(nRow, 2, cstrMark);
		m_listCharger.SetItemText(nRow, 3, cstrCtrl);
	}

	return;
}

void CChargerListDlg::DeleteItem(unsigned int unCharger)
{
	for (int i = 0; i < m_listCharger.GetItemCount(); i++)
	{
		if (unCharger == _ttoi(m_listCharger.GetItemText(i, 0)))
		{
			m_listCharger.DeleteItem(i);

			return;
		}
	}

	return;
}

void CChargerListDlg::InitList()
{
	m_listCharger.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCharger.InsertColumn(1, _T("编号"), LVCFMT_CENTER);
	m_listCharger.InsertColumn(2, _T("坐标"), LVCFMT_CENTER);
	m_listCharger.InsertColumn(3, _T("地标卡"), LVCFMT_CENTER);
	m_listCharger.InsertColumn(4, _T("控制器"), LVCFMT_CENTER);
	m_listCharger.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listCharger.DeleteColumn(0);

	return;
}

void CChargerListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_LOCATION)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| m_listCharger.GetSafeHwnd() == nullptr)
	{
		return;
	}

	CRect rect;
	GetClientRect(&rect);

	/*!
	 * 调整对话框中的空间位置
	*/

	int nWidth = 100;	/*!< 宽 */
	int nHeight = 30;	/*!< 高 */

	rect.top += 5;
	int nDis = (rect.Width() - 4 * nWidth) / 5;

	GetDlgItem(IDC_BUTTON_LOCATION)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listCharger.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listCharger.GetClientRect(&rect);

	m_listCharger.SetColumnWidth(0, rect.Width() / 4);
	m_listCharger.SetColumnWidth(1, rect.Width() / 4);
	m_listCharger.SetColumnWidth(2, rect.Width() / 4);
	m_listCharger.SetColumnWidth(3, rect.Width() / 4);

	return;
}

BOOL CChargerListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CChargerListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();

	return;
}

BOOL CChargerListDlg::PreTranslateMessage(MSG * pMsg)
{
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		return TRUE;
	case VK_RETURN:
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CChargerListDlg::OnBnClickedButtonLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCharger.GetItemCount(); i++)
	{
		if (m_listCharger.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Locate(_ttoi(m_listCharger.GetItemText(i, 0)));

			return;
		}
	}

	return;
}


void CChargerListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCharger.GetItemCount(); i++)
	{
		if (m_listCharger.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetChargerDlg setChargerDlg;
			setChargerDlg.m_unNo = _ttoi(m_listCharger.GetItemText(i, 0));
			setChargerDlg.m_unMark = _ttoi(m_listCharger.GetItemText(i, 2));
			setChargerDlg.m_unCtrl = _ttoi(m_listCharger.GetItemText(i, 3));
			setChargerDlg.m_bEdit = true;

			if (setChargerDlg.DoModal() == IDOK)
			{
				CString cstrCtrl = _T("");
				CString cstrMark = _T("");
				cstrCtrl.Format(_T("%d"), setChargerDlg.m_unCtrl);
				cstrMark.Format(_T("%d"), setChargerDlg.m_unMark);

				m_listCharger.SetItemText(i, 2, cstrMark);
				m_listCharger.SetItemText(i, 3, cstrCtrl);

				Update(setChargerDlg.m_unNo, setChargerDlg.m_unCtrl, setChargerDlg.m_unMark);
			}

			return;
		}
	}

	return;
}


void CChargerListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	/*!
	 * 删除勾选的
	*/
	for (int i = 0; i < m_listCharger.GetItemCount();)
	{
		if (m_listCharger.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			Delete(_ttoi(m_listCharger.GetItemText(i, 0)));
			m_listCharger.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}


void CChargerListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	DeleteAll();
	m_listCharger.DeleteAllItems();

	return;
}
