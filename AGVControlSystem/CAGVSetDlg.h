#pragma once
#include <map>
#include <set>

// CAGVSetDlg 对话框

class CAGVSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVSetDlg)

public:
	CAGVSetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGVSET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboStatus;						/*!< 状态下拉列表 */
	CComboBox m_comboLocation;						/*!< 坐标下拉列表 */
	unsigned short m_usLocation;					/*!< 坐标 */
	unsigned char m_byStatus;						/*!< 状态码 */
	std::map<CString, unsigned char> m_mapStatus;	/*!< 状态列表 */
	std::set<unsigned short> m_listLoaction;		/*!< 坐标列表 */
	CString m_cstrLocation;							/*!< 坐标 */

protected:
	/*!
	 * @brief 初始化坐标下拉列表
	*/
	void InitComboLoaction();

	/*!
	 * @brief 初始化状态列表
	*/
	void InitComboStatus();
public:
	/*!
	 * @brief 清除数据
	*/
	void Clear();
public:
	afx_msg void OnCbnSelchangeComboStatus();
	afx_msg void OnCbnSelchangeComboLocation();
	afx_msg void OnEditupdateComboLocation();

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
