#pragma once
#include "CTrafficConfigDlg.h"

// CTrafficConfigPane

class CTrafficConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CTrafficConfigPane)

public:
	CTrafficConfigPane();
	virtual ~CTrafficConfigPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTrafficConfigDlg m_dlgTrafficConfig;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


