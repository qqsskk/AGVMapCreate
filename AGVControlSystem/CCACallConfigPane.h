#pragma once
#include "CCACallConfigDlg.h"

// CCACallConfigPane

class CCACallConfigPane : public CDockablePane
{
	DECLARE_DYNAMIC(CCACallConfigPane)

public:
	CCACallConfigPane();
	virtual ~CCACallConfigPane();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CCACallConfigDlg m_dlgCallConfig;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


