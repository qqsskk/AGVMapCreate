/*!
 * @file CGraphicAGV.h
 * @brief CGraphicAGV类头文件
 * @author FanKaiyu
 * @date 2018-10-15
 * @version 2.0
*/

#pragma once
#include "CGraph.h"

#ifndef _CGRAPHICAGV_H
#define _CGRAPHICAGV_H

enum _GraphicAGVActionStatus
{
	_GRAPHICAGVACTSTA_NONE,
	_GRAPHICAGVACTSTA_LOAD,
	_GRAPHICAGVACTSTA_UNLOAD,
};

/*!
 * @class CGraphicAGV
 * @brief AGV图类
 *
 * 继承基层图类,绘制AGV图形以及根据AGV状态绘制AGV图形等操作
*/
class CGraphicAGV :
	public CGraph
{
public:
	CGraphicAGV(unsigned char byNo,CString cstrType,CString cstrMove);
	~CGraphicAGV();

protected:
	unsigned char m_byNo;		/*!< 编号 */
	CString m_cstrType;			/*!< 类型 P牵引式 S潜入式 T移载式 F叉车式 A机械手式 L举升式 */
	CString m_cstrMove;			/*!< 可运动方向 S单向 D双向 F全向 */

protected:
	bool m_bRun;				/*!< 移动标识 */
	bool m_bUnVol;				/*!< 电量不足标识 */
	bool m_bError;				/*!< 异常标识 */
	bool m_bObs;				/*!< 避障标识 */
	bool m_bCargo;				/*!< 载货状态 */
	unsigned char m_byAction;	/*!< 动作状态 0-无动作/完成 1-上料中 2-下料中 */

protected:
	bool m_bShow;				/*!< 显示标识符 */
	CPoint m_ptCenter;			/*!< 中心坐标 矢量坐标 */

protected:
	static unsigned int g_unWidth;	/*!< 宽 */
	static unsigned int g_unHeight;	/*!< 高 */
	static CString g_cstrAGV;		/*!< AGV贴图 */
	static CString g_cstrRun;		/*!< 移动贴图 */
	static CString g_cstrUnVol;		/*!< 电量不足贴图 */
	static CString g_cstrError;		/*!< 异常贴图 */
	static CString g_cstrObs;		/*!< 避障贴图 */
	static CString g_cstrLoad;		/*!< 上料贴图 */
	static CString g_cstrUnload;	/*!< 下料贴图 */
	static CString g_cstrCargo;		/*!< 货料贴图 */

public:
	/*!
	 * @brief 设置类型
	 * @param cstrType 类型
	*/
	void SetType(CString cstrType);

	/*!
	 * @brief 设置可运动方向
	 * @param cstrMove 可运动方向
	*/
	void SetMove(CString cstrMove);

	/*!
	 * @brief 获取编号
	 * @return unsigned int 编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 获取类型
	 * return CString 类型 P牵引式 S潜入式 T移载式 F叉车式 A机械手式 L举升式
	*/
	CString GetType() const;

	/*!
	 * @brief 获取可运动方向
	 * @return CString 可运动方向 S单向 D双向 F全向
	*/
	CString GetMove() const;

	/*!
	 * @brief 设置贴图
	 * @param CString AGV贴图路径
	 * @param CString 移动贴图路径
	 * @param CString 电量不足贴图路径
	 * @param CString 异常贴图路径
	 * @param CString 避障贴图路径
	 * @param CString 上料贴图路径
	 * @param CString 下料贴图路径
	 * @param CString 载货贴图路径
	*/
	static void SetImage(CString cstrAGV, CString cstrRun, CString cstrUnVol, CString cstrError, CString cstrObs, CString cstrLoad, CString cstrUnload, CString cstrCargo);

	/*!
	 * @brief 设置AGV贴图
	 * @param CString AGV贴图路径
	*/
	static void SetAGVImage(CString cstrAGV);

	/*!
	 * @brief 设置移动贴图
	 * @param CString 移动贴图路径
	*/
	static void SetRunImage(CString cstrRun);

	/*!
	 * @brief 设置电量不足贴图
	 * @param CString 电量不足贴图路径
	*/
	static void SetUnVolImage(CString cstrUnVol);

	/*!
	 * @brief 设置异常贴图
	 * @param CString 异常贴图路径
	*/
	static void SetErrorImage(CString cstrError);

	/*!
	 * @brief 设置避障贴图
	 * @param CString 避障贴图路径
	*/
	static void SetObsImage(CString cstrObs);

	/*!
	 * @brief 设置上料贴图
	 * @param CString 上料贴图路径
	*/
	static void SetLoadImage(CString cstrLoad);

	/*!
	 * @brief 设置下料贴图
	 * @param CString 下料贴图路径
	*/
	static void SetUnloadImage(CString cstrUnload);

	/*!
	 * @brief 设置载货贴图
	 * @param CString 载货贴图路径
	*/
	static void SetCargoImage(CString cstrCargo);

	/*!
	 * @brief 获取贴图
	 * @param CString& 返回AGV贴图路径
	 * @param CString& 返回移动贴图路径
	 * @param CString& 返回电量不足贴图路径
	 * @param CString& 返回异常贴图路径
	 * @param CString& 返回避障贴图路径
	 * @param CString& 上料贴图路径
	 * @param CString& 下料贴图路径
	 * @param CString& 载货贴图路径
	*/
	static void GetImage(CString &cstrAGV, CString &cstrRun, CString &cstrUnVol, CString &cstrError, CString &cstrObs, CString& cstrLoad, CString& cstrUnload, CString& cstrCargo);

	/*!
	 * @brief 获取AGV贴图路径
	 * @return CString AGV贴图路径
	*/
	static CString GetAGVImage();

	/*!
	 * @brief 获取移动贴图路径
	 * @return CString 移动贴图路径
	*/
	static CString GetRunImage();

	/*!
	 * @brief 获取电量不足贴图路径
	 * @return CString 电量不足贴图路径
	*/
	static CString GetUnVolImage();

	/*!
	 * @brief 获取异常贴图路径
	 * @return CString 异常贴图路径
	*/
	static CString GetErrorImage();

	/*!
	 * @brief 获取避障贴图路径
	 * @return CString 避障贴图路径
	*/
	static CString GetObsImage();

	/*!
	 * @brief 获取上料贴图路径
	 * @return CString 上料贴图路径
	*/
	static CString GetLoadImage();

	/*!
	 * @brief 获取下料贴图路径
	 * @return CString 下料贴图路径
	*/
	static CString GetUnloadImage();

	/*!
	 * @brief 获取载货贴图路径
	 * @return CString 载货贴图路径
	*/
	static CString GetCargoImage();
	
	/*!
	 * @brief 设置状态
	 * @param bool 移动标识
	 * @param bool 电量不足标识
	 * @param bool 异常标识
	 * @param bool 避障标识
	 * @param bool 载货状态
	 * @param unsigned char 动作状态
	*/
	void SetStatus(bool bRun, bool bUnVol, bool bError, bool bObs, bool bCargo, unsigned char byAction);

	/*!
	 * @brief 设置移动状态
	 * @param bool 移动标识 
	*/
	void SetRunStatus(bool bRun);

	/*!
	 * @brief 设置电量不足状态
	 * @param bool 电量不足标识
	*/
	void SetUnVolStatus(bool bUnVol);

	/*!
	 * @brief 设置异常状态
	 * @param bool 异常标识
	*/
	void SetErrorStatus(bool bError);

	/*!
	 * @brief 设置避障状态
	 * @param bool 避障标识
	*/
	void SetObsStatus(bool bObs);

	/*!
	 * @brief 设置载货状态
	 * @param bool 载货标识
	*/
	void SetCargoStatus(bool bCargo);

	/*!
	 * @brief 设置动作状态
	 * @param unsigned char 动作状态
	*/
	void SetActionStatus(unsigned char byAction);

	/*!
	 * @brief 是否显示
	 * @param bShow true为显示 false为隐藏
	*/
	void IsShow(bool bShow);

	/*!
	 * @brief 设置中心坐标
	 * @param ptCenter 中心坐标(矢量坐标)
	*/
	void SetCenterPoint(CPoint ptCenter);

	/*!
	 * @brief 获取中心坐标
	 * @return CPoint 中心坐标(矢量坐标)
	*/
	CPoint GetCenterPoint() const;

	/*!
	 * @brief 绘图
	 * @param pDC 窗口句柄
	*/
	void Draw(CDC *pDC);
};

#endif //!_CGRAPHICAGV_H