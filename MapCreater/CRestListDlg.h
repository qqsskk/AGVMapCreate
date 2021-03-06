#pragma once


// CRestListDlg 对话框

class CRestListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRestListDlg)

public:
	CRestListDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRestListDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESTLIST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listRest;

protected:
	/*!
	 * @brief 为列表成员排序
	*/
	void SortList();

	/*!
	 * @brief 删除列表成员
	 *
	 * 通知界面删除待机位图形
	 * @param unRest 待机位编号
	*/
	void Delete(unsigned int unRest);

	/*!
	 * @brief 删除全部成员
	 *
	 * 通知界面删除所有待机位图形
	*/
	void DeleteAll();

	/*!
	 * @brief 定位
	 *
	 * 通知界面定位待机位图形
	 * @param unStation 待机位编号
	*/
	void Locate(unsigned int unRest);

	/*!
	 * @brief 更新列表成员
	 *
	 * 通知界面更新待机位图形
	 * @param unRest 待机位编号
	 * @param unMark 待机位所在地标卡编号
	*/
	void Update(unsigned int unRest, unsigned int unMark);

public:
	/*!
	 * @brief 添加列表成员
	 * @param unStation 待机位编号
	 * @param ptCenter 待机位中心坐标
	 * @param unMark 待机位所在地标卡编号
	*/
	void Add(unsigned int unRest, CPoint ptCenter, unsigned int unMark);

	/*!
	 * @brief 删除列表成员
	 * @param unStation 待机位编号
	*/
	void DeleteItem(unsigned int unRest);

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
	afx_msg void OnBnClickedButtonLocation();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonDelall();
};
