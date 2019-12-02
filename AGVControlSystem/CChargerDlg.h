#pragma once
#include "CListCtrlEx.h"
#include "CMNavCharger.h"

// CChargerDlg 对话框

class CChargerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChargerDlg)

public:
	CChargerDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CChargerDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARGER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	enum LISTITEM
	{
		LISTITEM_NO,			/*!< 编号 */
		LISTITEM_STATUS,		/*!< 状态 */
		LISTITEM_DURATIVETIME,	/*!< 充电时间 */
		LISTITEM_MINTIME,		/*!< 最短充电时长 */
		LISTITEM_POWER,			/*!< 当前电池电量 */
		LISTITEM_NAME,			/*!< 名称 */
	};

public:
	CListCtrlEx m_listCtrlCharger;	/*!< 充电器列表 */

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
