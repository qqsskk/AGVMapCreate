// CAGVSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVSetDlg.h"
#include "afxdialogex.h"


// CAGVSetDlg 对话框

IMPLEMENT_DYNAMIC(CAGVSetDlg, CDialogEx)

CAGVSetDlg::CAGVSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGVSET_DIALOG, pParent)
	, m_cstrLocation(_T(""))
{
	m_usLocation = 0;
	m_byStatus = 0;
}

CAGVSetDlg::~CAGVSetDlg()
{
}

void CAGVSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATUS, m_comboStatus);
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboLocation);
	DDX_CBString(pDX, IDC_COMBO_LOCATION, m_cstrLocation);
}

BEGIN_MESSAGE_MAP(CAGVSetDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STATUS, &CAGVSetDlg::OnCbnSelchangeComboStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, &CAGVSetDlg::OnCbnSelchangeComboLocation)
	ON_CBN_EDITUPDATE(IDC_COMBO_LOCATION, &CAGVSetDlg::OnEditupdateComboLocation)
	ON_BN_CLICKED(IDOK, &CAGVSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAGVSetDlg 消息处理程序
void CAGVSetDlg::InitComboLoaction()
{
	int i = 0;
	for (std::set<unsigned short>::iterator it = m_listLoaction.begin(); it != m_listLoaction.end(); ++it,++i)
	{
		CString strLocation = _T("");
		strLocation.Format(_T("%d"), *it);

		m_comboLocation.InsertString(i, strLocation);
	}

	if (m_comboLocation.GetCount() > 0)
	{
		m_comboLocation.SetCurSel(0);
	}

	return;
}

void CAGVSetDlg::InitComboStatus()
{
	int i = 0;

	for (std::map<CString, unsigned char>::iterator it = m_mapStatus.begin(); it != m_mapStatus.end(); ++it, ++i)
	{
		m_comboStatus.InsertString(i, it->first);
	}

	if (m_comboStatus.GetCount() > 0)
	{
		m_comboStatus.SetCurSel(0);
	}

	return;
}


void CAGVSetDlg::Clear()
{
	m_comboStatus.ResetContent();
	m_comboLocation.ResetContent();
	m_usLocation = 0;
	m_byStatus = 0;
	m_mapStatus.clear();
	m_listLoaction.clear();
	m_cstrLocation = _T("");
}

void CAGVSetDlg::OnCbnSelchangeComboStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrStatus = _T("");
	m_comboStatus.GetLBText(m_comboStatus.GetCurSel(), cstrStatus);

	if (m_mapStatus.find(cstrStatus) != m_mapStatus.end())
	{
		m_byStatus = m_mapStatus[cstrStatus];
	}

	return;
}


void CAGVSetDlg::OnCbnSelchangeComboLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboLocation.GetCurSel() == -1)
	{
		return;
	}

	CString cstrLocation = _T("");
	m_comboLocation.GetLBText(m_comboLocation.GetCurSel(), cstrLocation);

	if (cstrLocation != _T(""))
	{
		m_usLocation = _ttoi(cstrLocation);
	}
	
	return;
}


void CAGVSetDlg::OnEditupdateComboLocation()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrLocation.GetLength(); ++i)
	{
		if (m_cstrLocation.GetAt(i) < '0' || m_cstrLocation.GetAt(i) > '9')
		{
			MessageBox(_T("仅可以输入数字"), _T("输入错误"), MB_ICONERROR);
			m_cstrLocation.Delete(i, 1);
			UpdateData(false);
			m_comboLocation.SetEditSel(m_cstrLocation.GetLength(), m_cstrLocation.GetLength());
			return;
		}
	}


	m_usLocation = _ttoi(m_cstrLocation);

	return;
}


void CAGVSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboStatus.GetCurSel() == -1)
	{
		MessageBox(_T("请选择要设置的状态"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	if (m_cstrLocation == _T(""))
	{
		MessageBox(_T("请选择要设置的坐标"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	if (m_usLocation > UINT16_MAX)
	{
		MessageBox(_T("请输入0到65535之间的数字"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	CDialogEx::OnOK();
}


BOOL CAGVSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboLoaction();
	InitComboStatus();

	OnCbnSelchangeComboStatus();
	OnCbnSelchangeComboLocation();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
