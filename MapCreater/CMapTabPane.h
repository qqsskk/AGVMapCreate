#pragma once
#include "CMapTabDlg.h"

// CMapTabPane

class CMapTabPane : public CDockablePane
{
	DECLARE_DYNAMIC(CMapTabPane)

public:
	CMapTabPane();
	virtual ~CMapTabPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMapTabDlg m_dlgMapTab;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod = DM_UNKNOWN, bool bShow = true) { return FALSE; }
	afx_msg void OnDestroy();
	virtual BOOL CanBeClosed() const { return false; }
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


