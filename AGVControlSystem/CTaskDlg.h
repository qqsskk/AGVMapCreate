#pragma once
#include "CListCtrlEx.h"
#include "CTaskManager.h"

#define WM_COMMAND_TASK WM_USER+2

// CTaskDlg 对话框

class CTaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskDlg)

public:
	CTaskDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTaskDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlTask;	/*!< 任务列表 */
	CMenu m_menu;				/*!< 右键菜单 */

protected:
	enum LISTITEM
	{
		LISTITEM_NO,		/*!< 任务号 */
		LISTITEM_LEVLE,		/*!< 阶段号 */
		LISTITEM_TARGET,	/*!< 任务坐标 */
		LISTITEM_BEGIN,		/*!< 起始坐标 */
		LISTITEM_ACTION,	/*!< 任务动作 */
		LISTITEM_EXECUTER,	/*!< 任务执行者 */
		LISTITEM_STATUS,	/*!< 任务状态 */
		LISTITEM_ERROR,		/*!< 异常信息 */
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

public:
	/*!
	 * @brief 更新ListCtrlItem
	 * @param AGVTask 任务指针
	*/
	void UpdateListCtrlItem(AGVTask* pTask);

	/*!
	 * @brief 清除ListCtrlItem
	*/
	void ClearListCtrlItem();

	/*!
	 * @brief 排序
	*/
	void Sort();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
