// CAGVListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CAGVListDlg.h"
#include "afxdialogex.h"
#include "CSetAGVDlg.h"
#include "Xml.h"


// CAGVListDlg 对话框

IMPLEMENT_DYNAMIC(CAGVListDlg, CDialogEx)

CAGVListDlg::CAGVListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGVLIST_DIALOG, pParent)
{

}

CAGVListDlg::~CAGVListDlg()
{

}

void CAGVListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AGV, m_listAGV);
}


BEGIN_MESSAGE_MAP(CAGVListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAGVListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CAGVListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CAGVListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CAGVListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()

// CAGVListDlg 消息处理程序

void CAGVListDlg::SortList()
{
	int nRow = m_listAGV.GetItemCount();
	int nCol = 5;

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
			ppCStrList[i][l] = m_listAGV.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * AGV的排序条件为AGV编号
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
			m_listAGV.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

bool CAGVListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出AGV信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("AGV信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("AGV信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入AGV信息
	*/
	for (int i = 0; i < m_listAGV.GetItemCount(); i++)
	{
		TiXmlElement element("AGV");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listAGV.GetItemText(i, AGVLIST_NO)));
		
		CString cstrType = m_listAGV.GetItemText(i, AGVLIST_TYPE);
		if (cstrType == _T("牵引式"))
		{
			element.SetAttribute("Type", "P");
		}
		else if (cstrType == _T("潜入式"))
		{
			element.SetAttribute("Type", "S");
		}
		else if (cstrType == _T("移载式"))
		{
			element.SetAttribute("Type", "T");
		}
		else if (cstrType == _T("叉车式"))
		{
			element.SetAttribute("Type", "F");
		}
		else if (cstrType == _T("激光式"))
		{
			element.SetAttribute("Type", "L");
		}
		else if (cstrType == _T("机械手式"))
		{
			element.SetAttribute("Type", "A");
		}
		
		CString cstrMove = m_listAGV.GetItemText(i, AGVLIST_MOVE);
		if (cstrMove == _T("单向"))
		{
			element.SetAttribute("Move", "S");
		}
		else if (cstrMove == _T("双向"))
		{
			element.SetAttribute("Move", "D");
		}
		else if (cstrMove == _T("全向"))
		{
			element.SetAttribute("Move", "F");
		}

		element.SetAttribute("IP", CStringA(m_listAGV.GetItemText(i, AGVLIST_IP)));
		element.SetAttribute("Port", CStringA(m_listAGV.GetItemText(i, AGVLIST_PORT)));

		pXmlRoot->InsertEndChild(element);
		
	}

	TRACE(_T("AGV信息导出完成.\r\n"));

	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CAGVListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入AGV信息.\r\n"));
	m_listAGV.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("AGV信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("AGV信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取AGV信息
		*/
		if (cstrElemtName == _T("AGV"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrNo = _T("0");	/*!< 编号 */
			CString cstrType = _T("");	/*!< 类型 */
			CString cstrMove = _T("");	/*!< 可运动方向 */
			CString cstrIp = _T("0.0.0.0");	/*!< IP地址 */
			CString cstrPort = _T("0");	/*!< 端口 */

			while (pAttr)
			{
				CString cstrAttrName(pAttr->Name());	/*!< 属性名称*/
				CString cstrAttrValue(pAttr->Value()); /*!< 属性值 */

				pAttr = pAttr->Next();

				if (cstrAttrName == _T("No"))
				{
					cstrNo = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Type"))
				{
					if (cstrAttrValue == _T("P"))
					{
						cstrType = _T("牵引式");
					}
					else if (cstrAttrValue == _T("S"))
					{
						cstrType = _T("潜入式");
					}
					else if (cstrAttrValue == _T("T"))
					{
						cstrType = _T("移载式");
					}
					else if (cstrAttrValue == _T("F"))
					{
						cstrType = _T("叉车式");
					}
					else if (cstrAttrValue == _T("L"))
					{
						cstrType = _T("激光式");
					}
					else if (cstrAttrValue == _T("A"))
					{
						cstrType = _T("机械手式");
					}
				}
				else if (cstrAttrName == _T("Move"))
				{
					if (cstrAttrValue == _T("S"))
					{
						cstrMove = _T("单向");
					}
					else if (cstrAttrValue == _T("D"))
					{
						cstrMove = _T("双向");
					}
					else if (cstrAttrValue == _T("F"))
					{
						cstrMove = _T("全向");
					}
				}
				else if (cstrAttrName == _T("IP"))
				{
					cstrIp = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Port"))
				{
					cstrPort = cstrAttrValue;
				}
			}

			int nRow = m_listAGV.InsertItem(m_listAGV.GetItemCount(), cstrNo);
			m_listAGV.SetItemText(nRow, AGVLIST_TYPE, cstrType);
			m_listAGV.SetItemText(nRow, AGVLIST_MOVE, cstrMove);
			m_listAGV.SetItemText(nRow, AGVLIST_IP, cstrIp);
			m_listAGV.SetItemText(nRow, AGVLIST_PORT, cstrPort);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("AGV信息导入完成.\r\n"));
	return true;
}

void CAGVListDlg::InitList()
{
	m_listAGV.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listAGV.InsertColumn(AGVLIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listAGV.InsertColumn(AGVLIST_TYPE + 1, _T("类型"), LVCFMT_CENTER);
	m_listAGV.InsertColumn(AGVLIST_MOVE + 1, _T("可运动方向"), LVCFMT_CENTER);
	m_listAGV.InsertColumn(AGVLIST_IP + 1, _T("IP地址"), LVCFMT_CENTER);
	m_listAGV.InsertColumn(AGVLIST_PORT + 1, _T("端口"), LVCFMT_CENTER);
	m_listAGV.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listAGV.DeleteColumn(0);

	return;
}

void CAGVListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listAGV.GetSafeHwnd() == nullptr)
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
	m_listAGV.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listAGV.GetClientRect(&rect);

	m_listAGV.SetColumnWidth(AGVLIST_NO, rect.Width() / 6);
	m_listAGV.SetColumnWidth(AGVLIST_TYPE, rect.Width() / 6);
	m_listAGV.SetColumnWidth(AGVLIST_MOVE, rect.Width() / 6);
	m_listAGV.SetColumnWidth(AGVLIST_IP, rect.Width() * 2 / 6);
	m_listAGV.SetColumnWidth(AGVLIST_PORT, rect.Width() / 6);

	return;
}

BOOL CAGVListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CAGVListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CAGVListDlg::PreTranslateMessage(MSG * pMsg)
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

void CAGVListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetAGVDlg setAGVDlg;

	if (setAGVDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listAGV.GetItemCount(); i++)
		{
			if (setAGVDlg.m_unNo == _ttoi(m_listAGV.GetItemText(i, AGVLIST_NO)))
			{
				MessageBox(_T("已存在相同编号的AGV设备!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrNo = _T("");
		cstrNo.Format(_T("%d"), setAGVDlg.m_unNo);

		int nRow = m_listAGV.InsertItem(m_listAGV.GetItemCount(), cstrNo);

		m_listAGV.SetItemText(nRow, AGVLIST_TYPE, setAGVDlg.m_cstrType);
		m_listAGV.SetItemText(nRow, AGVLIST_MOVE, setAGVDlg.m_cstrMove);
		m_listAGV.SetItemText(nRow, AGVLIST_IP, setAGVDlg.m_cstrIp);

		CString cstrPort = _T("");
		cstrPort.Format(_T("%d"), setAGVDlg.m_unPort);
		m_listAGV.SetItemText(nRow, AGVLIST_PORT, cstrPort);

		SortList();
	}

	return;
}

void CAGVListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listAGV.GetItemCount(); i++)
	{
		if (m_listAGV.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetAGVDlg setAGVDlg;

			setAGVDlg.m_unNo = _ttoi(m_listAGV.GetItemText(i, AGVLIST_NO));
			setAGVDlg.m_cstrType = m_listAGV.GetItemText(i, AGVLIST_TYPE);
			setAGVDlg.m_cstrMove = m_listAGV.GetItemText(i, AGVLIST_MOVE);
			setAGVDlg.m_cstrIp = m_listAGV.GetItemText(i, AGVLIST_IP);
			setAGVDlg.m_unPort = _ttoi(m_listAGV.GetItemText(i, AGVLIST_PORT));
			setAGVDlg.m_bEdit = true;

			if (setAGVDlg.DoModal() == IDOK)
			{
				m_listAGV.SetItemText(i, AGVLIST_TYPE,setAGVDlg.m_cstrType);
				m_listAGV.SetItemText(i, AGVLIST_MOVE, setAGVDlg.m_cstrMove);
				m_listAGV.SetItemText(i, AGVLIST_IP, setAGVDlg.m_cstrIp);

				CString cstrPort = _T("");
				cstrPort.Format(_T("%d"), setAGVDlg.m_unPort);
				m_listAGV.SetItemText(i, AGVLIST_PORT, cstrPort);
			}

			return;
		}
	}

	return;
}

void CAGVListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listAGV.GetItemCount();)
	{
		if (m_listAGV.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listAGV.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}

void CAGVListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listAGV.DeleteAllItems();
	return;
}
