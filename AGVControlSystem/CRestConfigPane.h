#pragma once
#include "CRestConfigDlg.h"

// CRestConfigPane

class CRestConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CRestConfigPane)

public:
	CRestConfigPane();
	virtual ~CRestConfigPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CRestConfigDlg m_dlgRestConfig;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


