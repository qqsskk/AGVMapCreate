// CWatchListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CWatchListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetWatchDlg.h"


// CWatchListDlg 对话框

IMPLEMENT_DYNAMIC(CWatchListDlg, CDialogEx)

CWatchListDlg::CWatchListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WATCHLIST_DIALOG, pParent)
{

}

CWatchListDlg::~CWatchListDlg()
{
}

void CWatchListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WATCH, m_listWatch);
}


BEGIN_MESSAGE_MAP(CWatchListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CWatchListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CWatchListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CWatchListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CWatchListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CWatchListDlg 消息处理程序

void CWatchListDlg::SortList()
{
	int nRow = m_listWatch.GetItemCount();
	int nCol = 3;

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
			ppCStrList[i][l] = m_listWatch.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 无线手表的排序条件为手表编号
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
			m_listWatch.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

bool CWatchListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出无线手表信息.\r\n"));

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线手表信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("无线手表信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入无线手表信息
	*/
	for (int i = 0; i < m_listWatch.GetItemCount(); i++)
	{
		TiXmlElement element("Watch");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listWatch.GetItemText(i, WATCHLIST_NO)));
		element.SetAttribute("Station", CStringA(m_listWatch.GetItemText(i, WATCHLIST_STATION)));
		element.SetAttribute("CA", CStringA(m_listWatch.GetItemText(i, WATCHLIST_CA)));

		pXmlRoot->InsertEndChild(element);

	}

	TRACE(_T("无线手表信息导出完成.\r\n"));

	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CWatchListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入无线手表信息.\r\n"));

	m_listWatch.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线手表信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("无线手表信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取无线呼叫器信息
		*/
		if (cstrElemtName == _T("Watch"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrNo = _T("");			/*!< 编号 */
			CString cstrStation = _T("");		/*!< 工位 */
			CString cstrCA = _T("");			/*!< 主机号 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					cstrNo = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Station"))
				{
					cstrStation = cstrAttrValue;
				}
				else if (cstrAttrName == _T("CA"))
				{
					cstrCA = cstrAttrValue;
				}
			}

			int nRow = m_listWatch.InsertItem(m_listWatch.GetItemCount(), cstrNo);
			m_listWatch.SetItemText(nRow, WATCHLIST_STATION, cstrStation);
			m_listWatch.SetItemText(nRow, WATCHLIST_CA, cstrCA);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("无线手表信息导入完成.\r\n"));

	return true;
}

void CWatchListDlg::InitList()
{
	m_listWatch.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listWatch.InsertColumn(WATCHLIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listWatch.InsertColumn(WATCHLIST_CA + 1, _T("主机号"), LVCFMT_CENTER);
	m_listWatch.InsertColumn(WATCHLIST_STATION + 1, _T("工位"), LVCFMT_CENTER);
	m_listWatch.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listWatch.DeleteColumn(0);

	return;
}

void CWatchListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listWatch.GetSafeHwnd() == nullptr)
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

	GetDlgItem(IDC_BUTTON_ADD)->MoveWindow(CRect(rect.left + nDis, rect.top, rect.left + nDis + nWidth, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(CRect(rect.left + nDis * 2 + nWidth, rect.top, rect.left + nDis * 2 + nWidth * 2, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DEL)->MoveWindow(CRect(rect.left + nDis * 3 + nWidth * 2, rect.top, rect.left + nDis * 3 + nWidth * 3, rect.top + nHeight));
	GetDlgItem(IDC_BUTTON_DELALL)->MoveWindow(CRect(rect.left + nDis * 4 + nWidth * 3, rect.top, rect.left + nDis * 4 + nWidth * 4, rect.top + nHeight));

	rect.top += nHeight + 5;
	m_listWatch.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listWatch.GetClientRect(&rect);

	m_listWatch.SetColumnWidth(WATCHLIST_NO, rect.Width() / 3);
	m_listWatch.SetColumnWidth(WATCHLIST_CA, rect.Width() / 3);
	m_listWatch.SetColumnWidth(WATCHLIST_STATION, rect.Width() / 3);

	return;
}

BOOL CWatchListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CWatchListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CWatchListDlg::PreTranslateMessage(MSG * pMsg)
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

void CWatchListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetWatchDlg setWatchDlg;

	if (setWatchDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listWatch.GetItemCount(); i++)
		{
			if (setWatchDlg.m_unNo == _ttoi(m_listWatch.GetItemText(i, WATCHLIST_NO)))
			{
				MessageBox(_T("已存在相同编号的无线手表设备!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrText = _T("");
		cstrText.Format(_T("%d"), setWatchDlg.m_unNo);

		int nRow = m_listWatch.InsertItem(m_listWatch.GetItemCount(), cstrText);

		cstrText.Format(_T("%d"), setWatchDlg.m_unStation);

		m_listWatch.SetItemText(nRow, WATCHLIST_STATION, cstrText);

		cstrText.Format(_T("%d"), setWatchDlg.m_unCA);

		m_listWatch.SetItemText(nRow, WATCHLIST_CA, cstrText);

		SortList();
	}

	return;
}

void CWatchListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listWatch.GetItemCount(); i++)
	{
		if (m_listWatch.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetWatchDlg setWatchDlg;

			setWatchDlg.m_unNo = _ttoi(m_listWatch.GetItemText(i, WATCHLIST_NO));
			setWatchDlg.m_unStation = _ttoi(m_listWatch.GetItemText(i, WATCHLIST_STATION));
			setWatchDlg.m_unCA = _ttoi(m_listWatch.GetItemText(i, WATCHLIST_CA));
			setWatchDlg.m_bEdit = true;

			if (setWatchDlg.DoModal() == IDOK)
			{
				CString cstrText = _T("");

				cstrText.Format(_T("%d"), setWatchDlg.m_unStation);

				m_listWatch.SetItemText(i, WATCHLIST_STATION, cstrText);

				cstrText.Format(_T("%d"), setWatchDlg.m_unCA);

				m_listWatch.SetItemText(i, WATCHLIST_CA, cstrText);
			}

			return;
		}
	}

	return;
}

void CWatchListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listWatch.GetItemCount();)
	{
		if (m_listWatch.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listWatch.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}

void CWatchListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listWatch.DeleteAllItems();
	return;
}