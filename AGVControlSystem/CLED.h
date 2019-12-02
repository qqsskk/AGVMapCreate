/*!
 * @file CLED.h
 * @brief LED设备基类文件
 *
 * 描述了LED设备的基本属性信息
 * @author FanKaiyu
 * @date 2019-02-14
 * @version 1.0
*/

#pragma once
#include <string>

#ifndef _LED_H
#define _LED_H

/*!
 * @class CLED
 * @brief LED设备基类
 *
 * 描述了LED设备的基本属性信息
*/
class CLED
{
public:
	CLED(unsigned char byNo,unsigned short usWidth,unsigned short usHeight,std::string strIP,unsigned short usPort,std::string strText = "");
	CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, std::wstring wstrIP, unsigned short usPort, std::wstring wstrText = L"");
#ifdef _AFX
	CLED(unsigned char byNo, unsigned short usWidth, unsigned short usHeight, CString cstrIP, unsigned short usPort, CString cstrText = _T(""));
#endif //!_AFC
	~CLED();
protected:
	unsigned char m_byNo;		/*!< 编号 */
	unsigned short m_usWidth;	/*!< 宽 */
	unsigned short m_usHeight;	/*!< 高 */
	std::string m_strIP;		/*!< IP地址 */
	unsigned short m_usPort;	/*!< 端口 */
	std::string m_strText;		/*!< 备注信息，通常用于描述此LED的位置以及实际作用 */

public:
	/*!
	 * @brief 设置编号
	 * @param unsigend char LED新编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char LED编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 获取编号
	 * @param string& LED编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& LED编号
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief 设置宽
	 * @param unsigned short LED新宽
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetWidth(unsigned short usWidth);

	/*!
	 * @brief 获取宽
	 * @return unsigned short LED宽
	*/
	unsigned short GetWidth() const;

	/*!
	 * @brief 获取宽
	 * @param string& LED宽
	*/
	void GetWidth(std::string& strWidth);

	/*!
	 * @brief 获取宽
	 * @param wstring& LED宽
	*/
	void GetWidth(std::wstring& wstrWidth);

	/*!
	 * @brief 设置高
	 * @param unsigned short LED新高
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetHeight(unsigned short usHeight);

	/*!
	 * @brief 获取高
	 * @return unsigned short LED高
	*/
	unsigned short GetHeight() const;

	/*!
	 * @brief 获取高
	 * @param string& LED高
	*/
	void GetHeight(std::string strHeight);

	/*!
	 * @brief 获取高
	 * @param wstring& LED高
	*/
	void GetHeight(std::wstring wstrHeight);

	/*!
	 * @brief 设置IP地址
	 * @param string LED新IP地址
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetIP(std::string strIP);

	/*!
	 * @brief 设置IP
	 * @param wstring LED新IP地址
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetIP(std::wstring wstrIP);

	/*!
	 * @brief 获取IP
	 * @param string& LEDIP地址
	*/
	void GetIP(std::string& strIP);

	/*!
	 * @brief 获取IP
	 * @param wstring& LEDIP地址
	*/
	void GetIP(std::wstring& wstrIP);

	/*!
	 * @brief 设置端口
	 * @param unsigned short LED新端口
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetPort(unsigned short usPort);

	/*!
	 * @brief 获取端口
	 * @return unsigned short LED端口
	*/
	unsigned short GetPort() const;

	/*!
	 * @brief 获取端口
	 * @param string& LED端口
	*/
	void GetPort(std::string& strPort);

	/*!
	 * @brief 获取端口
	 * @param wstring& LED端口
	*/
	void GetPort(std::wstring wstrPort);

	/*!
	 * @brief 设置备注
	 * @param string LED新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(std::string strText);

	/*!
	 * @brief 设置备注
	 * @param wstring LED新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(std::wstring wstrText);
	
	/*!
	 * @brief 获取备注
	 * @param string& LED备注信息
	*/
	void GetText(std::string& strText);

	/*!
	 * @brief 获取备注
	 * @param wstring& LED备注信息
	*/
	void GetText(std::wstring& wstrText);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& LED编号
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief 获取宽
	 * @param CString& LED宽
	*/
	void GetWidth(CString& cstrWidth);

	/*!
	 * @brief 获取高
	 * @param CString& LED宽
	*/
	void GetHeight(CString& cstrHeight);

	/*!
	 * @brief 设置IP
	 * @param CString LED新IP地址
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetIP(CString cstrIP);

	/*!
	 * @brief 获取IP
	 * @param CString& LEDIP地址
	*/
	void GetIP(CString& cstrIP);

	/*!
	 * @brief 获取端口
	 * @param CString& LED端口
	*/
	void GetPort(CString& cstrPort);

	/*!
	 * @brief 设置备注
	 * @param CString LED新备注信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetText(CString cstrText);

	/*!
	 * @brief 获取备注
	 * @param CString& LED备注信息
	*/
	void GetText(CString& cstrText);
#endif //!_AFX
};

#endif // !_LED_H