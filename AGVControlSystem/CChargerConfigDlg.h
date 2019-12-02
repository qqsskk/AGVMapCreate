#pragma once
#include "CListCtrlEx.h"
#include "CMNavCharger.h"

// CChargerConfigDlg 对话框

class CChargerConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChargerConfigDlg)

public:
	CChargerConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChargerConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARGERCONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlCharger;

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
	 * @param CMNavCharger* 充电器指针
	*/
	void UpdateListCtrlItem(CMNavCharger* pCharger);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
