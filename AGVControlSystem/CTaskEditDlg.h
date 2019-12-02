#pragma once
#include <set>

// CTaskEditDlg 对话框

class CTaskEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskEditDlg)

public:
	CTaskEditDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskEditDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKEDIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;			/*!< 任务号 */
	unsigned int m_unLevel;			/*!< 阶段号 */
	unsigned int m_unTarget;		/*!< 任务坐标 */
	unsigned int m_unBegin;			/*!< 起始坐标 */
	std::set<CString> m_setActions;	/*!< 任务动作列表 */
	CString m_cstrAction;			/*!< 任务动作 */
	CComboBox m_comboAction;		/*!< 任务动作下拉列表*/
protected:
	/*!
	 * @brief 初始化任务动作下拉列表
	*/
	void InitComboAction();
public:
	afx_msg void OnEnChangeEditTarget();
	afx_msg void OnEnChangeEditBegin();
	afx_msg void OnCbnSelchangeComboAction();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
