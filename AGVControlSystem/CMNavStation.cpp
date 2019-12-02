#include "stdafx.h"
#include "CMNavStation.h"

#ifdef _STATION_H

CMNavStation::CMNavStation(unsigned char byNo, unsigned short usLocation) :CStation(byNo,""), CMagneticTrackLocation(usLocation)
{
}

CMNavStation::CMNavStation(unsigned char byNo, std::string strName, unsigned short usLocation) : CStation(byNo, strName), CMagneticTrackLocation(usLocation)
{
}

CMNavStation::CMNavStation(unsigned char byNo, std::wstring wstrName, unsigned short usLocation) : CStation(byNo, wstrName), CMagneticTrackLocation(usLocation)
{
}

#ifdef _AFX
CMNavStation::CMNavStation(unsigned char byNo, CString cstrName, unsigned short usLocation) : CStation(byNo, cstrName), CMagneticTrackLocation(usLocation)
{
}
#endif //!_AFX

CMNavStation::~CMNavStation()
{
}

#endif // !_STATION_H