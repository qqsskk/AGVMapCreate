/*!
 * @file CSerialPort.h
 * @brief RS-232串口通信文件
 *
 * 用于与串口进行数据交互的模块
 * @author FanKaiyu
 * @date 2019-01-02
 * @version 2.0
*/
#pragma once
#include <thread>
#include <mutex>
#include "Buffer.h"

#ifndef _SERIALPORT_H
#define _SERIALPORT_H

/*!
 * @brief RS-232串口9针脚说明（DB9接头）
 * 1	CD（Carier Detect）			载波检测
 * 2	RXD（Recevie Data）			接收数据
 * 3	TXD（Transmit Data）		发送数据
 * 4	DTR（Data Terminal Ready）	数据终端就绪
 * 5	GND（System Ground）		系统接地
 * 6	DSR（Data Set Readys）		数据设备就绪
 * 7	RTS（Request To Send）		请求发送
 * 8	CTS（Clear To Send）		允许发送
 * 9	RI（Ring Indicator）		响铃指示器
*/

/*!
 * @brief 波特率常见常量
 * 值			winbase.h中的定义				说明
 * CBR_110		#define CBR_110		110			波特率为110
 * CBR_300		#define CBR_300		300			波特率为300
 * CBR_600		#define CBR_600		600			波特率为600
 * CBR_1200		#define CBR_1200	1200		波特率为1200
 * CBR_2400		#define CBR_2400	2400		波特率为2400
 * CBR_4800		#define CBR_4800	4800		波特率为4800
 * CBR_9600		#define CBR_9600	9600		波特率为9600
 * CBR_14400	#define CBR_14400	14400		波特率为14400
 * CBR_19200	#define CBR_19200	19200		波特率为19200
 * CBR_38400	#define CBR_38400	38400		波特率为38400
 * CBR_56000	#define CBR_56000	56000		波特率为56000
 * CBR_57600	#define CBR_57600	57600		波特率为57600
 * CBR_115200	#define CBR_115200	115200		波特率为115200
 * CBR_128000	#define CBR_128000	128000		波特率为128000
 * CBR_256000	#define CBR_256000	256000		波特率为256000
*/

/*!
 * @brief 奇偶校验常见常量
 * 值			winbase.h中的定义			说明
 * NOPARITY		#define NOPARITY	0		采用无校验方式
 * ODDPARITY	#define ODDPARITY	1		采用奇校验方式
 * EVENPARITY	#define EVENPARITY	2		采用偶校验方式
 * MARKPARITY	#define MARKPARITY	3		采用标记校验方式
*/

/*!
 * @brief 停止位常见常量
 * 值				winbase,h中的定义				说明
 * ONESTOPBIT		#define ONESTOPBIT		0		1位停止位
 * ONE5STOPBITS		#define ONE5STOPBITS	1		1.5位停止位
 * TWOSTOPBITS		#define TWOSTOPBITS		2		2位停止位
*/

/*!
 * @brief 清空缓冲区的值
 * 值				说明
 * PURGE_RXABORT	中断所有读操作并立即返回，即使读操作还没有完成
 * PURGE_TXABORT	中断所有写操作并立即返回，即使写操作还没有完成
 * PURGE_TXCLEAR	清楚输出缓冲区
 * PURGE_RXCLEAR	清楚输入缓冲区
*/

/*!
 * @class CSerialPort
 * @brief RS-232串口通信类
 *
 * 用于与串口进行数据交互的模块
*/
class CSerialPort
{
public:
	CSerialPort(unsigned short usPort,unsigned int unBaud,char chParity,unsigned char byDataBits,unsigned char byStopBits);
	~CSerialPort();
protected:
	/*!
	 * 串口的基本属性
	*/
	unsigned short m_usPort;		/*!< 串口号 */
	unsigned int m_unBaud;			/*!< 波特率 */
	char m_chParity;				/*!< 奇偶校验 */
	unsigned char m_byDataBits;		/*!< 数据位 */
	unsigned char m_byStopBits;		/*!< 停止位 */

protected:
	HANDLE m_hComm;					/*!< 串口操作句柄 */
	std::thread* m_pThreadComm;		/*!< 线程指针 */
	std::mutex m_mutexLock;			/*!< 线程互斥锁 */

	/*!
	 * 串口事件
	*/
	HANDLE m_hWriteEvent;				/*!< 写事件 */
	HANDLE m_hShutdownEvent;			/*!< 关闭事件 */
	HANDLE m_hEventArray[3];			/*!< 事件数组 */
	DWORD m_dwCommEvents;				/*!< 串口事件掩码 */
	OVERLAPPED m_ov;					/*!< 设置异步I/0操作 */
	COMMTIMEOUTS m_CommTimeouts;		/*!< 串口超时结构体 */
	DCB m_dcb;							/*!< 串口通信中的DCB结构 */
	BufferString::Buffer m_WriteBuff;	/*!< 写数据 */
	BufferString::Buffer m_ReadBuff;	/*!< 读数据 */

public:
	/*!
	 * @brief 打开串口
	 * @return bool 打开成功返回true,失败返回false
	*/
	bool OpenSerialPort();

	/*!
	 * @brief 打开指定串口
	 *
	 * 注:调用此函数会中断当前已打开的串口，并更改串口属性
	 * @param unsigned int 串口号
	 * @param unsigned int 波特率
	 * @param char 奇偶校验位
	 * @param unsigned char 数据位
	 * @param unsigned char 停止位
	 * @return bool 打开成功返回true,失败返回false
	*/
	bool OpenSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits);

	/*!
	 * @brief 关闭串口
	*/
	void ClosePort();

	/*!
	 * @brief 获取串口事件
	 * @return unsigned long 串口事件
	*/
	DWORD GetCommEvents();

	/*!
	 * @brief 获取串口配置信息
	 * @return DCB 串口配置信息
	*/
	DCB GetDCB();

	/*!
	 * @brief 判断串口是否打开
	 * @return bool 打开返回true,未打开返回false
	*/
	bool IsOpened();

	/*!
	 * @brief 获取并保存将要发送得数据，并触发发送事件
	 * @param char* 数据块首地址
	 * @param int 数据长度
	 * @return bool 数据保存成功返回true,数据保存失败返回false
	*/
	bool WriteToPort(char* lpszData, int nLen);

	/*!
	 * @brief 获取读取数据
	 * @return Buffer 获取串口读取的数据
	*/
	BufferString::Buffer GetReadBuffer();

	/*!
	 * @brief 插入数据
	 * 将数据块插入数据缓冲区头部
	 * @param char* 数据块首地址
	 * @param unsigned int 数据库长度
	*/
	void InsertData(char *pData, unsigned int unLen);

protected:
	/*!
	 * @brief 开启线程
	*/
	void StartCommThread();

	/*!
	 * @brief 关闭线程
	*/
	void EndCommThread();

	/*!
	 * @brief 串口线程处理
	*/
	void CommThread();

	/*!
	 * @brief 读取串口数据
	 * @param COMSTAT 串口状态
	*/
	void ReceiveChar(COMSTAT comstat);

	/*!
	 * @brief 向串口写入数据
	*/
	void WriteChar();

	/*!
	 * @brief 错误事件处理
	 * @param char* 错误信息
	*/
	void ProcessErrorMessage(char* ErrorText);
};



#endif //!_SERIALPORT_H