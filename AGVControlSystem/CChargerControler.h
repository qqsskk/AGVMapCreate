/*!
 * @file CChargerControler.h
 * @brief 基于TCP通信的充电控制器类文件
 *
 * 描述了充电控制器的基本属性信息
 * @author FanKaiyu
 * @date 2019-02-14
 * @version 1.0
*/

#pragma once
#include "CTCPClient.h"
#include "CProcessData.h"
#include <list>

#ifndef _CHARGERCONTROLER_H
#define _CHARGERCONTROLER_H

/*!
 * @class CTCPMNavAGV
 * @brief 基于TCP通信的充电控制器类
 *
 * 描述了充电控制器的基本属性信息
*/
class CChargerControler :
	public CTCPClient, CProcessData
{
public:
	CChargerControler(unsigned char byNo,std::string strIP,short sPort = -1);
	CChargerControler(unsigned char byNo, std::wstring wstrIP, short sPort = -1);
#ifdef _AFX
	CChargerControler(unsigned char byNo, CString cstrIP, short sPort = -1);
#endif
	~CChargerControler();
protected:
	unsigned char m_byNo;	/*!< 编号 */

protected:
	std::list<BufferString::Buffer> m_listSend;	/*!< 数据发送列表 */
	std::thread *m_pThreadSend;					/*!< 数据发送线程 */
	std::mutex m_mutexLock;						/*!< 互斥锁 */
	HANDLE m_hShutdownEvent;					/*!< 关闭事件 */

public:
	/*!
	 * @brief 设置编号
	 * @param unsigned char 充电控制器新编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char 充电控制器编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 获取编号
	 * @param string& 充电控制器编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& 充电控制器编号
	*/
	void GetNo(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 充电控制器编号
	*/
	void GetNo(CString& cstrNo);
#endif

public:
	/*!
	 * @brief 控制充电器充电
	 * @param unsigned char 充电器编号
	*/
	void Charge(unsigned char byNo);

	/*!
	 * @brief 控制充电器断电
	 * @param unsigned char 充电器编号
	*/
	void Uncharge(unsigned char byNo);

	/*!
	 * @brief 获取充电器状态
	 * @param unsigend char 充电器编号
	*/
	void GetChargerStatus(unsigned char byNo);

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
	void SendPackage(byte *byPackage, unsigned int unPackageLen);

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

#endif // !_CHARGERCONTROLER_H