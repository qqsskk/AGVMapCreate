﻿#pragma once
#include"CListCtrlEx.h"
#include "CCA.h"

// CCAWatchConfigDlg 对话框

class CCAWatchConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCAWatchConfigDlg)

public:
	CCAWatchConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCAWatchConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAWATCHCONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlWatch;	/*!< 无线手表列表 */

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
	 * @param CAWatch* 手表指针
	*/
	void UpdateListCtrlItem(CAWatch* pWatch);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
