#pragma once


// CSetStationDlg 对话框

class CSetStationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetStationDlg)

public:
	CSetStationDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetStationDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETSTATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;	/*!< 编号 */
	CString m_cstrName;		/*!< 名称 */
	unsigned int m_unMark;	/*!< 工位所在地标卡编号 */
	bool m_bEdit;			/*!< 编辑标识 */

public:
	afx_msg void OnEnChangeEditNo();
	afx_msg void OnEnChangeEditName();
	afx_msg void OnEnChangeEditMark();
	virtual BOOL OnInitDialog();
};
