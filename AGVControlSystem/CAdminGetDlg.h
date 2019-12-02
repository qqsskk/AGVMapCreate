#pragma once


// CAdminGetDlg 对话框

class CAdminGetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminGetDlg)

public:
	CAdminGetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAdminGetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMINGET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrPassword;
	CEdit m_editPassword;
public:
	afx_msg void OnEnChangeEditPassword();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
