/*!
 * @file AGVPath.h
 * @brief AGV路径结构体文件
 *
 * AGV路径块结构体属性
 * @author FanKaiyu
 * @date 2019-01-06
 * @version 1.0
*/
#pragma once
#include "Buffer.h"

#ifndef _AGVPATH_H
#define _AGVPATH_H

/*! @brief RFID卡功能 */
enum _RFIDFUNCTION
{
	_RFIDFUNC_LEFT = 0x1F,				/*!< 沿左侧磁条移动 */
	_RFIDFUNC_RIGHT = 0x2F,				/*!< 沿右侧磁条移动 */
	_RFIDFUNC_MIDDLE = 0x3F,			/*!< 沿中间磁条移动 */
	_RFIDFUNC_STATION = 0x4F,			/*!< 工位：本RFID卡为工位所在卡 */
	_RFIDFUNC_REST = 0x5F,				/*!< 待机位：本RFID卡为待机位所在卡 */
	_RFIDFUNC_CHARGER = 0x6F,			/*!< 充电位：本RFID卡为充电位所在卡 */
	_RFIDFUNC_TRAFFIC = 0x7F,			/*!< 交通管制位：本RFID卡为交通管制位所在卡 */
};

#define _RFIDFUNC_OBS_CLOSE 0x01		/*!< AGV非接触式避障关 */
#define _RFIDFUNC_MUSIC_CLOSE 0x02		/*!< AGV音乐关 */

/*! @brief 工位/充电位相对AGV前向的朝向 */
enum _RFIDDIRECTION
{
	_RFIDFUNC_DIRECTION_NONE = 0,	/*!< 无 0000*/
	_RFIDFUNC_DIRECTION_FRONT,		/*!< 前 0001*/
	_RFIDFUNC_DIRECTION_BACK,		/*!< 后 0010*/
	_RFIDFUNC_DIRECTION_LEFT,		/*!< 左 0011*/
	_RFIDFUNC_DIRECTION_RIGHT,		/*!< 右 0100*/
};

/*!
 * @brief AGV路径块其他功能块结构体
 * 其他功能码的说明:
 * 1字节 由 8位组成 即 (高)0000  (低)0000
 * 其中高4位 为 工位/充电位相对AGV前向的朝向
 * 取值详见_RFIDDIRECTION
 * 低4位 为 AGV避障开关状态以及AGV音乐开关状态
 * 取值_RFIDFUNC_OBS_CLOSE 或 _RFIDFUNC_MUSIC_CLOSE
 * 也可以使用_RFIDFUNC_OBS_CLOSE|_RFIDFUNC_MUSIC_CLOSE 表示同时关闭避障以及音乐
 *
 * 其他功能吗的合成方式为:
 * ((工位/充电位相对AGV前向的朝向) << 4) | (AGV避障开关状态以及AGV音乐开关状态)
*/
typedef struct _AGVPathPointOtherFunction
{
protected:
	unsigned char m_byDirection:4;
	unsigned char m_byMusic : 1;
	unsigned char m_byObs : 1;
public:
	_AGVPathPointOtherFunction()
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		m_byMusic = 0;
		m_byObs = 0;
	}

	_AGVPathPointOtherFunction(bool bMusic, bool bObs, unsigned char byDirection)
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		m_byMusic = bMusic & 0x01;
		m_byObs = bObs & 0x01;
	}

	_AGVPathPointOtherFunction(unsigned char byObsAndMusic, unsigned char byDirection)
	{
		m_byDirection = _RFIDFUNC_DIRECTION_NONE;
		SetObsAndMusic(byObsAndMusic);
	}

	_AGVPathPointOtherFunction(unsigned char byFlag)
	{
		SetObsAndMusicAndDirection(byFlag);
	}

	_AGVPathPointOtherFunction(const _AGVPathPointOtherFunction& func)
	{
		m_byDirection = func.m_byDirection;
		m_byMusic = func.m_byMusic;
		m_byObs = func.m_byObs;
	}

	void operator=(const _AGVPathPointOtherFunction& func)
	{
		m_byDirection = func.m_byDirection;
		m_byMusic = func.m_byMusic;
		m_byObs = func.m_byObs;
	}
public:
	/*!
	 * @brief 设置避障状态
	 * @param bool 新避障开关状态
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetObs(bool bObs)
	{
		if (m_byObs == bObs)
		{
			return false;
		}

		m_byObs = bObs & 0x01;

		return;
	}

	/*!
	 * @brief 获取避障状态
	 * @return bool 避障开关状态
	*/
	bool GetObs() const
	{
		return m_byObs;
	}

	/*!
	 * @brief 设置音乐状态
	 * @param bool 新音乐开关状态
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetMusic(bool bMusic)
	{
		if (m_byMusic == bMusic)
		{
			return false;
		}

		m_byMusic = bMusic & 0x01;

		return;
	}

	/*!
	 * @brief 获取音乐状态
	 * @return bool 音乐开关状态
	*/
	bool GetMusic() const
	{
		return m_byMusic;
	}

	/*!
	 * @brief 设置音乐与避障状态
	 * @param unsigned char 音乐与避障状态码
	*/
	void SetObsAndMusic(unsigned char byFlag)
	{
		m_byObs = byFlag & 0x1;
		m_byMusic = (byFlag >> 1) & 0x1;

		return;
	}

	/*!
	 * @brief 设置方向
	 * @param unsigned char 方向码
	*/
	void SetDirection(unsigned char byFlag)
	{
		m_byDirection = byFlag;

		return;
	}

	/*!
	 * @brief 获取方向
	 * @return unsigned char 方向码
	*/
	unsigned char GetDirection() const
	{
		return m_byDirection;
	}

	/*!
	 * @brief 获取方向
	 * @param string& 方向说明
	*/
	void GetDirection(std::string& strDirection)
	{
		switch (m_byDirection)
		{
		case _RFIDFUNC_DIRECTION_NONE:
			strDirection = "无朝向";
			break;
		case _RFIDFUNC_DIRECTION_FRONT:
			strDirection = "前向";
			break;
		case _RFIDFUNC_DIRECTION_BACK:
			strDirection = "后向";
			break;
		case _RFIDFUNC_DIRECTION_LEFT:
			strDirection = "左向";
			break;
		case _RFIDFUNC_DIRECTION_RIGHT:
			strDirection = "右向";
			break;
		}

		return;
	}

	/*!
	 * @brief 获取方向
	 * @param wstring& 方向说明
	*/
	void GetDirection(std::wstring& wstrDirection)
	{
		std::string strDirection = "";
		GetDirection(strDirection);

		std_string_to_wstring(wstrDirection, strDirection);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief 获取方向
	 * @param CString& 方向说明
	*/
	void GetDirection(CString &cstrDirection)
	{
		std::string strDirection = "";
		GetDirection(strDirection)

			std_string_to_CStrig(cstrDirection, strDirection);

		return;
	}
#endif // !_AFX

	/*!
	 * @brief 设置音乐、避障以及方向
	 * @param unsigned char 音乐、避障以及方向组合码
	*/
	void SetObsAndMusicAndDirection(unsigned char byFlag)
	{
		SetObsAndMusic(byFlag & 0xF);
		SetDirection((byFlag >> 4) & 0xF);

		return;
	}

	/*!
	 * @brief 获取音乐、避障以及方向组合码
	 * @return unsigned char 音乐、避障以及方向组合码
	*/
	unsigned char GetFlag() const
	{
		return (m_byDirection << 4) | ((m_byMusic << 1) | m_byObs);
	}

} _OtherFunc;

/*!
 * @brief AGV路径块结构体
 *
 * 主要由
 * RFID卡编号(2字节)
 * 卡功能(1字节)：详见_RFIDFUNCTION
 * 卡速度(1字节)： 正值为前进,负值为后退,0为停止
 * 其他功能(1字节)：其中包括工位/充电位相对AGV前向的朝向、AGV避障开关状态以及AGV音乐开关状态
 * 组成
*/
typedef struct _AGVPathPointStruct
{
protected:
	unsigned short m_usRFID;		/*!< RFID卡编号 */
	unsigned char m_byFunc;			/*!< 卡功能码 */
	char m_chSpeed;					/*!< 卡速度 */
public:
	_OtherFunc m_otherFunc;			/*!< 其他功能 */

public:
	_AGVPathPointStruct(unsigned short usRFID,unsigned char byFunc,char chSpeed,bool bMusic = true,bool bObs = true,unsigned char byDirection = _RFIDFUNC_DIRECTION_NONE)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObs(bObs);
		m_otherFunc.SetMusic(bMusic);
		m_otherFunc.SetDirection(byDirection);
	}

	_AGVPathPointStruct(unsigned short usRFID, unsigned char byFunc, char chSpeed ,unsigned char byObsAndMusic = 0, unsigned char byDirection = _RFIDFUNC_DIRECTION_NONE)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObsAndMusic(byObsAndMusic);
		m_otherFunc.SetDirection(byDirection);
	}

	_AGVPathPointStruct(unsigned short usRFID, unsigned char byFunc, char chSpeed, unsigned char byFlag = 0)
	{
		m_usRFID = usRFID;
		m_byFunc = byFunc;
		m_chSpeed = chSpeed;
		m_otherFunc.SetObsAndMusicAndDirection(byFlag);
	}

	_AGVPathPointStruct(const _AGVPathPointStruct& path)
	{
		m_usRFID = path.m_usRFID;
		m_byFunc = path.m_byFunc;
		m_chSpeed = path.m_chSpeed;
		m_otherFunc = path.m_otherFunc;
	}

	void operator=(const _AGVPathPointStruct& path)
	{
		m_usRFID = path.m_usRFID;
		m_byFunc = path.m_byFunc;
		m_chSpeed = path.m_chSpeed;
		m_otherFunc = path.m_otherFunc;
	}

	/*!
	 * @brief 设置RFID卡编号
	 * @param unsigned short 新RFID卡编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetRFID(unsigned short usRFID)
	{
		if (m_usRFID == usRFID)
		{
			return false;
		}

		m_usRFID = usRFID;

		return true;
	}

	/*!
	 * @brief 获取RFID卡编号
	 * @return unsigned short RFID卡编号
	*/
	unsigned short GetRFID() const
	{
		return m_usRFID;
	}

	/*!
	 * @brief 设置功能
	 * @param unsigned char 新功能
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetFunction(unsigned char byFync)
	{
		if (m_byFunc == byFync)
		{
			return false;
		}

		m_byFunc = byFync;

		return true;
	}

	/*!
	 * @brief 获取功能
	 * @return unsigned char 功能码
	*/
	unsigned char GetFunction()
	{
		return m_byFunc;
	}

	/*!
	 * @brief 获取功能
	 * @param string& 功能说明
	*/
	void GetFunction(std::string &strFunc)
	{
		switch (m_byFunc)
		{
		case _RFIDFUNC_LEFT:
			strFunc = "沿左侧磁条移动";
			break;
		case _RFIDFUNC_RIGHT:
			strFunc = "沿右侧磁条移动";
			break;
		case _RFIDFUNC_MIDDLE:
			strFunc = "沿中间磁条移动";
			break;
		case _RFIDFUNC_STATION:
			strFunc = "工位";
			break;
		case _RFIDFUNC_REST:
			strFunc = "待机位";
			break;
		case _RFIDFUNC_CHARGER:
			strFunc = "充电位";
			break;
		case _RFIDFUNC_TRAFFIC:
			strFunc = "交通管制";
			break;
		}

		return;
	}

	/*!
	 * @brief 获取功能
	 * @param wstring& 功能说明
	*/
	void GetFunction(std::wstring &wstrFunc)
	{
		std::string strFunc = "";
		GetFunction(strFunc);

		std_string_to_wstring(wstrFunc, strFunc);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief 获取功能
	 * @param CString& 功能说明
	*/
	void GetFunction(CString &cstrFunc)
	{
		std::string strFunc = "";
		GetFunction(strFunc);

		std_string_to_CStrig(cstrFunc, strFunc);

		return;
	}
#endif // !_AFX

	/*!
	 * @brief 设置速度
	 * @param char 新速度
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetSpeed(char chSpeed)
	{
		if (m_chSpeed == chSpeed)
		{
			return false;
		}

		if (chSpeed > 100 | chSpeed < -100)
		{
			return false;
		}

		m_chSpeed = chSpeed;

		return true;
	}

	/*!
	 * @brief 获取速度
	 * @return char 速度
	*/
	char GetSpeed() const
	{
		return m_chSpeed;
	}

}_AGVPatchPoint;

#endif // !_AGVPATH_H