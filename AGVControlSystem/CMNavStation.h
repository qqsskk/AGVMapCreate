/*!
 * @file CMNavStation.h
 * @brief 磁导航工位文件
 *
 * @author FanKaiyu
 * @date 2019-01-17
 * @version 1.0
*/

#pragma once
#include "CMagneticTrackNavigation.h"
#include "CStation.h"

#ifdef _STATION_H

/*!
 * @class CMNavStation
 * @brief 磁导航工位类
*/
class CMNavStation
	:public CMagneticTrackLocation, public CStation
{
public:
	CMNavStation(unsigned char byNo, unsigned short usLocation);
	CMNavStation(unsigned char byNo, std::string strName, unsigned short usLocation);
	CMNavStation(unsigned char byNo, std::wstring wstrName, unsigned short usLocation);
#ifdef _AFX
	CMNavStation(unsigned char byNo, CString cstrName, unsigned short usLocation);
#endif // _AFX
	~CMNavStation();
};

#endif // !_STATION_H