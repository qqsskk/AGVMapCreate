/*!
 * @file CCA.h
 * @brief 多嘴猫设备属性文件
 * 用于描述多嘴猫CA主机的基本属性
 * 用于描述多嘴猫无线呼叫器的基本属性
 * 用于描述多嘴猫无线手表的基本属性
 *
 * @author FanKaiyu
 * @date 2019-01-24
 * @version 2.1
*/
#pragma once
#include <string>
#include <list>
#include <mutex>
#include <map>
#include <thread>
#include "CTCPClient.h"
#include "Buffer.h"
#include "CAGV.h"
#include "CTaskManager.h"

#define CA_MAX_CLIENT_ADDR 0x8999	/*!< 客户端最大地址 */
#define CA_SERVER_ADDR 0x9999		/*!< 服务器地址 */
#define CA_TCPCMD_LOGON 0x01		/*!< 注册命令符 */
#define CA_TCPCMD_HEARTBEAT 0x02	/*!< 心跳命令符 */
#define CA_TCPCMD_DATACOMM 0x04		/*!< 数据通信命令符 */
#define CA_USBCMD_SEND 0xb1			/*!< 发送数据命令符 */
#define CA_USBCMD_UPLOAD_SEND 0xd1	/*!< 上传已发送的数据命令符 */
#define CA_USBCMD_UPLOAD_CALL 0xd2	/*!< 呼叫器特征码数据命令符 */
#define CA_USBCMD_UPLOAD_RECV 0xd4	/*!< 上传接收的数据命令符 */

#ifndef _CA_H
#define _CA_H

/*!
 * @class CCA
 * @brief 基于TCP通信的多嘴猫CA主机类
 *
 * 用于描述多嘴猫CA主机的基本属性
*/
class CCA
	:public CTCPClient
{
public:
	CCA(unsigned short usNo, std::string strIP,short sPort = 0, std::string strText = "");
	CCA(unsigned short usNo, std::wstring wstrIP, short sPort = 0, std::wstring wstrText = L"");
#ifdef _AFX
	CCA(unsigned short usNo, CString cstrIP, short sPort = 0, CString strText = _T(""));
#endif // !_AFX
	~CCA();
protected:
	unsigned short m_usNo;						/*!< CA主机号:在通信时为目的地址,取值范围为0-8999 */
	std::string m_strText;						/*!< 备注信息:通常用于描述此CA的位置以及实际作用 */

protected:
	unsigned int m_unLoginCheck;				/*!< 注册校验码 */
	unsigned int m_unDisTime;					/*!< 心跳时间间隔(s) */
	std::thread *m_pThreadHeart;				/*!< 心跳线程指针 */
	HANDLE m_hShutdownEvent;					/*!< 关闭事件 */
	bool m_bHeart;								/*!< 心跳发送标识 */

protected:
	static std::list<std::string> g_listRecv;	/*!< 已接收的信息列表 */
	static std::mutex g_lock;					/*!< 互斥锁 */

public:
	/*!
	 * @brief 设置主机编号
	 * @param unsigend short 新主机编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned short usNo);

	/*!
	 * @brief 获取主机编号
	 * @return unsigned short 主机编号
	*/
	unsigned short GetNo() const;

	/*!
	 * @brief 获取主机编号
	 * @param string 主机编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取主机编号
	 * @param wstring 主机编号
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief 设置备注信息
	 * @param string 主机新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(std::string strText);

	/*!
	 * @brief 设置备注信息
	 * @param wstring 主机新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(std::wstring wstrText);

	/*!
	 * @brief 获取备注信息
	 * @param string& 主机备注信息
	*/
	void GetText(std::string& strText);

	/*!
	 * @brief 获取备注信息
	 * @param wstring& 主机备注信息
	*/
	void GetText(std::wstring& wstrText);

#ifdef _AFX
	/*!
	 * @brief 获取主机编号
	 * @param CString 主机编号
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief 设置备注信息
	 * @param CString 主机新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(CString cstrText);

	/*!
	 * @brief 获取备注信息
	 * @param CString& 主机备注信息
	*/
	void GetText(CString& cstrText);
#endif //!_AFX

public:
	/*!
	 * @brief 连接设备
	 * @param SOCKET 客户端SOCKET描述符
	 * @return bool 连接成功返回true,失败返回false
	*/
	bool Connect(SOCKET socket);

	/*!
	 * @brief 中断与设备的连接
	*/
	void Break();

	/*!
	 * @brief 获取连接状态
	 * @return boll 已连接返回true,未连接返回false
	*/
	bool IsConnected();

	/*!
	 * @brief 获取接收的信息
	 * @return 从接收信息列表中返回1条信息
	*/
	static std::string GetRecvMsg();

	/*!
	 * @brief 向手表发送信息
	 * @param unsigned char 目标手表编号
	 * @param string 文本信息.文本信息应小于40个字节.
	 * @return bool 存入发送列表返回true,失败返回false
	*/
	bool SendToWatch(unsigned char byWatch, std::string strText);

	/*!
	 * @brief 向手表发送信息
	 * @param unsigned int 目标手表编号
	 * @param wstring 文本信息.文本信息应小于40个字节.
	 * @return bool 存入发送列表返回true,失败返回false
	*/
	bool SendToWatch(unsigned char byWatch, std::wstring wstrText);

#ifdef _AFX
	/*!
	 * @brief 向手表发送信息
	 * @param unsigned int 目标手表编号
	 * @param CString 文本信息.文本信息应小于40个字节.
	 * @return bool 存入发送列表返回true,失败返回false
	*/
	bool SendToWatch(unsigned char byWatch, CString cstrText);
#endif //!_AFX
protected:
	/*!
	 * @brief 合成注册码
	 * @param unsigned int 返回合成后的注册码
	 * @param unsigned int 返回合成后的注册码校验值
	*/
	void CreateLogonMessage(unsigned int& unLogin, unsigned int& unCheck);

	/*!
	 * @brief 请求注册
	*/
	void AskLogon();

	/*!
	 * @brief 合成校验码
	 * @param unsigned char* 需要校验的数据块
	 * @param unsigned int 需要校验的数据长度
	 * @return unsigned short 返回合成的校验信息
	*/
	unsigned short CreateCheck(unsigned char* pData, unsigned int unLen);

	/*!
	 * @brief 合成数据通信报文
	 * @param unsigned char 命令字
	 * @param unsigned short 目的地址
	 * @param unsigned char* 数据包指针
	 * @param unsigned int 数据包长度
	 * @return Buffer 合成后的报文包
	*/
	BufferString::Buffer CreatePackage(unsigned char byCmd, unsigned short usTarget, unsigned char *pData, unsigned int unLen);

	/*!
	 * @brief 心跳
	 *
	 * 发送心跳报文至CA主机
	 * @param unsigned int 心跳间隔时间 0-990
	*/
	void HeartBeat(unsigned int unDisTime);

	/*!
	 * @brief 报文转义
	 *
	 * 除数据头尾外
	 * 将报文中所有0x1A 转义为 0x1A + 0x01
	 * 将报文中所有0x1A 转义为 0x1A + 0x02
	 * @param Buffer& 需要转义的报文
	 * @return Buffer& 转义后的报文
	*/
	BufferString::Buffer Translate(BufferString::Buffer&);

	/*!
	 * @brief 报文反转义
	 *
	 * 除数据头尾外
	 * 将报文中所有0x1A + 0x01 转义为 0x1A
	 * 将报文中所有0x1A + 0x02 转义为 0x1B
	 * @param Buffer 需要反转义的报文
	 * @return Buffer 反转义后的报文
	*/
	BufferString::Buffer Analysis(BufferString::Buffer&);

	/*!
	 * @brief 数据处理
	 *
	 * 将报文从接收的数据中取出并将剩余的数据返回
	 * @param buffer 接收的数据
	 * @post buffer参数中应为解析后剩余的数据.
	*/
	void ProcessData(BufferString::Buffer &buffer);

	/*!
	 * @brief 报文处理
	 *
	 * 根据报文内容解析报文功能
	 * @param unsigned char* 报文数据指针
	 * @param unsigned int 报文长度
	*/
	void ProcessPackage(unsigned char* pData, unsigned int unLen);

	/*!
	 * @brief 开启线程
	*/
	void StartThread();

	/*!
	 * @brief 关闭线程
	*/
	void CloseThread();

	/*!
	 * @brief 数据处理线程
	 *
	 * 定时处理数据
	*/
	void ProcessDataThread();
};

/*! @brief 多嘴猫CA主机无线呼叫器结构体 */
typedef struct _CACallStruct
{
	unsigned short m_usNo;			/*!< 编号 */
	unsigned short m_usEndPoint;	/*!< 终点坐标 即AGV的动作终点,此参数为0时无效 */
	unsigned short m_usStartPoint;	/*!< 起点坐标 即AGV的动作起点,通常通过此参数选择有那个坐标上的AGV执行任务，此参数为0，则由调度系统选择AGV */
	char m_cType;					/*!< AGV类型 即选择此类型的AGV 此参数为0 则任意AGV均可 */
	std::string m_strMsg;			/*!< 携带信息 字符串信息，可用于显示也可以根据调度系统需求按照格式填写 */
	std::string m_strAction;		/*!< 任务动作 呼叫器绑定的任务动作 */

	_CACallStruct()
	{
		m_usNo = 0;
		m_usEndPoint = 0;
		m_usStartPoint = 0;
		m_cType = 0;
		m_strMsg = "";
	}

	_CACallStruct(unsigned short usNo,unsigned short usEndPoint,unsigned short usStartPoint = 0, char cType = 0,std::string strAction = _TASKACTION_A_MOVE,std::string strMsg = "")
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		m_strAction = strAction;
		m_strMsg = strMsg;
	}

	_CACallStruct(unsigned short usNo, unsigned short usEndPoint, unsigned short usStartPoint = 0, char cType = 0, std::wstring wstrAction = _TASKACTION_W_MOVE, std::wstring wstrMsg = L"")
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		BufferString::std_wstring_to_string(m_strAction, wstrAction);
		BufferString::std_wstring_to_string(m_strMsg, wstrMsg);
	}

#ifdef _AFX
	_CACallStruct(unsigned short usNo, unsigned short usEndPoint, unsigned short usStartPoint = 0, char cType = 0, CString cstrAction = _TASKACTION_MOVE, CString cstrMsg = _T(""))
	{
		m_usNo = usNo;
		m_usEndPoint = usEndPoint;
		m_usStartPoint = usStartPoint;
		m_cType = cType;
		BufferString::CString_to_std_string(m_strAction, cstrAction);
		BufferString::CString_to_std_string(m_strMsg, cstrMsg);
	}
#endif

	_CACallStruct(const _CACallStruct& call)
	{
		m_usNo = call.m_usNo;
		m_usEndPoint = call.m_usEndPoint;
		m_usStartPoint = call.m_usStartPoint;
		m_cType = call.m_cType;
		m_strMsg = call.m_strMsg;
	}

	void operator=(const _CACallStruct& call)
	{
		m_usNo = call.m_usNo;
		m_usEndPoint = call.m_usEndPoint;
		m_usStartPoint = call.m_usStartPoint;
		m_cType = call.m_cType;
		m_strMsg = call.m_strMsg;
	}

	/*!
	 * @brief 设置编号
	 * @param unsigned short 呼叫器新编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned short usNo)
	{
		if (usNo == 0)
		{
			return false;
		}

		if (m_usNo == usNo)
		{
			return false;
		}

		m_usNo = usNo;

		return true;
	}

	/*!
	 * @brief 获取编号
	 * @return unsigned short 呼叫器编号
	*/
	unsigned short GetNo() const
	{
		return m_usNo;
	}

	/*!
	 * @brief 获取编号
	 * @param string& 呼叫器编号
	*/
	void GetNo(std::string& strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_usNo);

		return;
	}

	/*!
	 * @brief 获取编号
	 * @param wstring& 呼叫器编号
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief 设置终点坐标
	 * @param unsigned short 终点新坐标
	 * @return bool 设置成功返回true,失败返回falsed
	*/
	bool SetEndPoint(unsigned short usEndPoint)
	{
		if (usEndPoint == 0)
		{
			return false;
		}

		if (m_usEndPoint == usEndPoint)
		{
			return false;
		}

		m_usEndPoint = usEndPoint;

		return true;
	}

	/*!
	 * @brief 获取终点坐标
	 * @return unsigned short 终点坐标
	*/
	unsigned short GetEndPoint() const
	{
		return m_usEndPoint;
	}

	/*!
	 * @brief 获取终点坐标
	 * @param string& 终点坐标
	*/
	void GetEndPoint(std::string& strEndPoint)
	{
		BufferString::std_string_format(strEndPoint, "%d", m_usEndPoint);

		return;
	}

	/*!
	 * @brief 获取终点坐标
	 * @param string& 终点坐标
	*/
	void GetEndPoint(std::wstring& wstrEndPoint)
	{
		std::string strEndPoint = "";
		GetEndPoint(strEndPoint);

		BufferString::std_string_to_wstring(wstrEndPoint, strEndPoint);

		return;
	}

	/*!
	 * @brief 设置起点坐标
	 * @param unsigned short 起点新坐标
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetStartPoint(unsigned short usStartPoint)
	{
		if (m_usStartPoint == usStartPoint)
		{
			return false;
		}

		m_usStartPoint = usStartPoint;

		return true;
	}

	/*!
	 * @brief 获取起点坐标
	 * @return unsigned short 起点坐标
	*/
	unsigned short GetStartPoint() const
	{
		return m_usStartPoint;
	}

	/*!
	 * @brief 获取起点坐标
	 * @param std::string& 起点坐标
	*/
	void GetStartPoint(std::string &strStartPoint)
	{
		if (m_usStartPoint == 0)
		{
			strStartPoint = "由调度系统分配AGV";
		}
		else
		{
			BufferString::std_string_format(strStartPoint, "%d", m_usStartPoint);
		}

		return;
	}

	/*!
	 * @brief 获取起点坐标
	 * @param std::wstring& 起点坐标
	*/
	void GetStartPoint(std::wstring &wstrStartPoint)
	{
		std::string strStartPoint = "";
		GetStartPoint(strStartPoint);

		BufferString::std_string_to_wstring(wstrStartPoint, strStartPoint);

		return;
	}

	/*!
	 * @brief 设置类型
	 * @param char 新AGV类型
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetType(char cType)
	{
		if (m_cType == cType)
		{
			return false;
		}

		switch (cType)
		{
		case 0:
			break;
		case _AGVTYPE_ARM:
			break;
		case _AGVTYPE_FORK:
			break;
		case _AGVTYPE_LIFT:
			break;
		case _AGVTYPE_PULL:
			break;
		case _AGVTYPE_SNEAK:
			break;
		case _AGVTYPE_TRAVEL:
			break;
		default:
			return false;
		}

		m_cType = cType;
		return true;
	}

	/*!
	 * @brief 获取类型
	 * @return char AGV类型
	*/
	char GetType() const
	{
		return m_cType;
	}

	/*!
	 * @brief 获取类型
	 * @param string& AGV类型
	*/
	void GetType(std::string& strType)
	{
		switch (m_cType)
		{
		case 0:
			strType = "全部类型";
			break;
		case _AGVTYPE_ARM:
			strType = "机械手式AGV";
			break;
		case _AGVTYPE_FORK:
			strType = "叉车式AGV";
			break;
		case _AGVTYPE_LIFT:
			strType = "举升式AGV";
			break;
		case _AGVTYPE_PULL:
			strType = "牵引式AGV";
			break;
		case _AGVTYPE_SNEAK:
			strType = "潜入式AGV";
			break;
		case _AGVTYPE_TRAVEL:
			strType = "移载式AGV ";
			break;
		}

		return;
	}

	/*!
	 * @brief 获取类型
	 * @param wstring& AGV类型
	*/
	void GetType(std::wstring& wstrType)
	{
		std::string strType = "";
		GetType(strType);

		BufferString::std_string_to_wstring(wstrType, strType);

		return;
	}

	/*!
	 * @brief 设置附加信息
	 * @param string 新附加信息
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetOverheadInfomation(std::string strMsg)
	{
		if (m_strMsg == strMsg)
		{
			return false;
		}

		m_strMsg = strMsg;

		return true;
	}

	/*!
	 * @brief 设置附加信息
	 * @param wstring 新附加信息
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetOverheadInfomation(std::wstring wstrMsg)
	{
		std::string strMsg = "";
		BufferString::std_wstring_to_string(strMsg, wstrMsg);

		return SetOverheadInfomation(strMsg);
	}

	/*!
	 * @brief 获取附加信息
	 * @param string& 附加信息
	*/
	void GetOverheadInfomation(std::string& strMsg)
	{
		strMsg = m_strMsg;

		return;
	}

	/*!
	 * @brief 获取附加信息
	 * @param wstring& 附加信息
	*/
	void GetOverheadInfomation(std::wstring& wstrMsg)
	{
		BufferString::std_string_to_wstring(wstrMsg, m_strMsg);

		return;
	}

	/*!
	 * @brief 设置动作
	 * @param string 新动作
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetAction(std::string strAction)
	{
		if (m_strAction == strAction)
		{
			return false;
		}

		m_strAction = strAction;

		return true;
	}

	/*!
	 * @brief 设置动作
	 * @param wstring 新动作
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetAction(std::wstring wstrAction)
	{
		std::string strAction = "";
		BufferString::std_wstring_to_string(strAction, wstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief 获取动作
	 * @param string& 动作
	*/
	void GetAction(std::string& strAction)
	{
		strAction = m_strAction;

		return;
	}

	/*!
	 * @brief 获取动作
	 * @param wstring& 动作
	*/
	void GetAction(std::wstring& wstrAction)
	{
		BufferString::std_string_to_wstring(wstrAction, m_strAction);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 呼叫器编号
	*/
	void GetNo(CString& cstrNo)
	{
		cstrNo.Format(_T("%d"), m_usNo);

		return;
	}

	/*!
	 * @brief 获取终点坐标
	 * @param CString& 终点坐标
	*/
	void GetEndPoint(CString& cstrEndPoint)
	{
		cstrEndPoint.Format(_T("%d"), m_usEndPoint);

		return;
	}

	/*!
	 * @brief 获取起点坐标
	 * @param CString& 起点坐标
	*/
	void GetStartPoint(CString &cstrStartPoint)
	{
		std::string strStartPoint = "";
		GetStartPoint(strStartPoint);

		BufferString::std_string_to_CString(cstrStartPoint, strStartPoint);

		return;
	}

	/*!
	 * @brief 获取类型
	 * @param CString& AGV类型
	*/
	void GetType(CString& cstrType)
	{
		std::string strType = "";
		GetType(strType);

		BufferString::std_string_to_CString(cstrType, strType);

		return;
	}

	/*!
	 * @brief 设置附加信息
	 * @param CString 新附加信息
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetOverheadInfomation(CString cstrMsg)
	{
		std::string strMsg = "";
		BufferString::CString_to_std_string(strMsg, cstrMsg);

		return SetOverheadInfomation(strMsg);
	}

	/*!
	 * @brief 获取附加信息
	 * @param CString& 附加信息
	*/
	void GetOverheadInfomation(CString& cstrMsg)
	{
		BufferString::std_string_to_CString(cstrMsg, m_strMsg);

		return;
	}

	/*!
	 * @brief 设置动作
	 * @param CString 新动作
	 * @return bool 设置成功返回trrue,失败返回false
	*/
	bool SetAction(CString cstrAction)
	{
		std::string strAction = "";
		BufferString::CString_to_std_string(strAction, cstrAction);

		return SetAction(strAction);
	}

	/*!
	 * @brief 获取动作
	 * @param wstring& 动作
	*/
	void GetAction(CString& cstrAction)
	{
		BufferString::std_string_to_CString(cstrAction, m_strAction);

		return;
	}
#endif
}CACall;

/*! @brief 多嘴猫CA主机无线手表结构体*/
typedef struct _CAWatchStruct
{
	unsigned char m_byNo;
	unsigned short m_usCA;
	unsigned char m_byStation;

	_CAWatchStruct()
	{
		m_byNo = 0;
		m_usCA = 0;
		m_byStation = 0;
	}

	_CAWatchStruct(unsigned char byNo, unsigned short usCA = 0, unsigned char byStation = 0)
	{
		m_byNo = byNo;
		m_usCA = usCA;
		m_byStation = byStation;
	}

	_CAWatchStruct(const _CAWatchStruct& watch)
	{
		m_byNo = watch.m_byNo;
		m_usCA = watch.m_usCA;
		m_byStation = watch.m_byStation;
	}

	void operator=(const _CAWatchStruct& watch)
	{
		m_byNo = watch.m_byNo;
		m_usCA = watch.m_usCA;
		m_byStation = watch.m_byStation;
	}

	/*!
	 * @brief 设置编号
	 * @param unsigned char 手表新编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo)
	{
		if (byNo == 0)
		{
			return false;
		}

		if (m_byNo == byNo)
		{
			return false;
		}

		m_byNo = byNo;

		return true;
	}

	/*!
	 * @brief 获取编号
	 * @return unsigned char 手表编号
	*/
	unsigned char GetNo() const
	{
		return m_byNo;
	}

	/*!
	 * @brief 获取编号
	 * @param string& 手表编号
	*/
	void GetNo(std::string& strNo)
	{
		BufferString::std_string_format(strNo, "%d", m_byNo);

		return;
	}

	/*!
	 * @brief 获取编号
	 * @param wstring& 手表编号
	*/
	void GetNo(std::wstring& wstrNo)
	{
		std::string strNo = "";
		GetNo(strNo);

		BufferString::std_string_to_wstring(wstrNo, strNo);

		return;
	}

	/*!
	 * @brief 设置受控CA编号
	 * @param unsigned short 新CA编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetControlCA(unsigned short usCA)
	{
		if (m_usCA == usCA)
		{
			return false;
		}

		m_usCA = usCA;

		return true;
	}

	/*!
	 * @brief 获取受控CA编号
	 * @return unsigned short CA编号
	*/
	unsigned short GetControlCA() const
	{
		return m_usCA;
	}

	/*!
	 * @brief 获取受控CA编号
	 * @param string& CA编号
	*/
	void GetControlCA(std::string& strCA)
	{
		if(m_usCA == 0)
		{ 
			strCA = "全部主机";
		}
		else
		{
			BufferString::std_string_format(strCA, "%d", m_usCA);
		}

		return;
	}

	/*!
	 * @brief 获取受控CA编号
	 * @param wstring& CA编号
	*/
	void GetControlCA(std::wstring& wstrCA)
	{
		std::string strCA = "";
		GetControlCA(strCA);

		BufferString::std_string_to_wstring(wstrCA, strCA);

		return;
	}

	/*!
	 * @brief 设置工位
	 * @param unsigned char 手表新工位
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetStation(unsigned char byStation)
	{
		if (m_byStation == byStation)
		{
			return false;
		}

		m_byStation = byStation;

		return true;
	}

	/*!
	 * @brief 获取工位
	 * @return unsigned char 工位
	*/
	unsigned char GetStation() const
	{
		return m_byStation;
	}

	/*!
	 * @brief 获取工位
	 * @param string& 工位
	*/
	void GetStation(std::string& strStation)
	{
		if (m_byStation == 0)
		{
			strStation = "全部工位";
		}
		else
		{
			BufferString::std_string_format(strStation, "%d", m_byStation);
		}
		
		return;
	}

	/*!
	 * @brief 获取工位
	 * @param wstring& 工位
	*/
	void GetStation(std::wstring& wstrStation)
	{
		std::string strStation = "";
		GetStation(strStation);

		BufferString::std_string_to_wstring(wstrStation, strStation);

		return;
	}

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 手表编号
	*/
	void GetNo(CString& cstrNo)
	{
		cstrNo.Format(_T("%d"), m_byNo);

		return;
	}

	/*!
	 * @brief 获取受控CA编号
	 * @param CString& 手表编号
	*/
	void GetControlCA(CString& cstrCA)
	{
		std::string strCA = "";
		GetControlCA(strCA);

		BufferString::std_string_to_CString(cstrCA, strCA);

		return;
	}

	/*!
	 * @brief 获取工位
	 * @param CString& 手表编号
	*/
	void GetStation(CString& cstrStation)
	{
		std::string strStation = "";
		GetStation(strStation);

		BufferString::std_string_to_CString(cstrStation, strStation);

		return;
	}
#endif 
}CAWatch;

#endif // !_CA_H