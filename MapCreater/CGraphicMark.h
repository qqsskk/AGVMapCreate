/*!
 * @file CGraphicMark.h
 * @brief CGraphicMark类头文件
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICMARK_H
#define _CGRAPHICMARK_H

/*!
 * @class CGraphicMark
 * @brief 地标卡类
 *
 * 继承基层图类,绘制地标卡图形以及拖拽等操作
*/
class CGraphicMark :
	public CGraph
{
public:
	CGraphicMark(unsigned short usNo,CPoint ptCenter = CPoint(0,0));
	~CGraphicMark();

protected:
	unsigned short m_usNo;	/*!< 编号 */

protected:
	static unsigned int g_unWidth;	/*!< 宽 */
	static unsigned int g_unHeight;	/*!< 高 */

protected:
	CPoint m_ptCenter;		/*!< 中心坐标 */
	CPoint m_ptPrepoint;	/*!< 起始坐标 */
	CPoint m_ptLastpoint;	/*!< 终止坐标 */
	CPoint m_ptLocal;		/*!< 记录坐标 */
	bool m_bMove;			/*!< 移动标识 */
	bool m_bSelect;			/*!< 选中标识 */

public:
	/*!
	 * @brief 获取编号
	 * @return unsigned short 编号
	*/
	unsigned short GetNo() const;

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
	 * @brief 获取中心坐标
	 * @return CPoint 中心坐标
	*/
	CPoint GetCenterPoint() const;

	/*!
	 * @brief 设置中心坐标
	 * @param ptCenter 中心坐标
	*/
	void SetCenterPoint(CPoint ptCenter);

	/*!
	 * @brief 绘制
	 * @param pDC 窗口句柄
	*/
	void Draw(CDC *pDC);
};

#endif //!_CGRAPHICMARK_H