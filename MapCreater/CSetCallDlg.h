#pragma once


// CSetCallDlg 对话框

class CSetCallDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetCallDlg)

public:
	CSetCallDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetCallDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETCALL_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;		/*!< 编号 */
	unsigned int m_unStation;	/*!< 工位 */
	unsigned int m_unRestQueue;	/*!< 待机队列 */
	CString m_cstrAGV;			/*!< AGV类型 */
	CString m_cstrMsg;			/*!< 附加信息 */
	bool m_bEdit;				/*!< 编辑标识 */
protected:
	CComboBox m_comboAGV;		/*!< AGV类型下拉列表 */

protected:
	/*!
	 * @brief 初始化下拉列表
	*/
	void InitComboBox();

public:
	afx_msg void OnEnChangeEditNo();
	afx_msg void OnEnChangeEditStation();
	afx_msg void OnEnChangeEditRestqueue();
	afx_msg void OnEnChangeEditMsg();
	afx_msg void OnCbnSelchangeComboAgv();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
