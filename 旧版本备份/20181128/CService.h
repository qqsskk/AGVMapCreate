/*!
 * @file CService.h
 * @brief CService类头文件
 * @author FanKaiyu
 * @date 2018-11-20
 * @version 3.0
*/

#pragma once
#include <thread>
#include <map>
#include <list>
#include <mutex>
#include "CTcpServer.h"
#include "CTcpClient.h"
#include "CCA.h"
#include "CArm.h"
#include "CPipeline.h"
#include "CStocker.h"
#include "LEDCtrl.h"
#include "Xml.h"
#include "ADOConn.h"
#include "Task.h"
#include "Cargo.h"

#ifndef _SERVICE_H
#define _SERVICE_H

/*!
 * @class CService
 * @brief 程序业务逻辑运行类
 *
 * 业务处理等操作
*/
class CService
{
public:
	CService();
	~CService();

protected:
	CTcpServer *m_pTcpServer;						/*!< 服务端 */
	list<CTcpClient*> m_listTcpClients;				/*!< 客户端连接列表 */
	CCA *m_pCA;										/*!< CA主机指针 */
	CArm *m_pArm;									/*!< 机械手指针 */
	CStocker *m_pStocker;							/*!< 堆垛机指针 */
	map<unsigned int, CPipeline*> m_mapPipeline;	/*!< 输送线组指针 */
	CLEDCtrl m_led;									/*!< LED显示器 */

protected:
	thread *m_pThreadService;						/*!< 业务线程指针 */
	HANDLE m_hShutdownEvent;						/*!< 关闭事件 */
	mutex m_lock;									/*!< 互斥锁 */
	ADOConn m_adoDB;								/*!< 数据库ADO连接串 */
	string m_strLed[10];							/*!< LED输出文本 */

public:
	unsigned int m_unNo;							/*!< 货架号 */
	unsigned int m_unRow;							/*!< 行号 */
	unsigned int m_unCol;							/*!< 列号 */

public:
	/*!
	 * @brief 开启业务处理
	*/
	bool Open();

	/*!
	 * @brief 关闭业务处理
	*/
	void Close();

	/*!
	 * @brief 自动出库
	 * @param unsigned int 货架号
	 * @param unsigned int 货架行
	 * @param unsigned int 货架列
	 * @return bool 出库成功返回true,失败返回false
	*/
	bool AutoGetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol);

	/*!
	 * @brief 出库
	 * @param unsigned int 货架号
	 * @param unsigned int 货架行
	 * @param unsigned int 货架列
	 * @return bool 出库成功返回true,失败返回false
	*/
	bool GetOut(unsigned int unNo, unsigned int unRow, unsigned int unCol);

	/*!
	 * @brief 入库
	 *
	 * 本项目入库采用手动入库的方式，即将货物人工放至货架上，在通过程序记录至数据库中
	 * @param unsigned int 货架号
	 * @param unsigned int 货架行
	 * @param unsigned int 货架列
	 * @return bool 入库成功返回true,失败返回false
	*/
	bool PutIn(unsigned int unNo, unsigned int unRow, unsigned int unCol);

protected:
	/*!
	 * @biref 读取配置信息
	 * @param wstring 配置文件路径
	*/
	void LoadConfig(wstring wstrPath);

	/*!
	 * @brief 客户端连接
	*/
	void ClientConnect();

	/*!
	 * @brief 清楚无用的TCP客户端链接
	*/
	void ClearFreeClient();

	/*!
	 * @brief 开启线程
	*/
	void StartThread();

	/*!
	 * @brief 关闭线程
	*/
	void CloseThread();

	/*!
	 * @brief 业务线程
	*/
	void ServiceThread();

	/*!
	 * @brief 任务处理
	*/
	void ProcessTask();

	/*!
	 * @brief 处理出库任务
	 * @param _WMSTASK& WMS任务结构体
	*/
	void ProcessTaskOut(_WMSTASK &task);

	/*!
	 * @brief 处理入库任务
	 * @param _WMSTASK& WMS任务结构体
	*/
	void ProcessTaskIn(_WMSTASK &task);

	/*!
	 * @brief 打开LED屏幕
	*/
	void OpenScreen();

	/*!
	 * @brief 关闭LED屏幕
	*/
	void CloseScreen();

	/*!
	 * @brief 发送文本至LED
	 * @param string 文本信息
	*/
	void SendToLED(string strText);

	/*!
	 * @brief 获取呼叫信息
	*/
	void GetCallMsg();
};

#endif // !_SERVICE_H