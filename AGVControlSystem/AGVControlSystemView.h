
// AGVControlSystemView.h: CAGVControlSystemView 类的接口
//

#pragma once
#include "../MapCreater/CGraphicAGV.h"
#include "../MapCreater/CGraphicBackground.h"
#include "../MapCreater/CGraphicCharger.h"
#include "../MapCreater/CGraphicMark.h"
#include "../MapCreater/CGraphicRest.h"
#include "../MapCreater/CGraphicStation.h"
#include "../MapCreater/CGraphicTrack.h"
#include <map>
#include <list>

using namespace std;

class CAGVControlSystemView : public CView
{
protected: // 仅从序列化创建
	CAGVControlSystemView() noexcept;
	DECLARE_DYNCREATE(CAGVControlSystemView)

	// 特性
public:
	CAGVControlSystemDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// 实现
public:
	virtual ~CAGVControlSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	mutex m_mutexLock;									/*!< 互斥锁 */

protected:
	CGraphicBackground *m_pBackground;					/*!< 背景图指针 */
	map<unsigned char, CGraphicAGV*> m_mapAGVs;			/*!< AGV集合 */
	map<unsigned short, CGraphicMark*> m_mapMarks;		/*!< 地标卡集合 */
	map<unsigned char, CGraphicStation*> m_mapStations;	/*!< 工位集合 */
	map<unsigned char, CGraphicCharger*> m_mapChargers;	/*!< 充电器集合 */
	map<unsigned char, CGraphicRest*> m_mapRests;		/*!< 待机位集合 */
	list<CGraphicTrack*> m_listTracks;					/*!< 轨道列表 */

public:
	/*!
	 * @brief 创建背景
	 * @param float 缩放比例
	 * @param CPoint 偏移量
	*/
	void CreateBackground(float fScale, CPoint ptOffset);

	/*!
	 * @brief 删除背景
	*/
	void DeleteBackground();

	/*!
	 * @brief 生成AGV
	 * @param unsigned char AGV编号
	 * @param CString AGV类型
	 * @param CString AGV可运动方向
	*/
	void CreateAGV(unsigned char byNo, CString cstrType, CString cstrMove);

	/*!
	 * @brief 销毁AGV
	 * @param unsigned char AGV编号
	*/
	void DeleteAGV(unsigned char byNo);

	/*!
	 * @brief 销毁全部AGV
	*/
	void DeleteAllAGV();

	/*!
	 * @brief 设置AGV位置
	 * @param unsigned char AGV编号
	 * @param unsigned short 地标卡编号
	*/
	void SetAGVLocation(unsigned char byNo, unsigned short usMark);

	/*!
	 * @brief 显示或隐藏AGV
	 * @param unsigned char AGV编号
	 * @param bool 显示AGV为true,隐藏AGV为false
	*/
	void IsAGVShow(unsigned char byNo, bool bShow);

	/*!
	 * @brief 设置AGV状态
	 * @param unsigned char AGV编号
	 * @param bool 移动标识
	 * @param bool 电量不足标识
	 * @param bool 异常标识
	 * @param bool 避障标识
	 * @param bool 载货状态
	 * @param unsigned char 动作状态
	*/
	void SetAGVStatus(unsigned char byNo, bool bRun, bool bUnVol, bool bError, bool bObs, bool bCargo, unsigned char byAction);

	/*!
	 * @brief 生成地标卡
	 * @param unsigned short 地标卡编号
	 * @param CPoint 中心坐标
	*/
	void CreateMark(unsigned short usNo, CPoint ptCenter);

	/*!
	 * @brief 销毁地标卡
	 * @param unsigned short 地标卡编号
	*/
	void DeleteMark(unsigned short usNo);

	/*!
	 * @brief 销毁全部地标卡
	*/
	void DeleteAllMark();

	/*!
	 * @brief 生成工位
	 * @param unsigned char 工位编号
	 * @param unsigned short 地标卡编号
	 * @param CPoint 中心坐标
	 * @param CString 工位名称
	*/
	void CreateStation(unsigned char byNo, unsigned short usMark, CPoint ptCenter, CString cstrName = _T(""));

	/*!
	 * @brief 销毁工位
	 * @param unsigned char 工位编号
	*/
	void DeleteStation(unsigned char byNo);

	/*!
	 * @brief 销毁全部工位
	*/
	void DeleteAllStation();

	/*!
	 * @brief 生成待机位
	 * @param unsigned char 待机位编号
	 * @param unsigned short 地标卡编号
	 * @param CPoint 中心坐标
	 * @param CString 待机位名称
	*/
	void CreateRestPosition(unsigned char byNo, unsigned short usMark, CPoint ptCenter, CString cstrName = _T(""));

	/*!
	 * @brief 删除待机位
	 * @param unsigned char 待机位编号
	*/
	void DeleteRestPosition(unsigned char byNo);

	/*!
	 * @brief 删除全部待机位
	*/
	void DeleteAllRestPosition();

	/*!
	 * @brief 生成充电位
	 * @param unsigned char 充电位编号
	 * @param unsigned short 地标卡编号
	 * @param unsigend char 充电器控制器编号
	 * @param CPoint 中心坐标
	 * @param CString 充电位名称
	*/
	void CreateCharger(unsigned char byNo, unsigned short usMark, unsigned char byCtrl,CPoint ptCenter, CString cstrName = _T(""));

	/*!
	 * @brief 删除充电位
	 * @param unsigned char 充电位编号
	*/
	void DeleteCharger(unsigned char byNo);

	/*!
	 * @brief 删除全部充电位
	*/
	void DeleteAllCharger();

#ifdef _WIN64
	/*!
	 * @brief 设置充电位状态
	 * @param unsigned char 充电位编号
	 * @param bool 充电器状态
	 * @param unsigned long long 充电器开始充电时间
	*/
	void SetPower(unsigned char byNo,bool bPower, unsigned long long ullChargerTime);
#elif _WIN32
	/*!
	 * @brief 设置充电位状态
	 * @param unsigned char 充电位编号
	 * @param bool 充电器状态
	 * @param unsigned long 充电器开始充电时间
	*/
	void SetPower(unsigned char byNo, bool bPower, unsigned long ulChargerTime);
#endif

	/*!
	 * @brief 创建轨道
	 * @param CPoint 轨道起点坐标
	 * @param CPoint 轨道终点坐标
	 * @param bool 弧形标识
	*/
	void CreateTrack(CPoint ptStart, CPoint ptEnd, bool bArc);

	/*!
	 * @brief 删除轨道
	 * @param CPoint 轨道起点坐标
	 * @param CPoint 轨道终点坐标
	 * @param bool 弧形标识
	*/
	void DeleteTrack(CPoint ptStart, CPoint ptEnd, bool bArc);

	/*!
	 * @brief 删除全部轨道
	*/
	void DeleteAllTrack();

	/*!
	 * @brief 数据清空
	*/
	void ClearAll();

	/*!
	 * @brief 数据读取
	*/

	/*!
	 * @brief 数据导入
	*/

	/*!
	 * @brief 数据导出
	*/
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // AGVControlSystemView.cpp 中的调试版本
inline CAGVControlSystemDoc* CAGVControlSystemView::GetDocument() const
   { return reinterpret_cast<CAGVControlSystemDoc*>(m_pDocument); }
#endif

