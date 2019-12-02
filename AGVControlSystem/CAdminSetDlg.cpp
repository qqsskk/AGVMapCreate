// CAdminSetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAdminSetDlg.h"
#include "afxdialogex.h"


// CAdminSetDlg 对话框

IMPLEMENT_DYNAMIC(CAdminSetDlg, CDialogEx)

CAdminSetDlg::CAdminSetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMINSET_DIALOG, pParent)
	, m_cstrOldPassword(_T(""))
	, m_cstrNewPassword(_T(""))
	, m_cstrConfirmPassword(_T(""))
{

}

CAdminSetDlg::~CAdminSetDlg()
{
}

void CAdminSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OLD, m_editOld);
	DDX_Control(pDX, IDC_EDIT_NEW, m_editNew);
	DDX_Control(pDX, IDC_EDIT_CONFIRM, m_editConfirm);
	DDX_Text(pDX, IDC_EDIT_OLD, m_cstrOldPassword);
	DDX_Text(pDX, IDC_EDIT_NEW, m_cstrNewPassword);
	DDX_Text(pDX, IDC_EDIT_CONFIRM, m_cstrConfirmPassword);
}


BEGIN_MESSAGE_MAP(CAdminSetDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_OLD, &CAdminSetDlg::OnEnChangeEditOld)
	ON_EN_CHANGE(IDC_EDIT_NEW, &CAdminSetDlg::OnEnChangeEditNew)
	ON_EN_CHANGE(IDC_EDIT_CONFIRM, &CAdminSetDlg::OnEnChangeEditConfirm)
	ON_BN_CLICKED(IDOK, &CAdminSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAdminSetDlg 消息处理程序


void CAdminSetDlg::OnEnChangeEditOld()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrOldPassword.GetLength(); ++i)
	{
		if (m_cstrOldPassword.GetAt(i) == ' ')
		{
			MessageBox(_T("不能输入' '"), _T("输入错误"), MB_ICONERROR);

			m_cstrOldPassword.Delete(i, 1);

			m_editOld.SetSel(-1, -1, false);

			return;
		}
	}

	return;
}


void CAdminSetDlg::OnEnChangeEditNew()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrNewPassword.GetLength(); ++i)
	{
		if (m_cstrNewPassword.GetAt(i) == ' ')
		{
			MessageBox(_T("不能输入' '"), _T("输入错误"), MB_ICONERROR);

			m_cstrNewPassword.Delete(i, 1);

			m_editNew.SetSel(-1, -1, false);

			return;
		}
	}

	return;
}


void CAdminSetDlg::OnEnChangeEditConfirm()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrConfirmPassword.GetLength(); ++i)
	{
		if (m_cstrConfirmPassword.GetAt(i) == ' ')
		{
			MessageBox(_T("不能输入' '"), _T("输入错误"), MB_ICONERROR);

			m_cstrConfirmPassword.Delete(i, 1);

			m_editConfirm.SetSel(-1, -1, false);

			return;
		}
	}

	return;
}


void CAdminSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cstrConfirmPassword != m_cstrNewPassword)
	{
		MessageBox(_T("两次输入的密码不相同"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	if (m_cstrNewPassword.GetLength() < 8 || m_cstrNewPassword.GetLength() > 16)
	{
		MessageBox(_T("密码长度为8至16位"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	CDialogEx::OnOK();
}


BOOL CAdminSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
