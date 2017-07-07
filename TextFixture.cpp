// TextFixture.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TextFixture.h"
#include "TextFixtureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureApp

BEGIN_MESSAGE_MAP(CTextFixtureApp, CWinApp)
	//{{AFX_MSG_MAP(CTextFixtureApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureApp construction

CTextFixtureApp::CTextFixtureApp()
{
//	EnableHtmlHelp();
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTextFixtureApp object

CTextFixtureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureApp initialization

BOOL CTextFixtureApp::InitInstance()
{
	m_mutex = CreateMutex(NULL,TRUE,"NetMutex");
	if(m_mutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			AfxMessageBox("程序已运行!");
			return TRUE;
		}
	}
	AfxEnableControlContainer();

	AfxEnableControlContainer();
	CWinApp::InitInstance();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTextFixtureDlg dlg;
	m_pMainWnd = &dlg;
	
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
