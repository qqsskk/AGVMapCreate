#pragma once


// CSetCADlg 对话框

class CSetCADlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetCADlg)

public:
	CSetCADlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetCADlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETCA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;	/*!< 编号 */
	CString m_cstrIp;		/*!< IP地址 */
	unsigned int m_unPort;	/*!< 端口*/
	bool m_bEdit;			/*!< 编辑标识 */

protected:
	DWORD m_dwIP;			/*!< IP地址 */
public:
	afx_msg void OnEnChangeEditNo();
	afx_msg void OnIpnFieldchangedIpaddressCa(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPort();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
