#pragma once
#include "CLEDConfigDlg.h"

// CLEDConfigPane

class CLEDConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CLEDConfigPane)

public:
	CLEDConfigPane();
	virtual ~CLEDConfigPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CLEDConfigDlg m_dlgLEDConfig;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


