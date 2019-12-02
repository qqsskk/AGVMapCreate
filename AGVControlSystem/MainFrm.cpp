
// MainFrm.cpp: CMainFrame 类的实现
//

#include "stdafx.h"
#include "AGVControlSystem.h"

#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_MESSAGE(WM_COMMAND_AGV,&CMainFrame::OnAGVCommand)
	ON_MESSAGE(WM_COMMAND_TASK, &CMainFrame::OnTaskCommand)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	EnableActiveAccessibility();
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_paneConnect.Create(_T("接入设备状态"), this, CRect(0, 0, 500, 500), TRUE, 601,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneAGV.Create(_T("AGV实时状态"), this, CRect(0, 0, 500, 200), TRUE, 602,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneTask.Create(_T("任务状态"), this, CRect(0, 0, 500, 500), TRUE, 614,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_RIGHT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneRestConfig.Create(_T("待机位配置信息"), this, CRect(0, 0, 500, 200), TRUE, 604,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneStationConfig.Create(_T("工位配置信息"), this, CRect(0, 0, 500, 200), TRUE, 605,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);

	m_paneTrafficConfig.Create(_T("交通管制区域配置信息"), this, CRect(0, 0, 500, 200), TRUE, 607,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneAGVConfig.Create(_T("AGV配置信息"), this, CRect(0, 0, 500, 200), TRUE, 608,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
#ifdef _SELECT_FUNC
	m_paneCharger.Create(_T("充电器状态"), this, CRect(0, 0, 500, 200), TRUE, 603,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneChargerConfig.Create(_T("充电器配置信息"), this, CRect(0, 0, 500, 200), TRUE, 606,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
#ifdef _CA_H
	m_paneCAConfig.Create(_T("无线主机配置信息"), this, CRect(0, 0, 500, 200), TRUE, 609,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneCACallConfig.Create(_T("无线呼叫器配置信息"), this, CRect(0, 0, 500, 200), TRUE, 612,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
	m_paneCAWatchConfig.Create(_T("无线手表配置信息"), this, CRect(0, 0, 500, 200), TRUE, 613,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
#endif
#ifdef _LED_H
	m_paneLEDConfig.Create(_T("LED配置信息"), this, CRect(0, 0, 500, 200), TRUE, 610,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
#endif
#ifdef _CHARGERCONTROLER_H
	m_paneChargerCtrlerConfig.Create(_T("充电器控制器配置信息"), this, CRect(0, 0, 500, 200), TRUE, 611,
		WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_FLOAT_MULTI);
#endif
#endif


	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_paneConnect.EnableDocking(CBRS_ALIGN_ANY);
	m_paneTask.EnableDocking(CBRS_ALIGN_ANY);
	m_paneAGV.EnableDocking(CBRS_ALIGN_ANY);
	m_paneRestConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_paneStationConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_paneTrafficConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_paneAGVConfig.EnableDocking(CBRS_ALIGN_ANY);
#ifdef _SELECT_FUNC
	m_paneCharger.EnableDocking(CBRS_ALIGN_ANY);
	m_paneChargerConfig.EnableDocking(CBRS_ALIGN_ANY);
#ifdef _CA_H
	m_paneCAConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_paneCACallConfig.EnableDocking(CBRS_ALIGN_ANY);
	m_paneCAWatchConfig.EnableDocking(CBRS_ALIGN_ANY);
#endif
#ifdef _LED_H
	m_paneLEDConfig.EnableDocking(CBRS_ALIGN_ANY);
#endif
#ifdef _CHARGERCONTROLER_H
	m_paneChargerCtrlerConfig.EnableDocking(CBRS_ALIGN_ANY);
#endif
#endif

	EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_paneAGV);
	DockPane(&m_paneTask);
	DockPane(&m_paneConnect);
	DockPane(&m_paneRestConfig);
	DockPane(&m_paneStationConfig);
	DockPane(&m_paneTrafficConfig);
	DockPane(&m_paneAGVConfig);
#ifdef _SELECT_FUNC
	DockPane(&m_paneCharger);
	DockPane(&m_paneChargerConfig);
	m_paneCharger.DockToWindow(&m_paneConnect, CBRS_ALIGN_BOTTOM);
#ifdef _CA_H
	DockPane(&m_paneCAConfig);
	DockPane(&m_paneCACallConfig);
	DockPane(&m_paneCAWatchConfig);
#endif
#ifdef _LED_H
	DockPane(&m_paneLEDConfig);
#endif
#ifdef _CHARGERCONTROLER_H
	DockPane(&m_paneChargerCtrlerConfig);
#endif
#endif

	CDockablePane* pTabbedBar = NULL;
	// 放到同一个选项卡中，并默认选择1
	m_paneStationConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
	m_paneTrafficConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
	m_paneAGVConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
#ifdef _SELECT_FUNC
	m_paneChargerConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
#ifdef _CA_H
	m_paneCAConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
	m_paneCACallConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
	m_paneCAWatchConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
#endif
#ifdef _LED_H
	m_paneLEDConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
#endif
#ifdef _CHARGERCONTROLER_H
	m_paneChargerCtrlerConfig.AttachToTabWnd(&m_paneRestConfig, DM_SHOW, FALSE, &pTabbedBar);
#endif
#endif

	m_paneRestConfig.ShowPane(false, false, false);
	m_paneStationConfig.ShowPane(false, false, false);
	m_paneTrafficConfig.ShowPane(false, false, false);
	m_paneAGVConfig.ShowPane(false, false, false);
#ifdef _SELECT_FUNC
	m_paneChargerConfig.ShowPane(false, false, false);
#ifdef _CA_H
	m_paneCAConfig.ShowPane(false, false, false);
	m_paneCACallConfig.ShowPane(false, false, false);
	m_paneCAWatchConfig.ShowPane(false, false, false);
#endif
#ifdef _LED_H
	m_paneLEDConfig.ShowPane(false, false, false);
#endif
#ifdef _CHARGERCONTROLER_H
	m_paneChargerCtrlerConfig.ShowPane(false, false, false);
#endif
#endif


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

#ifdef _MAGNETICTRACK_H
	InitAGVActionDialog();
	InitAGVSetDialog(std::set<unsigned short>());
	InitTaskEditDialog();
#endif
	m_service.Open(m_hWnd);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


LRESULT CMainFrame::OnAGVCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	/*!
	 * 用户操作
	 *
	 * 普通用户操作
	 *
	 * 1、急停
	 * 2、关机
	 * 3、状态复位
	 * 4、移动
	 * 5、动作控制
	 *	5.1、上料
	 *	5.2、下料
	 *	5.3、动作停止
	 *	5.4、其他动作
	 *
	 * 管理员操作
	 *
	 * 1、状态重置
	 * 2、交通管制通行
	 * 3、重新移动（对话框,指示目标点）
	 * 4、设置当前坐标与状态
	 * 5、模式切换
	 *	5.1、手动
	 *	5.2、自动
	 *
	*/

	std::string strComand = "";
	BufferString::std_string_format(strComand, "通知【%d】号AGV", lParam);

	switch (LOWORD(wParam))
	{
	case ID_AGV_SCREAM:		// 急停
		strComand = "【用户User】" + strComand;
		strComand += "【急停】";
		// 提交申请
		if (m_service.AGVScream(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_AGV_RESTART:	// 复位
		strComand = "【用户User】" + strComand;
		strComand += "【复位】";
		// 提交申请
		if (m_service.AGVRestart(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_AGV_MOVE:		// 移动
		/*
		 * 需要提供坐标
		*/
		strComand = "【用户User】" + strComand;
		strComand += "【移动】";
		// 用户填写坐标
		/*!
		 * 导入坐标组
		*/
		if (m_dlgAGVMove.DoModal() != IDOK)
		{
			return 0;
		}
		BufferString::std_string_format(strComand, "%s至【坐标:%d】", strComand.c_str(), m_dlgAGVMove.m_usTarget);
		// 提交申请
		if (m_service.AGVMove(lParam, m_dlgAGVMove.m_usTarget) == false)
		{
			return 0;
		}
		break;
	case ID_ACTION_STOP:	// 动作停止
		strComand = "【用户User】" + strComand;
		strComand += "【动作停止】";
		// 提交申请
		if (m_service.AGVStopAction(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_ACTION_LOAD:	// 上料
		strComand = "【用户User】" + strComand;
		strComand += "【上料】";
		// 提交申请
		if (m_service.AGVLoad(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_ACTION_UNLOAD:	// 下料
		strComand = "【用户User】" + strComand;
		strComand += "【下料】";
		// 提交申请
		if (m_service.AGVUnload(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_ACTION_OTHER:	// 其他动作
		/*
		 * 需要提供动作码
		*/
		strComand = "【用户User】" + strComand;
		strComand += "【执行其他动作】";
		// 用户选择动作
		if (m_dlgAGVAction.DoModal() != IDOK)
		{
			return 0;
		}
		BufferString::std_string_format(strComand, "%s【动作:0x%02X】", strComand.c_str(), m_dlgAGVAction.m_byActCode);
		// 提交申请
		if (m_service.AGVAction(lParam, m_dlgAGVAction.m_byActCode) == false)
		{
			return 0;
		}
		break;
	case ID_AGV_REMOVE:		// 重新移动
		strComand = "【管理员Admin】" + strComand;
		strComand += "【重新移动】";
		// 提交申请
		if (m_service.AGVRemove(lParam) == false)
		{
			return 0;
		}
		break;
		/*!
		 * 以下功能需要校验管理员身份
		*/
	case ID_AGV_RESET:		// 重置
		strComand = "【管理员Admin】" + strComand;
		strComand += "【重置】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		if (m_dlgAdminGet.DoModal() != IDOK)
		{
			return false;
		}
		/*!
		 * 校验密码正确性
		 */
		if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
		{
			return 0;
		}
		 // 提交申请
		if (m_service.AGVReset(lParam) == false)
		{
			return 0;
		}
		break;
	case ID_AGV_SET:		// 设置当前坐标与状态
	{
		/*
		 * 需要提供坐标以及状态码
		*/
		strComand = "【管理员Admin】" + strComand;
		strComand += "【设置当前坐标与状态】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		/*!
		 * 校验密码正确性
		 */
		 // 用户填写坐标与选择状态
		if (m_dlgAGVSet.DoModal() != IDOK)
		{
			return 0;
		}
		BufferString::std_string_format(strComand, "%s【坐标:%d】【状态码:0x%02X】", strComand.c_str(), m_dlgAGVSet.m_usLocation, m_dlgAGVSet.m_byStatus);
		// 提交申请
		if (m_service.AGVSet(lParam, m_dlgAGVSet.m_usLocation, m_dlgAGVSet.m_byStatus) == false)
		{
			return 0;
		}
		break;
	}
	case ID_AGV_TRAFFIC:	// 交通管制通行
	{
		strComand = "【管理员Admin】" + strComand;
		strComand += "【交通管制通行】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		if (m_dlgAdminGet.DoModal() != IDOK)
		{
			return false;
		}
		/*!
		 * 校验密码正确性
		 */
		if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
		{
			return 0;
		}
		 // 提交申请
		if (m_service.AGVTrafficPass(lParam) == false)
		{
			return 0;
		}
		break;
	}
	case ID_MODE_HANDE:		// 手动模式
	{
		strComand = "【管理员Admin】" + strComand;
		strComand += "【切换至手动模式】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		if (m_dlgAdminGet.DoModal() != IDOK)
		{
			return false;
		}
		/*!
		 * 校验密码正确性
		 */
		if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
		{
			return 0;
		}
		 // 提交申请
		if (m_service.AGVHandleMode(lParam) == false)
		{
			return 0;
		}
		break;
	}
	case ID_MODE_AUTO:		// 自动模式
	{
		strComand = "【管理员Admin】" + strComand;
		strComand += "【切换至自动模式】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		if (m_dlgAdminGet.DoModal() != IDOK)
		{
			return false;
		}
		/*!
		 * 校验密码正确性
		 */
		if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
		{
			return 0;
		}
		 // 提交申请
		if (m_service.AGVAutoMode(lParam) == false)
		{
			return 0;
		}
		break;
	}
	case ID_AGV_SHUTDOWN:	// 关机
	{
		strComand = "【管理员Admin】" + strComand;
		strComand += "【关机】";
		// 管理员身份验证
		/*!
		 * 输入管理员密码
		*/
		if (m_dlgAdminGet.DoModal() != IDOK)
		{
			return false;
		}
		/*!
		 * 校验密码正确性
		 */
		if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
		{
			return 0;
		}
		 // 提交申请
		if (m_service.AGVShutdown(lParam) == false)
		{
			return 0;
		}
		break;
	}
	}

	DebugPrint(strComand.c_str());

	return 0;
}

LRESULT CMainFrame::OnTaskCommand(WPARAM wParam, LPARAM lParam)
{
	// 管理员身份验证
	/*!
	 * 输入管理员密码
	*/
	if (m_dlgAdminGet.DoModal() != IDOK)
	{
		return false;
	}
	/*!
	 * 校验密码正确性
	 */
	if (m_service.GetAdmin(m_dlgAdminGet.m_cstrPassword) == false)
	{
		return 0;
	}
	std::string strComand = "";

	CPoint* pPtTask = (CPoint*)lParam;
	unsigned short usTaskNo = pPtTask->x;	/*!< 任务号 */
	unsigned char byTaskLevel = pPtTask->y;	/*!< 阶段号 */

	if (pPtTask == NULL)
	{
		strComand = "【管理员Admin】操作全部任务";
	}
	else
	{
		BufferString::std_string_format(strComand, "【管理员Admin】操作任务【编号:%d】【阶段:%d】", pPtTask->x, pPtTask->y);
	}

	switch (LOWORD(wParam))
	{
	case ID_TASK_CANCEL:	// 取消任务
		strComand += "【取消】";
		// 提交申请
		if (m_service.CancelTask(usTaskNo,byTaskLevel) == false)
		{
			return 0;
		}
		break;
	case ID_TASK_EDIT:		// 编辑任务
	{
		strComand += "【编辑】";
		/*!
		 * 提取任务信息
		*/
		AGVTask task = m_service.GetTask(usTaskNo,byTaskLevel);	/*!< 原任务 */
		m_dlgTaskEdit.m_unNo = task.GetNo();
		m_dlgTaskEdit.m_unLevel = task.GetLevel();
		m_dlgTaskEdit.m_unTarget = task.GetTarget();
		m_dlgTaskEdit.m_unBegin = task.GetBegin();
		task.GetAction(m_dlgTaskEdit.m_cstrAction);

		if (m_dlgTaskEdit.DoModal() != IDOK)
		{
			return 0;
		}

		AGVTask newTask(m_dlgTaskEdit.m_unNo, m_dlgTaskEdit.m_unLevel, m_dlgTaskEdit.m_cstrAction,m_dlgTaskEdit.m_unTarget, m_dlgTaskEdit.m_unBegin);	/*!< 新任务 */
		task.SetExecuter(task.GetExecuter());

		/*!
		 * 用户修改任务信息
		*/
		std::string strAction = "";
		BufferString::CString_to_std_string(strAction, m_dlgTaskEdit.m_cstrAction);
		BufferString::std_string_format(strComand, "%s为【任务坐标:%d】【起始坐标:%d】【任务动作:%s】",
			m_dlgTaskEdit.m_unTarget, m_dlgTaskEdit.m_unBegin, strAction.c_str());

		// 提交申请
		if (m_service.EditTask(usTaskNo, byTaskLevel, newTask) == false)
		{
			return 0;
		}
		break;
	}
	case ID_TASK_FINISH:	// 强制完成任务
		strComand += "【强制完成】";
		// 提交申请
		if (m_service.FinishTask(usTaskNo, byTaskLevel) == false)
		{
			return 0;
		}
		break;
	case ID_TASK_PAUSE:		// 暂停任务
		strComand += "【暂停】";
		// 提交申请
		if (m_service.PauseTask(usTaskNo, byTaskLevel) == false)
		{
			return 0;
		}
		break;
	case ID_TASK_RESET:		// 重置任务
		strComand += "【重置】";
		// 提交申请
		if (m_service.ResetTask(usTaskNo, byTaskLevel) == false)
		{
			return 0;
		}
		break;
	case ID_TASK_CANCELALL:	// 取消全部任务
		strComand += "【取消】";
		// 提交申请
		m_service.CancelAllTasks();
		break;
	case ID_TASK_PAUSEALL:	// 暂停全部任务
		strComand += "【暂停】";
		// 提交申请
		m_service.PauseAllTasks();
		break;
	case ID_TASK_RESTALL:	// 重置全部任务
		strComand += "【重置】";
		// 提交申请
		m_service.ResetAllTasks();
		break;
	}

	DebugPrint(strComand.c_str());

	delete pPtTask;

	return 0;
}

void CMainFrame::ClearAllData()
{
	m_paneAGV.m_dlgAGV.ClearListCtrlItem();
	m_paneTask.m_dlgTask.ClearListCtrlItem();
	m_paneConnect.m_dlgConnect.ClearListCtrlItem();
	m_paneRestConfig.m_dlgRestConfig.ClearListCtrlItem();
	m_paneStationConfig.m_dlgStationConfig.ClearListCtrlItem();
	m_paneTrafficConfig.m_dlgTrafficConfig.ClearListCtrlItem();
	m_paneAGVConfig.m_dlgAGVConfig.ClearListCtrlItem();
#ifdef _SELECT_FUNC
	m_paneCharger.m_dlgCharger.ClearListCtrlItem();
	m_paneChargerConfig.m_dlgChargerConfig.ClearListCtrlItem();
#ifdef _CA_H
	m_paneCAConfig.m_dlgCAConfig.ClearListCtrlItem();
	m_paneCACallConfig.m_dlgCallConfig.ClearListCtrlItem();
	m_paneCAWatchConfig.m_dlgWatchConfig.ClearListCtrlItem();
#endif // _CA_H
#ifdef _LED_H
	m_paneLEDConfig.m_dlgLEDConfig.ClearListCtrlItem();
#endif // _LED_H
#ifdef _CHARGERCONTROLER_H
	m_paneChargerCtrlerConfig.m_dlgChargerCtrlerConfig.ClearListCtrlItem();
#endif // _CHARGERCONTROLER_H
#endif // _SELECT_FUNC

	m_dlgAGVMove.Clear();
	m_dlgAGVAction.Clear();
	m_dlgAGVSet.Clear();
	return;
}

#ifdef _MAGNETICTRACK_H
void CMainFrame::InitAGVMoveDialog(std::set<unsigned short> setRFIDs)
{
	m_dlgAGVMove.m_listTargets = setRFIDs;

	return;
}

void CMainFrame::InitAGVSetDialog(std::set<unsigned short> setRFIDs)
{
	m_dlgAGVSet.m_listLoaction = setRFIDs;

	CString cstrStatus = _T("");	/*!< 状态 */
	CAGV::GetStatus(_AGVSTATUS::_AGVSTA_WAIT, cstrStatus);
	m_dlgAGVSet.m_mapStatus[cstrStatus] = _AGVSTA_WAIT;

	CAGV::GetStatus(_AGVSTATUS::_AGVSTA_SCREAM, cstrStatus);
	m_dlgAGVSet.m_mapStatus[cstrStatus] = _AGVSTA_SCREAM;

	CAGV::GetStatus(_AGVSTATUS::_AGVSTA_STOP, cstrStatus);
	m_dlgAGVSet.m_mapStatus[cstrStatus] = _AGVSTA_STOP;
	
	return;
}

void CMainFrame::InitTaskEditDialog()
{
	m_dlgTaskEdit.m_setActions.insert(_TASKACTION_EMPTYMOVE);
	m_dlgTaskEdit.m_setActions.insert(_TASKACTION_FULLMOVE);
	m_dlgTaskEdit.m_setActions.insert(_TASKACTION_LOAD);
	m_dlgTaskEdit.m_setActions.insert(_TASKACTION_MOVE);
	m_dlgTaskEdit.m_setActions.insert(_TASKACTION_UNLOAD);

	return;
}
#endif

void CMainFrame::InitAGVActionDialog()
{
	CString cstrAction = _T("");	/*!< 动作 */

	// 上料 
	CAGV::GetAction(_AGVACTION::_AGVACT_LOAD, cstrAction);
	m_dlgAGVAction.m_mapAction[cstrAction] = _AGVACT_LOAD;

	// 下料
	CAGV::GetAction(_AGVACTION::_AGVACT_UNLOAD, cstrAction);
	m_dlgAGVAction.m_mapAction[cstrAction] = _AGVACT_UNLOAD;

	for (int i = 6; i <= 0xFF; ++i)
	{
		CAGV::GetAction(i, cstrAction);
		if (cstrAction != _T(""))
		{
			m_dlgAGVAction.m_mapAction[cstrAction] = i;
		}
	}

	return;
}
