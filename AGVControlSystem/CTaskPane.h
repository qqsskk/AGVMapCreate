#pragma once
#include "CTaskDlg.h"

// CTaskPane

class CTaskPane : public CDockablePane
{
	DECLARE_DYNAMIC(CTaskPane)

public:
	CTaskPane();
	virtual ~CTaskPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTaskDlg m_dlgTask;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


