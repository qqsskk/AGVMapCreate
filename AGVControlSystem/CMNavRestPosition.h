/*!
 * @file CMNavRestPosition.h
 * @brief 磁导航待机位文件
 *
 * @author FanKaiyu
 * @date 2019-01-17
 * @version 1.0
*/

#pragma once
#include "CMagneticTrackNavigation.h"
#include "CRestPosition.h"

#ifdef _RESTPOSITION_H

/*!
 * @class CMNavRestPosition
 * @brief 磁导航待机位类
*/
class CMNavRestPosition
	:public CMagneticTrackLocation, public CRestPosition
{
public:
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation,std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation,std::set<unsigned short> setStopList, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::string strName = "");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName = L"");
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, std::wstring wstrName = L"");
#ifdef _AFX
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation,CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName = _T(""));
	CMNavRestPosition(unsigned char byNo, unsigned char byNext, unsigned short usLocation, std::set<unsigned short> setStopList, CString cstrName = _T(""));
#endif // _AFX
	CMNavRestPosition(const CMNavRestPosition& rest);
	~CMNavRestPosition();
	void operator=(const CMNavRestPosition& rest);

protected:
	std::set<unsigned short> m_setStop;	/*!< 禁行RFID卡编号集合 */

public:
	/*!
	 * @brief 添加禁行RFID卡
	 * @param unsigned short 新禁行RFID卡编号信息
	 * @return bool 添加成功返回true,失败返回false
	*/
	bool AddStopRFID(unsigned short usRFID);

	/*!
	 * @brief 删除禁行RFID卡
	 * @param unsigned short 禁行RFID卡编号信息
	*/
	void DeleteStopRFID(unsigned short usRFID);

	/*!
	 * @brief 获取禁行RFID卡集合
	 * @return set<unsigned short> 禁行RFID卡编号信息集合
	*/
	std::set<unsigned short> GetStopRFIDList() const;

public:
	/*!
	 * @brief 获取禁行RFID卡集合
	 * @param string& 禁行RFID卡编号信息集合
	*/
	void GetStopRFIDList(std::string& strRFIDList);

	/*!
	 * @brief 获取禁行RFID卡集合
	 * @param wstring& 禁行RFID卡编号信息集合
	*/
	void GetStopRFIDList(std::wstring& wstrRFIDList);

	/*!
	 * @brief 设置禁行RFID卡集合
	 * @param string 禁行RFID卡编号信息集合
	*/
	void SetStopRFIDList(std::string strRFIDList);

	/*!
	 * @brief 设置禁行RFID卡集合
	 * @param wstring 禁行RFID卡编号信息集合
	*/
	void SetStopRFIDList(std::wstring wstrRFIDList);

#ifdef _AFX
	/*!
	 * @brief 获取禁行RFID卡集合
	 * @param CString& 禁行RFID卡编号信息集合
	*/
	void GetStopRFIDList(CString& cstrRFIDList);

	/*!
	 * @brief 设置禁行RFID卡集合
	 * @param CString 禁行RFID卡编号信息集合
	*/
	void SetStopRFIDList(CString cstrRFIDList);
#endif // !_AFX
};

#endif // !_RESTPOSITION_H