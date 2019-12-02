/*!
 * @file CAGV.h
 * @brief AGV基类文件
 *
 * AGV类的基本属性文件
 * @author FanKaiyu
 * @date 2019-01-06
 * @version 2.0
*/
#pragma once
#include <string>

#ifndef _AGV_H
#define _AGV_H

/*!
 * AGV分类
 * 根据功能分类，可分为：
 * 1、移载式（Transfer）
 * 2、牵引式（Pull）
 * 3、潜入式（Steal）
 * 4、叉车式（Forklift）
 * 5、机械手式（Arm）
 * 6、举升式（Lift）
 *
 * 根据导航方式分类，可分为：
 * 1、磁轨导航(Magnetic Track)
 * 2、激光导航(Laser)
 * 3、二维码导航(QR code)
 *
 * 根据根据通信方式分类，可分为：
 * 1、TCP/IP SOCKET通信
 * 2、TCP/IP HTTP通信
 * 3、串口广播通信
*/

/*! @brief AGV类型 */
enum _AGVTYPE
{
	_AGVTYPE_ARM = 'A',		/*!< 机械手式AGV */
	_AGVTYPE_FORK = 'F',	/*!< 叉车式AGV */
	_AGVTYPE_LIFT = 'L',	/*!< 举升式AGV */
	_AGVTYPE_PULL = 'P',	/*!< 牵引式AGV */
	_AGVTYPE_SNEAK = 'S',	/*!< 潜入式AGV */
	_AGVTYPE_TRAVEL = 'T',	/*!< 移载式AGV */
};

/*! @brief AGV可移动方向 */
enum _AGVDRIVER
{
	_AGVDRIVER_SINGLE = 'S',	/*! 单向 */
	_AGVDRIVER_DOUBLE = 'D',	/*! 双向 */
	_AGVDRIVER_FULL = 'F',		/*! 全向 */
};

/*! @brief AGV状态信息码 */
enum _AGVSTATUS
{
	_AGVSTA_WAIT,		/*!< 待机状态.
						上位机接收到此状态,认为AGV已经准备好并可以接收上位机的控制调度.
						此时下位机可以接收并处理上位机发送的所有类型的报文. 
						*/
	_AGVSTA_RUN,		/*!< 运行状态.
						上位机接收到此状态,认为AGV在移动或动作中.此时上位机除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.但移动或动作结束后必须将AGV状态恢复为待机状态.
						*/
	_AGVSTA_STOP,		/*!< 停止状态.
						上位机接收到此状态,认为AGV被人为停止.属于手动操作;此时上位机除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.用户可以手动控制AGV,但用户操作结束后必须将AGV状态恢复为待机状态.
						*/
	_AGVSTA_SCREAM,		/*!< 急停状态.
						上位机接收到此状态,认为AGV被紧急停止运行,属于手动操作.此时上位机会上报警告至用户,并且除了心跳状态外不做任务处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.但用户操作结束后必须将AGV状态恢复为待机状态.
						*/
	_AGVSTA_FIND,		/*!< 寻磁/定位状态.
						上位机接收到此状态,认为AGV脱离轨道.此时上位机除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.下位机应启动寻磁/定位机制,在一段时间内寻磁/定位失败后,应将AGV状态置为停止,并将异常状态置为脱磁/无法定位.
						*/
	_AGVSTA_OBSDOWN,	/*!< 非接触式避障减速状态.
						上位机接收到此状态,认为AGV触发非接触式避障并处于减速运行状态,此时上位机除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.
						*/
	_AGVSTA_OBSSTOP,	/*!< 非接触式避障停止状态.
						上位机接收到此状态,认为AGV触发非接触式避障并处于减速运行状态,此时上位机除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.
						*/
	_AGVSTA_MOBSSTOP,	/*!< 接触式避障停止状态.
						上位机接收到此状态,认为AGV触发接触式避障并处于停止状态,此时上位机上报异常至用户,并且除了会请求心跳状态外不做任何处理.
						此时下位机除了处理上位机发出的心跳报文,其他报文均可以不处理.
						*/
	_AGVSTA_TRAFFIC,	/*!< 交通管制停止状态.
						上位机接收到此状态,认为AGV处于交通管制点等待上位机放行,此时上位机根据交通管制规则决定此状态的AGV是否允许通行.
						此时下位机除了处理上位机发出的心跳报文以及交通管制控制报文,其他报文均可以不处理.
						*/
};

/*! @brief AGV动作信息码 */
enum _AGVACTION
{
	_AGVACT_NONE,		/*!< 无/停止动作.
						未执行状态:AGV收到此动作,若AGV正在执行其他动作,则停止动作的执行.
						执行状态:不处理.
						执行完成状态:不处理.
						*/
	_AGVACT_LOAD,		/*!< 上料动作.
						未执行状态:对移载式AGV而言是辊筒执行上料动作;
								对于潜入式或牵引式AGV而言是升降杆执行上升动作;
								对叉车式AGV而言是货叉执行升起动作;
								对机械手AGV而言是机械手执行抓取动作,当机械手AGV在同一个点需要完成多个不同的动作时,通过增加动作码执行相应的动作;
								执行动作前AGV应判断是否处于可执行此动作的指定位置上.若不在,则应将当前地标卡置为0并上报异常.
								若开始执行动作,则将动作状态置为执行中.
						执行状态:动作完成后将动作状态置为执行完成.拥有货料检测功能的AGV应在确认货料已存放至AGV上后再将动作状态置为执行完成.
						执行完成状态:不处理.
						*/
	_AGVACT_UNLOAD,		/*!< 下料动作
						未执行状态:对移载式AGV而言是辊筒执行下料动作;
								对于潜入式或牵引式AGV而言是升降杆执行下降动作;
								对叉车式AGV而言是货叉执行下降动作;
								对机械手AGV而言是机械手执行下放动作,当机械手AGV在同一个点需要完成多个不同的动作时,通过增加动作码执行相应的动作;
								执行动作前AGV应判断是否处于可执行此动作的指定位置上.若不在,则应将当前地标卡置为0并上报异常.
								若开始执行动作,则将动作状态置为执行中.
						执行状态:动作完成后将动作状态置为执行完成.拥有货料检测功能的AGV应在确认货料已离开AGV后再将动作状态置为执行完成.
						执行完成状态:不处理.
						*/
	_AGVACT_MOVE,		/*!< 移动动作 
						未执行状态:AGV收到此动作,则AGV开始移动至终止地标卡.若AGV当前坐标与终止坐标相同,则不用移动.将动作状态置为执行完成.否则将动作状态置为执行中.
						执行状态:若AGV当前坐标与终止坐标相同,则不用移动,将动作状态置为执行完成.
						执行完成状态:不处理.
						*/
	_AGVACT_TRAFFIC,	/*!< 交通管制动作 
						未执行状态:不处理.
						执行状态:不处理.
						执行完成状态:AGV收到此动作,则AGV可以离开终止地标卡.
						*/
	_AGVACT_REMOVE,		/*!< 重新移动动作
						未执行状态:AGV收到此动作,则AGV重新移动至终止坐标.若AGV因当前坐标与终止坐标相同不能移动时,则AGV可将当前坐标重置后重新移动.将动作状态置为执行中.
						执行状态:若AGV当前坐标与终止坐标相同,则不用移动.将动作状态置为执行完成.
						执行完成状态:不处理.
						*/
};

/*! @brief AGV动作状态信息码 */
enum _AGVACTIONSTATUS
{
	_AGVACTSTA_NONE,	/*!< 动作未执行状态 */
	_AGVACTSTA_EXE,		/*!< 动作执行中状态 */
	_AGVACTSTA_FINISH,	/*!< 动作执行完成状态 */
};

/*! @brief AGV异常信息码*/
enum _AGVERROR
{
	_AGVERR_NONE,		/*!< 无/正常
						说明:AGV运行正常.
						上位机:不处理.
						下位机:不处理.
						*/
	_AGVERR_MISS,		/*!< 脱磁/无法定位
						说明:提醒用户将AGV恢复至轨道上,防止因脱磁造成的运行流程卡顿.
						上位机:上报异常至用户.
						下位机:变更状态为停止.
						*/
	_AGVERR_PATH,		/*!< 路径异常
						说明:因上位机发送的移动指令错误（即没有对应的移动路径）引起的.
						上位机:上报异常至用户.
						下位机:变更状态为停止.
						*/
	_AGVERR_LOCATE,		/*!< 坐标异常
						说明:因AGV未处于上位机动作的指定位置引起的.上位机应发送重新移动或移动动作,使AGV重新到达指定位置,保证执行动作的安全性.
						上位机:上报异常至用户.
						下位机:将当前做坐标重置.
						*/
	_AGVERR_DIRECTION,	/*!< 定向异常
						说明:因缺少相对朝向引起的异常。
						上位机:上报异常至用户.
						下位机:变更状态为停止.
						*/
	_AGVERR_ACT = -2,	/*!< 动作异常
						说明:由PC上位机检测出因执行某一动作长时间未完成引起的.
						上位机:上报异常至用户.
						下位机:不处理.
						*/
	_AGVERR_COMM,		/*!< 通信异常
						说明:由PC上位机检测出因无法发送报文或长时间未接收报文引起的.
						上位机:上报异常至用户.
						下位机:不处理.
						*/
};

/*!
 * @class CAGV
 * @brief AGV基类
 *
 * 根据不同类型的AGV可延伸出N种AGV类
 * 但是每种AGV均含有一些基本属性
 * 此类定义了AGV设备的通用基本属性信息
*/
class CAGV
{
public:
	CAGV(unsigned char byNo,unsigned char byType, unsigned char byDriver,float fMaxSepped, bool bAuto = true);
	~CAGV();
protected:
	/*!
	 * 基本属性信息
	*/
	unsigned char m_byNo;			/*!< AGV编号,每台AGV均含有一个用于识别的号码即AGV编号 */
	unsigned char m_byType;			/*!< AGV类型,根据不同类型的AGV执行特定的任务 */
	unsigned char m_byDriver;		/*!< AGV可移动方向 */
	float m_fMaxSpeed;				/*!< AGV的最大速度值，最大速度值 X 当前速度 = 实际速度，单位 m/min */
									/*!< AGV坐标信息，因不同的AGV类型上传的坐标信息可能不尽相同，所以此属性并非AGV设备的通用属性 */
									/*!< AGV目标坐标信息，同样因不同的AGV类型上传的坐标信息不尽相同，所以此属性并非AGV设备的通用数据。此属性多用以判断当前AGV移动的终点是否为系统指定的终点，系统是否要更改其移动路线 */
	unsigned char m_byPower;		/*!< AGV电量，每台AGV剩余的电量信息，取值为0-100%，此属性用于AGV任务的管理，自动充电功能的实现 */
	char m_chSpeed;					/*!< AGV速度，每台AGV当前的速度，取值为-100-100%，若想得出实际速度值还需要AGV最大速度属性 */
	unsigned char m_byStatus;		/*!< AGV状态，用以判断AGV运行是否正常，是否可接收系统控制等状态信息 */
	char m_chError;					/*!< AGV异常信息，多用以提示用户AGV异常，请用户对异常状况进行排查，异常分为2种：1、系统检测出的异常；2、AGV自身检测出的异常 */
	unsigned short m_usAction;		/*!< AGV动作，用以记录AGV当前正在执行的动作类型的状态码，多用以记录移动控制，交通管制，升降杆升降等 */
	unsigned char m_byActionStatus;	/*!< AGV动作状态，用以记录AGV动作的完成状态，多以0标识动作未开始执行，1表示动作正在执行中，2表示动作已经执行完成 */
	unsigned char m_byCargo;		/*!< AGV载货，0为空载，255（0xFF）为满载 */
	bool m_bAuto;					/*!< AGV模式，为true时为自动模式，AGV可通过程序进行远程控制，为false时为手动模式，AGV只能通过用户手动操作进行控制 */
	void *m_pTask;					/*!< 用以记录AGV当前执行的任务内容,通常为任务结构体 */

public:
	/*!
	 * 基本函数动作
	*/
	/*!
	 * @brief 设置编号
	 * @param unsigned char AGV新编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char AGV编号 
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 设置类型
	 * @param unsigned char AGV新类型
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetType(unsigned char byType);

	/*!
	 * @brief 获取类型
	 * @return unsigned char AGV类型
	*/
	unsigned char GetType() const;

	/*!
	 * @brief 设置可移动方向
	 * @param unsigned char AGV新可移动方向
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetDriver(unsigned char byDriver);

	/*!
	 * @brief 获取可移动方向
	 * @return unsigned char AGV可移动方向
	*/
	unsigned char GetDriver() const;

	/*!
	 * @brief 设置最大速度
	 * @param float AGV新最大速度
	 * @return bool 设置成功返回true,失败返回falses
	*/
	bool SetMaxSpeed(float fMaxSpeed);
	
	/*!
	 * @brief 获取最大速度
	 * @return float AGV最大速度
	*/
	float GetMaxSpeed() const;

	/*!
	 * @brief 设置电量
	 * @param unsigned char AGV新电量
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetPower(unsigned char byPower);

	/*!
	 * @brief 获取电量
	 * @return unsigned char AGV电量
	*/
	unsigned char GetPower() const;

	/*!
	 * @brief 设置当前速度(%)
	 * @param char AGV新当前速度
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetSpeed(char chSpeed);

	/*!
	 * @brief 获取当前速度(%)
	 * @return char AGV当前速度
	*/
	char GetSpeed() const;

	/*!
	 * @brief 获取实际速度(m/min)
	 * @return float AGV当前实际速度
	*/
	float GetRealitySpeed() const;

	/*!
	 * @brief 设置状态
	 * @param unsigned char AGV新状态
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetStatus(unsigned char byStatus);

	/*!
	 * @brief 获取状态
	 * @return unsinged char AGV状态
	*/
	unsigned char GetStatus() const;

	/*!
	 * @brief 设置动作
	 * @param unsigned short AGV动作
	 * @param unsigned char AGV新动作状态
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAction(unsigned short usAction, unsigned char byStatus);

	/*!
	 * @brief 获取动作
	 * @param unsigned short& AGV动作
	 * @param unsigned char& AGV动作状态
	*/
	void GetAction(unsigned short& usAction, unsigned char& byStatus);

	/*!
	 * @brief 设置异常
	 * @param char AGV新异常码
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetError(char chError);

	/*!
	 * @brief 获取异常
	 * @return char AGV异常码
	*/
	char GetError() const;

	/*!
	 * @brief 下发任务
	 * @param void* 任务结构体指针
	*/
	void DownloadTask(void* pTask);

	/*!
	 * @brief 清除任务
	*/
	void ClearTask();

	/*!
	 * @brief 获取任务
	*/
	void* GetTask() const;

	/*!
	 * @brief 设置模式
	 * @param bool 为true时为自动模式，为false为手动模式
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetMode(bool bAuto);

	/*!
	 * @brief 获取模式
	 * @return bool 为true时为自动模式，为false为手动模式
	*/
	bool GetMode() const;

	/*!
	 * @brief 设置载货数量
	 * @param unsigned char 新载货数量
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetCargo(unsigned char byCargo);

	/*!
	 * @brief 获取载货数量
	 * @return unsigned char 载货数量
	*/
	unsigned char GetCargo() const;

public:
	/*!
	 * @brief 获取编号
	 * @param string& AGV编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& AGV编号
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief 获取类型
	 * @param string& AGV类型
	*/
	void GetType(std::string& strType);

	/*!
	 * @brief 获取类型
	 * @param wstring& AGV类型
	*/
	void GetType(std::wstring& wstrType);

	/*!
	 * @brief 获取可移动方向
	 * @param string& AGV可移动方向
	*/
	void GetDriver(std::string& strDriver);

	/*!
	 * @brief 获取可移动方向
	 * @param wstring& AGV可移动方向
	*/
	void GetDriver(std::wstring& wstrDriver);

	/*!
	 * @brief 获取最大速度(m/min)
	 * @param string& AGV最大速度(m/min)
	*/
	void GetMaxSpeed(std::string& strSpeed);

	/*!
	 * @brief 获取最大速度(m/min)
	 * @param wstring& AGV最大速度(m/min)
	*/
	void GetMaxSpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief 获取当前电量(%)
	 * @param string& AGV当前电量(%)
	*/
	void GetPower(std::string& strPower);

	/*!
	 * @brief 获取当前电量(%)
	 * @param wstring& AGV当前电量(%)
	*/
	void GetPower(std::wstring& wstrPower);

	/*!
	 * @brief 获取当前速度(%)
	 * @param string& AGV当前速度(%)
	*/
	void GetSpeed(std::string& strSpeed);

	/*!
	 * @brief 获取当前速度(%)
	 * @param wstring& AGV当前速度(%)
	*/
	void GetSpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief 获取当前实际速度(m/min)
	 * @param string& AGV实际速度(m/min)
	*/
	void GetRealitySpeed(std::string& strSpeed);

	/*!
	 * @brief 获取当前实际速度(m/min)
	 * @param wstring& AGV实际速度(m/min)
	*/
	void GetRealitySpeed(std::wstring& wstrSpeed);

	/*!
	 * @brief 获取状态信息
	 * @param string& AGV状态
	*/
	void GetStatus(std::string& strStatus);

	/*!
	 * @brief 获取状态信息
	 * @param wstring& AGV状态
	*/
	void GetStatus(std::wstring& wstrStatus);

	/*!
	 * @brief 获取动作信息
	 * @param string& AGV动作
	 * @param string& AGV动作状态
	*/
	void GetAction(std::string& strAction, std::string& strStatus);

	/*!
	 * @brief 获取动作信息
	 * @param wstring& AGV动作
	 * @param wstring& AGV动作状态
	*/
	void GetAction(std::wstring& wstrAction, std::wstring& wstrStatus);

	/*!
	 * @brief 获取异常信息
	 * @param string& AGV异常信息
	*/
	void GetError(std::string& strError);

	/*!
	 * @brief 获取异常信息
	 * @param wstring& AGV异常信息
	*/
	void GetError(std::wstring& wstrError);

	/*!
	 * @brief 获取模式
	 * @param string& 模式
	*/
	void GetMode(std::string& strMode);

	/*!
	 * @brief 获取模式
	 * @param wstring& 模式
	*/
	void GetMode(std::wstring& wstrMode);

	/*!
	 * @brief 获取载货数量
	 * @param string& 载货数量
	*/
	void GetCargo(std::string& strCargo);

	/*!
	 * @brief 获取载货数量
	 * @param wstring& 载货数量
	*/
	void GetCargo(std::wstring& wstrCargo);

	/*!
	 * @brief 获取动作码
	 * @param string 动作
	 * @return unsigned char 动作码
	*/
	static unsigned char GetActionCode(std::string strAction);

	/*!
	 * @brief 获取动作码
	 * @param wstring 动作
	 * @return unsigned char 动作码
	*/
	static unsigned char GetActionCode(std::wstring wstrAction);

	/*!
	 * @brief 获取动作
	 * @param unsigned char 动作码
	 * @param string& 动作
	*/
	static void GetAction(unsigned char byActCode, std::string& strAction);

	/*!
	 * @brief 获取动作
	 * @param unsigned char 动作码
	 * @param wstring& 动作
	*/
	static void GetAction(unsigned char byActCode, std::wstring& wstrAction);

	/*!
	 * @brief 获取状态码
	 * @param string 状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(std::string strStatus);

	/*!
	 * @brief 获取状态码
	 * @param wstring 状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(std::wstring wstrStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param string& 状态
	*/
	static void GetStatus(unsigned char byStaCode, std::string& strStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param wstring& 状态
	*/
	static void GetStatus(unsigned char byStaCode, std::wstring& wstrStatus);

	/*!
	 * @brief 获取异常码
	 * @param string 异常信息
	 * @return char 异常码
	*/
	static char GetErrorCode(std::string strError);

	/*!
	 * @brief 获取异常码
	 * @param wstring 异常信息
	 * @return char 异常码
	*/
	static char GetErrorCode(std::wstring wstrError);

	/*!
	 * @brief 获取异常信息
	 * @param char 异常码
	 * @param string& 异常信息
	*/
	static void GetErrorMessage(char chErrorCode, std::string& strErrorMsg);

	/*!
	 * @brief 获取异常信息
	 * @param char 异常码
	 * @param wstring& 异常信息
	*/
	static void GetErrorMessage(char chErrorCode, std::wstring& wstrErrorMsg);

	/*!
	 * @brief 获取动作状态码
	 * @param string 动作状态
	 * @return unsigned char 动作状态
	*/
	static unsigned char GetActionStatusCode(std::string strActStatus);

	/*!
	 * @brief 获取动作状态码
	 * @param wstring 动作状态
	 * @return unsigned char 动作状态
	*/
	static unsigned char GetActionStatusCode(std::wstring wstrActStatus);

	/*!
	 * @brief 获取动作状态
	 * @param unsigned char 动作状态码
	 * @param string& 动作状态
	*/
	static void GetActionStatus(unsigned char byActStaCode, std::string& strActStatus);

	/*!
	 * @brief 获取动作状态
	 * @param unsigned char 动作状态码
	 * @param wstring& 动作状态
	*/
	static void GetActionStatus(unsigned char byActStaCode, std::wstring& wstrActStatus);

	/*!
	 * @brief 获取类型码
	 * @param string& 类型
	 * @return unsigned char 类型码
	*/
	static unsigned char GetTypeCode(std::string strType);

	/*!
	 * @brief 获取类型码
	 * @param wstring& 类型
	 * @return unsigned char 类型码
	*/
	static unsigned char GetTypeCode(std::wstring wstrType);

	/*!
	 * @brief 获取类型
	 * @param unsigned char 类型码
	 * @param string& 类型
	*/
	static void GetType(unsigned char byTypeCode, std::string& strType);

	/*!
	 * @brief 获取类型
	 * @param unsigned char 类型码
	 * @param wstring& 类型
	*/
	static void GetType(unsigned char byTypeCode, std::wstring& wstrType);

	/*!
	 * @brief 获取可运行方向码
	 * @param string 可运行方向
	 * @return unsigned char 运行方向
	*/
	static unsigned char GetDriveCode(std::string strDrive);

	/*!
	 * @brief 获取可运行方向码
	 * @param wstring 可运行方向
	 * @return unsigned char 运行方向
	*/
	static unsigned char GetDriveCode(std::wstring wstrDrive);

	/*!
	 * @brief 获取可运行方向信息
	 * @param unsigned char 可运行方向
	 * @param string& 运行方向信息
	*/
	static void GetDrive(unsigned char byDriveCode, std::string& strDriveMsg);

	/*!
	 * @brief 获取可运行方向信息
	 * @param unsigned char 可运行方向
	 * @param wstring& 运行方向信息
	*/
	static void GetDrive(unsigned char byDriveCode, std::wstring& wstrDriveMsg);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& AGV编号
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief 获取类型
	 * @param CString& AGV类型
	*/
	void GetType(CString& cstrType);

	/*!
	 * @brief 获取可移动方向
	 * @param CString& AGV可移动方向
	*/
	void GetDriver(CString& cstrDriver);

	/*!
	 * @brief 获取最大速度(m/min)
	 * @param CString& AGV最大速度(m/min)
	*/
	void GetMaxSpeed(CString& cstrSpeed);

	/*!
	 * @brief 获取当前电量(%)
	 * @param CString& AGV当前电量(%)
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetPower(CString& cstrPower, COLORREF& color);

	/*!
	 * @brief 获取当前速度(%)
	 * @param CString& AGV当前速度(%)
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetSpeed(CString& cstrSpeed, COLORREF& color);

	/*!
	 * @brief 获取当前实际速度(m/min)
	 * @param CString& AGV实际速度(m/min)
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetRealitySpeed(CString& cstrSpeed, COLORREF& color);

	/*!
	 * @brief 获取状态信息
	 * @param CString& AGV状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetStatus(CString& cstrStatus, COLORREF& color);

	/*!
	 * @brief 获取动作信息
	 * @param CString& AGV动作
	 * @param CString& AGV动作状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetAction(CString& cstrAction, CString& cstrStatus, COLORREF &color);

	/*!
	 * @brief 获取异常信息
	 * @param CString& AGV异常信息
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void GetError(CString& cstrError, COLORREF& color);

	/*!
	 * @brief 获取模式
	 * @param CString& 模式
	*/
	void GetMode(CString& cstrMode);

	/*!
	 * @brief 获取载货数量
	 * @param CString& 载货数量
	*/
	void GetCargo(CString& cstrCargo);

	/*!
	 * @brief 获取动作码
	 * @param CString 动作
	 * @return unsigned char 动作码
	*/
	static unsigned char GetActionCode(CString cstrAction);

	/*!
	 * @brief 获取动作
	 * @param unsigned char 动作码
	 * @param CString& 动作
	*/
	static void GetAction(unsigned char byActCode, CString& cstrAction);

	/*!
	 * @brief 获取状态码
	 * @param CString 状态
	 * @return unsigned char 状态码
	*/
	static unsigned char GetStatusCode(CString cstrStatus);

	/*!
	 * @brief 获取状态
	 * @param unsigned char 状态码
	 * @param CString& 状态
	*/
	static void GetStatus(unsigned char byStaCode, CString& cstrStatus);

	/*!
	 * @brief 获取异常码
	 * @param CString 异常信息
	 * @return char 异常码
	*/
	static char GetErrorCode(CString cstrError);

	/*!
	 * @brief 获取异常信息
	 * @param char 异常码
	 * @param CString& 异常信息
	*/
	static void GetErrorMessage(char chErrorCode, CString& cstrErrorMsg);

	/*!
	 * @brief 获取动作状态码
	 * @param CString 动作状态
	 * @return unsigned char 动作状态
	*/
	static unsigned char GetActionStatusCode(CString cstrActStatus);

	/*!
	 * @brief 获取动作状态
	 * @param unsigned char 动作状态码
	 * @param CString& 动作状态
	*/
	static void GetActionStatus(unsigned char byActStaCode, CString& cstrActStatus);

	/*!
	 * @brief 获取类型码
	 * @param CString& 类型
	 * @return unsigned char 类型码
	*/
	static unsigned char GetTypeCode(CString cstrType);

	/*!
	 * @brief 获取类型
	 * @param unsigned char 类型码
	 * @param CString& 类型
	*/
	static void GetType(unsigned char byTypeCode, CString& cstrType);

	/*!
	 * @brief 获取可运行方向码
	 * @param CString 可运行方向
	 * @return unsigned char 运行方向
	*/
	static unsigned char GetDriveCode(CString cstrDrive);

	/*!
	 * @brief 获取可运行方向信息
	 * @param unsigned char 可运行方向
	 * @param CString& 运行方向信息
	*/
	static void GetDrive(unsigned char byDriveCode, CString& cstrDriveMsg);
#endif // !_AFX
};

#endif //!_AGV_H