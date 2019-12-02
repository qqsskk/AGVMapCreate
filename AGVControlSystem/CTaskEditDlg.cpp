// CTaskEditDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CTaskEditDlg.h"
#include "afxdialogex.h"


// CTaskEditDlg 对话框

IMPLEMENT_DYNAMIC(CTaskEditDlg, CDialogEx)

CTaskEditDlg::CTaskEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASKEDIT_DIALOG, pParent)
	, m_unNo(0)
	, m_unLevel(0)
	, m_unTarget(0)
	, m_unBegin(0)
{
	m_cstrAction = _T("");
}

CTaskEditDlg::~CTaskEditDlg()
{
}

void CTaskEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NO, m_unNo);
	DDV_MinMaxInt(pDX, IDC_EDIT_NO, 1, UINT16_MAX);
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_unLevel);
	DDV_MinMaxInt(pDX, IDC_EDIT_LEVEL, 1, 255);
	DDX_Text(pDX, IDC_EDIT_TARGET, m_unTarget);
	DDV_MinMaxUInt(pDX, m_unTarget, 1, 255);
	DDX_Text(pDX, IDC_EDIT_BEGIN, m_unBegin);
	DDV_MinMaxUInt(pDX, m_unBegin, 0, 255);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
}


BEGIN_MESSAGE_MAP(CTaskEditDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TARGET, &CTaskEditDlg::OnEnChangeEditTarget)
	ON_EN_CHANGE(IDC_EDIT_BEGIN, &CTaskEditDlg::OnEnChangeEditBegin)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, &CTaskEditDlg::OnCbnSelchangeComboAction)
	ON_BN_CLICKED(IDOK, &CTaskEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTaskEditDlg 消息处理程序


void CTaskEditDlg::InitComboAction()
{
	int i = 0;
	for (std::set<CString>::iterator it = m_setActions.begin(); it != m_setActions.end(); ++it,++i)
	{
		m_comboAction.InsertString(i,*it);
	}

	if (m_comboAction.GetCount() > 0)
	{
		m_comboAction.SetCurSel(0);
	}

	return;
}

void CTaskEditDlg::OnEnChangeEditTarget()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CTaskEditDlg::OnEnChangeEditBegin()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
}


void CTaskEditDlg::OnCbnSelchangeComboAction()
{
	// TODO: 在此添加控件通知处理程序代码
	m_comboAction.GetLBText(m_comboAction.GetCurSel(), m_cstrAction);
}


BOOL CTaskEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboAction();

	UpdateData(false);

	int i = 0;
	for (std::set<CString>::iterator it = m_setActions.begin(); it != m_setActions.end(); ++it, ++i)
	{
		if (*it == m_cstrAction)
		{
			m_comboAction.SetCurSel(i);

			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTaskEditDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboAction.GetCurSel() == -1)
	{
		MessageBox(_T("请选择任务动作"), _T("输入错误"), MB_ICONERROR);
	}

	CDialogEx::OnOK();
}
