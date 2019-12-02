// CAGVActionDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "AGVControlSystem.h"
#include "CAGVActionDlg.h"
#include "afxdialogex.h"


// CAGVActionDlg 对话框

IMPLEMENT_DYNAMIC(CAGVActionDlg, CDialogEx)

CAGVActionDlg::CAGVActionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AGVACTION_DIALOG, pParent)
{
	m_byActCode = 0;
}

CAGVActionDlg::~CAGVActionDlg()
{
}

void CAGVActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
}

void CAGVActionDlg::InitComboAction()
{
	int i = 0;
	for (std::map<CString, unsigned char>::iterator it = m_mapAction.begin(); it != m_mapAction.end(); ++it,++i)
	{
		m_comboAction.InsertString(i, it->first);
	}

	if (m_comboAction.GetCount() > 0)
	{
		m_comboAction.SetCurSel(0);
	}

	return;
}


BEGIN_MESSAGE_MAP(CAGVActionDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, &CAGVActionDlg::OnCbnSelchangeComboAction)
	ON_BN_CLICKED(IDOK, &CAGVActionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAGVActionDlg 消息处理程序


void CAGVActionDlg::OnCbnSelchangeComboAction()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cstrAction = _T("");
	m_comboAction.GetLBText(m_comboAction.GetCurSel(), cstrAction);

	if (m_mapAction.find(cstrAction) != m_mapAction.end())
	{
		m_byActCode = m_mapAction[cstrAction];
	}

	return;
}


BOOL CAGVActionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboAction();
	OnCbnSelchangeComboAction();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CAGVActionDlg::Clear()
{
	m_comboAction.ResetContent();
	m_mapAction.clear();
	m_byActCode = 0;
}


void CAGVActionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboAction.GetCurSel() == -1)
	{
		MessageBox(_T("请选择要执行的动作"), _T("输入错误"), MB_ICONERROR);
	}

	CDialogEx::OnOK();
}
