// ADOConn.cpp: implementation of the ADOConn class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ADOConn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ADOConn::ADOConn()
{
	m_bstrConnect = "";

	m_bstrUser = "";

	m_bstrPassword = "";

	m_pConnection = nullptr;

	m_pRecordset = nullptr;

	CoInitialize(nullptr);
}


ADOConn::ADOConn(const ADOConn &cAdo)
{
	m_bstrConnect = cAdo.m_bstrConnect;

	m_bstrUser = cAdo.m_bstrUser;

	m_bstrPassword = cAdo.m_bstrPassword;

	m_pConnection = nullptr;

	m_pRecordset = nullptr;

	CoInitialize(nullptr);
}


ADOConn::~ADOConn()
{
	CoUninitialize();
}

bool ADOConn::OnInitADOConn()
{
	try
	{
		std::lock_guard<std::mutex> lock(m_mutexLock);

       m_pConnection.CreateInstance("ADODB.Connection");

	   if (m_pConnection == nullptr)
	   {
		   return false;
	   }

	   if (m_bstrConnect == _bstr_t(""))
	   {
		   DebugPrint("ADO连接串为空.\r\n");

		   return false;
	   }

	   m_pConnection->Open(m_bstrConnect, m_bstrUser, m_bstrPassword,adModeUnknown); // 连接数据库
	}
	catch(_com_error e)
	{
		DebugPrint(e.Description() + "\r\n");

		m_pConnection = nullptr;

		return false;
	}
	return true;
}


BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if (m_pConnection == nullptr) 
		{
			if (OnInitADOConn() == false)
			{
				return false;
			}
		}

		std::lock_guard<std::mutex> lock(m_mutexLock);

        m_pConnection->Execute(bstrSQL, nullptr,adCmdText);

		return true;
	}
	catch(_com_error e)
	{
		DebugPrint(e.Description() + "\r\n");

		ExitConnect();

		return false;
	}
}


void ADOConn::ExitConnect()
{
	std::lock_guard<std::mutex> lock(m_mutexLock);

	if (m_pRecordset != nullptr) 
	{
		m_pRecordset->Close();
		m_pRecordset = nullptr;
	}

	if (m_pConnection != nullptr)
	{
		m_pConnection->Close();
		m_pConnection = nullptr;
	}
}


_RecordsetPtr& ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		if (m_pConnection == nullptr) 
		{
			if (OnInitADOConn() == false) 
			{
				m_pRecordset = nullptr;
				return m_pRecordset;
			}
		}

		std::lock_guard<std::mutex> lock(m_mutexLock);

		if (m_pRecordset != nullptr)
		{
			m_pRecordset->Close();
			m_pRecordset = nullptr;
		}

		m_pRecordset.CreateInstance(__uuidof(Recordset));

        m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
    catch(_com_error e)
	{
		DebugPrint(e.Description() + "\r\n");

		m_pRecordset = nullptr;
	}
	return m_pRecordset;
}


void ADOConn::operator=(const ADOConn &cAdo)
{
	m_bstrConnect = cAdo.m_bstrUser;

	m_bstrUser = cAdo.m_bstrUser;

	m_bstrPassword = cAdo.m_bstrPassword;

	return;
}