/*!
 * @file CMagneticTrackNavigation.h
 * @brief 磁轨导航基本属性文件
 * 用于描述RFID卡的基本属性
 * 用于描述交通管制区域的基本属性
 * 用于描述定位信息的基本属性 
 *
 * @author FanKaiyu
 * @date 2019-01-07
 * @version 1.0
*/

#pragma once
#include <list>
#include <set>
#include <map>

#ifndef _MAGNETICTRACK_H
#define _MAGNETICTRACK_H
/*!
 * @class CRFID
 * @brief RFID卡的基本属性类
*/
class CRFID
{
public:
	CRFID(unsigned short usNo = 0);
	CRFID(const CRFID& rfid);
	~CRFID();

protected:
	unsigned short m_usNo;					/*!< RFID卡编号 */
	void *m_pUser;							/*!< 使用RFID卡的用户指针,通常为AGV指针 */
#ifdef _WIN64
	unsigned long long m_ullUseTime;		/*!< 开始使用RFID卡的时间（系统运行时间） */
#elif _WIN32
	unsigned long m_ulUseTime;				/*!< 开始使用RFID卡的时间（系统运行时间） */
#endif //  _WIN64
	void *m_pLocker;						/*!< 锁住RFID卡的用户指针，通常为AGV指针。用于防止非锁定用户使用RFID卡 */

public:
	/*!
	 * @brief 获取编号
	 * @return unsigned short 获取RFID卡编号信息
	*/
	unsigned short GetNo() const;

	/*!
	 * @brief 设置编号
	 * @param unsigned short 新RFID卡编号
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetNo(unsigned short usNo);

	/*!
	 * @brief 开始使用RFID卡
	 * @param void* 使用RFID卡的用户指针
	 * @return void* 正在使用RFID的用户指针
	*/
	void* Use(void* pUser);

	/*!
	 * @brief 结束使用RFID卡
	 * @param void* 使用RFID卡的用户指针
	*/
	void Finish(void* pUser);

	/*!
	 * @brief 获取使用者
	 * @return void* 使用RFID卡的用户指针
	*/
	void* GetUser() const;

#ifdef  _WIN64
	/*!
	 * @brief 获取使用者
	 * @param unsigned long long& 用户开始使用RFID卡的时间（系统运行时间）
	 * @return void* 使用RFID卡的用户指针
	*/
	void* GetUser(unsigned long long& ullUseTime) const;
#elif _WIN32
	/*!
	 * @brief 获取使用者
	 * @param unsigned long& 用户开始使用RFID卡的时间（系统运行时间）
	 * @return void* 使用RFID卡的用户指针
	*/
	void* GetUser(unsigned long& ulUseTime) const;
#endif //  _WIN64

	/*!
	 * @brief 锁定RFID卡
	 * @param void* 锁定RFID卡的用户指针
	 * @return void* 已经锁定RFID的用户指针
	*/
	void* Lock(void* pLocker);

	/*!
	 * @brief 解除RFID卡锁定
	 * @param void* 锁定RFID卡的用户指针
	*/
	void Unlock(void* pLocker);

	/*!
	 * @brief 获取锁定者
	 * @return void* 锁定RFID卡的用户指针
	*/
	void* GetLocker() const;

	void operator=(const CRFID& rfid);
};

/*!
 * @class CMagneticTrackLocation
 * @brief 磁轨导航方式基本定位属性类
 *
 * 用于为工位、待机位、充电位等赋予定位信息
*/
class CMagneticTrackLocation
{
public:
	CMagneticTrackLocation(unsigned short usRFID);
	~CMagneticTrackLocation();

protected:
	unsigned short m_usLocateRFID; /*!< 定位RFID卡编号信息 */

public:
	/*!
	 * @brief 设置定位信息
	 * @param unsigend short 新定位RFID卡编号信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetLocation(unsigned short usRFID);

	/*!
	 * @brief 获取定位信息
	 * @return unsigned short 定位RFID卡编号信息
	*/
	unsigned short GetLocation() const; 

public:
	/*!
	 * @brief 获取定位信息
	 * @param string& 定位RFID卡编号信息
	*/
	void GetLocation(std::string& strRFID);

	/*!
	 * @brief 获取定位信息
	 * @param wstring& 定位RFID卡编号信息
	*/
	void GetLocation(std::wstring& wstrRFID);

#ifdef _AFX
	/*!
	 * @brief 获取定位信息
	 * @param CString& 定位RFID卡编号信息
	*/
	void GetLocation(CString& cstrRFID);
#endif // !_AFX
};

typedef class CMagneticTrackLocation CMTLocation;

/*!
 * @class CMagneticTrackNavigation
 * brief 磁轨导航方式基本属性类
*/
class CMagneticTrackNavigation
{
public:
	CMagneticTrackNavigation();
	~CMagneticTrackNavigation();

protected:
	unsigned short m_usCurRFID;					/*!< 当前RFID卡编号信息 */
	unsigned short m_usOldRFID;					/*!< 过去RFID卡编号信息 */
	unsigned short m_usNextRFID;				/*!< 下一RFID卡编号信息 */
	unsigned short m_usEndRFID;					/*!< 终止RFID卡编号信息 */
	std::list<unsigned short> m_listOldRFID;	/*!< 过去RFID卡集合 */

public:
	/*!
	 * @brief 设置当前RFID卡编号信息
	 * @param unsigned short 新RFID卡编号信息
	 * @return bool 保存成功返回true,失败返回false
	*/
	bool SetCurRFID(unsigned short usRFID);

	/*!
	 * @brief 设置当前RFID卡编号信息
	 * @param unsigned short 新RFID卡编号信息
	 * @param unsigned short& 旧RFID卡编号信息
	 * @return bool 保存成功返回true,失败返回false
	*/
	bool SetCurRFID(unsigned short usRFID,unsigned short& usOldRFID);

	/*!
	 * @brief 获取当前RFID卡编号信息
	 * @return unsigned short 当前RFID卡编号信息
	*/
	unsigned short GetCurRFID() const;

	/*!
	 * @brief 获取过去RFID卡编号信息
	 * @return unsigned short 过去RFID卡编号信息
	*/
	unsigned short GetOldRFID() const;

	/*!
	 * @brief 设置下一RFID卡编号信息
	 * @param unsigned short 新下一RFID卡编号信息
	 * @return bool 保存成功返回true,失败返回false
	*/
	bool SetNextRFID(unsigned short usRFID);

	/*!
	 * @brief 获取下一RFID卡编号信息
	 * @return unsigned short 下一RFID卡编号信息
	*/
	unsigned short GetNextRFID() const;

	/*!
	 * @brief 设置终止RFID卡编号信息
	 * @param unsigend short 新终止RFID卡编号信息
	 * @return bool 保存成功返回true,失败返回false
	*/
	bool SetEndRFID(unsigned short usRFID);

	/*!
	 * @brief 获取终止RFID卡编号信息
	 * @return unsigned short 终止RFID卡编号信息
	*/
	unsigned short GetEndRFID() const;

	/*!
	 * @brief 获取过去RFID卡集合
	 * @return list<unsigned short> 过去RFID卡集合
	*/
	std::list<unsigned short> GetOldRFIDList() const;

public:
	/*!
	 * @brief 获取当前RFID卡编号信息
	 * @param std::string& 当前RFID卡编号信息
	*/
	void GetCurRFID(std::string& strRFID);

	/*!
	 * @brief 获取当前RFID卡编号信息
	 * @param std::wstring& 当前RFID卡编号信息
	*/
	void GetCurRFID(std::wstring& wstrRFID);

	/*!
	 * @brief 获取过去RFID卡编号信息
	 * @param std::string& 过去RFID卡编号信息
	*/
	void GetOldRFID(std::string& strRFID);

	/*!
	 * @brief 获取过去RFID卡编号信息
	 * @param std::wstring& 过去RFID卡编号信息
	*/
	void GetOldRFID(std::wstring& wstrRFID);

	/*!
	 * @brief 获取下一RFID卡编号信息
	 * @param string& 下一RFID卡编号信息
	*/
	void GetNextRFID(std::string& strRFID);

	/*!
	 * @brief 获取下一RFID卡编号信息
	 * @param wstring& 下一RFID卡编号信息
	*/
	void GetNextRFID(std::wstring& wstrRFID);

	/*!
	 * @brief 获取终止RFID卡编号信息
	 * @param string& 终止RFID卡编号信息
	*/
	void GetEndRFID(std::string& strRFID);

	/*!
	 * @brief 获取终止RFID卡编号信息
	 * @param wstring& 终止RFID卡编号信息
	*/
	void GetEndRFID(std::wstring& wstrRFID);

	/*!
	 * @brief 获取过去RFID卡集合
	 * @param string& 过去RFID卡集合
	*/
	void GetOldRFIDList(std::string& strList);

	/*!
	 * @brief 获取过去RFID卡集合
	 * @param wstring& 过去RFID卡集合
	*/
	void GetOldRFIDList(std::wstring& wstrList);

#ifdef _AFX
	/*!
	 * @brief 获取当前RFID卡编号信息
	 * @param CString& 当前RFID卡编号信息
	*/
	void GetCurRFID(CString& cstrRFID);

	/*!
	 * @brief 获取过去RFID卡编号信息
	 * @param CString& 过去RFID卡编号信息
	*/
	void GetOldRFID(CString& cstrRFID);

	/*!
	 * @brief 获取下一RFID卡编号信息
	 * @param CString& 下一RFID卡编号信息
	*/
	void GetNextRFID(CString& cstrRFID);

	/*!
	 * @brief 获取终止RFID卡编号信息
	 * @param CString& 终止RFID卡编号信息
	*/
	void GetEndRFID(CString& cstrRFID);

	/*!
	 * @brief 获取过去RFID卡集合
	 * @param CString& 过去RFID卡集合
	*/
	void GetOldRFIDList(CString& cstrList);
#endif //!_AFX
};

typedef class CMagneticTrackNavigation CMTNav;

/*!
 * @class CMagneticTrackTrafficControlArea
 * @brief 磁轨导航方式交通管制控制区域块
 *
 * 当禁行RFID卡集合存在AGV时,此区域块内AGV必须在交通管制RFID卡进行交通管制
 * 否则当管前RFID或管制RFID卡上存在AGV时,与比对区域块集合内的区域块进行比对
 * 根据先到先行的原则,判断此块内AGV是否允许通行
*/
class CMagneticTrackTrafficControlArea
{
public:
	CMagneticTrackTrafficControlArea();
	CMagneticTrackTrafficControlArea(unsigned short usCtrl,unsigned short usBefore = 0);
	CMagneticTrackTrafficControlArea(unsigned short usCtrl, std::set<unsigned short> setStop, std::set<unsigned short> setComparison,unsigned short usBefore = 0);
	CMagneticTrackTrafficControlArea(const CMagneticTrackTrafficControlArea& area);
	~CMagneticTrackTrafficControlArea();

protected:
	unsigned short m_usBefore;					/*!< 交通管制之前RFID卡编号信息 */
	unsigned short m_usCtrl;					/*!< 交通管制RFID卡编号信息 */
	std::set<unsigned short> m_setStop;			/*!< 进行RFID卡集合.即当禁行RFID卡上存在AGV时,本区域块以及与本区域块比对的区域块管制AGV均不允许通行. */
	std::set<unsigned short> m_setComparison;	/*!< 比对区域块集合.*/

public:
	/*!
	 * @brief 设置交通管制之前RFID卡编号信息
	 * @param unsigned short 新交通管制之前RFID卡编号信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetBeforeRFID(unsigned short usRFID);

	/*!
	 * @brief 获取交通管制之前RFID卡编号信息
	 * @return unsigned short 交通管制之前RFID卡编号信息
	*/
	unsigned short GetBeforeRFID() const;

	/*!
	 * @brief 设置交通管制RFID卡编号信息
	 * @param unsigned short 新交通管制RFID卡编号信息
	 * @return bool 设置成功返回true,失败返回false
	*/
	bool SetCtrlRFID(unsigned short usRFID);

	/*!
	 * @brief 获取交通管制RFID卡编号信息
	 * @return unsigned short 交通管制RFID卡编号信息
	*/
	unsigned short GetCtrlRFID() const;

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

	/*!
	 * @brief 添加比对区域
	 * @param unsigned short 区域交通管制RFID卡编号信息
	 * @return bool 添加成功返回true,失败返回false
	*/
	bool AddCompareArea(unsigned short usAreaCtrl);

	/*!
	 * @brief 删除比对区域
 * @param unsigned short 区域交通管制RFID卡编号信息
	*/
	void DeleteCompareArea(unsigned short usAreaCtrl);

	/*!
	 * @brief 获取比对区域集合
	 * @return set<unsigned short> 比对区域集合
	*/
	std::set<unsigned short> GetCompareAreaList() const;

	void operator=(const CMagneticTrackTrafficControlArea& area);

public:
	/*!
	 * @brief 获取交通管制之前RFID卡编号信息
	 * @param string& 交通管制之前RFID卡编号信息
	*/
	void GetBeforeRFID(std::string& strRFID);

	/*!
	 * @brief 获取交通管制之前RFID卡编号信息
	 * @param wstring& 交通管制之前RFID卡编号信息
	*/
	void GetBeforeRFID(std::wstring& wstrRFID);

	/*!
	 * @brief 获取交通管制RFID卡编号信息
	 * @param string& 交通管制RFID卡编号信息
	*/
	void GetCtrlRFID(std::string& strRFID);

	/*!
	 * @brief 获取交通管制RFID卡编号信息
	 * @param wstring& 交通管制RFID卡编号信息
	*/
	void GetCtrlRFID(std::wstring& wstrRFID);

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

	/*!
	 * @brief 获取比对区域集合
	 * @param string& 比对区域集合
	*/
	void GetCompareAreaList(std::string& strCompareList);

	/*!
	 * @brief 获取比对区域集合
	 * @param wstring& 比对区域集合
	*/
	void GetCompareAreaList(std::wstring& wstrCompareList);

	/*!
	 * @brief 设置比对区域集合
	 * @param string 比对区域集合
	*/
	void SetCompareAreaList(std::string strCompareList);

	/*!
	 * @brief 设置比对区域集合
	 * @param wstring 比对区域集合
	*/
	void SetCompareAreaList(std::wstring wstrCompareList);

#ifdef _AFX
	/*!
	 * @brief 获取交通管制之前RFID卡编号信息
	 * @param CString& 交通管制之前RFID卡编号信息
	*/
	void GetBeforeRFID(CString& cstrRFID);

	/*!
	 * @brief 获取交通管制RFID卡编号信息
	 * @param CString& 交通管制RFID卡编号信息
	*/
	void GetCtrlRFID(CString& cstrRFID);

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

	/*!
	 * @brief 获取比对区域集合
	 * @param CString& 比对区域集合
	*/
	void GetCompareAreaList(CString& cstrCompareList);

	/*!
	 * @brief 设置比对区域集合
	 * @param CString 比对区域集合
	*/
	void SetCompareAreaList(CString cstrCompareList);
#endif //!_AFX
};

typedef class CMagneticTrackTrafficControlArea CMTTrafficCtrlArea;

#endif // !_MAGNETICTRACK_H