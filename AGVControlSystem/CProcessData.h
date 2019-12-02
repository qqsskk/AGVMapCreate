/*!
 * @file CProcessData.h
 * @brief 数据处理基类文件
 *
 * @author FanKaiyu
 * @date 2019-01-09
 * @version 1.2
 *
 * @update 2019-01-18
 * @author FanKaiyu
 * @brief 增加对路径报文的合成、解析、转义、反转义.
 *
 * @update 2019-01-21
 * @author FanKaiyu
 * @brief 报文结构中增加组码字节,用于大数据的拼接.
*/
#pragma once
#include "Buffer.h"
#include <list>
#include "CRC.h"

#ifndef _PROCESSDATA_H
#define _PROCESSDATA_H

/*! @brief 拼接数据块 */
typedef struct _JointBufferStruct
{
public:
	unsigned short m_usGroup;	/*!< 组号 */
	unsigned short m_usTotal;	/*!< 总包数 */
	BufferString::Buffer *m_pArray;			/*!< 拼接数据包数组 */

public:
	_JointBufferStruct(unsigned short usGroup, unsigned short usTotal)
	{
		m_usGroup = usGroup;
		m_usTotal = usTotal;
		m_pArray = new BufferString::Buffer[m_usTotal];
	}
	          
	_JointBufferStruct(const _JointBufferStruct& joint)
	{		
		m_usGroup = joint.m_usGroup;
		m_usTotal = joint.m_usTotal;

		m_pArray = new BufferString::Buffer[m_usTotal];

		for (int i = 0; i < m_usTotal; i++)
		{
			m_pArray[i] = joint.m_pArray[i];
		}
	}

	~_JointBufferStruct()
	{
		if (m_pArray)
		{
			delete[] m_pArray;
		}
	}

	void operator=(const _JointBufferStruct& joint)
	{
		if (m_pArray)
		{
			delete[] m_pArray;
		}

		m_usGroup = joint.m_usGroup;
		m_usTotal = joint.m_usTotal;

		m_pArray = new BufferString::Buffer[m_usTotal];

		for (int i = 0; i < m_usTotal; i++)
		{
			m_pArray[i] = joint.m_pArray[i];
		}
	}

public:
	/*!
	 * @brief 数据是否已经拼接完整
	 * @return bool 数据完整返回true,否则返回false
	*/
	bool IsFull() const
	{
		if (m_pArray == nullptr)
		{
			return false;
		}

		for (int i = 0; i < m_usTotal; i++)
		{
			if (m_pArray[i].m_unSize == 0 || m_pArray[i].m_pszBuffer == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	/*!
	 * @brief 数据是否已经拼接完整
	 * @param Buffer& 完整的数据块
	 * @return bool 数据完整返回true,否则返回false
	*/
	bool IsFull(BufferString::Buffer& bfData) const
	{
		if (IsFull())
		{
			bfData = CreateFullData();

			if (bfData.m_unSize == 0)
			{
				return false;
			}

			return true;
		}

		return false;
	}

	/*!
	 * @brief 合成完整的数据
	 * @return Buffer 完整的数据块
	*/
	BufferString::Buffer CreateFullData() const
	{
		BufferString::Buffer fullData;

		if (m_pArray == nullptr)
		{
			return fullData;
		}

		unsigned int unDefaultLen = 14;		/*!< 默认报文长度 */

		int nIndex = 11;	/*!< 下标 */

		for (int i = 0; i < m_usTotal; i++)
		{
			if (m_pArray[i].m_unSize == 0 || m_pArray[i].m_pszBuffer == nullptr)
			{
				return fullData;
			}

			fullData += BufferString::Buffer(&m_pArray[i].m_pszBuffer[nIndex], m_pArray->m_unSize - unDefaultLen);	/*!< 组合数据体 */
		}

		unsigned short usGroup = CRC::CRC16(fullData.m_pszBuffer, fullData.m_unSize);	/*!< 组码 */

		fullData = BufferString::Buffer(&m_pArray[0].m_pszBuffer[nIndex - 2], 2) + fullData;	/*!< 完整数据 */

		if (usGroup != m_usGroup)
		{
			return BufferString::Buffer();
		}

		return fullData;
	}

	/*!
	 * @brief 添加拼接数据包
	 * @return 添加成功返回true,失败返回false
	*/
	bool AddJointData(BufferString::Buffer bfData)
	{
		int nIndex = 3;	/*!< 下标 */

		unsigned short usGroup = (bfData.m_pszBuffer[nIndex] << 8) | bfData.m_pszBuffer[++nIndex];	/*!< 组码 */

		if (usGroup != m_usGroup)
		{
			return false;
		}

		unsigned short usTotal = (bfData.m_pszBuffer[++nIndex] << 8) | bfData.m_pszBuffer[++nIndex];	/*!< 总包数 */
		unsigned short usCur = (bfData.m_pszBuffer[++nIndex] << 8) | bfData.m_pszBuffer[++nIndex];		/*!< 当前包号 */

		if (m_usTotal != usTotal)
		{
			if (m_pArray)
			{
				delete[] m_pArray;
			}

			m_usTotal = usTotal;
			m_pArray = new BufferString::Buffer[usTotal];
		}

		m_pArray[usCur - 1] = bfData;

		return true;
	}
} JointBuffer;

/*!
 * @class CProcessData
 * @brief 数据处理基类
 *
 * 根据报文指定规则，将数据进行转义、反转义、解析、合成等操作
*/
class CProcessData
{
public:
	CProcessData(byte byType);
	~CProcessData();
protected:
	byte m_byType; /*!< 设备类型 */

protected:
	/*!
	 * @brief 转义
	 * @param Buffer 源数据块
	 * @return Buffer 转义后数据
	*/
	BufferString::Buffer Transfer(BufferString::Buffer bfSource);

	/*!
	 * @brief 转义
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return Buffer 转义后数据
	*/
	BufferString::Buffer Transfer(byte* bySource, unsigned int unLen);

	/*!
	 * @brief 反转义
	 * @param Buffer 源数据
	 * @return Buffer 反转义后的数据
	*/
	BufferString::Buffer Retransfer(BufferString::Buffer bfSource);

	/*!
	 * @brief 反转义
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return Buffer 反转义后的数据
	*/
	BufferString::Buffer Retransfer(byte* bySource, unsigned int unLen);

	/*!
	 * @brief 合成
	 * @param Buffer 源数据块
	 * @param unsigned char 设备编号
	 * @return std::list<Buffer> 合成后的报文集合
	*/
	std::list<BufferString::Buffer> CreatePackge(BufferString::Buffer bfSource,unsigned char byNo);

	/*!
	 * @brief 合成
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @param unsigned char 设备编号
	 * @return std::list<Buffer> 合成后的报文集合
	*/
	std::list<BufferString::Buffer> CreatePackge(byte* bySource,unsigned int unLen, unsigned char byNo);

	/*!
	 * @brief 数据和校验
	 *
	 * 将数据按字节依次相加
	 * 将相加得出的和取反
	 * @param Buffer 源数据块
	 * @return unsigned int 校验码
	*/
	unsigned int Sum(BufferString::Buffer bfSource);

	/*!
	 * @brief 数据和校验
	 *
	 * 将数据按字节依次相加
	 * 将相加得出的和取反
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return unsigned int 校验码
	*/
	unsigned int Sum(byte* bySource, unsigned int unLen);

	/*!
	 * @brief CRC16校验
	 * @param Buffer 源数据块
	 * @return unsigned short 校验码
	*/
	unsigned short CRC16(BufferString::Buffer bfSource);

	/*!
	 * @brief CRC16校验
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return unsigned short 校验码
	*/
	unsigned short CRC16(byte* bySource, unsigned int unLen);

public:
	/*!
	 * @brief 解析
	 * @param Buffer 源数据块
	 * @param Buffer& 剩余数据块
	 * @return 解析出的报文集合
	*/
	std::list<BufferString::Buffer> Process(BufferString::Buffer bfSource, BufferString::Buffer &bfResidue);

	/*!
	 * @brief 解析
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @param Buffer& 剩余数据块
	 * @return 解析出的报文集合
	*/
	std::list<BufferString::Buffer> Process(byte* bySource, unsigned int unLen, BufferString::Buffer &bfResidue);

	/*!
	 * @brief 拼接
	 * @param std::list<Buffer> 待拼接的数据集合
	 * @param std::list<Buffer>& 剩余未拼接的数据集合
	 * @return std::list<Buffer> 拼接完成的数据集合
	*/
	std::list<BufferString::Buffer> Joint(std::list<BufferString::Buffer> bfList, std::list<BufferString::Buffer>& bfLiftList);

/*-------------------------------------处理、解析路径报文-----------------------------------------------*/
protected:
	/*!
	 * @brief 转义路径包
	 * @param Buffer 源数据块
	 * @return Buffer 转义后数据
	*/
	BufferString::Buffer TransferPath(BufferString::Buffer bfSource);

	/*!
	 * @brief 转义路径包
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return Buffer 转义后数据
	*/
	BufferString::Buffer TransferPath(byte* bySource, unsigned int unLen);

	/*!
	 * @brief 反转义路径包
	 * @param Buffer 源数据
	 * @return Buffer 反转义后的数据
	*/
	BufferString::Buffer RetransferPath(BufferString::Buffer bfSource);

	/*!
	 * @brief 反转义路径包
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @return Buffer 反转义后的数据
	*/
	BufferString::Buffer RetransferPath(byte* bySource, unsigned int unLen);

	/*!
	 * @brief 合成路径包
	 * @param Buffer 源数据块
	 * @param unsigned char 设备编号
	 * @return Buffer 合成后的报文
	*/
	BufferString::Buffer CreatePackgePath(BufferString::Buffer bfSource, unsigned char byNo);

	/*!
	 * @brief 合成路径包
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @param unsigned char 设备编号
	 * @return Buffer 合成后的报文
	*/
	BufferString::Buffer CreatePackgePath(byte* bySource, unsigned int unLen, unsigned char byNo);

public:
	/*!
	 * @brief 解析路径包
	 * @param Buffer 源数据块
	 * @param Buffer& 剩余数据块
	 * @return 解析出的报文集合
	*/
	std::list<BufferString::Buffer> ProcessPath(BufferString::Buffer bfSource, BufferString::Buffer &bfResidue);

	/*!
	 * @brief 解析路径包
	 * @param byte* 源数据块首地址指针
	 * @param unsigned ine 源数据块长度
	 * @param Buffer& 剩余数据块
	 * @return 解析出的报文集合
	*/
	std::list<BufferString::Buffer> ProcessPath(byte* bySource, unsigned int unLen, BufferString::Buffer &bfResidue);
};

#endif // !_PROCESSDATA_H