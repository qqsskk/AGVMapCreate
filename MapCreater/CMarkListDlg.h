#pragma once


// CMarkListDlg 对话框

class CMarkListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMarkListDlg)

public:
	CMarkListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMarkListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MARKLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listMark; /*!< 地标卡列表 */

protected:
	/*!
	 * @brief 为列表成员排序
	*/
	void SortList();

	/*!
	 * @brief 删除列表成员
	 * 
	 * 通知界面删除地标卡图形
	 * @param unMark 地标卡编号
	*/
	void Delete(unsigned int unMark);

	/*!
	 * @brief 删除全部成员
	 *
	 * 通知界面删除所有地标卡图形
	*/
	void DeleteAll();

	/*!
	 * @brief 定位
	 *
	 * 通知界面定位地标卡图形
	 * @param unMark 地标卡编号
	*/
	void Locate(unsigned int unMark);

public:
	/*!
	 * @brief 添加列表成员
	 * @param unMark 地标卡编号
	 * @param ptCenter 地标卡中心坐标
	*/
	void Add(unsigned int unMark,CPoint ptCenter);

	/*!
	 * @brief 删除列表成员
	 * @param unMark 地标卡编号
	*/
	void DeleteItem(unsigned int unMark);

protected:
	/*!
	 * @brief 初始化列表
	*/
	void InitList();

	/*!
	 * @brief 列表尺寸变更
	*/
	void ListSize();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonDelall();
	afx_msg void OnBnClickedButtonLocation();
};
