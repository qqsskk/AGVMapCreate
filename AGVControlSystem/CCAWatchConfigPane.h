#pragma once
#include "CCAWatchConfigDlg.h"

// CCAWatchConfigPane

class CCAWatchConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CCAWatchConfigPane)

public:
	CCAWatchConfigPane();
	virtual ~CCAWatchConfigPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CCAWatchConfigDlg m_dlgWatchConfig;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


