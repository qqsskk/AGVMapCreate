#pragma once
#include "CListCtrlEx.h"
#include "CMNavRestPosition.h"

// CRestConfigDlg 对话框

class CRestConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRestConfigDlg)

public:
	CRestConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRestConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESTCONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	enum LISTITEM
	{
		LISTITEM_NO,		/*!< 编号 */
		LISTITEM_LOCATION,	/*!< 坐标 */
		LISTITEM_NEXT,		/*!< 下一待机位 */
		LISTITEM_STOPLIST,	/*!< 禁行坐标组 */
		LISTITEM_NAME,		/*!< 名称 */
	};
public:
	CListCtrlEx m_listCtrlRest;	/*!< 待机位列表 */

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
	 * @param CMNavRestPosition* 待机位指针
	*/
	void UpdateListCtrlItem(CMNavRestPosition* pRest);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
