#pragma once


// CSetRestQueueDlg 对话框

class CSetRestQueueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRestQueueDlg)

public:
	CSetRestQueueDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetRestQueueDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETRESTQUEUE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unQueue; /*!< 队列编号 */
	unsigned int m_unRest;	/*!< 待机位编号 */
	CString m_cstrStop;		/*!< 禁行地标卡组 */
	bool m_bEdit;			/*!< 编辑标识 */
protected:
	CString m_cstrStopHelp; /*!< 禁行地标卡提示文字 */
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKillfocusEditStop();
	afx_msg void OnSetfocusEditStop();
	afx_msg void OnEnChangeEditQueue();
	afx_msg void OnEnChangeEditRest();
	afx_msg void OnEnChangeEditStop();
	afx_msg void OnBnClickedOk();
};
