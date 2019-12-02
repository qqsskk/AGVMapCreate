#pragma once
#include "CListCtrlEx.h"
#include "CCA.h"

// CCACallConfigDlg 对话框

class CCACallConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCACallConfigDlg)

public:
	CCACallConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCACallConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CACALLCONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlCall;		/*!< 呼叫器列表 */

protected:
	enum LISTITEM
	{
		LISTITEM_NO,
		LISTITEM_ENDPOINT,
		LISTITEM_STARTPOINT,
		LISTITEM_AGV,
		LISTITEM_ACTION,
		LISTITEM_MSG,
	};
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
	 * @param CACall* 呼叫器指针
	*/
	void UpdateListCtrlItem(CACall* pCall);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
