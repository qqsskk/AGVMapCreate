#include "stdafx.h"
#include "CChargerControler.h"

#ifdef _CHARGERCONTROLER_H

CChargerControler::CChargerControler(unsigned char byNo, std::string strIP, short sPort):CTCPClient(strIP, sPort), CProcessData(0xd2)
{
	m_byNo = byNo;

	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}

CChargerControler::CChargerControler(unsigned char byNo, std::wstring wstrIP, short sPort) : CTCPClient(wstrIP, sPort), CProcessData(0xd2)
{
	m_byNo = byNo;

	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}

#ifdef _AFX
CChargerControler::CChargerControler(unsigned char byNo, CString cstrIP, short sPort) :CTCPClient(cstrIP, sPort), CProcessData(0xd2)
{
	m_byNo = byNo;

	m_pThreadSend = nullptr;
	m_hShutdownEvent = nullptr;
}
#endif

CChargerControler::~CChargerControler()
{
	Break();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}
}

bool CChargerControler::SetNo(unsigned char byNo)
{
	if (m_byNo == byNo)
	{
		return false;
	}

	m_byNo = byNo;

	return true;
}

unsigned char CChargerControler::GetNo() const
{
	return m_byNo;
}

void CChargerControler::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_byNo);

	return;
}

void CChargerControler::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

#ifdef _AFX
void CChargerControler::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}
#endif

void CChargerControler::Charge(unsigned char byNo)
{
}

void CChargerControler::Uncharge(unsigned char byNo)
{
}

void CChargerControler::GetChargerStatus(unsigned char byNo)
{
}

void CChargerControler::SendPackage(BufferString::Buffer bfPackage)
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	for (std::list<BufferString::Buffer>::iterator it = m_listSend.begin(); it != m_listSend.end(); it++)
	{
		if (*it == bfPackage)
		{
			return;
		}
	}

	if (m_listSend.max_size() == m_listSend.size())
	{
		m_listSend.pop_front();
	}

	m_listSend.push_back(bfPackage);

	return;
}

void CChargerControler::SendPackage(byte * byPackage, unsigned int unPackageLen)
{
	return SendPackage(BufferString::Buffer(byPackage, unPackageLen));
}

void CChargerControler::StartSendThread()
{
	if (m_pThreadSend && m_pThreadSend->joinable())
	{
		EndSendThread();
	}

	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_mutexLock.lock();

	m_listSend.clear();

	m_mutexLock.unlock();

	m_pThreadSend = new std::thread(&CChargerControler::SendThread, this);

	DebugPrint("AGVControlSystem/CChargerControler.cpp/StartSendThread AGV:%d IP:%s:%d 创建并开启数据发送线程.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
	TRACE(_T("[New]数据发送线程地址:%08X.\r\n"), m_pThreadSend);

	return;
}

void CChargerControler::EndSendThread()
{
	if (m_pThreadSend)
	{
		if (m_pThreadSend->joinable())
		{
			SetEvent(m_hShutdownEvent);

			DebugPrint("AGVControlSystem/CChargerControler.cpp/StartSendThread AGV:%d IP:%s:%d 等待数据发送线程结束.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
			m_pThreadSend->join();
		}

		DebugPrint("----AGVControlSystem/CChargerControler.cpp/StartSendThread AGV:%d IP:%s:%d 销毁数据发送线程.", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);
		TRACE(_T("[Delete]数据接收线程地址:%08X.\r\n"), m_pThreadSend);

		delete m_pThreadSend;
		m_pThreadSend = nullptr;
	}

	return;
}

void CChargerControler::SendThread()
{
	DebugPrint("--------AGVControlSystem/CChargerControler.cpp/SendThread AGV:%d IP:%s:%d 数据发送线程开始.--------", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);


	while (true)
	{
		if (IsConnected() == false)
		{
			break;
		}

		m_mutexLock.lock();

		std::list<BufferString::Buffer> listSend = m_listSend;
		m_listSend.clear();

		m_mutexLock.unlock();

		unsigned int unLift = 1000;	/*!< 剩余时间 */

		for (std::list<BufferString::Buffer>::iterator it = listSend.begin(); it != listSend.end(); ++it)
		{
			if (SendData((char*)(*it).m_pszBuffer, (*it).m_unSize) == false)
			{
				break;
			}

			if (WaitForSingleObject(m_hShutdownEvent, 100) == WAIT_OBJECT_0)
			{
				CTCPClient::Disconnect();

				goto Exit;
			}

			unLift -= 100;
		}

		if (unLift > 0)
		{
			if (WaitForSingleObject(m_hShutdownEvent, unLift) == WAIT_OBJECT_0)
			{
				CTCPClient::Disconnect();

				break;
			}
		}
	}

Exit:
	DebugPrint("--------AGVControlSystem/CChargerControler.cpp/SendThread CChargerControler:%d IP:%s:%d 退出数据发送线程.--------", m_byNo, m_strIP.c_str(), (unsigned short)m_sPort);

	return;
}

bool CChargerControler::Connect(SOCKET socket)
{
	if (CTCPClient::Connect(socket))
	{
		StartSendThread();

		return true;
	}

	return false;
}

bool CChargerControler::Connect(std::string strIP, unsigned short usPort)
{
	if (CTCPClient::Connect(strIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}

bool CChargerControler::Connect(std::wstring wstrIP, unsigned short usPort)
{
	if (CTCPClient::Connect(wstrIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}

#ifdef _AFX
bool CChargerControler::Connect(CString cstrIP, unsigned short usPort)
{
	if (CTCPClient::Connect(cstrIP, usPort))
	{
		StartSendThread();

		return true;
	}

	return false;
}
#endif // !_AFX

void CChargerControler::Break()
{
	EndSendThread();

	return;
}

bool CChargerControler::IsConnected() const
{
	if (m_pThreadSend && m_pThreadSend->joinable())
	{
		return true;
	}

	return CTCPClient::IsConnected();
}

void CChargerControler::IsConnected(std::string & strConnect)
{
	if (IsConnected())
	{
		strConnect = "已连接";
	}
	else
	{
		strConnect = "未连接";
	}

	return;
}

void CChargerControler::IsConnected(std::wstring & wstrConnect)
{
	std::string strConnect = "";
	IsConnected(strConnect);

	BufferString::std_string_to_wstring(wstrConnect, strConnect);

	return;
}

#ifdef _AFX
void CChargerControler::IsConnected(CString & cstrConnect, COLORREF & color)
{
	std::string strConnect = "";
	IsConnected(strConnect);

	BufferString::std_string_to_CString(cstrConnect, strConnect);

	if (IsConnected())
	{
		color = COLOR_GREEN;
	}
	else
	{
		color = COLOR_GRAY;
	}

	return;
}
#endif // !_AFX

#endif // !_CHARGERCONTROLER_H