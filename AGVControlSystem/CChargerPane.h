#pragma once
#include "CChargerDlg.h"

// CChargerPane

class CChargerPane : public CDockablePane
{
	DECLARE_DYNAMIC(CChargerPane)

public:
	CChargerPane();
	virtual ~CChargerPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CChargerDlg m_dlgCharger;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


