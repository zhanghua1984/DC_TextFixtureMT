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
#define VC_EXTRALEAN								// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER										// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0400								//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT								// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400							//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

//#ifndef _WIN32_WINDOWS							// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
//#define _WIN32_WINDOWS 0x0410						//Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
//#endif

#ifndef _WIN32_IE									// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400							//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS			// ĳЩ CString ���캯��������ʽ��


#define _AFX_ALL_WARNINGS							// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC �׽�����չ
#define  SATS_UVER			1.0				//2012.06.01

#define TIMER				100
#define TIMER_EVENT			1000

//ͨ�������ļ�·��
#define CONFIG_FILE_PATH	".\\CONFIG.INI";

const CString SERIAL_PORT	= "COM2";
const CString SERIAL_VERIFY	= "N";
#define SERIAL_BTR			115200
#define SERIAL_DATA_BIT		8
#define SERIAL_STOP_BIT		1

#define SERIAL_DATA_LENGTH	8
//��Ϣ
#define WM_MSG_CERIALREVC WM_USER+1
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8CDCB709_1274_4A71_93FE_8446C70721A8__INCLUDED_)
