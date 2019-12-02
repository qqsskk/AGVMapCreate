#include "stdafx.h"
#include "CMNavCharger.h"

#ifdef _CHARGER_H

CMNavCharger::CMNavCharger(unsigned char byNo, unsigned short usLocation, unsigned char byController, std::string strName): CCharger(byNo, byController, strName), CMagneticTrackLocation(usLocation)
{
}
CMNavCharger::CMNavCharger(unsigned char byNo, unsigned short usLocation, unsigned char byController, std::wstring wstrName): CCharger(byNo, byController, wstrName), CMagneticTrackLocation(usLocation)
{
}

#ifdef _AFX
CMNavCharger::CMNavCharger(unsigned char byNo, unsigned short usLocation, unsigned char byController, CString cstrName): CCharger(byNo, byController, cstrName), CMagneticTrackLocation(usLocation)
{
}
#endif 

CMNavCharger::~CMNavCharger()
{
}

#endif // _CHARGER_H