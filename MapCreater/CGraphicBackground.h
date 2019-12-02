/*!
 * @file CGraphicBackground.h
 * @brief CGraphicBackground类头文件
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraph.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICBACKGROUND_H
#define _CGRAPHICBACKGROUND_H

/*!
 * @class CGraph
 * @brief 背景图类
 *
 * 继承基层图类,绘制背景图、辅助线以及拖拽、缩放等操作
*/
class CGraphicBackground :
	public CGraph
{
public:
	CGraphicBackground(Color colorLine = Color::Black,float fWidth = 2.0f,CString cstrPath = _T("../Image/background.jpg"));
	~CGraphicBackground();

protected:
	Image *m_pImage;		/*!< 背景图 */
	Pen *m_pPen;			/*!< 画笔 */
	Color m_colorLine;		/*!< 辅助线颜色 */
	float m_fWidth;			/*!< 辅助线宽度 */
	CString m_cstrPath;		/*!< 背景图路径 */
	bool m_bMove;			/*!< 移动标识 */
	CPoint m_ptPrepoint;	/*!< 起始坐标 */
	CPoint m_ptLastpoint;	/*!< 终止坐标 */
	CPoint m_ptLocal;		/*!< 偏移量记录 */

protected:
	mutex m_mutexLock;

public:
	/*!
	 * @brief 设置辅助线
	 * @param colorLine 辅助线颜色
	 * @param fWidth 辅助线宽度
	*/
	void SetLine(Color colorLine, float fWidth);

	/*!
	 * @brief 获取辅助线
	 * @param colorLine 返回辅助线颜色
	 * @param fWidth 返回辅助线宽度
	*/
	void GetLine(Color &colorLine, float &fWidth);

	/*!
	 * @brief 设置背景图
	 * @param cstrPath 背景图路径
	*/
	void SetImage(CString cstrPath);

	/*!
	 * @brief 获取背景图
	 * @return CString 背景图路径
	*/
	CString GetImage();

	/*!
	 * @brief 缩放
	 *
	 * 默认放大
	 * @param bool 缩小标识
	*/
	void Scale(bool bShrink = false);

	/*!
	 * @brief 复位
	*/
	void Reset();

	/*!
	 * @brief 拖拽
	 * @param ptWinPoint 窗口坐标
	 * @return HCURSOR 鼠标指针形状
	*/
	HCURSOR Drag(CPoint ptWinPoint);

	/*!
	 * @brief 选中
	 *
	 * 鼠标左键点击底图,触发此函数
	 * @param ptWinPoint 窗口坐标
	*/
	void Select(CPoint ptWinPoint);

	/*!
	 * @brief 取消拖拽
	*/
	void Cancel();

	/*!
	 * @brief 完成拖拽
	*/
	void Confirm();

public:
	/*!
	 * @brief 绘制背景图
	 * @param pDC 视图区句柄
	*/
	void DrawBackground(CDC *pDC);

	/*!
	 * @brief 绘制辅助线
	 * @param pDC 视图区句柄
	*/
	void DrawLine(CDC *pDC);

	/*!
	 * @brief 绘制坐标
	 * @param pDC 视图区句柄
	*/
	void DrawPoint(CDC *pDC);
};

#endif // !_CGRAPHICBACKGROUND_H