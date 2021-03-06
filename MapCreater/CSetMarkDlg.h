#pragma once


// CSetMarkDlg 对话框

class CSetMarkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetMarkDlg)

public:
	CSetMarkDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetMarkDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETMARK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_unNo;	/*!< 编号*/

public:
	afx_msg void OnEnChangeEditNo();
};
