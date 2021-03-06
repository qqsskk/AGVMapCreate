// CSetAGVDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CSetAGVDlg.h"
#include "afxdialogex.h"


// CSetAGVDlg 对话框

IMPLEMENT_DYNAMIC(CSetAGVDlg, CDialogEx)

CSetAGVDlg::CSetAGVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETAGV_DIALOG, pParent)
	, m_unNo(1)
	, m_dwIP(0)
	, m_unPort(0)
	, m_nMove(1)
{
	m_cstrIp = _T("0.0.0.0");
	m_cstrType = _T("");
	m_cstrMove = _T("单向");
	m_bEdit = false;
}

CSetAGVDlg::~CSetAGVDlg()
{
}

void CSetAGVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, m_unNo);
	DDV_MinMaxUInt(pDX, m_unNo, 1, 65535);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_IPAddress(pDX, IDC_IPADDRESS_AGV, m_dwIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_unPort);
	DDV_MinMaxUInt(pDX, m_unPort, 0, 65535);
	DDX_Radio(pDX, IDC_RADIO_FULL, m_nMove);
}

BEGIN_MESSAGE_MAP(CSetAGVDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NO, &CSetAGVDlg::OnEnChangeEditNo)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CSetAGVDlg::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, &CSetAGVDlg::OnBnClickedRadioSingle)
	ON_BN_CLICKED(IDC_RADIO_DOUBLE, &CSetAGVDlg::OnBnClickedRadioDouble)
	ON_BN_CLICKED(IDC_RADIO_FULL, &CSetAGVDlg::OnBnClickedRadioFull)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_AGV, &CSetAGVDlg::OnIpnFieldchangedIpaddressAgv)
	ON_EN_CHANGE(IDC_EDIT_PORT, &CSetAGVDlg::OnEnChangeEditPort)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CSetAGVDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetAGVDlg 消息处理程序
void CSetAGVDlg::InitComboBox()
{
	/*!
	 * 初始化类型列表
	*/
	m_comboType.InsertString(0, _T("牵引式"));
	m_comboType.InsertString(1, _T("潜入式"));
	m_comboType.InsertString(2, _T("移载式"));
	m_comboType.InsertString(3, _T("叉车式"));
	m_comboType.InsertString(4, _T("激光式"));
	m_comboType.InsertString(5, _T("机械手式"));

	m_comboType.SetCurSel(-1);

	return;
}

void CSetAGVDlg::OnEnChangeEditNo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CSetAGVDlg::OnCbnSelchangeComboType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_comboType.GetLBText(m_comboType.GetCurSel(), m_cstrType);

	return;
}


void CSetAGVDlg::OnBnClickedRadioSingle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMove = 1;

	m_cstrMove = _T("单向");

	UpdateData(false);

	return;
}


void CSetAGVDlg::OnBnClickedRadioDouble()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMove = 2;

	m_cstrMove = _T("双向");

	UpdateData(false);

	return;
}


void CSetAGVDlg::OnBnClickedRadioFull()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMove = 0;

	m_cstrMove = _T("全向");

	UpdateData(false);

	return;
}


void CSetAGVDlg::OnIpnFieldchangedIpaddressAgv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	UpdateData(true);

	m_cstrIp.Format(_T("%d.%d.%d.%d"),
		(m_dwIP >> 24) & 0xff,
		(m_dwIP >> 16) & 0xff,
		(m_dwIP >> 8) & 0xff,
		m_dwIP & 0xff);
}


void CSetAGVDlg::OnEnChangeEditPort()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


BOOL CSetAGVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboBox();

	if (m_bEdit)
	{
		if (m_cstrType == _T("牵引式"))
		{
			m_comboType.SetCurSel(0);
		}
		else if (m_cstrType == _T("潜入式"))
		{
			m_comboType.SetCurSel(1);
		}
		else if (m_cstrType == _T("移载式"))
		{
			m_comboType.SetCurSel(2);
		}
		else if (m_cstrType == _T("叉车式"))
		{
			m_comboType.SetCurSel(3);
		}
		else if (m_cstrType == _T("激光式"))
		{
			m_comboType.SetCurSel(4);
		}
		else if (m_cstrType == _T("机械手式"))
		{
			m_comboType.SetCurSel(5);
		}

		if (m_cstrMove == _T("全向"))
		{
			m_nMove = 0;
		}
		if (m_cstrMove == _T("单向"))
		{
			m_nMove = 1;
		}
		if (m_cstrMove == _T("双向"))
		{
			m_nMove = 2;
		}

		m_dwIP = ntohl(inet_addr(CStringA(m_cstrIp).GetBuffer()));

		UpdateData(false);

		GetDlgItem(IDC_EDIT_NO)->EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSetAGVDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CSetAGVDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboType.GetCurSel() < 0
		|| m_nMove < 0)
	{
		MessageBox(_T("关键信息不完整!"), _T("错误"), MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}
