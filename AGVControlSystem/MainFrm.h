
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "CService.h"
#include "CConnectPane.h"
#include "CAGVPane.h"
#include "CChargerPane.h"
#include "CRestConfigPane.h"
#include "CStationConfigPane.h"
#include "CChargerConfigPane.h"
#include "CTrafficConfigPane.h"
#include "CAGVConfigPane.h"
#include "CCAConfigPane.h"
#include "CLEDConfigPane.h"
#include "CChargerControlerPane.h"
#include "CCACallConfigPane.h"
#include "CCAWatchConfigPane.h"
#include "CTaskPane.h"
#include "CAGVMoveDlg.h"
#include "CAGVActionDlg.h"
#include "CAGVSetDlg.h"
#include "CTaskEditDlg.h"
#include "CService.h"
#include "CAdminGetDlg.h"
#include "CAdminSetDlg.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

protected:
	CConnectPane m_paneConnect;
	CAGVPane m_paneAGV;
	CTaskPane m_paneTask;
	CRestConfigPane m_paneRestConfig;
	CStationConfigPane m_paneStationConfig;
	CTrafficConfigPane m_paneTrafficConfig;
	CAGVConfigPane m_paneAGVConfig;
#ifdef _SELECT_FUNC
	CChargerPane m_paneCharger;
	CChargerConfigPane m_paneChargerConfig;
#ifdef _CA_H
	CCAConfigPane m_paneCAConfig;
	CCACallConfigPane m_paneCACallConfig;
	CCAWatchConfigPane m_paneCAWatchConfig;
#endif
#ifdef _LED_H
	CLEDConfigPane m_paneLEDConfig;
#endif
#ifdef _CHARGERCONTROLER_H
	CChargerControlerPane m_paneChargerCtrlerConfig;
#endif
#endif

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAGVCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTaskCommand(WPARAM wParam, LPARAM lParam);

protected:
	CAdminGetDlg m_dlgAdminGet;
	CAdminSetDlg m_dlgAdiminSet;
	CAGVActionDlg m_dlgAGVAction;
#ifdef _MAGNETICTRACK_H
	CAGVMoveDlg m_dlgAGVMove;
	CAGVSetDlg m_dlgAGVSet;
	CTaskEditDlg m_dlgTaskEdit;
#endif

public:
	CService m_service;

public:
	/*!
	 * @brief 清除全部数据
	*/
	void ClearAllData();

	/*!
	 * @brief 初始化AGV动作对话框
	*/
	void InitAGVActionDialog();

#ifdef _MAGNETICTRACK_H
	/*!
	 * @brief 初始化AGV移动对话框
	 * @param set<unsigned short> RFID卡列表
	*/
	void InitAGVMoveDialog(std::set<unsigned short> setRFIDs);

	/*!
	 * @brief 初始化AGV设置对话框
	 * @param set<unsigned short> RFID卡列表
	*/
	void InitAGVSetDialog(std::set<unsigned short> setRFIDs);

	/*!
	 * @brief 初始化任务编辑对话框
	*/
	void InitTaskEditDialog();
#endif
};


