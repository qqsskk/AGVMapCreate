/*!
 * @file CCharger.h
 * @brief 充电器基类文件
 *
 * 包含充电器的基本属性与基本动作
 * @author FanKaiyu
 * @date 2019-01-11
 * @version 2.0
*/

#pragma once
#include <string>

#define ONEDAY 86400000L	/*!< 1天(ms) */
#define ONESECOND 1000L		/*!< 1秒(ms)*/
#define ONEMINUTE 60000L	/*!< 1分钟(ms) */
#define ONEHOUR 3600000L	/*!< 1小时(ms) */

#ifndef _CHARGER_H
#define _CHARGER_H

/*! @brief 充电器状态 */
enum _CHARGERSTATUS
{
	_CHARGERSTATUS_NONE,	/*!< 未使用:无用户锁定或未开始充电时为此状态,当用户使用完成离开充电区域后重置为此状态 */
	_CHARGERSTATUS_USING,	/*!< 使用中:充电开始或用户锁定时置为此状态 */
	_CHARGERSTATUS_FINISH,	/*!< 使用完成:充电结束后置为此状态 */
};

/*!
 * @class CCharger
 * @brief 充电器基类
 *
 * 包含充电器的基本属性与基本动作
*/
class CCharger
{
public:
	CCharger(unsigned char byNo, unsigned char byController = 0, std::string strName = "");
	CCharger(unsigned char byNo, unsigned char byController = 0, std::wstring wstrName = L"");
#ifdef _AFX
	CCharger(unsigned char byNo, unsigned char byController = 0, CString cstrName = _T(""));
#endif // _AFX
	~CCharger();
protected:
	unsigned char m_byNo;				/*!< 充电器编号 */
#ifdef _WIN64
	unsigned long long m_ullChargeTime;	/*!< 充电开始时间,通过此属性可以计算出通电时长 */
#elif _WIN32
	unsigned long m_ulChargeTime;		/*!< 充电开始时间,通过此属性可以计算出通电时长 */
#endif // !_WIN64
	unsigned long m_ulMinDuration;		/*!< 最短充电持续时间(单位:ms) */
	void *m_pUser;						/*!< 使用此充电器的用户指针,通常为AGV指针,多用于判断用户是否在占用此充电器 */
	unsigned char m_byStatus;			/*!< 充电器状态 */
	unsigned char m_byController;		/*!< 控制本充电器的控制器编号 */
	unsigned char m_byPower;			/*!< 当前电池电量 */
	std::string m_strName;				/*!< 名称 */

public:
	/*!
	 * @brief 设置编号
	 * @param unsigned char 新充电器编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char 充电器编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 设置名称
	 * @param string 新充电位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief 设置名称
	 * @param wstring 新充电位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief 设置名称
	 * @param CString 新充电位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief 获取充电位名称
	 * @param string& 工位名称
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief 获取充电位名称
	 * @param wstring& 工位名称
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief 获取充电位名称
	 * @param CString& 工位名称
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

	/*!
	 * @brief 开始充电
	 * @param unsigned long 最短充电持续时间(单位:ms)
	 * @param void* 用户之中，填写此参数则在开始充电的同时用户锁定此充电器
	*/
	void StartCharging(unsigned long ulMinDuration = 0, void* pUser = nullptr);

	/*!
	 * @brief 结束充电
	 * @param void* 用户指针,填写此参数则在结束充电的同时解除用户对充电器的占用.
	*/
	void EndCharging(void *pUser = nullptr);

	/*!
	 * @brief 重置充电器
	*/
	void Reset();

	/*!
	 * @brief 锁定
	 * @param void *pUser 用户指针
	 * @return bool 锁定成功返回true,失败返回false
	*/
	bool Lock(void *pUser);

	/*!
	 * @brief 解锁
	 * @param void *pUser 用户指针
	*/
	void Unlock(void *pUser);

#ifdef _WIN64
	/*!
	 * @brief 获取开始充电时间
	 * @return unsigned long long 充电开始时间(单位:ms)
	*/
	unsigned long long GetStartTime();
#elif _WIN32
	/*!
	 * @brief 获取开始充电时间
	 * @return unsigned long 充电开始时间(单位:ms)
	*/
	unsigned long GetStartTime();
#endif // !_WIN64

#ifdef _WIN64
	/*!
	 * @brief 获取充电持续时间
	 * @return unsigned long long 充电持续时间(单位:ms)
	*/
	unsigned long long GetDurativeTime();
#elif _WIN32
	/*!
	 * @brief 获取充电持续时间
	 * @return unsigned long 充电持续时间(单位:ms)
	*/
	unsigned long GetDurativeTime();
#endif // !_WIN64

	/*!
	 * @brief 最短充电持续时间
	 * @return unsigned long 最短充电持续时间(单位:ms)
	*/
	unsigned long GetMinDurativeTime();

	/*!
	 * @brief 获取状态
	 * @return unsigned char 充电器状态
	*/
	unsigned char GetStatus() const;

	/*!
	 * @brief 获取用户
	 * @return void* 用户指针
	*/
	void* GetUser() const;

	/*!
	 * @brief 设置控制器
	 * @param unsigned char 新控制器编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetController(unsigned char byController);

	/*!
	 * @brief 获取控制器
	 * @return unsigned char 控制器编号
	*/
	unsigned char GetController() const;

	/*!
	 * @brief 设置当前电池电量(%)
	 * @param unsigned char 当前电池新电量
	 * @return 设置成功返回true,失败返回false
	*/
	bool SetPower(unsigned char byPower);

	/*!
	 * @brief 获取当前电池电量(%)
	 * @return unsigned char 当前电池电量
	*/
	unsigned char GetPower() const;
public:
	/*!
	 * @brief 获取编号
	 * @param string& 编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& 编号
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief 获取充电持续时间
	 * @param string& 充电持续时间
	*/
	void GetDurativeTime(std::string& strTime);

	/*!
	 * @brief 获取充电持续时间
	 * @param wstring& 充电持续时间
	*/
	void GetDurativeTime(std::wstring& wstrTime);

	/*!
	 * @brief 最短充电持续时间
	 * @param string& 最短充电持续时间
	*/
	void GetMinDurativeTime(std::string& strTime);

	/*!
	 * @brief 最短充电持续时间
	 * @param wstring& 最短充电持续时间
	*/
	void GetMinDurativeTime(std::wstring& wstrTime);

	/*!
	 * @brief 获取充电器状态
	 * @param string& 充电器状态
	*/
	void GetStatus(std::string& strStatus);

	/*!
	 * @brief 获取充电器状态
	 * @param wstring& 充电器状态
	*/
	void GetStatus(std::wstring& wstrStatus);

	/*!
	 * @brief 获取控制器
	 * @param string& 控制器编号
	*/
	void GetController(std::string& strController);

	/*!
	 * @brief 获取控制器
	 * @param wstring& 控制器编号
	*/
	void GetController(std::wstring& wstrController);

	/*!
	 * @brief 获取当前电池电量(%)
	 * @return string 当前电池电量
	*/
	void GetPower(std::string& strPower);

	/*!
	 * @brief 获取当前电池电量(%)
	 * @return wstring 当前电池电量
	*/
	void GetPower(std::wstring& wstrPower);

	/*!
	 * @brief 获取状态码
	 * @param string 状态
	 * @return unsigned char 状态吗
	*/
	static unsigned char GetStatusCode(std::string strStatus);

	/*!
	 * @brief 获取状态码
	 * @param wstring 状态
	 * @return unsigned char 状态吗
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param string& 状态
	*/
	static void GetStatus(unsigned char byStatus, std::string& strStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param wstring& 状态
	*/
	static void GetStatus(unsigned char byStatus, std::wstring& wstrStatus);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 编号
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief 获取充电持续时间
	 * @param CString& 充电持续时间
	*/
	void GetDurativeTime(CString& cstrTime);

	/*!
	 * @brief 最短充电持续时间
	 * @param CString& 最短充电持续时间
	*/
	void GetMinDurativeTime(CString& cstrTime);

	/*!
	 * @brief 获取充电器状态
	 * @param CString& 充电器状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetStatus(CString& cstrStatus,COLORREF& color);

	/*!
	 * @brief 获取控制器
	 * @param CString& 控制器编号
	*/
	void GetController(CString& cstrController);

	/*!
	 * @brief 获取当前电池电量(%)
	 * @return CString 当前电池电量
	*/
	void GetPower(CString& cstrPower);

	/*!
	 * @brief 获取状态码
	 * @param CString 状态
	 * @return unsigned char 状态吗
	*/
	static unsigned char GetStatusCode(CString cstrStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param CString& 状态
	*/
	static void GetStatus(unsigned char byStatus, CString& cstrStatus);
#endif // !_AFX
};

#endif // !_CHARGER_H