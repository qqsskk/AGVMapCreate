#include "stdafx.h"
#include "CSerialPort.h"

#ifdef _SERIALPORT_H

CSerialPort::CSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits)
{
	m_usPort = usPort;
	m_unBaud = unBaud;
	m_chParity = chParity;
	m_byDataBits = byDataBits;
	m_byStopBits = byStopBits;

	m_hComm = nullptr;
	m_pThreadComm = nullptr;

	// 初始化事件
	m_hWriteEvent = nullptr;
	m_hShutdownEvent = nullptr;
	// 初始化异步操作成员变量
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	// 创建异步操作的事件成员变量
	m_ov.hEvent = nullptr;

	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;
}


CSerialPort::~CSerialPort()
{
	EndCommThread();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}

	if (m_ov.hEvent)
	{
		CloseHandle(m_ov.hEvent);
		m_ov.hEvent = nullptr;
	}

	if (m_hWriteEvent)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = nullptr;
	}
}

bool CSerialPort::OpenSerialPort()
{
	// 关闭线程
	EndCommThread();

	m_dwCommEvents = EV_RXFLAG | EV_RXCHAR;

	// 创建串口异步通信事件
	if (m_ov.hEvent)
	{
		ResetEvent(m_ov.hEvent);
	}
	else
	{
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE,NULL);
	}

	// 创建发送事件
	if (m_hWriteEvent)
	{
		ResetEvent(m_hWriteEvent);
	}
	else
	{
		m_hWriteEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	// 创建关闭事件
	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	}

	m_hEventArray[0] = m_hShutdownEvent; // 具有最高的优先级
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	char *szPort = new char[50];
	char *szBaud = new char[50];

	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = nullptr;
	}

	sprintf_s(szPort,50, "COM%d", m_usPort);
	sprintf_s(szBaud, 50, "baud=%d parity=%c data=%d stop=%d", m_unBaud,m_chParity,m_byDataBits,m_byStopBits);

	// 打开串口
	m_hComm = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		delete[] szPort;
		delete[] szBaud;

		return false;
	}

	// 超时参数
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// 设置串口参数
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		// 超时设置
		if (SetCommMask(m_hComm, m_dwCommEvents))
		{
			// 事件设置
			if (GetCommState(m_hComm, &m_dcb))
			{
				// 参数设置
				m_dcb.EvtChar = 'q';
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;

				if (BuildCommDCBA(szBaud, &m_dcb))
				{
					if (!SetCommState(m_hComm, &m_dcb))
					{
						ProcessErrorMessage("SetCommState()");
					}
				}
				else
				{
					// 串口参数设置失败
					ProcessErrorMessage("BuildCommDCBA()");
				}
			}
			else
			{
				// 串口参数获取失败
				ProcessErrorMessage("GetCommState()");
			}
		}
		else
		{
			// 串口事件设置失败
			ProcessErrorMessage("SetCommMask()");
		}
	}
	else
	{
		// 串口超时设置失败
		ProcessErrorMessage("SetCommTimeouts()");
	}

	delete[] szPort;
	delete[] szBaud;

	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	StartCommThread();

	return true;
}

bool CSerialPort::OpenSerialPort(unsigned short usPort, unsigned int unBaud, char chParity, unsigned char byDataBits, unsigned char byStopBits)
{
	m_usPort = usPort;
	m_unBaud = unBaud;
	m_chParity = chParity;
	m_byDataBits = byDataBits;
	m_byStopBits = byStopBits;

	return OpenSerialPort();
}

void CSerialPort::ClosePort()
{
	EndCommThread();

	return;
}

DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

bool CSerialPort::IsOpened()
{
	if (m_hComm)
	{
		return true;
	}

	return false;
}

bool CSerialPort::WriteToPort(char * lpszData, int nLen)
{
	if (m_hComm == nullptr)
	{
		return false;
	}

	m_WriteBuff = BufferString::Buffer((unsigned char*)lpszData, nLen);

	// 置发送事件为有的状态
	SetEvent(m_hWriteEvent);

	return true;
}

BufferString::Buffer CSerialPort::GetReadBuffer()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	BufferString::Buffer read = m_ReadBuff;
	m_ReadBuff.Clear();

	return read;
}

void CSerialPort::InsertData(char * pData, unsigned int unLen)
{
	std::lock_guard<std::mutex> m_lockGuard(m_mutexLock);

	m_ReadBuff = BufferString::Buffer((unsigned char*)pData, unLen) + m_ReadBuff;

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < m_ReadBuff.m_unSize; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", m_ReadBuff.m_pszBuffer[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CSerialPort.cpp/InsertData COM:%d TotalRead Data:%s.\nString:%s.", m_usPort, strDataRecv.c_str(), m_ReadBuff.m_pszBuffer);

	return;
}

void CSerialPort::StartCommThread()
{
	EndCommThread();

	m_pThreadComm = new std::thread(&CSerialPort::CommThread, this);

	return;
}

void CSerialPort::EndCommThread()
{
	if (m_pThreadComm)
	{
		if (m_pThreadComm->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadComm->join();
		}

		delete m_pThreadComm;
		m_pThreadComm = nullptr;
	}

	return;
}

void CSerialPort::CommThread()
{
	// 串口信息变量
	DWORD BytesTransfered = 0;
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL bResult = TRUE;

	// 强制关闭串口
	if (m_hComm)
	{
		// 清空串口缓冲
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	// 线程循环
	while (true)
	{
		bResult = WaitCommEvent(m_hComm, &Event, &m_ov);

		// 读取失败处理
		if (!bResult)
		{
			switch (dwError = GetLastError())
			{
			case ERROR_IO_PENDING:
				// 读取串口为空
				break;
			case 87:
				break;
			default:
				ProcessErrorMessage("WaitCommEvent()");
				break;
			}
		}
		else
		{
			// 清空串口错误
			bResult = ClearCommError(m_hComm, &dwError,&comstat);
			if (comstat.cbInQue == 0)
			{
				continue;
			}
		}

		// 获取串口事件并进行相应处理
		Event = WaitForMultipleObjects(3, m_hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
			// 关闭串口
			CloseHandle(m_hComm);
			m_hComm = nullptr;
			return;
		case 1:
			// 读串口
			GetCommMask(m_hComm, &CommEvent);
			if (CommEvent & EV_RXCHAR)
			{
				ReceiveChar(comstat);
			}
			break;
		case 2:
			// 写串口
			WriteChar();
			break;
		}
	}

	return;
}

void CSerialPort::ReceiveChar(COMSTAT comstat)
{
	BOOL bRead = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff[1500];

	// 读取串口数据
	while (true)
	{
		std::lock_guard<std::mutex> lock(m_mutexLock);
		bResult = ClearCommError(m_hComm, &dwError, &comstat);
		if (comstat.cbInQue == 0)
		{
			// 无数据可读
			continue;
		}

		if (bRead)
		{
			memset(RXBuff, 0, 1500);
			bResult = ReadFile(m_hComm, &RXBuff, 1500, &BytesRead, &m_ov);

			if (!bResult)
			{
				// 读取失败
				switch (dwError = GetLastError())
				{
					// 错误代码
				case ERROR_IO_PENDING:
					// 数据尚未读完
					bRead = FALSE;
					break;
				default:
					// 一般错误处理方法
					ProcessErrorMessage("ReadFile()");
					break;
				}
			}
			else
			{
				// 读取成功
				bRead = TRUE;
			}
		}

		if (!bRead)
		{
			// 读取失败处理
			bRead = TRUE;
			bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesRead, TRUE);

			if (!bResult)
			{
				ProcessErrorMessage("GetOverlappedResult() in ReadFile()");
			}
		}

		// 将接收的数据储存
		m_ReadBuff += BufferString::Buffer(RXBuff, BytesRead);

		std::string strDataRecv = "";
		for (unsigned int i = 0; i < BytesRead; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", RXBuff[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("AGVControlSystem/CSerialPort.cpp/ReceiveChar COM:%d Read Data:%s.\nString:%s.", m_usPort, strDataRecv.c_str(), RXBuff);

		strDataRecv = "";
		for (unsigned int i = 0; i < m_ReadBuff.m_unSize; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", m_ReadBuff.m_pszBuffer[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("----AGVControlSystem/CSerialPort.cpp/ReceiveChar COM:%d TotalRead Data:%s.\nString:%s.", m_usPort, strDataRecv.c_str(), m_ReadBuff.m_pszBuffer);
	}

	return;
}

void CSerialPort::WriteChar()
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	ResetEvent(m_hWriteEvent);
	
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (bWrite)
	{
		m_ov.Offset = 0;
		m_ov.OffsetHigh = 0;

		// 清空串口缓冲
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(m_hComm,m_WriteBuff.m_pszBuffer,m_WriteBuff.m_unSize,&BytesSent,&m_ov);

		if (!bResult)
		{
			// 写串口失败
			DWORD dwError = GetLastError();

			switch (dwError)
			{
			case ERROR_IO_PENDING:
				// 尚未写完数据
				BytesSent = 0;
				bWrite = FALSE;
				break;
			default:
				ProcessErrorMessage("WriteFile()");
				break;
			}
		}
	}

	if (!bWrite)
	{
		bWrite = TRUE;

		// 等待写动作完成
		bResult = GetOverlappedResult(m_hComm, &m_ov, &BytesSent, TRUE);

		if (!bResult)
		{
			ProcessErrorMessage("GetOverlappedResult() in WriteFile()");
		}
	}

	std::string strDataRecv = "";
	for (unsigned int i = 0; i < m_WriteBuff.m_unSize; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", m_WriteBuff.m_pszBuffer[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CSerialPort.cpp/WriteChar COM:%d Write Data:%s.\nString:%s.", m_usPort, strDataRecv.c_str(), m_WriteBuff.m_pszBuffer);

	// 发送失败
	if (BytesSent != m_WriteBuff.m_unSize)
	{
		DebugPrint("WARNING：WriteFile() error.. Bytes Sent：%d；Message Length：%d\n", BytesSent, m_WriteBuff.m_unSize);
	}

	return;
}

void CSerialPort::ProcessErrorMessage(char * ErrorText)
{
	char *Temp = new char[200];
	LPVOID lpMsgBuf; /*!< 保存错误信息的指针 */

	// 将GetLastError()得到的错误信息转化成字符串信息
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

	// 合成完整的错误信息
	sprintf_s(Temp, 200, "WARNING：%s Failed with the following error：\n%s\nPort：%d\n", ErrorText, (LPSTR)lpMsgBuf, m_usPort);

	// 提示错误信息
	DebugPrint(Temp);

	// 释放资源
	LocalFree(lpMsgBuf);
	delete[] Temp;

	return;
}

#endif // !_SERIALPORT_H