#pragma once
#include <map>

/*!
 * 必选属性
*/
#include "CMagneticTrackNavigation.h"
#include "CTCPMNavAGV.h"
#include "CMNavStation.h"
#include "CMNavRestPosition.h"
#include "CTCPServer.h"
#include "ADO/ADOConn.h"
#include "AES.h"

/*!
 * 可选属性
*/
#include "CMNavCharger.h"
#include "CCA.h"
#include "CChargerControler.h"

/*!
 * 业务属性
*/
#include "CTaskManager.h"

#ifndef _SERVICE_H
#define _SERVICE_H

#define _SELECT_FUNC			/*!< 全部可选功能 */
#define _SELECT_AUTO_GOUP		/*!< 自动提位 */

#ifdef _AFX
#define WM_SERVICE_UPDATE	WM_USER+3
#define ID_DATA_AGV 1
#endif //_AFX

class CService
{
public:
	CService();
	~CService();

protected:
	/*!
	 * 通用属性
	*/
	AES m_aes;															/*!< AES加密狗 */
	CTCPServer *m_pServer;												/*!< TCP服务端 */
	std::thread *m_pThreadFunc;											/*!< 功能线程指针 */
	HANDLE m_hShutdownEvent;											/*!< 关闭事件 */
	std::mutex m_mutexLock;												/*!< 互斥锁 */
	ADOConn m_adoDB;													/*!< ADO数据库链接 */
	bool m_bPermission;													/*!< 用户使用权限 */
#ifdef _AFX
	HWND m_hParent;														/*!< 窗口句柄 */
#endif // _AFC

#ifdef _SELECT_FUNC
protected:
#ifdef _CHARGERCONTROLER_H
	std::map<unsigned char, CChargerControler*> m_mapChargerCtrlers;	/*!< 充电控制器组 */
#endif // !_CHARGERCONTROLER_H

#ifdef _CA_H
	std::map<unsigned short, CCA*> m_mapCAs;							/*!< 无线呼叫主机(多嘴猫CA主机)组 */
	std::map<unsigned char, CAWatch*> m_mapCAWatchs;					/*!< 无线(多嘴猫)手表结构体组 */
#endif // !_CA_H

#ifdef _LED_H
	/*!< LED组 */
#endif // !_LED_H

#endif // !_SELECT_FUNC

protected:
	/*!
	 * 通用功能
	*/
	/*!
	 * @brief 下载配置参数
	*/
	void DownLoadConfig();

	/*!
	 * @brief 上传配置参数
	*/
	void UpLoadConfig();

	/*!
	 * @brief 载入XML中的配置参数
	*/
	void LoadXMLConfig();

	/*!
	 * @brief 载入数据库中的配置参数
	*/
	void LoadDatabaseConfig();

	/*!
	 * @brief 设备连接
	*/
	void Connect();

	/*!
	 * @brief 设备中断
	*/
	void Disconnect();

	/*!
	 * @brief 功能线程
	*/
	void FunctionThread();

	/*!
	 * @brief 启动线程
	*/
	void StartThread();

	/*!
	 * @brief 结束线程
	*/
	void EndThread();

	/*!
	 * @brief 销毁数据
	*/
	void DestroyData();

#ifdef _SELECT_FUNC
protected:
	/*!
	 * 通用可选功能
	*/

#ifdef _CA_H
	/*!
	 * @brief 处理呼叫信息
	*/
	void ProcessCallMsg();

	/*!
	 * @brief 信息通知
	 * @param string 信息文本
	 * @param unsigned char 工位编号
	*/
	void SendMsgToWatch(std::string strMsg,unsigned char byStation);

	/*!
	 * @brief 信息通知
	 * @param wstring 信息文本
	 * @param unsigned char 工位编号
	*/
	void SendMsgToWatch(std::wstring wstrMsg, unsigned char byStation);

#ifdef _AFX
	/*!
	 * @brief 信息通知
	 * @param CString 信息文本
	 * @param unsigned char 工位编号
	*/
	void SendMsgToWatch(CString cstrMsg, unsigned char byStation);
#endif // _AFX
#endif // !_CA_H

#ifdef _LED_H
	/*!
	 * @brief 输出文本至LED
	 * @param string 信息文本
	 * @param unsigned char LED编号
	*/
	void SendMsgToLED(std::string strMsg, unsigned char byLed);

	/*!
	 * @brief 输出文本至LED
	 * @param wstring 信息文本
	 * @param unsigned char LED编号
	*/
	void SendMsgToLED(std::wstring strMsg, unsigned char byLed);

#ifdef _AFX
	/*!
	 * @brief 输出文本至LED
	 * @param CString 信息文本
	 * @param unsigned char LED编号
	*/
	void SendMsgToLED(CString cstrMsg, unsigned char byLed);
#endif // _AFX

	/*!
	 * @brief LED显示
	*/

#endif // !_LED_H

#ifdef _CHARGERCONTROLER_H
	/*!
	 * @brief 处理充电控制器数据
	 *
	 * 控制充电控制器定时获取充电器状态
	 * 处理充电控制器返回的信息,并更新充电器状态
	*/
	void ProcessChargerControlerData();

#endif // !_CHARGERCONTROLER_H

#endif // !_SELECT_FUNC

public:
	/*!
	 * 数据更新
	*/

	/*!
	 * @brief AGV实时状态更新
	*/

	/*!
	 * @brief 设备状态更新
	*/

	/*!
	 * @brief 充电器状态更新
	*/

	/*!
	 * @brief 任务状态更新
	*/

public:
	/*!
	 * 程序控制
	*/
	bool Open();
#ifdef _AFX
	/*!
	 * @brief 启动
	 * @param HWND 父窗口指针
	*/
	bool Open(const HWND hParent);
#endif
	/*!
	 * @brief 关闭
	*/
	void Close();

	/*!
	 * @brief 销毁
	*/
	void Destroy();

	/*!
	 * @brief 管理员身份验证
	 * @param string 管理员密匙(明文)
	 * @return bool 验证成功返回true,失败返回false
	*/
	bool GetAdmin(std::string strPassword);

	/*!
	 * @brief 管理员身份验证
	 * @param wstring 管理员密匙(明文)
	 * @return bool 验证成功返回true,失败返回false
	*/
	bool GetAdmin(std::wstring wstrPassword);

#ifdef _AFX
	/*!
	 * @brief 管理员身份验证
	 * @param CString 管理员密匙(明文)
	 * @return bool 验证成功返回true,失败返回false
	*/
	bool GetAdmin(CString cstrPassword);
#endif //_AFX

	/*!
	 * @brief 管理员密码设置
	 * @param string 旧管理员密匙
	 * @param string 新管理员密匙
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAdmin(std::string strOldPassword,std::string strNewPassword);

	/*!
	 * @brief 管理员密码设置
	 * @param wstring 旧管理员密匙
	 * @param wstring 新管理员密匙
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAdmin(std::wstring wstrOldPassword, std::wstring wstrNewPassword);
#ifdef _AFX

	/*!
	 * @brief 管理员密码设置
	 * @param CSstring 旧管理员密匙
	 * @param CString 新管理员密匙
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetAdmin(CString cstrOldPassword, CString cstrNewPassword);
#endif //_AFX


#ifdef _MAGNETICTRACK_H
	/*!
	 * @brief 磁轨导航专用属性
	*/
protected:
	/*!
	 * 基础属性
	*/
	std::map<unsigned char, CTCPMNavAGV*> m_mapAGVs;				/*!< AGV组 */
	std::map<unsigned short, CRFID*> m_mapRFIDs;					/*!< RFID卡组 */
	std::map<unsigned char, CMNavStation*> m_mapStations;			/*!< 工位组 */
	std::map<unsigned char, CMNavRestPosition*> m_mapRests;			/*!< 待机位组 */
	std::map<unsigned short, CMTTrafficCtrlArea*> m_mapTraffic;		/*!< 交通管制组*/
	CTaskManager m_TaskManager;										/*!< 任务管理器 */

#ifdef _SELECT_FUNC
protected:
	/*!
	 * 可选属性
	*/
	std::map<unsigned char, CMNavCharger*> m_mapChargers;			/*!< 充电位组 */

#ifdef _CA_H
	std::map<unsigned short, CACall*> m_mapCACalls;					/*!< 无线(多嘴猫)呼叫器结构体组 */
#endif // _CA_H

#endif // !_SELECT_FUNC

protected:
	/*!
	 * 基础功能
	*/

	/*!
	 * @brief 交通管制
	*/
	void TrafficControl();

	/*!
	 * @brief 更新AGV状态
	 * @param unsigned char AGV编号
	 * @param unsigned char AGV当前电量
	 * @param char AGV当前速度
	 * @param unsigned char AGV当前状态
	 * @param char AGV异常信息
	 * @param unsigned short AGV动作
	 * @param unsigned char AGV动作状态
	 * @param unsigned char AGV载货状态
	 * @param unsigned short AGV当前坐标
	 * @param unsigned short AGV下一坐标
	*/
	void UpdateAGVStatus(unsigned char byNo,unsigned char byPower, char chSpeed, unsigned char byStatus, 
		char chError, unsigned short usAction, unsigned char byActStatus, unsigned char byCargo,
		unsigned short usRFID, unsigned short usNextRFID = 0);

	/*!
	 * @brief 更新AGV异常
	 * @param unsigned char AGV编号
	 * @param char AGV异常信息
	*/
	void UpdateAGVError(unsigned char byNo, char chError);

	/*!
	 * @brief 更新AGV RFID卡
	 * @param unsigned char AGV编号
	 * @param unsigned short AGV当前RFID卡编号
	*/
	void UpdateAGVRFID(unsigned char byNo, unsigned short usRFID);

#ifdef _TASK_H
protected:
	/*!
	 * 任务管理
	*/
	/*!
	 * @brief 任务处理
	*/
	void ProcessTask();

	/*!
	 * @brief 发布任务
	*/

	/*!
	 * @brief 更新任务
	*/

	/*!
	 * @brief 删除任务
	*/

	/*!
	 * @brief 执行任务
	*/

	/*!
	 * @brief 获取后续任务
	*/

	/*!
	 * @brief 获取新任务
	*/
#endif _TASK_H

#ifdef _SELECT_FUNC
protected:
	/*!
	 * 可选基础功能
	*/
#ifdef _SELECT_AUTO_GOUP
	/*!
	 * @brief 待机提位
	*/
	void RestPositionControl();

#endif // !_SELECT_AUTO_GOUP

#ifdef _CHARGER_H
	/*!
	 * @brief 自动充电
	*/
	void ChargerControl();

#endif // !_CHARGER_H

#ifdef _CA_H
	/*!
	 * @brief 呼叫任务
	 * @param unsigned int 呼叫器编号
	 * @param string 呼叫器中携带的内容信息
	*/
	void PublishCallTask(unsigned int unNo, std::string strMsg);

	/*!
	 * @brief 呼叫任务
	 * @param unsigned int 呼叫器编号
	 * @param wstring 呼叫器中携带的内容信息
	*/
	void PublishCallTask(unsigned int unNo, std::wstring strMsg);

#ifdef _AFX
	/*!
	 * @brief 呼叫任务
	 * @param unsigned int 呼叫器编号
	 * @param cstring 呼叫器中携带的内容信息
	*/
	void PublishCallTask(unsigned int unNo, CString strMsg);
#endif // _AFX
#endif // _CA_H

#endif // !_SELECT_FUNC

public:
	/*!
	 * @brief 用户操作
	*/

	/*!
	 * @brief 急停
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVScream(unsigned char byAGVNo);

	/*!
	 * @brief 复位
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVRestart(unsigned char byAGVNo);

	/*!
	 * @breif 移动
	 * @param unsigned char AGV编号
	 * @param unsigned short 移动终点坐标即RFID卡编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVMove(unsigned char byAGVNo,unsigned short usRFIDNo);

	/*!
	 * @brief 动作停止
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVStopAction(unsigned char byAGVNo);

	/*!
	 * @brief 上料
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVLoad(unsigned char byAGVNo);

	/*!
	 * @brief 下料
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVUnload(unsigned char byAGVNo);

	/*!
	 * @brief 其他动作
	 * @param unsigned char AGV编号
	 * @param unsigned short 动作码
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVAction(unsigned char byAGVNo, unsigned short usActionCode);

	/*!
	 * @brief 重置
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVReset(unsigned char byAGVNo);

	/*!
	 * @brief 设置当前坐标与状态
	 * @param unsigned char AGV编号
	 * @param unsigned short 当前坐标
	 * @param unsigned char 当前状态
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVSet(unsigned char byAGVNo, unsigned short usRFIDNo, unsigned char byStatus);

	/*!
	 * @brief 重新移动
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVRemove(unsigned char byAGVNo);

	/*!
	 * @brief 交通管制通行
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVTrafficPass(unsigned char byAGVNo);

	/*!
	 * @brief 手动模式
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVHandleMode(unsigned char byAGVNo);

	/*!
	 * @brief 自动模式
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVAutoMode(unsigned char byAGVNo);

	/*!
	 * @brief 关机
	 * @param unsigned char AGV编号
	 * @return bool 成功返回true,失败返回false
	*/
	bool AGVShutdown(unsigned char byAGVNo);

#ifdef _TASK_H
	/*!
	 * @brief 获取任务信息
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return AGVTask 任务结构体
	*/
	AGVTask GetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 修改任务信息
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @param AGVTask 任务结构体
	 * return bool 修改成功返回true，失败返回false
	*/
	bool EditTask(unsigned short usNo, unsigned char byLevel,AGVTask newTask);

	/*!
	 * @brief 取消任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool CancelTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 强制完成任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool FinishTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 暂停任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool PauseTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 重置任务
	 * @param unsigned short 任务号
	 * @param unsigned char 阶段号
	 * @return bool 取消成功返回true,失败返回false
	*/
	bool ResetTask(unsigned short usNo, unsigned char byLevel);

	/*!
	 * @brief 取消全部任务
	*/
	void CancelAllTasks();

	/*!
	 * @brief 暂停全部任务
	*/
	void PauseAllTasks();

	/*!
	 * @brief 重置全部任务
	*/
	void ResetAllTasks();

	/*!
	 * @brief 插入任务
	 *
	 * 将新任务插入至指定任务点前
	 * @param unsigned short 插入点任务号
	 * @param unsigned char 插入点阶段号
	 * @param AGVTask 新任务
	 * @return bool 插入成功返回true,失败返回false
	*/
	bool InsertTask(unsigned short usNo,unsigned char byLevel,AGVTask newTask);

	/*!
	 * @brief 添加任务
	 * @param AGVTask 新任务
	 * @return bool 添加成功返回true,失败返回false
	*/
	bool AddTask(AGVTask newTask);
#endif // _TASK_H

#endif // !_MAGNETICTRACK_H
};

#endif //!_SERVICE_H