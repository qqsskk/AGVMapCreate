/*!
 * @file CRestPosition.h
 * @brief 工位基类文件
 *
 * 包含工位的基本属性与基本动作
 * @author FanKaiyu
 * @date 2019-01-15
 * @version 2.0
*/

#pragma once
#include <string>

#ifndef _STATION_H
#define _STATION_H

/*!
 * @class CStation
 * @brief 工位基类
 *
 * 包含工位的基本属性与基本动作
*/
class CStation
{
public:
	CStation(unsigned char byNo,std::string strName = "");
	CStation(unsigned char byNo, std::wstring wstrName = L"");
#ifdef _AFX
	CStation(unsigned char byNo, CString cstrName = _T(""));
#endif //!_AFX
	~CStation();
protected:
	unsigned char m_byNo; /*!< 编号 */
	std::string m_strName;	/*!< 名称 */

public:
	/*!
	 * @brief 设置编号
	 * @param unsigned char 新工位编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned char byNo);

	/*!
	 * @brief 获取编号
	 * @return unsigned char 工位编号
	*/
	unsigned char GetNo() const;

	/*!
	 * @brief 设置名称
	 * @param string 新工位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::string strName);

	/*!
	 * @brief 设置名称
	 * @param wstring 新工位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(std::wstring wstrName);

#ifdef _AFX
	/*!
	 * @brief 设置名称
	 * @param CString 新工位名称
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetName(CString cstrName);
#endif // !_AFX

	/*!
	 * @brief 获取工位名称
	 * @param string& 工位名称
	*/
	void GetName(std::string& strName);

	/*!
	 * @brief 获取工位名称
	 * @param wstring& 工位名称
	*/
	void GetName(std::wstring& wstrName);

#ifdef _AFX
	/*!
	 * @brief 获取工位名称
	 * @param CString& 工位名称
	*/
	void GetName(CString& cstrName);
#endif // !_AFX

public:
	/*!
	 * @brief 获取编号
	 * @param string& 工位编号
	*/
	void GetNo(std::string& strNo);

	/*!
	 * @brief 获取编号
	 * @param wstring& 工位编号
	*/
	void GetNo(std::wstring& wstrNo);

#ifdef _AFX
	/*!
	 * @brief 获取编号
	 * @param CString& 工位编号
	*/
	void GetNo(CString& cstrNo);
#endif //!_AFX
};

#endif // !_STATION_H