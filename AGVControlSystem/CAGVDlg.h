#pragma once
#include "CListCtrlEx.h"
#include "CTCPMNavAGV.h"

#define WM_COMMAND_AGV WM_USER+1

// CAGVDlg 对话框

class CAGVDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVDlg)

public:
	CAGVDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGV_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	enum LISTITEM
	{
		LISTITEM_NO,		/*!< 编号 */
		LISTITEM_TYPE,		/*!< 类型 */
		LISTITEM_STATUS,	/*!< 当前状态 */
		LISTITEM_POWER,		/*!< 当前电量 */
		LISTITEM_CURMARK,	/*!< 当前坐标 */
		LISTITEM_ENDMARK,	/*!< 终点坐标 */
		LISTITEM_ACTION,	/*!< 当前动作 */
		LISTITEM_ACTSTA,	/*!< 当前动作状态 */
		LISTITEM_CARGO,		/*!< 载货数量 */
		LISTITEM_ERROR,		/*!< 异常信息 */
		LISTITEM_SPEED,		/*!< 当前速度 */
		LISTITEM_OLDMARK,	/*!< 上一坐标 */
		LISTITEM_NEXTMARK,	/*!< 下一坐标 */
		LISTITEM_HISMARK,	/*!< 历史坐标 */
	};

protected:
	CListCtrlEx m_listCtrlAGV;	/*!< 设备列表 */
	CMenu m_menu;				/*!< 右键菜单 */

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
	 * @param CTCPMNavAGV* AGV指针
	*/
	void UpdateListCtrlItem(CTCPMNavAGV* pAGV);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
