#pragma once
#include "CCAConfigDlg.h"

// CCAConfigPane

class CCAConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CCAConfigPane)

public:
	CCAConfigPane();
	virtual ~CCAConfigPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CCAConfigDlg m_dlgCAConfig;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


