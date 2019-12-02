#pragma once
#include <set>

// CAGVMoveDlg 对话框

class CAGVMoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVMoveDlg)

public:
	CAGVMoveDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVMoveDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGVMOVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned short m_usTarget;	/*!< 移动坐标 */
	CComboBox m_comboTarget;	/*!< 移动坐标列表 */
	CString m_cstrText;
	std::set<unsigned short> m_listTargets; /*!< 移动坐标列表 */

protected:
	/*!
	 * @brief 初始化坐标下拉控件
	*/
	void InitComboTarget();
public:
	/*!
	 * @brief 清除数据
	*/
	void Clear();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboTarget();
	afx_msg void OnEditupdateComboTarget();
	afx_msg void OnBnClickedOk();
};
