// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8CDCB709_1274_4A71_93FE_8446C70721A8__INCLUDED_)
#define AFX_STDAFX_H__8CDCB709_1274_4A71_93FE_8446C70721A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN								// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER										// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0400								//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif

#ifndef _WIN32_WINNT								// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0400							//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif						

//#ifndef _WIN32_WINDOWS							// 允许使用 Windows 98 或更高版本的特定功能。
//#define _WIN32_WINDOWS 0x0410						//为 Windows Me 及更新版本改变为适当的值。
//#endif

#ifndef _WIN32_IE									// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0400							//为 IE 5.0 及更新版本改变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS			// 某些 CString 构造函数将是显式的


#define _AFX_ALL_WARNINGS							// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC 套接字扩展
#define  SATS_UVER			1.0				//2012.06.01

#define TIMER				100
#define TIMER_EVENT			1000

//通信配置文件路径
#define CONFIG_FILE_PATH	".\\CONFIG.INI";

const CString SERIAL_PORT	= "COM2";
const CString SERIAL_VERIFY	= "N";
#define SERIAL_BTR			115200
#define SERIAL_DATA_BIT		8
#define SERIAL_STOP_BIT		1

#define SERIAL_DATA_LENGTH	8
//消息
#define WM_MSG_CERIALREVC WM_USER+1
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8CDCB709_1274_4A71_93FE_8446C70721A8__INCLUDED_)
