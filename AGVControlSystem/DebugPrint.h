/*!
* @file DebugPrint.h
* @brief 输出调试信息
* @author FanKaiyu
* @date 2018-04-19
* @version 1.0
*/

#ifndef _DEBUGPRINT_H
#define _DEBUGPRINT_H
#pragma once
//#include <stdio.h>
#include <stdarg.h>
//#include <Windows.h>

static const int MAX_DBG_MSG_LEN = 4096;
static const int MAX_TIME_LEN = 255;

/*!
* @brief 调试信息输出
* @param clpszFormat 字符串格式化信息
*/
static void DebugPrint(const char *clpszFormat, ...)
{
	SYSTEMTIME sys; /*!< 系统时间*/
	char achBuf[MAX_DBG_MSG_LEN]; /*!< 调试信息*/
	char achStr[MAX_TIME_LEN + MAX_DBG_MSG_LEN]; /*!< 输出的字符串*/

	// 初始化
	memset(achBuf, 0, MAX_DBG_MSG_LEN);
	memset(achStr, 0, MAX_TIME_LEN + MAX_DBG_MSG_LEN);

	// 格式化
	va_list ap;
	va_start(ap, clpszFormat);
	_vsnprintf_s(achBuf, sizeof(achBuf), clpszFormat, ap);
	va_end(ap);

	// 获取系统当前时间
	GetLocalTime(&sys);
	wsprintfA(achStr, "%04d/%02d/%02d %02d:%02d:%02d %s.\r\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, achBuf);

	// 输出字符串
	OutputDebugStringA(achStr);
	

	return;
}

#endif // !_DEBUGPRINT_H