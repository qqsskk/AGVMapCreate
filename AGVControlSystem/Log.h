/*!
 * @file Log.h
 * @brief 系统日志文件
 * @author 范凯毓
 * @date 2018-10-10
 * @version 2.0
*/

#pragma once
#include <string>
#include <time.h>
#include <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include <string>

/*!
* @brief 系统输出日志
*/
static char g_recordFilePath[] = "..\\Log";
static const int g_nFileLen = 1024;

/*!
* @brief 日志文件检测
* @return bool 存在返回true,不存在返回false
*/
static bool FindRecordFile(char *lpszPath)
{
	char *lpszFile = new char[g_nFileLen];
	memset(lpszFile, 0, sizeof(g_nFileLen));
	strcpy_s(lpszFile, g_nFileLen, lpszPath);
	
	/*
	* 获取系统当前时间
	*/
	time_t t = time(0);
	tm tm;
	localtime_s(&tm,&t);

	/*
	* 合成文件名
	*/
	sprintf_s(&lpszFile[strlen(lpszPath)], g_nFileLen - strlen(lpszPath), "\\%04d%02d%02d_系统日志.log", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	if (::_access(lpszFile, 0) == -1)
	{
		delete[] lpszFile;
		return false;
	}

	delete[] lpszFile;

	return true;

}


/*!
* @brief 日志文件创建
* @return bool 创建成功返回true,失败返回false
*/
static bool CreateRecordFile(char *lpszPath)
{
	char *lpszFile = new char[g_nFileLen];
	memset(lpszFile, 0, sizeof(g_nFileLen));

	sprintf_s(lpszFile, g_nFileLen, "md %s", lpszPath);
	
	system(lpszFile);

	memset(lpszFile, 0, sizeof(g_nFileLen));
	strcpy_s(lpszFile, g_nFileLen, lpszPath);

	/*
	* 获取系统当前时间
	*/
	time_t t = time(0);
	tm tm;
	localtime_s(&tm, &t);

	/*
	* 合成文件名
	*/
	sprintf_s(&lpszFile[strlen(lpszPath)], g_nFileLen - strlen(lpszPath), "\\%04d%02d%02d_系统日志.log", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	// 创建文件
	FILE* pReadFile = nullptr;  /*!< 打开文件*/

	fopen_s(&pReadFile, lpszFile, "w+");

	if (pReadFile == nullptr)
	{
		delete[] lpszFile;
		return false;
	}

	fclose(pReadFile);

	delete[] lpszFile;

	return true;
}


/*!
* @brife 删除日志文件
* @param nDays 天数
*/
static bool ClearRecord(int nDays)
{
	/*
	* 获取系统当前时间
	*/
	time_t t = time(0);
	tm* pTm = nullptr;
	localtime_s(pTm, &t);

	char *lpszFile = new char[g_nFileLen];
	memset(lpszFile, 0, sizeof(g_nFileLen));

	strcpy_s(lpszFile, g_nFileLen, g_recordFilePath);
	strcpy_s(&lpszFile[strlen(g_recordFilePath)], g_nFileLen, "\\*.log");

	//char fileName[2000];

	struct _finddata_t fileInfo;

	long findResult = _findfirst(lpszFile, &fileInfo);
	if (findResult == -1)
	{
		_findclose(findResult);
		return false;
	}
	bool flag = 0;

	do
	{
		int nDisDay = difftime(t, fileInfo.time_create);
		nDisDay = nDisDay / (24 * 60 * 60);

		if (nDisDay >= nDays)
		{
			memset(lpszFile, 0, sizeof(g_nFileLen));
			sprintf_s(lpszFile, g_nFileLen, "%s\\%s", g_recordFilePath,fileInfo.name);
			remove(lpszFile);
		}
		

	} while (!_findnext(findResult, &fileInfo));

	_findclose(findResult);

	delete[] lpszFile;

	return true;
}


/*!
* @brief 记录日志
* @param string
* @return bool 成功返回true,失败返回false
*/
static bool Record(std::string strText)
{
	if (FindRecordFile(g_recordFilePath) == false)
	{
		if (CreateRecordFile(g_recordFilePath) == false)
		{
			return false;
		}
	}

	char *lpszFile = new char[g_nFileLen];
	memset(lpszFile, 0, sizeof(g_nFileLen));
	strcpy_s(lpszFile, g_nFileLen, g_recordFilePath);

	/*
	* 获取系统当前时间
	*/
	time_t t = time(0);
	tm tm;
	localtime_s(&tm, &t);

	/*
	* 合成文件名
	*/
	sprintf_s(&lpszFile[strlen(g_recordFilePath)], g_nFileLen - strlen(g_recordFilePath), "\\%04d%02d%02d_系统日志.log", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	/*
	* 打开文件
	*/
	FILE* pReadFile = nullptr; 

	fopen_s(&pReadFile, lpszFile, "a+");

	if (pReadFile == nullptr)
	{
		delete[] lpszFile;
		return false;
	}

	delete[] lpszFile;

	/*
	* 定位至文件尾部
	*/
	fseek(pReadFile, 0, SEEK_END);

	char *lpszRecord = new char[4096];
	memset(lpszRecord, 0, sizeof(4096));

	sprintf_s(lpszRecord, 4096, "[%d/%d/%d %d:%d:%d]\r\n->>%s\r\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, strText.c_str());

	fwrite(lpszRecord, sizeof(char), strlen(lpszRecord), pReadFile);

	fclose(pReadFile);

	delete[] lpszRecord;

	return true;
}

/*!
* @brief 记录日志
* @param clpszFormat 字符串格式化信息
* @return bool 成功返回true,失败返回false
*/
static bool Record(const char *clpszFormat, ...)
{
	if (FindRecordFile(g_recordFilePath) == false)
	{
		if (CreateRecordFile(g_recordFilePath) == false)
		{
			return false;
		}
	}

	char *lpszFile = new char[g_nFileLen];
	memset(lpszFile, 0, sizeof(g_nFileLen));
	strcpy_s(lpszFile, g_nFileLen,g_recordFilePath);

	/*
	* 获取系统当前时间
	*/
	time_t t = time(0);
	tm tm;
	localtime_s(&tm, &t);

	/*
	* 合成文件名
	*/
	sprintf_s(&lpszFile[strlen(g_recordFilePath)], g_nFileLen - strlen(g_recordFilePath),"\\%04d%02d%02d_系统日志.log", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	/*
	* 打开文件
	*/
	FILE* pReadFile = nullptr;

	fopen_s(&pReadFile, lpszFile, "a+");

	if (pReadFile == nullptr)
	{
		delete[] lpszFile;
		return false;
	}

	delete[] lpszFile;

	char achBuf[4096]; /*!< 调试信息*/

	// 初始化
	memset(achBuf, 0, 4096);

	// 格式化
	va_list ap;
	va_start(ap, clpszFormat);
	_vsnprintf_s(achBuf, sizeof(achBuf), clpszFormat, ap);
	va_end(ap);

	/*
	* 定位至文件尾部
	*/
	fseek(pReadFile, 0, SEEK_END);

	char *lpszRecord = new char[4096];
	memset(lpszRecord, 0, sizeof(4096));

	sprintf_s(lpszRecord, 4096, "[%d/%d/%d %d:%d:%d]\r\n->>%s.\r\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, achBuf);

	fwrite(lpszRecord, sizeof(char), strlen(lpszRecord), pReadFile);

	fclose(pReadFile);

	delete[] lpszRecord;

	return true;
}