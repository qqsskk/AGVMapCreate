#pragma once
#include "CPathTabDlg.h"

// CPathTabPane

class CPathTabPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPathTabPane)

public:
	CPathTabPane();
	virtual ~CPathTabPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CPathTabDlg m_dlgPathTab;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod = DM_UNKNOWN, bool bShow = true) { return FALSE; }
	afx_msg void OnDestroy();
	virtual BOOL CanBeClosed() const { return false; }
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


