/*!
 * @file CGraphicCharger.h
 * @brief CGraphicCharger类头文件
 * @author FanKaiyu
 * @date 2018-10-22
 * @version 2.0
*/

#pragma once
#include "CGraph.h"
#include <mutex>

using namespace std;

#ifndef _CGRAPHICCHARGER_H
#define _CGRAPHICCHARGER_H

/*!
 * @class CGraphicCharger
 * @brief 充电器图类
 *
 * 继承基层图类,绘制充电器图形以及拖拽等操作
*/
class CGraphicCharger :
	public CGraph
{
public:
	CGraphicCharger(unsigned char byNo,unsigned short usMark,unsigned char byCtrl,CString cstrName = _T(""));
	~CGraphicCharger();

protected:
	unsigned char m_byNo;		/*!< 编号 */
	unsigned short m_usMark;	/*!< 地标卡 */
	unsigned char m_byCtrl;		/*!< 控制器 */
	CString m_cstrName;			/*!< 名称 */
#ifdef _WIN64
	unsigned long long m_ullChargeTime;	/*!< 充电开始时间 */
#elif _WIN32
	unsigned long m_ulChargeTime;		/*!< 充电开始时间 */
#endif

protected:
	bool m_bPower;			/*!< 通电状态 */

protected:
	static unsigned int g_unWidth;	/*!< 宽 */
	static unsigned int g_unHeight;	/*!< 高 */
	static CString g_cstrPath;		/*!< 背景图路径 */
	static CString g_cstrPower;		/*!< 充电标识路径 */

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
	 * @brief 设置名称
	 * @param CString 名称
	*/
	void SetName(CString cstrName);

	/*!
	 * @brief 获取名称
	 * @return CString 名称
	*/
	CString GetName() const;

#ifdef _WIN64
	/*!
	 * @brief 设置充电状态
	 * @param bPower true为充电,false为断电
	*/
	void SetPower(bool bPower,unsigned long long ullChargeTime);
#elif _WIN32
	/*!
	 * @brief 设置充电状态
	 * @param bPower true为充电,false为断电
	*/
	void SetPower(bool bPower, unsigned long ulChargeTime);
#endif

	/*!
	 * @brief 设置控制器
	 * @param unsigned char 控制器编号
	*/
	void SetCtrl(unsigned char byCtrl);

	/*!
	 * @brief 获取控制器
	 * @return unsigned char 控制器编号
	*/
	unsigned char GetCtrl() const;

	/*!
	 * @brief 设置地标卡
	 * @param unsigned short 地标卡
	*/
	void SetMark(unsigned short usMark);

	/*!
	 * @brief 获取地标卡
	 * @return unsigned short 地标卡
	*/
	unsigned short GetMark() const;

	/*!
	 * @brief 设置背景图
	 * @param cstrPath 背景图路径
	 * @param cstrPower	充电标识路径
	*/
	static void SetImage(CString cstrPath,CString cstrPower);

	/*!
	 * @brief 获取背景图
	 * @param cstrPath 返回背景图路径
	 * @param cstrPower	返回充电标识路径
	*/
	static void GetImage(CString &cstrPath, CString &cstrPower);

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

#endif //!_CGRAPHICCHARGER_H