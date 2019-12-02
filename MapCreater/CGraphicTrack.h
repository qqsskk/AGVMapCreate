#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICTRACK_H
#define _CGRAPHICTRACK_H

class CGraphicTrack :
	public CGraph
{
public:
	CGraphicTrack(bool bArc);
	~CGraphicTrack();

protected:
	CPoint m_ptStart;	/*!< 起点坐标 */
	CPoint m_ptEnd;		/*!< 终点坐标 */
	bool m_bArc;		/*!< 弧线标识 */

protected:
	static float g_fWidth;	/*!< 宽 */

protected:
	CPoint m_ptPrepoint;	/*!< 起始坐标 */
	CPoint m_ptLastpoint;	/*!< 终止坐标 */
	CPoint m_ptLocal;		/*!< 记录起点坐标 */
	CPoint m_ptDistance;	/*!< 间距 */
	bool m_bMove;			/*!< 移动标识 */
	bool m_bSelect;			/*!< 选中标识 */
	bool m_bShow;			/*!< 显示标识 */

public:
	/*!
	 * @brief 设置起点坐标
	 * @param ptStart 起点坐标(矢量坐标)
	*/
	void SetStartPoint(CPoint ptStart);

	/*!
	 * @brief 设置终点坐标
	 * @param ptEnd 终点坐标(矢量坐标)
	*/
	void SetEndPoint(CPoint ptEnd);

	/*!
	 * @brief 设置轨道类型
	 * @param bArc 弧线为true,直线为false
	*/
	void SetType(bool bArc);

	/*!
	 * @brief 获取起点坐标(矢量坐标)
	 * @return CPoint 起点坐标
	*/
	CPoint GetStartPoint();

	/*!
	 * @brief 获取终点坐标(矢量坐标)
	 * @return CPoint 终点坐标
	*/
	CPoint GetEndPoint();

	/*!
	 * @brief 获取轨道类型
	 * @return bool true为弧形,false为直线
	*/
	bool GetType();

	/*!
	 * @brief 窗口坐标是否在区域内
	 * @param ptWinpoint 窗口坐标
	 * @return 在区域内返回true,不在返回false
	*/
	bool IsInside(CPoint ptWinpoint);

	/*!
	 * @brief 获取选中状态
	 * @return bool 选中返回true,未选中返回false
	*/
	bool IsSelected();

	/*!
	 * @brief 选中
	 * @param ptWinpoint 窗口坐标
	*/
	void Select(CPoint ptWinpoint);

	/*!
	 * @brief 拖拽
	 * @param  ptWinpoint 窗口坐标
	 * @return HCURSOR 鼠标指针形状
	*/
	HCURSOR Drag(CPoint ptWinPoint);

	/*!
	 * @brief 确认
	*/
	void Confirm();

	/*!
	 * @brief 取消
	*/
	void Cancel();

	/*!
	 * @brief 绘制
	 * @param pDC 窗口句柄
	*/
	void Draw(CDC *pDC);

	/*!
	 * @brief 设置轨道宽度
	 * @param fWidth 宽度
	*/
	static void SetWidth(float fWidth);

	/*!
	 * @brief 获取轨道宽度
	 * @return float 宽度
	*/
	static float GetWidth();

	/*!
	 * @brief 弧度转角度
	 * @param dbRadian 弧度值
	 * @return double 角度值
	*/
	static double RadianToAngle(double dbRadian);

	/*!
	 * @brief 角度转弧度
	 * @param dbAngle 角度值
	 * @return double 弧度值
	*/
	static double AngleToRadian(double dbAngle);
};

#endif //!_CGRAPHICTRACK_H