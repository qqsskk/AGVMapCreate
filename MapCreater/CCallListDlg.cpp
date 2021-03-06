// CCallListDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CCallListDlg.h"
#include "afxdialogex.h"
#include "Xml.h"
#include "CSetCallDlg.h"


// CCallListDlg 对话框

IMPLEMENT_DYNAMIC(CCallListDlg, CDialogEx)

CCallListDlg::CCallListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CALLLIST_DIALOG, pParent)
{

}

CCallListDlg::~CCallListDlg()
{
}

void CCallListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CALL, m_listCall);
}


BEGIN_MESSAGE_MAP(CCallListDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCallListDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CCallListDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCallListDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CCallListDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CCallListDlg 消息处理程序

void CCallListDlg::SortList()
{
	int nRow = m_listCall.GetItemCount();
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
			ppCStrList[i][l] = m_listCall.GetItemText(i, l);
		}
	}

	/*!
	 * 根据条件进行排序
	 * 无线呼叫器的排序条件为呼叫器编号
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
			m_listCall.SetItemText(i, j, ppCStrList[i][j]);
		}
	}

	for (int i = 0; i < nRow; i++)
	{
		delete[] ppCStrList[i];
	}

	delete[] ppCStrList;

	return;
}

bool CCallListDlg::Output(CString cstrFilePath)
{
	TRACE(_T("开始导出无线呼叫器信息.\r\n"));
	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线呼叫器信息导出失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL)
	{
		TRACE(_T("无线呼叫器信息导出失败.\r\n"));
		return false;
	}

	/*!
	 * 写入无线呼叫器信息
	*/
	for (int i = 0; i < m_listCall.GetItemCount(); i++)
	{
		TiXmlElement element("Call");	/*!< 子节点 */
		element.SetAttribute("No", CStringA(m_listCall.GetItemText(i, CALLLIST_NO)));
		element.SetAttribute("Station", CStringA(m_listCall.GetItemText(i, CALLLIST_STATION)));
		element.SetAttribute("RestQueue", CStringA(m_listCall.GetItemText(i, CALLLIST_RESTQUEUE)));

		CString cstrAGV = m_listCall.GetItemText(i, CALLLIST_AGV);
		if (cstrAGV == _T("全部"))
		{
			element.SetAttribute("AGVType", "All");
		}
		else if (cstrAGV == _T("牵引式"))
		{
			element.SetAttribute("AGVType", "P");
		}
		else if (cstrAGV == _T("潜入式"))
		{
			element.SetAttribute("AGVType", "S");
		}
		else if (cstrAGV == _T("移载式"))
		{
			element.SetAttribute("AGVType", "T");
		}
		else if (cstrAGV == _T("叉车式"))
		{
			element.SetAttribute("AGVType", "F");
		}
		else if (cstrAGV == _T("机械手式"))
		{
			element.SetAttribute("AGVType", "A");
		}
		else if (cstrAGV == _T("激光式"))
		{
			element.SetAttribute("AGVType", "L");
		}

		element.SetAttribute("Msg", CStringA(m_listCall.GetItemText(i, CALLLIST_MSG)));

		pXmlRoot->InsertEndChild(element);

	}

	TRACE(_T("无线呼叫器信息导出完成.\r\n"));

	return xmlDoc.SaveFile(CStringA(cstrFilePath));
}

bool CCallListDlg::Input(CString cstrFilePath)
{
	TRACE(_T("开始导入无线呼叫器信息.\r\n"));
	m_listCall.DeleteAllItems();

	TiXmlDocument xmlDoc; /*!< XML文档 */
	if (xmlDoc.LoadFile(CStringA(cstrFilePath).GetBuffer()) == false)
	{
		TRACE(_T("无线呼叫器信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlRoot = xmlDoc.FirstChildElement(); /*!< 根节点 */
	if (pXmlRoot == NULL || pXmlRoot->NoChildren())
	{
		TRACE(_T("无线呼叫器信息导入失败.\r\n"));
		return false;
	}

	TiXmlElement *pXmlElemt = pXmlRoot->FirstChildElement(); /*!< 子节点*/

	while (pXmlElemt)
	{
		CString cstrElemtName(pXmlElemt->Value()); /*!< 子节点名称 */

		/*!
		 * 读取无线呼叫器信息
		*/
		if (cstrElemtName == _T("Call"))
		{
			TiXmlAttribute *pAttr = pXmlElemt->FirstAttribute(); /*!< 子节点属性 */

			CString cstrNo = _T("");			/*!< 编号 */
			CString cstrStation = _T("");		/*!< 工位 */
			CString cstrRestQueue = _T("");		/*!< 待机队列 */
			CString cstrAGVType = _T("");		/*!< AGV类型 */
			CString cstrMsg = _T("");			/*!< 附带信息 */

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
				else if (cstrAttrName == _T("RestQueue"))
				{
					cstrRestQueue = cstrAttrValue;
				}
				else if (cstrAttrName == _T("AGVType"))
				{
					cstrAGVType = cstrAttrValue;
				}
				else if (cstrAttrName == _T("Msg"))
				{
					cstrMsg = cstrAttrValue;
				}
			}

			int nRow = m_listCall.InsertItem(m_listCall.GetItemCount(), cstrNo);
			m_listCall.SetItemText(nRow, CALLLIST_STATION, cstrStation);
			m_listCall.SetItemText(nRow, CALLLIST_RESTQUEUE, cstrRestQueue);

			if (cstrAGVType == _T("All"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("全部"));
			}
			else if (cstrAGVType == _T("P"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("牵引式"));
			}
			else if (cstrAGVType == _T("S"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("潜入式"));
			}
			else if (cstrAGVType == _T("T"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("移载式"));
			}
			else if (cstrAGVType == _T("F"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("叉车式"));
			}
			else if (cstrAGVType == _T("A"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("机械手式"));
			}
			else if (cstrAGVType == _T("L"))
			{
				m_listCall.SetItemText(nRow, CALLLIST_AGV, _T("激光式"));
			}

			m_listCall.SetItemText(nRow, CALLLIST_MSG, cstrMsg);
		}

		pXmlElemt = pXmlElemt->NextSiblingElement();
	}

	SortList();

	TRACE(_T("无线呼叫器信息导入完成.\r\n"));

	return true;
}

void CCallListDlg::InitList()
{
	m_listCall.InsertColumn(0, _T(""), LVCFMT_CENTER);
	m_listCall.InsertColumn(CALLLIST_NO + 1, _T("编号"), LVCFMT_CENTER);
	m_listCall.InsertColumn(CALLLIST_STATION + 1, _T("工位"), LVCFMT_CENTER);
	m_listCall.InsertColumn(CALLLIST_RESTQUEUE + 1, _T("待机队列"), LVCFMT_CENTER);
	m_listCall.InsertColumn(CALLLIST_AGV + 1, _T("AGV类型"), LVCFMT_CENTER);
	m_listCall.InsertColumn(CALLLIST_MSG + 1, _T("附加信息"), LVCFMT_CENTER);
	m_listCall.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listCall.DeleteColumn(0);

	return;
}

void CCallListDlg::ListSize()
{
	if (GetDlgItem(IDC_BUTTON_DEL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_DELALL)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_EDIT)->GetSafeHwnd() == nullptr
		|| GetDlgItem(IDC_BUTTON_ADD)->GetSafeHwnd() == nullptr
		|| m_listCall.GetSafeHwnd() == nullptr)
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
	m_listCall.MoveWindow(rect);

	/*!
	 * 调整列表中列的宽度
	*/
	m_listCall.GetClientRect(&rect);

	m_listCall.SetColumnWidth(CALLLIST_NO, rect.Width() / 7);
	m_listCall.SetColumnWidth(CALLLIST_STATION, rect.Width() / 7);
	m_listCall.SetColumnWidth(CALLLIST_RESTQUEUE, rect.Width() / 7);
	m_listCall.SetColumnWidth(CALLLIST_AGV, rect.Width() / 7);
	m_listCall.SetColumnWidth(CALLLIST_MSG, rect.Width() * 3 / 7);

	return;
}

BOOL CCallListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CCallListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ListSize();
}

BOOL CCallListDlg::PreTranslateMessage(MSG * pMsg)
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

void CCallListDlg::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetCallDlg setCallDlg;

	if (setCallDlg.DoModal() == IDOK)
	{
		for (int i = 0; i < m_listCall.GetItemCount(); i++)
		{
			if (setCallDlg.m_unNo == _ttoi(m_listCall.GetItemText(i, CALLLIST_NO)))
			{
				MessageBox(_T("已存在相同编号的无线呼叫器设备!"), _T("添加失败"), MB_ICONERROR);
				return;
			}
		}

		CString cstrText = _T("");
		cstrText.Format(_T("%d"), setCallDlg.m_unNo);

		int nRow = m_listCall.InsertItem(m_listCall.GetItemCount(), cstrText);

		cstrText.Format(_T("%d"), setCallDlg.m_unStation);

		m_listCall.SetItemText(nRow, CALLLIST_STATION, cstrText);

		cstrText.Format(_T("%d"), setCallDlg.m_unRestQueue);

		m_listCall.SetItemText(nRow, CALLLIST_RESTQUEUE, cstrText);

		m_listCall.SetItemText(nRow, CALLLIST_AGV, setCallDlg.m_cstrAGV);
		m_listCall.SetItemText(nRow, CALLLIST_MSG, setCallDlg.m_cstrMsg);

		SortList();
	}

	return;
}

void CCallListDlg::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCall.GetItemCount(); i++)
	{
		if (m_listCall.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			CSetCallDlg setCallDlg;

			setCallDlg.m_unNo = _ttoi(m_listCall.GetItemText(i, CALLLIST_NO));
			setCallDlg.m_unStation = _ttoi(m_listCall.GetItemText(i, CALLLIST_STATION));
			setCallDlg.m_unRestQueue = _ttoi(m_listCall.GetItemText(i, CALLLIST_RESTQUEUE));
			setCallDlg.m_cstrAGV = m_listCall.GetItemText(i, CALLLIST_AGV);
			setCallDlg.m_cstrMsg = m_listCall.GetItemText(i, CALLLIST_MSG);
			setCallDlg.m_bEdit = true;

			if (setCallDlg.DoModal() == IDOK)
			{
				CString cstrText = _T("");

				cstrText.Format(_T("%d"), setCallDlg.m_unStation);

				m_listCall.SetItemText(i, CALLLIST_STATION, cstrText);

				cstrText.Format(_T("%d"), setCallDlg.m_unRestQueue);

				m_listCall.SetItemText(i, CALLLIST_RESTQUEUE, cstrText);

				m_listCall.SetItemText(i, CALLLIST_AGV, setCallDlg.m_cstrAGV);
				m_listCall.SetItemText(i, CALLLIST_MSG, setCallDlg.m_cstrMsg);
			}

			return;
		}
	}

	return;
}

void CCallListDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_listCall.GetItemCount();)
	{
		if (m_listCall.GetItemState(i, LVNI_SELECTED) == LVNI_SELECTED)
		{
			m_listCall.DeleteItem(i);
		}
		else
		{
			i++;
		}
	}

	return;
}

void CCallListDlg::OnBnClickedButtonDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listCall.DeleteAllItems();
	return;
}