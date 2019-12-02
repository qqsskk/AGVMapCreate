#pragma once
#include "CAGVConfigDlg.h"

// CAGVConfigPane

class CAGVConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CAGVConfigPane)

public:
	CAGVConfigPane();
	virtual ~CAGVConfigPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CAGVConfigDlg m_dlgAGVConfig;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


