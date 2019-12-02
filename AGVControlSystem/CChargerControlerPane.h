#pragma once
#include "CChargerControlerConfigDlg.h"

// CChargerControlerPane

class CChargerControlerPane : public CDockablePane
{
	DECLARE_DYNAMIC(CChargerControlerPane)

public:
	CChargerControlerPane();
	virtual ~CChargerControlerPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CChargerControlerConfigDlg m_dlgChargerCtrlerConfig;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


