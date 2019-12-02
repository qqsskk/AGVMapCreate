#pragma once
#include "CListCtrlEx.h"
#include "CTCPMNavAGV.h"

// CAGVConfigDlg 对话框

class CAGVConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAGVConfigDlg)

public:
	CAGVConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAGVConfigDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AGVCONFIG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_listCtrlAGV;

private:
	enum LISTITEM
	{
		LISTITEM_NO,		/*!< 编号 */
		LISTITEM_TYPE,		/*!< 类型 */
		LISTITEM_IP,		/*!< IP地址 */
		LISTITEM_PORT,		/*!< 端口 */
		LISTITEM_MAXSPEED,	/*!< 最大速度*/
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
};
