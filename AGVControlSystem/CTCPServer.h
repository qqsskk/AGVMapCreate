/*!
 * @file CTCPServer.h
 * @brief TCP服务端模块文件
 *
 * 用于创建TCP服务端的模块文件
 * @author FanKaiyu
 * @date 2018-12-24
 * @version 3.0
*/
#pragma once
#include <string>
#include <thread>
#include <list>
#include <mutex>

#ifndef _TCPSERVER_H
#define _TCPSERVER_H

/*!
 * @class CTCPServer
 * @brief TCP服务端模块文件
 *
 * 用于创建TCP服务端的模块文件
*/
class CTCPServer
{
public:
	/*!
	 * @brief  默认构造函数
	 * @param unsigned short 服务端端口
	 * @param wstring 服务端IP,默认为空
	*/
	CTCPServer(unsigned short usPort, std::wstring wstrIP = L"");

	/*!
	 * @brief  默认构造函数
	 * @param unsigned short 服务端端口
	 * @param string 服务端IP,默认为空
	*/
	CTCPServer(unsigned short usPort,std::string strIP = "");

#ifdef _AFX
	/*!
	 * @brief  默认构造函数
	 * @param unsigned short 服务端端口
	 * @param CString 服务端IP,默认为空
	*/
	CTCPServer(unsigned short usPort, CString cstrIP = _T(""));
#endif // !_AFX

	~CTCPServer();

protected:
	std::string m_strIP;			/*!< 绑定的服务端IP,为空时默认为当前网卡IP地址 */
	unsigned short m_usPort;		/*!< 绑定的服务端端口 */
	std::thread* m_pThreadListen;	/*!< 服务端监听线程指针 */
	std::list<SOCKET> m_listClient;	/*!< 客户端接入列表 */
	std::mutex m_mutexLock;			/*!< 线程互斥锁 */
	SOCKET m_socket;				/*!< 服务端SOCKET描述符 */

public:
	/*!
	 * @brief 服务端绑定IP地址
	 * @param wstring 服务端IP
	*/
	void SetIP(std::wstring wstrIP);

	/*!
	 * @brief 服务端绑定IP地址
	 * @param string 服务端IP
	*/
	void SetIP(std::string strIP);

#ifdef _AFX
	/*!
	 * @brief 服务端绑定IP地址
	 * @param CString 服务端IP
	*/
	void SetIP(CString cstrIP);
#endif // !_AFX
	
	/*!
	 * @brief 获取服务端绑定IP
	 * @return wstring 服务端IP
	*/
	void GetIP(std::wstring &wstrIP) const;

	/*!
	 * @brief 获取服务端绑定IP
	 * @return string 服务端IP
	*/
	void GetIP(std::string &strIP) const;

#ifdef _AFX
	/*!
	 * @brief 获取服务端绑定IP
	 * @return CString 服务端IP
	*/
	void GetIP(CString &cstrIP) const;
#endif // !_AFX

	/*!
	 * @brief 设置服务端端口
	 * @param unsigned short 服务端端口,应尽量保证端口不会被其他应用程序占用.
	*/
	void SetPort(unsigned short unPort);

	/*!
	 * @breif 获取服务端端口
	 * @return unsigned short 服务端端口
	*/
	unsigned short GetPort() const;

	/*!
	 * @brief 开始监听
	 * @return bool 监听开始返回true,失败返回false
	*/
	bool StartListen();

	/*!
	 * @brief 结束监听
	*/
	void EndListen();

	/*!
	 * @brief 获取服务端监听状态
	 * @return bool 监听中返回true,未监听返回false
	*/
	bool GetListenStatus() const;

	/*!
	 * @brief 获取接入客户端SOCKET描述符
	 * @return SOCKET 接入端SOCKET描述符,无客户端接入返回INVALID_SOCKET
	*/
	SOCKET GetClient();

	/*!
	 * @brief 获取Socket描述符中的IP地址,端口信息
	 * @param SOCKET socket描述符
	 * @param string IP地址
	 * @param unsigned short 端口
	 * @return bool 获取成功返回true,失败返回false
	*/
	static bool GetSocketAddress(SOCKET& socket,std::string& strIP,unsigned short& usPort);

	/*!
	 * @brief 获取Socket描述符中的IP地址,端口信息
	 * @param SOCKET socket描述符
	 * @param wstring IP地址
	 * @param unsigned short 端口
	 * @return bool 获取成功返回true,失败返回false
	*/
	static bool GetSocketAddress(SOCKET& socket, std::wstring& wstrIP, unsigned short& usPort);

#ifdef _AFX
	/*!
	 * @brief 获取Socket描述符中的IP地址,端口信息
	 * @param SOCKET socket描述符
	 * @param CString IP地址
	 * @param unsigned short 端口
	 * @return bool 获取成功返回true,失败返回false
	*/
	static bool GetSocketAddress(SOCKET& socket,CString& cstrIP, unsigned short& usPort);
#endif //_AFX

protected:
	/*!
	 * @brief 开启监听线程
	*/
	void OpenListenThread();

	/*!
	 * @brief 关闭监听线程
	*/
	void CloseListenThread();

	/*!
	 * @brief 监听线程
	*/
	void ListenThread();

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
	 * @brief 获取服务端监听状态
	 * @param string& 服务端监听状态
	*/
	void IsListened(std::string& strListen);

	/*!
	 * @brief 获取服务端监听状态
	 * @param wstring& 服务端监听状态
	*/
	void IsListened(std::wstring& wstrListen);

#ifdef _AFX
	/*!
	 * @brief 获取端口
	 * @param CString& 端口
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief 获取服务端监听状态
	 * @param CString& 服务端监听状态
	 * @param COLORREF& 用于提示用户的颜色
	*/
	void IsListened(CString& cstrListen, COLORREF& color);
#endif // !_AFX
};

#endif // !_TCPSERVER_H