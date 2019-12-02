// CAGVMoveDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVMoveDlg.h"
#include "afxdialogex.h"


// CAGVMoveDlg 对话框

IMPLEMENT_DYNAMIC(CAGVMoveDlg, CDialogEx)

CAGVMoveDlg::CAGVMoveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGVMOVE_DIALOG, pParent)
	, m_usTarget(0)
	, m_cstrText(_T(""))
{

}

CAGVMoveDlg::~CAGVMoveDlg()
{
}

void CAGVMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TARGET, m_comboTarget);
	DDX_CBString(pDX, IDC_COMBO_TARGET, m_cstrText);
}


BEGIN_MESSAGE_MAP(CAGVMoveDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET, &CAGVMoveDlg::OnCbnSelchangeComboTarget)
	ON_CBN_EDITUPDATE(IDC_COMBO_TARGET, &CAGVMoveDlg::OnEditupdateComboTarget)
	ON_BN_CLICKED(IDOK, &CAGVMoveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAGVMoveDlg 消息处理程序


void CAGVMoveDlg::InitComboTarget()
{
	int i = 0;
	for (std::set<unsigned short>::iterator it = m_listTargets.begin(); it != m_listTargets.end(); ++it,++i)
	{
		CString cstrTarget = _T("");
		cstrTarget.Format(_T("%d"), *it);
		m_comboTarget.InsertString(i, cstrTarget);
	}

	if (m_comboTarget.GetCount() > 0)
	{
		m_comboTarget.SetCurSel(0);
	}

	return;
}

void CAGVMoveDlg::Clear()
{
	m_comboTarget.ResetContent();
	m_listTargets.clear();
	m_cstrText = _T("");
	m_usTarget = 0;

}

BOOL CAGVMoveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitComboTarget();
	OnCbnSelchangeComboTarget();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAGVMoveDlg::OnCbnSelchangeComboTarget()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboTarget.GetCurSel() == -1)
	{
		return;
	}

	CString cstrTarget = _T("");
	m_comboTarget.GetLBText(m_comboTarget.GetCurSel(), cstrTarget);

	if (cstrTarget != _T(""))
	{
		m_usTarget = _ttoi(cstrTarget);
	}
	
	return;
}


void CAGVMoveDlg::OnEditupdateComboTarget()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrText.GetLength(); ++i)
	{
		if (m_cstrText.GetAt(i) < '0' || m_cstrText.GetAt(i) > '9')
		{
			MessageBox(_T("仅可以输入数字"),_T("输入错误"),MB_ICONERROR);
			m_cstrText.Delete(i, 1);
			UpdateData(false);
			m_comboTarget.SetEditSel(m_cstrText.GetLength(), m_cstrText.GetLength());
			return;
		}
	}

	m_usTarget = _ttoi(m_cstrText);
	
	return;
}


void CAGVMoveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_usTarget < 1 || m_usTarget > UINT16_MAX)
	{
		MessageBox(_T("请输入1到65535之间的数字"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	CDialogEx::OnOK();
}
