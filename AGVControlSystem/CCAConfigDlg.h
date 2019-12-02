#pragma once
#include "CListCtrlEx.h"
#include "CCA.h"

// CCAConfigDlg 对话框

class CCAConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCAConfigDlg)

public:
	CCAConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCAConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CACONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlCA;	/*!< CA主机列表 */

protected:
	/*!
	 * @brief 初始化ListCtrl
	*/
	void InitListCtrl();

	/*!
	 * @brief 调整ListCtrl控件的尺寸
	 * @param CRect 客户区的尺寸
	*/
	void ListCtrlSize(CRect rcClient);

	/*!
	 * @brief 排序
	*/
	void Sort();

public:
	/*!
	 * @breif 更新ListCtrlItem
	 * @param CCA* CA指针
	*/
	void UpdateListCtrlItem(CCA* pCA);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
