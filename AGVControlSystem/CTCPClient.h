/*!
 * @file CTCPClient.h
 * @brief TCP客户端模块文件
 *
 * 用于创建TCP客户端的模块文件
 * @author FanKaiyu
 * @date 2018-12-24
 * @version 3.0
*/

#pragma once
#include <string>
#include <thread>
#include <mutex>
#include "Buffer.h"

#ifndef _TCPCLIENT_H
#define _TCPCLIENT_H

/*!
 * @class CTCPClient
 * @brief TCP客户端模块文件
 *
 * 用于创建TCP客户端的模块文件
*/
class CTCPClient
{
public:
	/*!
	 * @brief  默认构造函数
	 * @param wstring 客户端IP
	 * @param short 客户端端口,默认为-1即随机端口
	*/
	CTCPClient(std::wstring wstrIP,short sPort = -1);

	/*!
	 * @brief  默认构造函数
	 * @param string 客户端IP
	 * @param short 客户端端口,默认为-1即随机端口
	*/
	CTCPClient(std::string strIP, short sPort = -1);

#ifdef _AFX
	/*!
	 * @brief  默认构造函数
	 * @param CString 客户端IP
	 * @param short 客户端端口,默认为-1即随机端
	*/
	CTCPClient(CString cstrIP, short sPort = -1);
#endif // !_AFX
	
	~CTCPClient();

protected:
	std::string m_strIP;				/*!< 客户端绑定的IP地址 */
	short m_sPort;						/*!< 客户端绑定的端口 */
	std::thread *m_pThreadRecv;			/*!< 数据接收线程指针 */
	std::mutex m_mutexLockRecv;			/*!< 数据接收线程锁 */
	SOCKET m_socket;					/*!< 客户端SOCKET描述符 */
	BufferString::Buffer m_bufferRecv;	/*!< 用以接收数据的缓存区 */
#ifdef _WIN64
	unsigned long long m_ullRecvTime;	/*!< 接收数据的时间(系统运行时间) */
#elif _WIN32
	unsigned long m_ulRecvTime;			/*!< 接收数据的时间(系统运行时间) */
#endif // !_WIN64

public:
	/*!
	 * @brief 客户端绑定IP地址
	 * @param wstring 客户端绑定IP
	*/
	void SetIP(std::wstring wstrIP);

	/*!
	 * @brief 客户端绑定IP地址
	 * @param string 客户端绑定IP
	*/
	void SetIP(std::string strIP);

#ifdef _AFX
	/*!
	 * @brief 客户端绑定IP地址
	 * @param CString 客户端绑定IP
	*/
	void SetIP(CString cstrIP);
#endif // !_AFX

	/*!
	 * @brief 获取客户端绑定IP
	 * @return wstring 客户端绑定IP
	*/
	void GetIP(std::wstring &wstrIP) const;

	/*!
	 * @brief 获取客户端绑定IP
	 * @return string 客户端绑定IP
	*/
	void GetIP(std::string &strIP) const;

#ifdef _AFX
	/*!
	 * @brief 获取客户端绑定IP
	 * @return CString 客户端绑定IP
	*/
	void GetIP(CString &cstrIP) const;
#endif // !_AFX

	/*!
	 * @brief 设置客户端绑定端口
	 * @param short 客户端绑定端口,应尽量保证端口不会被其他应用程序占用.
	*/
	void SetPort(short sPort);

	/*!
	 * @breif 获取客户端绑定端口
	 * @return short 客户端绑定端口
	*/
	short GetPort() const;

	/*!
	 * @brief 连接客户端
	 * @param SOCKET 客户端socket描述符
	 * @return bool 连接成功返回true,失败返回false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief 连接服务端
	 * @param string 服务端IP地址
	 * @param unsigned short 服务端端口 
	 * @return bool 连接成功返回true,失败返回false
	*/
	bool Connect(std::string strIP, unsigned short usPort);

	/*!
	 * @brief 连接服务端
	 * @param wstring 服务端IP地址
	 * @param unsigned short 服务端端口
	 * @return bool 连接成功返回true,失败返回false
	*/
	bool Connect(std::wstring wstrIP, unsigned short usPort);

#ifdef _AFX
	/*!
	 * @brief 连接服务端
	 * @param CString 服务端IP地址
	 * @param unsigned short 服务端端口
	 * @return bool 连接成功返回true,失败返回false
	*/
	bool Connect(CString cstrIP, unsigned short usPort);
#endif // !_AFX

	/*!
	 * @brief 中断连接
	*/
	void Disconnect();

	/*!
	 * @brief 获取客户端连接状态
	 * @return bool 已连接返回true,未连接返回false
	*/
	bool IsConnected() const;

	/*!
	 * @brief 发送数据
	 * @param char* 数据块首地址
	 * @param unsigned int 数据库长度
	 * @return bool 发送成功返回true,失败返回false
	*/
	bool SendData(char *pData, unsigned int unLen);

	/*!
	 * @brief 插入数据
	 * 
	 * 将数据块插入数据缓冲区头部
	 * @param char* 数据块首地址
	 * @param unsigned int 数据块长度
	*/
	void InsertData(char *pData, unsigned int unLen);

	/*!
	 * @brief 添加数据
	 *
	 * 将数据块存入数据缓冲区尾部
	 * @param char* 数据块首地址
	 * @param unsigned int 数据库长度
	*/
	void AddData(char *pData, unsigned int unLen);

	/*!
	 * @brief 获取数据
	 *
	 * 将数据从数据缓冲区中提出，并清空数据缓冲区
	 * @return Buffer 数据块
	*/
	BufferString::Buffer GetBuffer();

#ifdef _WIN64
	/*!
	 * @brief 获取数据接收的时间(系统运行时间)
	 * @param unsigned long long 数据接收的时间(系统运行时间)
	*/
	void GetRecvTime(unsigned long long& ullRecvTime);
#elif _WIN32
	/*!
	 * @brief 获取数据接收的时间(系统运行时间)
	 * @param unsigned long 数据接收的时间(系统运行时间)
	*/
	void GetRecvTime(unsigned long& ulRecvTime);
#endif // !_WIN64

protected:
	/*!
	 * @brief 启动数据接收线程
	 * @return bool 启动成功返回true,失败返回false
	*/
	bool OpenDataRecvThread();

	/*!
	 * @brief 关闭数据接收线程
	*/
	void CloseDataRecvThread();

	/*!
	 * @brief 数据接收线程
	*/
	void DataRecvThread();

	/*!
	 * @brief 获取本地IP地址
	 * @param char* 本地IP地址字符串
	 * @return bool 成功返回true,失败返回false
	*/
	bool GetLocalIP(char* lpszIP) const;

public:
	/*!
	 * @brief 获取端口
	 * @param string& 端口
	*/
	void GetPort(std::string& strPort);

	/*!
	 * @brief 获取端口
	 * @param string& 端口
	*/
	void GetPort(std::wstring& wstrPort);

	/*! 
	 * @brief 获取客户端连接状态
	 * @param string& 客户端连接状态
	*/
	void IsConnected(std::string& strConnect);

	/*!
	 * @brief 获取客户端连接状态
	 * @param wstring& 客户端连接状态
	*/
	void IsConnected(std::wstring& wstrConnect);

#ifdef _AFX
	/*!
	 * @brief 获取端口
	 * @param CString& 端口
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief 获取客户端连接状态
	 * @param CString& 客户端连接状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void IsConnected(CString& cstrConnect, COLORREF& color);
#endif // !_AFX
};

#endif //!_TCPCLIENT_H