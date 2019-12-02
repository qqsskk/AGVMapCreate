// CAdminGetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAdminGetDlg.h"
#include "afxdialogex.h"


// CAdminGetDlg 对话框

IMPLEMENT_DYNAMIC(CAdminGetDlg, CDialogEx)

CAdminGetDlg::CAdminGetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADMINGET_DIALOG, pParent)
	, m_cstrPassword(_T(""))
{

}

CAdminGetDlg::~CAdminGetDlg()
{
}

void CAdminGetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_cstrPassword);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
}


BEGIN_MESSAGE_MAP(CAdminGetDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CAdminGetDlg::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDOK, &CAdminGetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAdminGetDlg 消息处理程序


void CAdminGetDlg::OnEnChangeEditPassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	for (int i = 0; i < m_cstrPassword.GetLength(); ++i)
	{
		if (m_cstrPassword.GetAt(i) == ' ')
		{
			MessageBox(_T("不能输入' '"), _T("输入错误"), MB_ICONERROR);

			m_cstrPassword.Delete(i, 1);

			m_editPassword.SetSel(-1, -1, false);

			return;
		}
	}

	return;
}


BOOL CAdminGetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAdminGetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cstrPassword.GetLength() < 8 || m_cstrPassword.GetLength() > 16)
	{
		MessageBox(_T("密码长度为8至16位"), _T("输入错误"), MB_ICONERROR);

		return;
	}

	CDialogEx::OnOK();
}
