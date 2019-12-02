/*!
 * @file CRestPosition.h
 * @brief 待机位基类文件
 *
 * 包含待机位的基本属性与基本动作
 * @author FanKaiyu
 * @date 2019-01-15
 * @version 2.0
*/

#pragma once
#include <string>

#ifndef _RESTPOSITION_H
#define _RESTPOSITION_H

/*!
 * @class CRestPosition
 * @brief 待机位基类
 *
 * 包含待机位的基本属性与基本动作
*/
class CRestPosition
{
public:
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, std::string strName = "");
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, std::wstring wstrName = L"");
#ifdef _AFX
	CRestPosition(unsigned char byNo, unsigned char byNext = 0, CString cstrName = _T(""));
#endif //!_AFX
	CRestPosition(const CRestPosition& rest);
	~CRestPosition();
protected:
	unsigned char m_byNo;	/*!< 待机位编号 */
	unsigned char m_byNext; /*!< 下一待机位编号，多用于待机位自动提位功能。*/
	std::string m_strName;	/*!< 名称 */

public:
	/*!
	 * @brief 设置编号
	 * @param unsigned char 新待机位编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char 获取待机位编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 设置下一位编号
	 * @param unsigned char 新下一待机位编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNext(unsigned char byNext);

	/*!
	 * @brief 获取下一位编号
	 * @return unsigned char 下一待机位编号
	*/
	unsigned char GetNext() const;

	/*!
	 * @brief 设置名称
	 * @param string 新待机位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief 设置名称
	 * @param wstring 新待机位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief 设置名称
	 * @param CString 新待机位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief 获取待机位名称
	 * @param string& 工位名称
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief 获取待机位名称
	 * @param wstring& 工位名称
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief 获取待机位名称
	 * @param CString& 工位名称
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

public:
	/*!
	 * @brief 获取编号
	 * @param string& 编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& 编号
	*/
	void GetNo(std::wstring& wstrNo);

	/*!
	 * @brief 获取下一位编号
	 * @param string& 下一位编号
	*/
	void GetNext(std::string& strNo);

	/*!
	 * @brief 获取下一位编号
	 * @param string& 下一位编号
	*/
	void GetNext(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 编号
	*/
	void GetNo(CString& cstrNo);

	/*!
	 * @brief 获取下一位编号
	 * @param string& 下一位编号
	*/
	void GetNext(CString& cstrNo);
#endif // !_AFX
};

#endif // !_RESTPOSITION_H