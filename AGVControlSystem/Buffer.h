/*!
 * @file Buffer.h
 * @brief 数据转换,数据存储
 * @author FanKaiyu
 * @date 2018-04-19
 * @version 1.0
 */

#ifndef _BUFFER_H
#define _BUFFER_H
#pragma once
#include <string>
#include <wchar.h>
#include <stdarg.h>

namespace BufferString
{
	/*!
	 * @brief ASCII码字符转换为16进制数
	 * @param chAscii 需要转换的ASCII码字符
	 * @return char 转换后的16进制数
	*/
	static char HexChar(char chAscii)
	{
		if ((chAscii >= '0') && (chAscii <= '9'))
		{
			return chAscii - 0x30;
		}
		else if ((chAscii >= 'A') && (chAscii <= 'F'))
		{
			return chAscii - 'A' + 10;
		}
		else if ((chAscii >= 'a') && (chAscii <= 'f'))
		{
			return chAscii - 'a' + 10;
		}
		else
		{
			return 0x10;
		}
	}


	/*!
	* @brief 字符串转换为16进制数
	* @param lpszStr 需要转换的字符串
	* @param lpszData 转换后的字符串
	* @return int 转换后的16进制数
	*/
	static int StrToHex(std::string lpszStr, char *lpszData)
	{
		int t, t1;
		int rlen = 0, len = lpszStr.length();

		if (len / 2 != 0)
		{
			lpszStr += '0';
			len += 1;
		}

		for (int i = 0; i < len;)
		{
			char l, h = lpszStr[i];

			if (h == ' ')
			{
				i++;
				continue;
			}

			i++;

			if (i >= len)
			{
				break;
			}

			l = lpszStr[i];
			t = HexChar(h);
			t1 = HexChar(l);

			if ((t == 16) || (t1 == 16))
			{
				break;
			}
			else
			{
				t = t * 16 + t1;
			}

			i++;
			lpszData[rlen] = (char)t;
			rlen++;
		}

		return rlen;
	}


	/*!
	* @brief 字符串转换为10进制数
	* @param lpszStr 需要转换的字符串
	* @return int 转换后的10进制数
	*/
	static int HexToDem(std::string lpszStr)
	{
		int dem = 0;

		for (unsigned int i = 0; i < lpszStr.length(); i++)
		{
			dem = dem * 16;

			//0~9之间的字符
			if ((lpszStr[i] <= '9') && (lpszStr[i] >= '0'))
			{
				dem += lpszStr[i] - '0';
			}
			//A~F之间的字符
			else if ((lpszStr[i] <= 'F') && (lpszStr[i] >= 'A'))
			{
				dem += lpszStr[i] - 'A' + 10;
			}
			//a~f之间的字符
			else if ((lpszStr[i] <= 'f') && (lpszStr[i] >= 'a'))
			{
				dem += lpszStr[i] - 'a' + 10;
			}
			//出错时返回-1
			else
			{
				return -1;
			}
		}

		return dem;
	}


	/*! @brief 数据块 */
	typedef struct _BufferStruct
	{
		unsigned char* m_pszBuffer; /*!< 指向数据块地址的指针*/
		unsigned int m_unSize; /*!< 数据块的大小*/

		/*!
		* @brief 无参构造函数
		*/
		_BufferStruct()
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;
		}

		/*!
		* @brief 有参构造函数
		* @param clpszBuffer 指向数据块首地址的指针
		* @param cunSize 数据库大小
		*/
		_BufferStruct(const unsigned char* clpszBuffer, const unsigned int cunSize)
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;

			// 缓存的数据块字节数不为0
			if (cunSize > 0)
			{
				m_pszBuffer = new unsigned char[cunSize];
				m_unSize = cunSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, clpszBuffer, m_unSize);
			}
		}

		/*!
		* @brief 拷贝构造函数
		* @param cBuffer 拷贝的数据块
		*/
		_BufferStruct(const _BufferStruct &cBuffer)
		{
			m_pszBuffer = nullptr;
			m_unSize = 0;

			// 拷贝的数据块字节数不为0
			if (cBuffer.m_unSize > 0)
			{
				m_pszBuffer = new unsigned char[cBuffer.m_unSize];
				m_unSize = cBuffer.m_unSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, cBuffer.m_pszBuffer, m_unSize);
			}
		}

		/*!
		* @brief 析构函数
		*/
		~_BufferStruct()
		{
			// 数据块内字节数不为0
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}
		}

		/*!
		* @brief 重载=操作符
		*
		* 将指定数据块内的数据拷贝至当前数据块
		* @param cBuffer 拷贝的数据块
		*/
		void operator= (const _BufferStruct &cBuffer)
		{
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}

			if (cBuffer.m_unSize > 0)
			{
				m_pszBuffer = new unsigned char[cBuffer.m_unSize];
				m_unSize = cBuffer.m_unSize;
				memset(m_pszBuffer, 0, m_unSize);
				memcpy(m_pszBuffer, cBuffer.m_pszBuffer, m_unSize);
			}
		}

		/*!
		* @brief 重载+操作符
		*
		* 将当前数据块与指定数据块内数据合并,并返回合并后的新数据块
		* @param cBuffer 合并的数据块
		* @return _BufferStruct 合并后的数据块
		*/
		_BufferStruct operator+ (const _BufferStruct &cBuffer)
		{
			_BufferStruct newBuffer(m_pszBuffer, m_unSize);
			newBuffer += cBuffer;

			return newBuffer;
		}

		/*!
		* @brief 重载+=操作符
		*
		* 将当前数据块与指定数据块内数据合并,并替换当前数据块内的数据
		* @param cBuffer 合并的数据块
		*/
		void operator+= (const _BufferStruct &cBuffer)
		{
			if (cBuffer.m_unSize == 0)
			{
				return;
			}

			unsigned int unLen = m_unSize + cBuffer.m_unSize; /*!< 合并后的数据块大小 */

			if (unLen == 0)
			{
				return;
			}

			unsigned char* lpszData = new unsigned char[unLen]; /*!< 合并后的数据块内存首地址*/
			unsigned int unDataLen = 0;/*!< 合并前的数据块大小*/
			memset(lpszData, 0, unLen);

			if (m_unSize > 0)
			{
				memcpy(lpszData, m_pszBuffer, m_unSize);
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				unDataLen = m_unSize;
				m_unSize = 0;
			}

			if (cBuffer.m_unSize > 0)
			{
				memcpy(lpszData + unDataLen, cBuffer.m_pszBuffer, cBuffer.m_unSize);
			}

			m_pszBuffer = lpszData;
			m_unSize = unLen;
			return;
		}

		/*!
		* @brief 重载==操作符
		*
		* 比较两个数据块内的数据是否相等
		* @param cBuffer 与之比较的数据块
		* @return bool 相等返回true,不相等返回false
		*/
		bool operator== (const _BufferStruct &cBuffer)
		{
			if (m_unSize != cBuffer.m_unSize)
			{
				return false;
			}

			for (unsigned int i = 0; i < m_unSize; i++)
			{
				if (m_pszBuffer[i] != cBuffer.m_pszBuffer[i])
				{
					return false;
				}
			}
			return true;
		}

		/*!
		* @brief 释放数据块内存空间
		*/
		void Clear()
		{
			if (m_unSize > 0)
			{
				delete[] m_pszBuffer;
				m_pszBuffer = nullptr;
				m_unSize = 0;
			}
		}

	} Buffer;

	/*!
	* @brief 在数据块内查找指定的数据块
	* @param lpszPacket 查找源数据块
	* @param unPacketSize 查找源数据块大小
	* @param lpszData 指定查找的数据块
	* @param unDataSize 指定查找的数据块大小
	* @return unsigned char* 第一次出现指定数据块的首数据的地址
	*/
	static unsigned char* FindData(unsigned char *lpszPacket, unsigned int unPacketSize, unsigned char *lpszData, unsigned int unDataSize)
	{
		if (unPacketSize < unDataSize)
		{
			return nullptr;
		}

		for (unsigned int d = 0; d < unPacketSize; d += unDataSize)
		{
			bool b_find = true;

			for (unsigned int f = 0; f < unDataSize; f++)
			{
				if (lpszPacket[d + f] != lpszData[f])
				{
					b_find = false;
					break;
				}
			}

			if (b_find)
			{
				return &lpszPacket[d];
			}
		}

		return nullptr;
	}

	/*!
	 * @brief string字符串转十进制数字
	 * @param string 源字符串
	 * @return int 转换后的十进制数字
	*/
	static int std_string_to_int(std::string _str)
	{
		int nSum = 0;
		double x = 0.0f;
		for (int i = _str.length() - 1; i > -1; i--, x += 1)
		{
			if (_str[i] == '+' || _str[i] == '-')
			{
				nSum = -nSum;
				break;
			}
			else if (_str[i] < '0' || _str[i] > '9')
			{
				break;
			}
			else
			{
				double dbDem = pow(10, x);
				nSum += (int)((_str[i] - '0') * dbDem);
			}
		}

		return nSum;
	}

	/*!
	 * @brief string字符串替换
	 * @param string& 源字符串
	 * @param char 要替换的字符
	 * @param char 替换后的字符
	*/
	static void std_string_replace(std::string& _str, char c, char r)
	{
		for (unsigned int i = 0; i < _str.length(); i++)
		{
			if (_str[i] == c)
			{
				_str[i] = r;
			}
		}

		return;
	}

	/*!
	 * @brief string字符串替换
	 * @param string& 源字符串
	 * @param int 要替换的字符（中文字符）
	 * @param char 替换后的字符
	*/
	static void std_string_replace(std::string& _str, int c, char r)
	{
		for (unsigned int i = 0; i < _str.length(); i++)
		{
			if (_str[i] == c)
			{
				_str[i] = r;
			}
		}

		return;
	}

	/*!
	 * @brief string字符串格式化输出
	 * @param string& 格式串
	 * @param string 格式化后的string字符串
	*/
	static void std_string_format(std::string & _str, const char * _Format, ...)
	{
		std::string tmp;

		va_list marker = NULL;
		va_start(marker, _Format);

		size_t num_of_chars = _vscprintf(_Format, marker);

		if (num_of_chars > tmp.capacity()) {
			tmp.resize(num_of_chars + 1);
		}

		vsprintf_s((char *)tmp.data(), tmp.capacity(), _Format, marker);

		va_end(marker);

		_str = tmp.c_str();
		return;
	}

	static void std_string_to_wstring(std::wstring& _wstr, std::string _str)
	{
#ifdef _ATL
		_wstr = (wchar_t*)_bstr_t(_str.c_str());
#else
		//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
		int len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.size(), NULL, 0);
		wchar_t* buffer = new wchar_t[len + 1];
		//多字节编码转换成宽字节编码  
		MultiByteToWideChar(CP_ACP, 0, _str.c_str(), _str.size(), buffer, len);
		buffer[len] = '\0';             //添加字符串结尾  
		//删除缓冲区并返回值  
		_wstr.append(buffer);
		delete[] buffer;
#endif // !_ATL

		return;
	}

	static void std_wstring_to_string(std::string& _str, std::wstring _wstr)
	{
#ifdef _ATL
		_str = (char*)_bstr_t(_wstr.c_str());
#else
		//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
		int len = WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), _wstr.size(), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];
		//宽字节编码转换成多字节编码  
		WideCharToMultiByte(CP_ACP, 0, _wstr.c_str(), _wstr.size(), buffer, len, NULL, NULL);
		buffer[len] = '\0';
		//删除缓冲区并返回值  
		_str.append(buffer);
		delete[] buffer;
#endif // !_ATL

		return;
	}

#ifdef _AFX
	static void CString_to_std_string(std::string& _str, CString _cstr)
	{
#ifdef _ATL
		_str = (char*)_bstr_t(_cstr);
#elif _UNICODE
		_str = CT2A(_cstr.GetBuffer());
#else
		_str = _cstr.GetBuffer();
#endif // !_ATL

		return;
	}

	static void std_string_to_CString(CString& _cstr, std::string _str)
	{
#ifdef _UNICODE
		std::wstring _wstr = L"";
		std_string_to_wstring(_wstr, _str);
		_cstr.Format(_T("%s"), _wstr.c_str());
#else
		_cstr.Format(_T("%s"), _str.c_str());
#endif
		return;
	}
#endif // _AFX
}

#endif // !_BUFFER_H