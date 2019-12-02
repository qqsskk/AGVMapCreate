#include "stdafx.h"
#include "CProcessData.h"
#include "CRC.h"

#ifdef _PROCESSDATA_H

CProcessData::CProcessData(byte byType)
{
	m_byType = byType;
}

CProcessData::~CProcessData()
{
}

BufferString::Buffer CProcessData::Transfer(BufferString::Buffer bfSource)
{
	return Transfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::Transfer(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";	/*!< 源数据 */
	std::string strDataTrans = "";	/*!< 转义数据 */
	std::string strDataHex = "";	/*!< 数据16进制字符串 */

	byte *byData = new byte[unLen * 2];	/*!< 转义数据块 */

	// 初始化数据块
	memset(byData, 0, unLen * 2);

	// 储存报文头
	byData[0] = bySource[0];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	int nCount = 1;	/*!< 转义数据位置 */

	// 数据转义
	for (unsigned int i = 1; i < unLen-1; i++,nCount++)
	{
		// 获取16进制数据字符串
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xB0)
		{
			byData[nCount] = bySource[i];

			nCount++;
			byData[nCount] = 0x01;

			strDataTrans += "B0 01 ";
		}
		else if (bySource[i] == 0xBA)
		{
			byData[nCount] = 0xB0;

			nCount++;
			byData[nCount] = 0x02;

			strDataTrans += "B0 02 ";
		}
		else if (bySource[i] == 0xBB)
		{
			byData[nCount] = 0xB0;

			nCount++;
			byData[nCount] = 0x03;

			strDataTrans += "B0 03 ";
		}
		else
		{
			byData[nCount] = bySource[i];

			// 获取16进制数据字符串
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataTrans += strDataHex;
		}
	}

	// 储存报文尾
	byData[nCount] = bySource[unLen - 1];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/Transfer 源数据:%s.字符串:%s.\n转义后数据:%s.字符串:%s.", strDataSource.c_str(), bySource, strDataTrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::Retransfer(BufferString::Buffer bfSource)
{
	return Retransfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::Retransfer(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";		/*!< 源数据 */
	std::string strDataRetrans = "";	/*!< 反转义数据 */
	std::string strDataHex = "";		/*!< 数据16进制字符串 */

	byte *byData = new byte[unLen];	/*!< 反转义数据块 */

	// 初始化数据块
	memset(byData, 0, unLen);

	// 储存报文头
	byData[0] = bySource[0];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	int nCount = 1;	/*!< 反转义数据位置 */

	// 数据反转义
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// 获取16进制数据字符串
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xB0)
		{
			i++;
			switch (bySource[i])
			{
			case 0x01:
				byData[nCount] = 0xB0;

				strDataRetrans += "B0 ";
				break;
			case 0x02:
				byData[nCount] = 0xBA;

				strDataRetrans += "BA ";
				break;
			case 0x03:

				byData[nCount] = 0xBB;

				strDataRetrans += "BB ";
				break;
			}
		}
		else
		{
			byData[nCount] = bySource[i];

			// 获取16进制数据字符串
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataRetrans += strDataHex;
		}
	}

	// 储存报文尾
	byData[nCount] = bySource[unLen - 1];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/Retransfer 源数据:%s.字符串:%s.\n反转义后数据:%s.字符串:%s.", strDataSource.c_str(), bySource, strDataRetrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

std::list<BufferString::Buffer> CProcessData::CreatePackge(BufferString::Buffer bfSource, unsigned char byNo)
{
	return CreatePackge(bfSource.m_pszBuffer, bfSource.m_unSize, byNo);
}

std::list<BufferString::Buffer> CProcessData::CreatePackge(byte * bySource, unsigned int unLen, unsigned char byNo)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	unsigned int unMaxLen = 500;		/*!< 报文最大长度 */
	unsigned int unDefaultLen = 14;		/*!< 默认报文长度 */

	std::list<BufferString::Buffer> listPackage;

	unsigned int unTotalPackage = unLen / (unMaxLen - unDefaultLen);	/*!< 总数据包数 */
	unsigned int unLiftLen = unLen % (unMaxLen - unDefaultLen);			/*!< 剩余数据长度 */

	if (unLiftLen > 0)
	{
		unTotalPackage++;
	}

	unsigned short usGroup = 0;			/*!< 组码 */
	usGroup = CRC16(bySource, unLen);

	for (unsigned int i = 0; i < unTotalPackage; i++)
	{
		unsigned int unDataLen = unMaxLen - unDefaultLen;		/*!< 数据长度 */

		if (i == unTotalPackage - 1)
		{
			if (unLiftLen > 0)
			{
				unDataLen = unLiftLen;
			}	
		}

		unsigned int unPackageLen = unDataLen + unDefaultLen;	/*!< 数据包总长 */

		int nIndex = 0;	/*!< 下标 */

		byte *byPackage = new byte[unPackageLen];	/*!< 报文数据 */
		// 初始化
		memset(byPackage, 0, unPackageLen);

		byPackage[nIndex] = 0xBA;													/*!< 报文头 */
		byPackage[++nIndex] = (unPackageLen >> 8) & 0xFF;							/*!< 数据长度(高) */
		byPackage[++nIndex] = unPackageLen & 0xFF;									/*!< 数据长度(低) */
		byPackage[++nIndex] = (usGroup >> 8) & 0xFF;								/*!< 组码(高) */
		byPackage[++nIndex] = usGroup & 0xFF;										/*!< 组码(低) */
		byPackage[++nIndex] = (unTotalPackage >> 8) & 0xFF;							/*!< 总包数(高) */
		byPackage[++nIndex] = unTotalPackage & 0xFF;								/*!< 总包数(低) */
		byPackage[++nIndex] = ((i + 1) >> 8) & 0xFF;								/*!< 当前包号(高) */
		byPackage[++nIndex] = (i + 1) & 0xFF;										/*!< 当前包号(低) */
		byPackage[++nIndex] = m_byType;												/*!< 设备类型 */
		byPackage[++nIndex] = byNo;													/*!< 设备编号 */

		memcpy_s(&byPackage[++nIndex], unPackageLen, bySource + (i * (unMaxLen - unDefaultLen)), unDataLen);	/*!< 数据体 */

		unsigned short usCRC16 = CRC16(byPackage, unPackageLen-3);					/*!< 校验码 */
		byPackage[unPackageLen-3] = (usCRC16 >> 8) & 0xFF;							/*!< 校验位(高) */
		byPackage[unPackageLen-2] = usCRC16 & 0xFF;									/*!< 校验位(低) */
		byPackage[unPackageLen-1] = 0xBB;											/*!< 报文尾 */

		std::string strDataRecv = "";
		for (unsigned int d = 0; d < unPackageLen; d++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", byPackage[d]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("AGVControlSystem/CProcessData.cpp/CreatePackge 报文数据:%s.字符串:%s.", strDataRecv.c_str(), byPackage);

		// 转义
		listPackage.push_back(Transfer(byPackage, unPackageLen));
		delete[] byPackage;
	}

	return listPackage;
}

unsigned int CProcessData::Sum(BufferString::Buffer bfSource)
{
	return Sum(bfSource.m_pszBuffer, bfSource.m_unSize);
}

unsigned int CProcessData::Sum(byte * bySource, unsigned int unLen)
{
	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/Sum 源数据:%s.字符串：%s.", strDataRecv.c_str(), bySource);

	int nSum = 0;

	for (unsigned int i = 0; i < unLen; i++)
	{
		nSum += bySource[i];
	}

	nSum = ~nSum;

	DebugPrint("----AGVControlSystem/CProcessData.cpp/Sum 校验码:%04X.十进制:%d.", nSum, nSum);

	return nSum;
}

unsigned short CProcessData::CRC16(BufferString::Buffer bfSource)
{
	return CRC::CRC16(bfSource.m_pszBuffer, bfSource.m_unSize);
}

unsigned short CProcessData::CRC16(byte * bySource, unsigned int unLen)
{
	std::string strDataRecv = "";
	for (unsigned int i = 0; i < unLen; i++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/CRC16 源数据:%s.字符串：%s.", strDataRecv.c_str(), bySource);

	return CRC::CRC16(bySource, unLen);
}

std::list<BufferString::Buffer> CProcessData::Process(BufferString::Buffer bfSource, BufferString::Buffer & bfResidue)
{
	return Process(bfSource.m_pszBuffer, bfSource.m_unSize, bfResidue);
}

std::list<BufferString::Buffer> CProcessData::Process(byte * bySource, unsigned int unLen, BufferString::Buffer & bfResidue)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	std::list<BufferString::Buffer> m_listPackage;	/*!< 报文集合 */

	byte* byResidue = bySource;			/*!< 剩余数据块 */
	unsigned int unResidueLen = unLen;	/*!< 剩余数据块大小 */

	while (byResidue)
	{
		byte* byHead = (byte*)memchr(byResidue, 0xBA, unResidueLen);	/*!< 报文头 */

		// 未在数据块中找到报文头
		if (byHead == nullptr)
		{
			byResidue = nullptr;
			unResidueLen = 0;
			break;
		}

		byResidue = byHead;
		unResidueLen = unLen - (byResidue - bySource);

		byte* byTail = (byte*)memchr(byResidue, 0xBB, unResidueLen);	/*!< 报文尾 */

		// 未在数据块中找到报文头
		if (byTail == nullptr)
		{
			break;
		}

		// 报文尾已经在数据块末尾
		if (byTail == &bySource[unLen - 1])
		{
			byResidue = nullptr;
			unResidueLen = 0;
		}
		else
		{
			byResidue = byTail + 1;
			unResidueLen = unLen - (byResidue - bySource);
		}

		BufferString::Buffer bfTrans = Retransfer(byHead, (byTail - byHead) + 1);		/*!< 反转义数据 */

		unsigned short usCRC16 = CRC16(byHead, (byTail - byHead) - 1);	/*!< CRC校验码 */
		unsigned short usCheck = *(byTail - 2) << 8 | *(byTail - 1);	/*!< 报文内校验值 */

		DebugPrint("AGVControlSystem/CProcessData.cpp/Process 设备类型校验:%04X:%04X.", m_byType, *(byHead + 6));

		// 设备类型校验
		if (*(byHead + 6) != m_byType)
		{
			continue;
		}

		DebugPrint("----AGVControlSystem/CProcessData.cpp/Process 数据校验:%04X:%04X.", usCRC16, usCheck);

		// 校验码校验
		if (usCRC16 != usCheck)
		{
			continue;
		}

		/*!
		 * 筛除完全相同的报文
		*/
		bool bSame = false;
		for (std::list<BufferString::Buffer>::iterator it = m_listPackage.begin(); it != m_listPackage.end();it++)
		{
			if (*it == bfTrans)
			{
				bSame = true;
				break;
			}
		}

		if (bSame)
		{
			continue;
		}

		m_listPackage.push_back(bfTrans);

		std::string strDataRecv = "";
		for (unsigned int i = 0; i < bfTrans.m_unSize; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", bfTrans.m_pszBuffer[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("--------AGVControlSystem/CProcessData.cpp/Process 报文包数据:%s.字符串:%s.", strDataRecv.c_str(), byHead);
	}

	if (unResidueLen > 0)
	{
		bfResidue = BufferString::Buffer(byResidue, unResidueLen);
	}
	else
	{
		bfResidue = BufferString::Buffer();
	}

	return m_listPackage;
}

std::list<BufferString::Buffer> CProcessData::Joint(std::list<BufferString::Buffer> bfList, std::list<BufferString::Buffer>& bfLiftList)
{
	std::list<BufferString::Buffer> listFull;
	std::list<JointBuffer> listJoint;

	for (std::list<BufferString::Buffer>::iterator it = bfList.begin();it != bfList.end();)
	{
		BufferString::Buffer& bfPackage = *it;		/*!< 报文包 */
		int nIndex = 3;				/*!< 下标 */

		unsigned int unGroup = (bfPackage.m_pszBuffer[nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];		/*!< 组码 */
		unsigned int unTotal = (bfPackage.m_pszBuffer[++nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];	/*!< 总包数 */
		unsigned int unCur = (bfPackage.m_pszBuffer[++nIndex] << 8) | bfPackage.m_pszBuffer[++nIndex];		/*!< 当前包号 */

		BufferString::Buffer bfData(&bfPackage.m_pszBuffer[++nIndex], bfPackage.m_unSize - 12);	/*!< 数据体 */

		// 单个数据包
		if (unTotal == unCur && unTotal == 1)
		{
			listFull.push_back(bfData);
		}
		// 多个数据包
		else
		{
			// 数据接收不完全
			if (unTotal > bfList.size())
			{
				break;
			}

			bool bJoint = false;

			for (std::list<JointBuffer>::iterator itJoint = listJoint.begin(); itJoint != listJoint.end();)
			{
				JointBuffer& pJoint = *itJoint;

				if (pJoint.AddJointData(bfPackage))
				{
					bJoint = true;

					BufferString::Buffer bfFull;
					if (pJoint.IsFull(bfFull))
					{
						listFull.push_back(bfFull);
					}

					break;
				}
			}

			if (bJoint == false)
			{
				JointBuffer joint(unGroup, unTotal);

				joint.AddJointData(bfPackage);

				listJoint.push_back(joint);
			}
		}

		it = bfList.erase(it);
	}

	bfLiftList = bfList;

	return listFull;
}

/*-------------------------------------处理、解析路径报文-----------------------------------------------*/
BufferString::Buffer CProcessData::TransferPath(BufferString::Buffer bfSource)
{
	return TransferPath(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::TransferPath(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";	/*!< 源数据 */
	std::string strDataTrans = "";	/*!< 转义数据 */
	std::string strDataHex = "";	/*!< 数据16进制字符串 */

	byte *byData = new byte[unLen * 2];	/*!< 转义数据块 */

	// 初始化数据块
	memset(byData, 0, unLen * 2);

	// 储存报文头
	byData[0] = bySource[0];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	int nCount = 1;	/*!< 转义数据位置 */

	// 数据转义
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// 获取16进制数据字符串
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xA0)
		{
			byData[nCount] = bySource[i];

			nCount++;
			byData[nCount] = 0x01;

			strDataTrans += "A0 01 ";
		}
		else if (bySource[i] == 0xAA)
		{
			byData[nCount] = 0xA0;

			nCount++;
			byData[nCount] = 0x02;

			strDataTrans += "A0 02 ";
		}
		else if (bySource[i] == 0xAB)
		{
			byData[nCount] = 0xA0;

			nCount++;
			byData[nCount] = 0x03;

			strDataTrans += "A0 03 ";
		}
		else
		{
			byData[nCount] = bySource[i];

			// 获取16进制数据字符串
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataTrans += strDataHex;
		}
	}

	// 储存报文尾
	byData[nCount] = bySource[unLen - 1];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataTrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/TransferPath 源数据:%s.字符串:%s.\n转义后数据:%s.字符串:%s.", strDataSource.c_str(), bySource, strDataTrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::RetransferPath(BufferString::Buffer bfSource)
{
	return Retransfer(bfSource.m_pszBuffer, bfSource.m_unSize);
}

BufferString::Buffer CProcessData::RetransferPath(byte * bySource, unsigned int unLen)
{
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	std::string strDataSource = "";		/*!< 源数据 */
	std::string strDataRetrans = "";	/*!< 反转义数据 */
	std::string strDataHex = "";		/*!< 数据16进制字符串 */

	byte *byData = new byte[unLen];	/*!< 反转义数据块 */

	// 初始化数据块
	memset(byData, 0, unLen);

	// 储存报文头
	byData[0] = bySource[0];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[0]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	int nCount = 1;	/*!< 反转义数据位置 */

	// 数据反转义
	for (unsigned int i = 1; i < unLen - 1; i++, nCount++)
	{
		// 获取16进制数据字符串
		BufferString::std_string_format(strDataHex, "%02X ", bySource[i]);
		strDataSource += strDataHex;

		if (bySource[i] == 0xA0)
		{
			i++;
			switch (bySource[i])
			{
			case 0x01:
				byData[nCount] = 0xA0;

				strDataRetrans += "A0 ";
				break;
			case 0x02:
				byData[nCount] = 0xAA;

				strDataRetrans += "AA ";
				break;
			case 0x03:

				byData[nCount] = 0xAB;

				strDataRetrans += "AB ";
				break;
			}
		}
		else
		{
			byData[nCount] = bySource[i];

			// 获取16进制数据字符串
			BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);
			strDataRetrans += strDataHex;
		}
	}

	// 储存报文尾
	byData[nCount] = bySource[unLen - 1];

	// 获取16进制数据字符串
	BufferString::std_string_format(strDataHex, "%02X ", byData[nCount]);

	strDataSource += strDataHex;
	strDataRetrans += strDataHex;

	DebugPrint("AGVControlSystem/CProcessData.cpp/RetransferPath 源数据:%s.字符串:%s.\n反转义后数据:%s.字符串:%s.", strDataSource.c_str(), bySource, strDataRetrans.c_str(), byData);

	BufferString::Buffer bfTrans(byData, nCount);

	delete[] byData;

	return bfTrans;
}

BufferString::Buffer CProcessData::CreatePackgePath(BufferString::Buffer bfSource, unsigned char byNo)
{
	return CreatePackgePath(bfSource.m_pszBuffer, bfSource.m_unSize, byNo);
}

BufferString::Buffer CProcessData::CreatePackgePath(byte * bySource, unsigned int unLen, unsigned char byNo)
{
	/* 2字节 总地标卡数  5字节 终点地标卡块 N*5字节 其他地标卡块 */
	if (bySource == nullptr || unLen == 0)
	{
		return BufferString::Buffer();
	}

	unsigned int unDefaultLen = 4;	/*!< 默认报文长度 */

	unsigned int unPackageLen = unLen + unDefaultLen;
	unsigned int unTotal = unLen / 5; /*!< 总RFID卡块数量 */

	byte *byPackage = new byte[unPackageLen];
	// 初始化
	memset(byPackage, 0, unPackageLen);

	int nIndex = 0;

	byPackage[nIndex] = 0xAA;												/*!< 报文头 */
	byPackage[++nIndex] = (unTotal >> 8) & 0xFF;							/*!< RFID卡总数 */
	byPackage[++nIndex] = unTotal & 0xFF;									/*!< RFID卡总数 */

	memcpy_s(&byPackage[++nIndex], unPackageLen, bySource, unLen);			/*!< 数据体 */

	unsigned short usCRC16 = CRC16(byPackage, unPackageLen - 3);			/*!< 校验码 */
	byPackage[unPackageLen - 3] = (usCRC16 >> 8) & 0xFF;					/*!< 校验位 */
	byPackage[unPackageLen - 2] = usCRC16 & 0xFF;							/*!< 校验位 */
	byPackage[unPackageLen - 1] = 0xAB;										/*!< 报文尾 */

	std::string strDataRecv = "";
	for (unsigned int d = 0; d < unPackageLen; d++)
	{
		std::string strDataHex = "";
		BufferString::std_string_format(strDataHex, "%02X ", byPackage[d]);
		strDataRecv += strDataRecv;
	}

	DebugPrint("AGVControlSystem/CProcessData.cpp/CreatePackgePath 报文数据:%s.字符串:%s.", strDataRecv.c_str(), byPackage);

	// 转义
	BufferString::Buffer tranf = Transfer(byPackage, unPackageLen);
	delete[] byPackage;

	return tranf;
}

std::list<BufferString::Buffer> CProcessData::ProcessPath(BufferString::Buffer bfSource, BufferString::Buffer & bfResidue)
{
	return Process(bfSource.m_pszBuffer, bfSource.m_unSize, bfResidue);
}

std::list<BufferString::Buffer> CProcessData::ProcessPath(byte * bySource, unsigned int unLen, BufferString::Buffer & bfResidue)
{
	if (bySource == nullptr || unLen == 0)
	{
		return std::list<BufferString::Buffer>();
	}

	std::list<BufferString::Buffer> m_listPackage;	/*!< 报文集合 */

	byte* byResidue = bySource;			/*!< 剩余数据块 */
	unsigned int unResidueLen = unLen;	/*!< 剩余数据块大小 */

	while (byResidue)
	{
		byte* byHead = (byte*)memchr(byResidue, 0xAA, unResidueLen);	/*!< 报文头 */

		// 未在数据块中找到报文头
		if (byHead == nullptr)
		{
			byResidue = nullptr;
			unResidueLen = 0;
			break;
		}

		byResidue = byHead;
		unResidueLen = unLen - (byResidue - bySource);

		byte* byTail = (byte*)memchr(byResidue, 0xAB, unResidueLen);	/*!< 报文尾 */

		// 未在数据块中找到报文头
		if (byTail == nullptr)
		{
			break;
		}

		// 报文尾已经在数据块末尾
		if (byTail == &bySource[unLen - 1])
		{
			byResidue = nullptr;
			unResidueLen = 0;
		}
		else
		{
			byResidue = byTail + 1;
			unResidueLen = unLen - (byResidue - bySource);
		}

		BufferString::Buffer bfTrans = Retransfer(byHead, (byTail - byHead) + 1);		/*!< 反转义数据 */

		unsigned short usCRC16 = CRC16(byHead, (byTail - byHead) - 1);	/*!< CRC校验码 */
		unsigned short usCheck = *(byTail - 2) << 8 | *(byTail - 1);	/*!< 报文内校验值 */

		DebugPrint("----AGVControlSystem/CProcessData.cpp/ProcessPath 数据校验:%04X:%04X.", usCRC16, usCheck);

		// 校验码校验
		if (usCRC16 != usCheck)
		{
			continue;
		}

		m_listPackage.push_back(bfTrans);

		std::string strDataRecv = "";
		for (unsigned int i = 0; i < bfTrans.m_unSize; i++)
		{
			std::string strDataHex = "";
			BufferString::std_string_format(strDataHex, "%02X ", bfTrans.m_pszBuffer[i]);
			strDataRecv += strDataRecv;
		}

		DebugPrint("--------AGVControlSystem/CProcessData.cpp/ProcessPath 报文包数据:%s.字符串:%s.", strDataRecv.c_str(), byHead);
	}

	if (unResidueLen > 0)
	{
		bfResidue = BufferString::Buffer(byResidue, unResidueLen);
	}
	else
	{
		bfResidue = BufferString::Buffer();
	}

	return m_listPackage;
}

#endif // !_PROCESSDATA_H