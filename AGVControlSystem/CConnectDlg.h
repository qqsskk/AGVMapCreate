#pragma once
#include "CListCtrlEx.h"

// CConnectDlg 对话框

class CConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CConnectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlDevice;		/*!< 设备列表 */

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

public:
	/*!
	 * @brief 更新ListCtrlItem
	 * @param CString 设备名称
	 * @param CString 设备连接状态
	 * @param CString 设备连接时间
	*/
	void UpdateListCtrlItem(CString cstrName,CString cstrStatus,CString cstrTime);

	/*!
	 * @brief 更新ListCtrlItem
	 * @param CString 设备名称
	 * @param CString 设备连接状态
	 * @param CString 设备连接时间
	 * @param COLORREF 背景颜色
	*/
	void UpdateListCtrlItem(CString cstrName, CString cstrStatus, CString cstrTime,COLORREF color);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};