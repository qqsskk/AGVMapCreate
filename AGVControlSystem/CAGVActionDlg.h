#pragma once
#include <map>

// CAGVActionDlg 对话框

class CAGVActionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVActionDlg)

public:
	CAGVActionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVActionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGVACTION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	std::map<CString, unsigned char> m_mapAction; /*!< 动作列表 */
public:
	CComboBox m_comboAction;	/*!< 动作下拉列表 */
	unsigned char m_byActCode;	/*!< 动作码 */
protected:
	/*!
	 * @brief 初始化动作下拉列表
	*/
	void InitComboAction();
public:
	/*!
	 * @brief 清除数据
	*/
	void Clear();
public:
	afx_msg void OnCbnSelchangeComboAction();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
