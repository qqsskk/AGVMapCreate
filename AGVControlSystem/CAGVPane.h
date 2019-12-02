#pragma once
#include "CAGVDlg.h"

// CAGVPane

class CAGVPane : public CDockablePane
{
	DECLARE_DYNAMIC(CAGVPane)

public:
	CAGVPane();
	virtual ~CAGVPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CAGVDlg m_dlgAGV;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


