#pragma once


// CAdminSetDlg 对话框

class CAdminSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdminSetDlg)

public:
	CAdminSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAdminSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADMINSET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editOld;
	CEdit m_editNew;
	CEdit m_editConfirm;
	CString m_cstrOldPassword;		/*!< 旧密码 */
	CString m_cstrNewPassword;		/*!< 新密码 */
	CString m_cstrConfirmPassword;	/*!< 确认密码 */
public:
	afx_msg void OnEnChangeEditOld();
	afx_msg void OnEnChangeEditNew();
	afx_msg void OnEnChangeEditConfirm();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
