/*!
 * @file CGraph.h
 * @brief CGraph类头文件
 * @author FanKaiyu
 * @date 2018-10-16
 * @version 2.0
*/
#pragma once
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef _CGRAPH_H
#define _CGRAPH_H

#define MAX_SCALE 10.0f
#define MIN_SCALE 0.4f
#define DEFALUT_SCALE 10.0f

/*!
 * @class CGraph
 * @brief 基层图类
 *
 * 绘图的基本属性,用于其他绘图对象继承
 * 10个点代表10cm
 * 地图最大尺寸为500*500m
 * 即50000 * 50000
*/
class CGraph
{
public:
	CGraph();
	~CGraph();

protected:
	GdiplusStartupInput m_gdiplusInput;		/*!< GDI+初始化输入句柄 */
	GdiplusStartupOutput m_gdiplusOutput;	/*!< GDI+初始化输出句柄 */
	ULONG_PTR m_ulGdiplusToken;

protected:
	static CPoint g_ptOffset;			/*!< 偏移量 */
	static float g_fScale;				/*!< 缩放比例 */
	static unsigned int g_unDistance;	/*!< 网格间距 */
	static CSize g_sizeVector;			/*!< 矢量图尺寸 */
	static CRect g_rcClient;			/*!< 客户区尺寸 */
	static bool g_bFlash;				/*!< 闪烁标识 */
#ifdef _WIN64
	static unsigned long long g_ullFlashTime;	/*!< 闪烁开始时间 */
#elif _WIN32
	static unsigned long g_ulFlashTime;			/*!< 闪烁开始时间 */
#endif 

public:
	/*!
	 * @brief 设置客户区尺寸
	 * @param rcClient 客户区尺寸
	*/
	static void SetClientRect(CRect rcClient);

	/*!
	 * @brief 获取偏移量
	 * @return CPoint 偏移量
	*/
	static CPoint GetOffset();

	/*!
	 * @brief 设置偏移量
	 * @param ptOffset 偏移量
	*/
	static void SetOffset(CPoint ptOffset);

	/*!
	 * @brief 获取缩放比例
	 * @return float fScale
	*/
	static float GetScale();

	/*!
	 * @brief 设置缩放比例
	 * @param fScale 缩放比例
	*/
	static void SetScale(float fScale);

	/*!
	 * @brief 将矢量坐标转换为窗口坐标
	 * @param ptVector 矢量坐标
	 * @return CPoint 窗口坐标
	*/
	static CPoint VecToWin(CPoint ptVector);

	/*!
	 * @brief 将窗口坐标转换未矢量坐标
	 * @param ptWindow 窗口坐标
	 * @return CPoint 矢量坐标
	*/
	static CPoint WinToVec(CPoint ptWindow);

	/*!
	 * @brief 矫正矢量坐标
	 * @param ptVector 矢量坐标
	 * @return CPoint 矫正后的矢量坐标
	*/
	static CPoint Correct(CPoint ptVector);

	/*!
	 * @brief 定位
	 *
	 * 将地图中心移动至指定坐标点上
	 * @param CPoint 定位坐标
	*/
	static void Locate(CPoint ptLocation);
};

#endif // !_CGRAPH_H