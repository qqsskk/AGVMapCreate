// CSetRestDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapCreater.h"
#include "CSetRestDlg.h"
#include "afxdialogex.h"


// CSetRestDlg 对话框

IMPLEMENT_DYNAMIC(CSetRestDlg, CDialogEx)

CSetRestDlg::CSetRestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETREST_DIALOG, pParent)
	, m_unNo(1)
	, m_unMark(1)
{
	m_bEdit = false;
}

CSetRestDlg::~CSetRestDlg()
{
}

void CSetRestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, m_unNo);
	DDV_MinMaxUInt(pDX, m_unNo, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_MARK, m_unMark);
	DDV_MinMaxUInt(pDX, m_unMark, 1, 65535);
}


BEGIN_MESSAGE_MAP(CSetRestDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_NO, &CSetRestDlg::OnEnChangeEditNo)
	ON_EN_CHANGE(IDC_EDIT_MARK, &CSetRestDlg::OnEnChangeEditMark)
END_MESSAGE_MAP()


// CSetRestDlg 消息处理程序


void CSetRestDlg::OnEnChangeEditNo()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CSetRestDlg::OnEnChangeEditMark()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


BOOL CSetRestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bEdit)
	{
		UpdateData(false);
		GetDlgItem(IDC_EDIT_NO)->EnableWindow(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
