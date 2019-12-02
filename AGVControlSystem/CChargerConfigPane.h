#pragma once
#include "CChargerConfigDlg.h"

// CChargerConfigPane

class CChargerConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CChargerConfigPane)

public:
	CChargerConfigPane();
	virtual ~CChargerConfigPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CChargerConfigDlg m_dlgChargerConfig;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


