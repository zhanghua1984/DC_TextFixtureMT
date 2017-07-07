// TextFixture.h : main header file for the TEXTFIXTURE application
//

#if !defined(AFX_TEXTFIXTURE_H__3E3EE068_3C52_4BF8_A69B_9B664D28CAC7__INCLUDED_)
#define AFX_TEXTFIXTURE_H__3E3EE068_3C52_4BF8_A69B_9B664D28CAC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureApp:
// See TextFixture.cpp for the implementation of this class
//

class CTextFixtureApp : public CWinApp
{
public:
	CTextFixtureApp();
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextFixtureApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTextFixtureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HANDLE	m_mutex;
};
extern CTextFixtureApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTFIXTURE_H__3E3EE068_3C52_4BF8_A69B_9B664D28CAC7__INCLUDED_)
