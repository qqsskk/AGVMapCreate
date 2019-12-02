/*!
 * @file CMNavCharger.h
 * @brief 磁导航充电位文件
 *
 * @author FanKaiyu
 * @date 2019-01-17
 * @version 1.0
*/

#pragma once
#include "CMagneticTrackNavigation.h"
#include "CCharger.h"

#ifdef _CHARGER_H

/*!
 * @class CMNavCharger
 * @brief 磁导航充电位类
*/
class CMNavCharger
	:public CMagneticTrackLocation,public CCharger
{
public:
	CMNavCharger(unsigned char byNo,unsigned short usLocation, unsigned char byController = 0,std::string strName = "");
	CMNavCharger(unsigned char byNo, unsigned short usLocation, unsigned char byController = 0, std::wstring wstrName = L"");
#ifdef _AFX
	CMNavCharger(unsigned char byNo, unsigned short usLocation, unsigned char byController = 0, CString cstrName = _T(""));
#endif 
	~CMNavCharger();
};

#endif // _CHARGER_H