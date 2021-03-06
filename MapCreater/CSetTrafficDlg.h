#pragma once


// CSetTrafficDlg 对话框

class CSetTrafficDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTrafficDlg)

public:
	CSetTrafficDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetTrafficDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTRAFFIC_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrCtrl;	/*!< 管制地标卡组 */
	CString m_cstrStop;	/*!< 禁行地标卡组 */
protected:
	CString m_cstrCtrlHelp;	/*!< 管制地标卡提示文字 */
	CString m_cstrStopHelp; /*!< 禁行地标卡提示文字 */
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKillfocusEditStop();
	afx_msg void OnSetfocusEditStop();
	afx_msg void OnKillfocusEditCtrl();
	afx_msg void OnSetfocusEditCtrl();
	afx_msg void OnEnChangeEditCtrl();
	afx_msg void OnEnChangeEditStop();
	afx_msg void OnBnClickedOk();
};
