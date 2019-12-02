/*!
 * @file CGraphicRest.h
 * @brief CGraphicRest类头文件
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICREST_H
#define _CGRAPHICREST_H

class CGraphicRest :
	public CGraph
{
public:
	CGraphicRest(unsigned char byNo,unsigned short usMark,CString cstrName = _T(""));
	~CGraphicRest();

protected:
	unsigned char m_byNo;			/*!< 编号 */
	unsigned short m_usMark;		/*!< 地标卡 */
	CString m_cstrName;				/*!< 别名 */ 

protected:
	static unsigned int g_unWidth;	/*!< 宽 */
	static unsigned int g_unHeight;	/*!< 高 */
	static CString g_cstrPath;		/*!< 背景图路径 */

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
	 * @return unsigned char 编号
	*/
	unsigned char GetNo() const;
	
	/*!
	 * @brief 设置地标卡
	 * @param usMark 地标卡
	*/
	void SetMark(unsigned short usMark);

	/*!
	 * @brief 获取地标卡
	 * @return unsigned short 地标卡
	*/
	unsigned short GetMark() const;

	/*!
	 * @brief 设置名称
	 * @param CString 名称
	*/
	void SetName(CString cstrName);

	/*!
	 * @brief 获取名称
	 * @return CString 名称
	*/
	CString GetName() const;

	/*!
	 * @brief 设置背景图
	 * @param cstrPath 背景图路径
	*/
	static void SetImage(CString cstrPath);

	/*!
	 * @brief 获取背景图
	 * @param cstrPath 返回背景图路径
	 * @param cstrPower	返回充电标识路径
	*/
	static CString GetImage();

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
	CPoint GetCenterPoint();

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

#endif // !_CGRAPHICREST_H