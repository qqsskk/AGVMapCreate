#pragma once
#include "CConnectDlg.h"

// CConnectPane

class CConnectPane : public CDockablePane
{
	DECLARE_DYNAMIC(CConnectPane)

public:
	CConnectPane();
	virtual ~CConnectPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CConnectDlg m_dlgConnect;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


