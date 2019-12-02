#pragma once
#include "CStationConfigDlg.h"

// CStationConfigPane

class CStationConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CStationConfigPane)

public:
	CStationConfigPane();
	virtual ~CStationConfigPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CStationConfigDlg m_dlgStationConfig;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


