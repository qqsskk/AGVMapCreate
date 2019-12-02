/*!
 * @file CAGV.h
 * @brief 基于TCP通信、磁导航的AGV属性文件
 *
 * 用于创建磁导航AGV的模块文件
 * @author FanKaiyu
 * @date 2018-11-10
 * @version 2.0
*/

#pragma once
#include "CAGV.h"
#include "CMagneticTrackNavigation.h"
#include "CTCPClient.h"
#include "CProcessData.h"

#ifdef _AGV_H

/*!
 * @class CTCPMNavAGV
 * @brief 基于TCP通信、磁导航的AGV
 *
 * 定义了磁导航AGV的可执行动作
*/
class CTCPMNavAGV :
	public CAGV,public CTCPClient,public CMagneticTrackNavigation, public CProcessData
{
public:
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::string strIP,  short sPort = -1, unsigned char byDevType = 0xd1);
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, std::wstring wstrIP,  short sPort = -1, unsigned char byDevType = 0xd1);
#ifdef _AFX
	CTCPMNavAGV(unsigned char byNo, unsigned char byType, unsigned char byDriver, float fMaxSpeed, CString cstrIP,  short sPort = -1, unsigned char byDevType = 0xd1);
#endif // !_AFX
	~CTCPMNavAGV();

protected:
	std::list<BufferString::Buffer> m_listSend;	/*!< 数据发送列表 */
	std::thread *m_pThreadSend;					/*!< 数据发送线程 */
	std::mutex m_mutexLock;						/*!< 互斥锁 */
	HANDLE m_hShutdownEvent;					/*!< 关闭事件 */

protected:
	/*!
	 * @brief 发送数据包
	 *
	 * 将数据包存入数据包发送列表
	 * @param Buffer 数据包
	*/
	void SendPackage(BufferString::Buffer bfPackage);

	/*!
	 * @brief 发送数据包
	 *
	 * 将数据包存入数据包发送列表
	 * @param byte* 数据包首地址
	 * @param unsigend int 数据包长度
	*/
	void SendPackage(byte *byPackage,unsigned int unPackageLen);

	/*!
	 * @brief 启动发送线程
	*/
	void StartSendThread();

	/*!
	 * @brief 关闭发送线程
	*/
	void EndSendThread();

	/*!
	 * @brief 发送线程
	*/
	void SendThread();

	/*!
	 * @brief 心跳
	*/
	void Heartbeat();

public:
	/*!
	 * @brief 移动
	 * @param unsigned short 终止RFID卡编号
	*/
	void MoveTo(unsigned short usRFID);

	/*!
	 * @brief 交通管制放行
	*/
	void TrafficPass();

	/*!
	 * @brief 交通管制停止
	 * @param unsigned short 管制RFID卡编号
	*/
	void TrafficStop(unsigned short usRFID);

	/*!
	 * @brief 上料
	*/
	void Load();

	/*!
	 * @brief 下料
	*/
	void Unload();

	/*!
	 * @brief 急停
	*/
	void Scream();

	/*!
	 * @brief 复位
	*/
	void Restart();

	/*!
	 * @brief 重置
	*/
	void Reset();

	/*!
	 * @brief 重新移动
	 * @param unsigned short 终止RFID卡编号
	*/
	void Remove(unsigned short usRFID);

	/*!
	 * @brief 停止动作
	*/
	void StopAction();

	/*!
	 * @brief 动作控制
	 * @param unsigned short 动作码
	*/
	void Action(unsigned short usAction);

	/*!
	 * @brief 动作控制
	 *
	 * 远程控制AGV执行动作的报文指令
	 * @param unsigned short 当前RFID卡编号信息
	 * @param unsigned short 执行动作的RFID卡编号信息
	 * @param unsigned short 动作码
	 * @param unsigend char 动作状态
	*/
	void Action(unsigned short usCurRFID,unsigned short usRFID, unsigned short usAction, unsigned char byActionStatus = _AGVACTSTA_NONE);

	/*!
	 * @brief 关机
	 *
	 * 远程控制AGV断电关机的报文指令
	*/
	void Shutdown();

	/*!
	 * @brief 手动设置
	 *
	 * 设置AGV当前RFID卡编号、状态信息
	 * @param unsigned short 当前RFID卡编号信息
	 * @param unsigned char 状态信息
	*/
	void ManualSet(unsigned short usRFID,unsigned char byStatus);

public:	
	/*!
	 * @brief 连接客户端
	 * @param SOCKET 客户端socket描述符
	 * @return bool 连接成功返回ture,失败返回false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief 连接服务端
	 * @param string 服务端IP
	 * @param unsigned short 端口
	 * @return bool 连接成功返回ture,失败返回false
	*/
	bool Connect(std::string strIP, unsigned short usPort);

	/*!
	 * @brief 连接服务端
	 * @param wstring 服务端IP
	 * @param unsigned short 端口
	 * @return bool 连接成功返回ture,失败返回false
	*/
	bool Connect(std::wstring wstrIP, unsigned short usPort);

#ifdef _AFX
	/*!
	 * @brief 连接服务端
	 * @param CString 服务端IP
	 * @param unsigned short 端口
	 * @return bool 连接成功返回ture,失败返回false
	*/
	bool Connect(CString cstrIP, unsigned short usPort);
#endif // !_AFX

	/*!
	 * @brief 中断连接
	*/
	void Break();

	/*!
	 * @brief 获取连接状态
	 * @return bool 已连接返回true,未连接返回false
	*/
	bool IsConnected() const;

	/*!
	 * @brief 获取连接状态
	 * @param string& 连接状态
	*/
	void IsConnected(std::string& strConnect);

	/*!
	 * @brief 获取连接状态
	 * @param wstring& 连接状态
	*/
	void IsConnected(std::wstring& wstrConnect);

#ifdef _AFX
	/*!
	 * @brief 获取客户端连接状态
	 * @param CString& 客户端连接状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void IsConnected(CString& cstrConnect, COLORREF& color);
#endif // !_AFX
};

#endif // !_AGV_H