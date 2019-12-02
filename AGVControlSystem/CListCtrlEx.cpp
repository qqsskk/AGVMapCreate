#include "stdafx.h"
#include "CListCtrlEx.h"


CListCtrlEx::CListCtrlEx()
{
	m_bEnableTips = false;
	m_nItem = -1;
	m_nSubItem = -1;
}


CListCtrlEx::~CListCtrlEx()
{
}
BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW *lplvdr = (NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;
	switch (lplvdr->nmcd.dwDrawStage) { // 判断状态
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT: // 如果未画ITEM之前就要进行颜色的改变
		TEXT_BK tb;
		DWORD dw = nmcd.dwItemSpec;
		if (MapItemColor.Lookup(dw, tb)) {
			// 根据在SetItemColor(DWORD item,COLORREF color)设置的ITEM号和COLORREF在模板中查找，然后进行颜色赋值。
			lplvdr->clrText = tb.colText;
			lplvdr->clrTextBk = tb.colTextBk;
			*pResult = CDRF_DODEFAULT;
		}
		break;
	}
}

// 设置某一行的前景色和背景色
void CListCtrlEx::SetItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor) 
{
	TEXT_BK tb;
	tb.colText = textColor;
	tb.colTextBk = textBkColor;

	MapItemColor.SetAt(item, tb); // 设置某行的颜色
	this->RedrawItems(item, item); // 重新染色
	this->SetFocus(); // 设置焦点
	UpdateWindow();
}

// 设置全部行的前景色和背景色
void CListCtrlEx::SetAllItemColor(DWORD item, COLORREF textColor, COLORREF textBkColor) 
{
	TEXT_BK tb;
	tb.colText = textColor;
	tb.colTextBk = textBkColor;

	if (item > 0) 
	{
		for (DWORD numItem = 0; numItem < item; numItem++) 
		{
			MapItemColor.SetAt(numItem, tb); // 设置某行的颜色
			this->RedrawItems(numItem, numItem); // 重新染色
		}

		UpdateWindow();
	}
	return;
}

// 清除颜色映射表
void CListCtrlEx::ClearColor() 
{
	MapItemColor.RemoveAll();
}

void CListCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bEnableTips)
	{
		CString str;
		LVHITTESTINFO lvhti;

		// 判断鼠标当前所在的位置(行, 列)  
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		// 如果鼠标移动到另一个单元格内, 则进行处理; 否则, 不做处理  
		if ((lvhti.iItem != m_nItem) || (lvhti.iSubItem != m_nSubItem))
		{
			// 保存当前鼠标所在的(行,列)  
			m_nItem = lvhti.iItem;
			m_nSubItem = lvhti.iSubItem;

			// 如果鼠标移动到一个合法的单元格内,则显示新的提示信息  
			// 否则, 不显示提示  

			if ((m_nItem != -1) && (m_nSubItem != -1))
			{
				// @@@@@@@@ 在这里修改要显示的提示信息  
				// 这里仅仅是一个例子---获得当前单元格的文字信息, 并设置为新的提示信息  
				str = GetItemText(m_nItem, m_nSubItem);
				m_toolTip.AddTool(this, str);
				// 显示提示框  
				m_toolTip.Pop();
			}
			else
			{
				m_toolTip.AddTool(this, _T(""));
				// 显示提示框  
				m_toolTip.Pop();
			}

		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (::IsWindow(m_toolTip.GetSafeHwnd()))
	{
		m_toolTip.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlEx::OnDestroy()
{
	CListCtrl::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	// listctrl销毁时, 同时销毁 tooltipctrl  
	m_toolTip.DestroyWindow();
	m_toolTip.Detach();
}

BOOL CListCtrlEx::EnableTips()
{
	EnableToolTips(TRUE);
	// 创建tooltip控件  
	m_bEnableTips = m_toolTip.Create(this, TTS_ALWAYSTIP);

	if (m_bEnableTips)
	{
		m_toolTip.Activate(TRUE);
		m_toolTip.SetDelayTime(TTDT_INITIAL, 0);
	}

	return m_bEnableTips;
}