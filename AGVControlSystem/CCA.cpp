#include "stdafx.h"
#include "CCA.h"

#ifdef _CA_H

std::list<std::string> CCA::g_listRecv;	
std::mutex CCA::g_lock;					

CCA::CCA(unsigned short usNo, std::string strIP, short sPort, std::string strText):CTCPClient(strIP, sPort)
{
	m_usNo = usNo;
	m_strText = strText;

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}

CCA::CCA(unsigned short usNo, std::wstring wstrIP, short sPort, std::wstring wstrText) : CTCPClient(wstrIP, sPort)
{
	m_usNo = usNo;
	BufferString::std_wstring_to_string(m_strText, wstrText);

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}

#ifdef _AFX
CCA::CCA(unsigned short usNo, CString cstrIP, short sPort, CString cstrText) : CTCPClient(cstrIP, sPort)
{
	m_usNo = usNo;
	BufferString::CString_to_std_string(m_strText, cstrText);

	m_unLoginCheck = 0;
	m_unDisTime = 0;
	m_pThreadHeart = nullptr;
	m_hShutdownEvent = nullptr;
	m_bHeart = false;
}
#endif // !_AFX

CCA::~CCA()
{
	Break();

	if (m_hShutdownEvent)
	{
		CloseHandle(m_hShutdownEvent);
		m_hShutdownEvent = nullptr;
	}
}

bool CCA::SetNo(unsigned short usNo)
{
	if (usNo > CA_MAX_CLIENT_ADDR)
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

unsigned short CCA::GetNo() const
{
	return m_usNo;
}

void CCA::GetNo(std::string & strNo)
{
	BufferString::std_string_format(strNo, "%d", m_usNo);

	return;
}

void CCA::GetNo(std::wstring & wstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_wstring(wstrNo, strNo);

	return;
}

bool CCA::SetText(std::string strText)
{
	if (m_strText == strText)
	{
		return false;
	}

	m_strText = strText;

	return true;
}

bool CCA::SetText(std::wstring wstrText)
{
	std::string strText = "";
	BufferString::std_wstring_to_string(strText, wstrText);

	return SetText(strText);
}

void CCA::GetText(std::string & strText)
{
	strText = m_strText;

	return;
}

void CCA::GetText(std::wstring & wstrText)
{
	BufferString::std_string_to_wstring(wstrText, m_strText);

	return;
}

#ifdef _AFX
void CCA::GetNo(CString & cstrNo)
{
	std::string strNo = "";
	GetNo(strNo);

	BufferString::std_string_to_CString(cstrNo, strNo);

	return;
}

bool CCA::SetText(CString cstrText)
{
	std::string strText = "";
	BufferString::CString_to_std_string(strText, cstrText);

	return SetText(strText);
}

void CCA::GetText(CString & cstrText)
{
	BufferString::std_string_to_CString(cstrText, m_strText);

	return;
}
#endif //!_AFX

bool CCA::Connect(SOCKET socket)
{
	CTCPClient::Connect(socket);

	StartThread();

	AskLogon();

	return true;
}

void CCA::Break()
{
	// 终止数据处理线程
	CloseThread();

	return;
}

bool CCA::IsConnected()
{
	if (m_pThreadHeart)
	{
		if (m_pThreadHeart->joinable())
		{
			return true;
		}
	}
	return CTCPClient::IsConnected();
}

std::string CCA::GetRecvMsg()
{
	std::string strText = "";

	if (g_listRecv.size() == 0)
	{
		return strText;
	}

	unsigned int unCall = 0;

	// 上锁
	g_lock.lock();

	try
	{
		strText = g_listRecv.front();
		g_listRecv.pop_front();
	}
	catch (std::exception& e)
	{
		DebugPrint("%s.", e.what());
	}

	// 解锁
	g_lock.unlock();

	return strText;
}

bool CCA::SendToWatch(unsigned char byWatch, std::string strText)
{
	if (byWatch > 200)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/SendToWatch CA:%d 发送信息至手表失败!无效的手表编号:%d.", m_usNo, byWatch);
		return false;
	}

	if (strText.length() > 40)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/SendToWatch CA:%d 发送信息至手表失败!过长的信息内容:%s(%d字节).", m_usNo, strText.c_str(),strlen(strText.c_str()));
		return false;
	}

	char lpszStr[48];

	// 初始化
	memset(lpszStr, ' ', 48);

	memcpy_s(lpszStr, 48, strText.c_str(), strText.length());

	lpszStr[47] = 0;

	/*!
	 * USB数据包合成
	*/
	BufferString::Buffer bfUsb; /*!< USB 数据 */

	bfUsb.m_unSize = 56 + 4;

	bfUsb.m_pszBuffer = new unsigned char[bfUsb.m_unSize];

	// 初始化数据包
	memset(bfUsb.m_pszBuffer, 0, bfUsb.m_unSize);

	bfUsb.m_pszBuffer[0] = 0x01;				/*!< 当前包数(TCP数据包) */
	bfUsb.m_pszBuffer[1] = 0x00;				/*!< 当前包数(TCP数据包) */
	bfUsb.m_pszBuffer[2] = 0x01;				/*!< 当前包数(TCP数据包) */
	bfUsb.m_pszBuffer[3] = 0x00;				/*!< 当前包数(TCP数据包) */
	bfUsb.m_pszBuffer[4] = 0x55;				/*!< 帧头 */
	bfUsb.m_pszBuffer[5] = 0xAA;				/*!< 帧头 */
	bfUsb.m_pszBuffer[6] = 0x01;				/*!< 地址 */
	bfUsb.m_pszBuffer[7] = CA_USBCMD_SEND;		/*!< 命令字 */
	bfUsb.m_pszBuffer[8] = 50;					/*!< 数据长度 */
	bfUsb.m_pszBuffer[9] = 0x00;				/*!< 信息机地址 */
	bfUsb.m_pszBuffer[10] = byWatch;			/*!< 信息机地址 */
	memcpy_s(&bfUsb.m_pszBuffer[11], 48, lpszStr, 48); /*!< 文本内容 */
	unsigned int unCheck = CreateCheck(bfUsb.m_pszBuffer + 6, bfUsb.m_unSize - 6); /*!< 校验 */
	bfUsb.m_pszBuffer[bfUsb.m_unSize - 1] = unCheck & 0xFF;

	/*!
	 * 合成TCP数据
	*/
	BufferString::Buffer package = CreatePackage(CA_TCPCMD_DATACOMM, m_usNo, bfUsb.m_pszBuffer, bfUsb.m_unSize); /*!< TCP数据包 */

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	return true;
}

bool CCA::SendToWatch(unsigned char byWatch, std::wstring wstrText)
{
	std::string strText = "";
	BufferString::std_wstring_to_string(strText, wstrText);
	return SendToWatch(byWatch, strText);
}

#ifdef _AFX
bool CCA::SendToWatch(unsigned char byWatch, CString cstrText)
{
	std::string strText = "";
	BufferString::CString_to_std_string(strText, cstrText);
	return SendToWatch(byWatch, strText);
}
#endif //!_AFX

void CCA::CreateLogonMessage(unsigned int & unLogin, unsigned int & unCheck)
{
	unLogin = 0;

	/*!
	 * 生成注册码
	*/
	// 初始化随机数发生器
	srand((unsigned)time(NULL));

	for (int i = 0; i < 8; i++)
	{
		unLogin += (unsigned int)((rand() % 0x0F) * pow(16.0, i));

		Sleep(1);
	}

	DebugPrint("AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d 生成注册码:%08X.", m_usNo, unLogin);

	/*!
	 * 生成注册校验码
	*/
	unCheck = unLogin ^ 0x3421bcda;

	DebugPrint("----AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d 阶段一:%08X.", m_usNo, unCheck);

	unCheck = ((unCheck & 0xFF000000) / 0x100) + ((unCheck & 0xFF0000) * 0x100) + ((unCheck & 0xFF00) / 0x100) + ((unCheck & 0xFF) * 0x100);

	DebugPrint("--------AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d 阶段二:%08X.", m_usNo, unCheck);

	unCheck ^= 0x4321abcd;

	DebugPrint("------------AGVControlSystem/CCA.cpp/CreateLogonMessage CA:%d 生成注册校验码:%08X.", m_usNo, unCheck);

	return;
}

void CCA::AskLogon()
{
	unsigned int unLogin = 0;
	CreateLogonMessage(unLogin, m_unLoginCheck);
	unsigned char aucLogin[4];
	memcpy_s(aucLogin, 4, &unLogin, 4);
	BufferString::Buffer package = CreatePackage(0x01, 0x0000, aucLogin, 4);

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	return;
}

unsigned short CCA::CreateCheck(unsigned char * pData, unsigned int unLen)
{
	unsigned int unSum = 0;

	for (unsigned int i = 0; i < unLen; i++)
	{
		unSum += pData[i];
	}

	unSum = ~unSum;

	return (unsigned short)(unSum & 0xFFFF);
}

BufferString::Buffer CCA::CreatePackage(unsigned char byCmd, unsigned short usTarget, unsigned char *pData, unsigned int unLen)
{
	unsigned int unPackageLen = 11 + unLen;
	unsigned char *pPackage = new unsigned char[unPackageLen];/*!< 数据包指针 */
	unsigned short usSrcAddr = CA_SERVER_ADDR;	/*!< 源地址 */
	// 初始化数据包
	memset(pPackage, 0, unPackageLen);

	pPackage[0] = 0x1b;									/*!< 报文头 */
	pPackage[unPackageLen - 1] = 0x1b;					/*!< 报文尾 */

	unsigned short usDataLen = unPackageLen - 4;		/*!< 数据长度 */
	memcpy_s(pPackage + 1, 2, &usDataLen, 2);
	memcpy_s(pPackage + 3, 2, &usTarget, 2);			/*!< 目的地址 */
	memcpy_s(pPackage + 5, 2, &usSrcAddr, 2);			/*!< 源地址 */
	pPackage[7] = byCmd;								/*!< 命令字 */
	memcpy_s(pPackage + 8, unLen, pData, unLen);		/*!< 数据包 */
	unsigned int unCheck = CreateCheck(pPackage + 1, unPackageLen - 4);	/*!< 校验 */
	pPackage[unPackageLen - 3] = unCheck & 0xFF;	
	pPackage[unPackageLen - 2] = (unCheck >> 8) & 0xFF;

	BufferString::Buffer package(pPackage, unPackageLen);
	delete pPackage;

	return Translate(package);
}

void CCA::HeartBeat(unsigned int unDisTime)
{
	unsigned int unTextLen = 14;
	char *lpszText = new char[unTextLen];
	sprintf_s(lpszText, unTextLen, "CA-Heart=%03dS", unDisTime);

	BufferString::Buffer package = CreatePackage(0x02, m_usNo, (unsigned char*)lpszText, unTextLen);

	SendData((char*)package.m_pszBuffer, package.m_unSize);

	delete lpszText;

	return;
}

BufferString::Buffer CCA::Translate(BufferString::Buffer &bfData)
{
	BufferString::Buffer bfTrans; /*!< 转义后的数据 */
	bfTrans.m_unSize = bfData.m_unSize * 2;

	bfTrans.m_pszBuffer = new unsigned char[bfTrans.m_unSize];

	// 初始化
	memset(bfTrans.m_pszBuffer, 0, bfTrans.m_unSize);

	// 帧头
	bfTrans.m_pszBuffer[0] = bfData.m_pszBuffer[0];

	int nT = 1;

	for (unsigned int nO = 1; nO < bfData.m_unSize - 1; nO++)
	{
		unsigned char ucData = bfData.m_pszBuffer[nO];

		if (ucData == 0x1a || ucData == 0x1b)
		{
			DebugPrint("AGVControlSystem/CCA.cpp/Analysis 转义.");
			bfTrans.m_pszBuffer[nT] = 0x1a;

			nT++;

			switch (ucData)
			{
			case 0x1a:
				bfTrans.m_pszBuffer[nT] = 0x01;
				break;
			case 0x1b:
				bfTrans.m_pszBuffer[nT] = 0x02;
				break;
			}
		}
		else
		{
			bfTrans.m_pszBuffer[nT] = ucData;
		}

		nT++;
	}

	// 帧尾
	bfTrans.m_pszBuffer[nT] = bfData.m_pszBuffer[bfData.m_unSize - 1];

	nT++;

	return BufferString::Buffer(bfTrans.m_pszBuffer, nT);
}

BufferString::Buffer CCA::Analysis(BufferString::Buffer &bfData)
{
	BufferString::Buffer bfOrigin; /*!< 原报文 */

	bfOrigin.m_unSize = bfData.m_unSize;

	bfOrigin.m_pszBuffer = new unsigned char[bfData.m_unSize];

	// 初始化
	memset(bfOrigin.m_pszBuffer, 0, bfOrigin.m_unSize);

	bfOrigin.m_pszBuffer[0] = bfData.m_pszBuffer[0];

	int nO = 1;

	for (unsigned int nR = 1; nR < bfData.m_unSize - 1; nR++, nO++)
	{
		unsigned char ucData = bfData.m_pszBuffer[nR];

		if (ucData == 0x1a)
		{
			DebugPrint("AGVControlSystem/CCA.cpp/Analysis 还原.");
			unsigned char ucNext = bfData.m_pszBuffer[nR + 1];

			switch (ucNext)
			{
			case 0x01:
				bfOrigin.m_pszBuffer[nO] = 0x1a;
				nR++;
				break;
			case 0x02:
				bfOrigin.m_pszBuffer[nO] = 0x1b;
				nR++;
				break;
			default:
				bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[nR];
			}
		}
		else
		{
			bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[nR];
		}
	}

	bfOrigin.m_pszBuffer[nO] = bfData.m_pszBuffer[bfData.m_unSize - 1];

	return bfOrigin;
}

void CCA::ProcessData(BufferString::Buffer & buffer)
{
	unsigned char *pData = buffer.m_pszBuffer;	/*!< 指向剩余数据的指针 */
	unsigned int unDataLen = buffer.m_unSize;	/*!< 剩余数据的字节数 */

	while (true)
	{
		/*!
		 * 查找报文头 0x1b
		 *
		 * 找不到报文头时可舍弃全部数据
		*/
		unsigned char *pHead = nullptr;				/*!< 指向报文头的指针 */
		pHead = (unsigned char*)memchr(pData, 0x1b, unDataLen);

		// 无报文头的情况
		if (pHead == nullptr)
		{
			pData = nullptr;
			break;
		}

		// 存在报文头的情况

		/*!
		 * 查找报文尾
		 *
		 * 找不到报文尾时保留报文头以后的数据
		*/
		unsigned char *pTail = nullptr; /*!< 指向报文尾的指针 */
		pTail = (unsigned char*)memchr(pHead + 1, 0x1b, buffer.m_unSize - (pHead - buffer.m_pszBuffer));

		// 无报文尾的情况
		if (pTail == nullptr)
		{
			pData = pHead;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
			break;
		}

		// 存在报文尾的情况

		/*!
		 * 对数据长度进行对比
		 *
		 * 数据长度不符时，保留报文尾数据
		*/
		unsigned int unLen = pTail - pHead - 3;	/*!< 数据长度 */
		unsigned int unPackageLen = 0; /*!< 报文长度 */
		memcpy_s(&unPackageLen, 2, pHead + 1, 2);

		// 数据长度不符的情况
		if (unLen != unPackageLen)
		{
			pData = pTail;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
			continue;
		}

		// 数据长度相符的情况

		/*!
		 * 对校验值进行对比
		 *
		 * 校验值不符时，舍弃头尾以及中间包含的数据
		*/
		unsigned short usCheck = CreateCheck(pHead + 1, unPackageLen); /*!< 数据校验值 */
		unsigned short usPackageCheck = 0; /*!< 报文校验值 */
		memcpy_s(&usPackageCheck, 2, pTail - 2, 2);

		// 校验值相符时
		if (usCheck == usPackageCheck)
		{
			ProcessPackage(pHead, pTail - pHead);
		}

		// 无剩余数据
		if (buffer.m_pszBuffer - pTail == buffer.m_unSize)
		{
			pData = nullptr;
			break;
		}
		// 还有未处理的数据
		else
		{
			pData = pTail + 1;
			unDataLen = buffer.m_unSize - (pData - buffer.m_pszBuffer);
		}
	}

	// 无剩余数据的情况
	if (pData == nullptr)
	{
		buffer.Clear();
	}
	else
	{
		buffer = BufferString::Buffer(pData, unDataLen);
	}

	return;
}

void CCA::ProcessPackage(unsigned char * pData, unsigned int unLen)
{
	BufferString::Buffer package = Analysis(BufferString::Buffer(pData, unLen));

	unsigned short usPackageLen = 0; /*!< 报文长度 */
	memcpy_s(&usPackageLen, 2, package.m_pszBuffer + 1, 2);

	unsigned short usTraget = 0; /*!< 目的地址 */
	memcpy_s(&usTraget, 2, package.m_pszBuffer + 3, 2);

	unsigned short usFromCA = 0; /*!< 源地址 = 主机号 */
	memcpy_s(&usFromCA, 2, package.m_pszBuffer + 5, 2);

	unsigned char byTCPCmd = *(package.m_pszBuffer + 7); /*!< 命令字 */

	// 注册信息回复
	if (byTCPCmd == CA_TCPCMD_LOGON)
	{
		unsigned int unLogin = 0; /*!< 注册码 */
		memcpy_s(&unLogin, 4, package.m_pszBuffer + 8, 4);

		DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d 注册信息回复.注册码:%08X.", m_usNo, unLogin);

		// 注册信息验证通过
		m_bHeart = true;

		DebugPrint("----AGVControlSystem/CCA.cpp/ProcessPackage CA:%d 开始发送心跳.",m_usNo);
	}
	// 心跳回复
	else if (byTCPCmd == CA_TCPCMD_HEARTBEAT)
	{
		DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d 心跳回复.", m_usNo);
	}
	// 从机回复
	else if (byTCPCmd == CA_TCPCMD_DATACOMM)
	{
		unsigned short usTotal = 0; /*!< 总包数 */
		memmove_s(&usTotal, 2, package.m_pszBuffer + 8, 2);

		unsigned short usCur = 0; /*!< 当前包数 */
		memmove_s(&usCur, 2, package.m_pszBuffer + 10, 2);

		unsigned short usDataLen = usPackageLen - 11;			/*!< 报文长度(USB部分) */
		unsigned char *pData = package.m_pszBuffer + 12;		/*!< 指向未处理的数据指针 */

		/*!
		 * 查找报文头(USB部分)
		*/
		unsigned char* pHead = nullptr; /*!< 指向报文头的指针 */
		pHead = (unsigned char*)memchr(pData, 0x55, usDataLen);

		// 未找到报文头的情况
		if (pHead == nullptr)
		{
			return;
		}

		// 存在报文头的情况

		// 报文头不完全匹配
		if (*(pHead + 1) != 0xAA)
		{
			return;
		}

		unsigned char byAddr = *(pHead + 2); /*!< 地址 */
		unsigned char byUsbCmd = *(pHead + 3); /*!< 命令字 */
		unsigned char byLen = *(pHead + 4); /*!< 数据长度(USB部分) */
		unsigned char* pUsbData = pHead + 5; /*!< 数据域 */
		unsigned char byPackageCheck = *(pHead + 5 + byLen); /*!< 报文校验值 */
		unsigned char byCheck = CreateCheck(pHead + 2, byLen + 3) & 0xFF; /*!< 数据校验值 */

		// 校验值不符合
		if (byCheck != byPackageCheck)
		{
			return;
		}

		// 监听数据上传
		if (byAddr == 0x02 && byUsbCmd == CA_USBCMD_UPLOAD_SEND)
		{
			unsigned char byStrLen = (*(pHead + 5) - '0') * 10 + (*(pHead + 6) - '0'); /*!< 文本长度 */

			if (byStrLen == 0 || byStrLen != byLen - 2)
			{
				return;
			}

			char *c = new char[byStrLen + 1];
			memset(c, 0, byStrLen + 1);
			memcpy(c, pHead + 7, byStrLen);

			std::string strText(c);

			// 上锁
			g_lock.lock();

			g_listRecv.push_back(strText);

			// 解锁
			g_lock.unlock();

			delete c;

			DebugPrint("AGVControlSystem/CCA.cpp/ProcessPackage CA:%d 接收信息:%s.\r\n", m_usNo, strText.c_str());
		}
	}

	return;
}

void CCA::StartThread()
{
	if (m_pThreadHeart != nullptr)
	{
		CloseThread();
	}

	// 初始化线程关闭事件
	if (m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(nullptr, true, false, nullptr);
	}

	m_pThreadHeart = new std::thread(&CCA::ProcessDataThread, this);

	return;
}

void CCA::CloseThread()
{
	if (m_pThreadHeart)
	{
		if (m_pThreadHeart->joinable())
		{
			SetEvent(m_hShutdownEvent);

			m_pThreadHeart->join();
		}

		delete m_pThreadHeart;
		m_pThreadHeart = nullptr;
	}

	return;
}

void CCA::ProcessDataThread()
{
	DebugPrint("--------AGVControlSystem/CCA.cpp/ProcessDataThread CA:%d 数据处理线程开始.--------", m_usNo);

#ifdef _WIN64
	unsigned long long ullHeartTime = 0;
#elif _WIN32
	unsigned long ulHeartTime = 0;
#endif

	while (true)
	{
		if (IsConnected() == false)
		{
			break;
		}

		BufferString::Buffer data = GetBuffer();
		if (data.m_unSize != 0)
		{
			ProcessData(data);
		}

		if (data.m_unSize != 0)
		{
			InsertData((char*)data.m_pszBuffer, data.m_unSize);
		}
	
#ifdef _WIN64

		if (m_bHeart && (ullHeartTime == 0 || GetTickCount64() - ullHeartTime >= m_unDisTime * 1000))
		{
			HeartBeat(m_unDisTime);
			ullHeartTime = GetTickCount64();
		}

		// 超过2次未接收到心跳数据
		unsigned long long ullRecvTime = 0;
		GetRecvTime(ullRecvTime);

		if (ullRecvTime != 0 && GetTickCount64() - ullRecvTime >= 3 * m_unDisTime * 1000)
		{
			CTCPClient::Disconnect();

			break;
		}
#elif _WIN32

		if (m_bHeart && (ulHeartTime == 0 || GetTickCount() - ulHeartTime >= m_unDisTime * 1000))
		{
			HeartBeat(m_unDisTime);
			ulHeartTime = GetTickCount();
		}

		// 超过2次未接收到心跳数据
		unsigned long ulRecvTime = 0;
		GetRecvTime(ulRecvTime);

		if (ulRecvTime != 0 && GetTickCount() - ulRecvTime >= 3 * m_unDisTime * 1000)
		{
			CTCPClient::Disconnect();

			break;
		}
#endif

		if (WaitForSingleObject(m_hShutdownEvent, 1000) == WAIT_OBJECT_0)
		{
			CTCPClient::Disconnect();

			break;
		}
	}

	m_bHeart = false;

	DebugPrint("--------AGVControlSystem/CCA.cpp/ProcessDataThread CA:%d 数据处理线程结束.--------", m_usNo);

	return;
}

#endif // !_CA_H