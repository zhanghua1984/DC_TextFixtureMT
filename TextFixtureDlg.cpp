// TextFixtureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TextFixture.h"
#include "TextFixtureDlg.h"
#include "ComData.h"
#include "math.h"
#include "afxmt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
double m_dstart;
float	m_fglobalspeed;
CString m_strgframecode;
CString m_strgsendframe1;
CString m_strgsendframe2;
double m_dtrackpos;
APRNode m_gaprnode[TRACKNUMBER];
pAPRNode m_gparpnode;
int m_framecounter;
CWinThread* pThread;
CWinThread* pThreadWriter;
CWinThread* pThreadRead;
HANDLE hCom;
OVERLAPPED m_overlap;
COMSTAT comstat;
BYTE	m_comrx[24];
BYTE	m_comtxstatus[24];
BYTE	m_comtxtcode[24];
BYTE	m_comtxheadt[24];
BYTE	m_comtxapr1[24];
BYTE	m_comtxapr2[24];

CEvent	m_eventcode;
CMutex	m_mutexcode;
CCriticalSection m_criticalSection;
BOOL	m_bSend;
int		m_gapoffset;
/*
//列车状态数据帧
CString m_cstypestatus="30";
//无源信标握手帧
CString	m_cstypenopowersignalhandle="70";
//无源信标数据帧
CString m_cstypenopowersigaldata="10";
//有源信标握手帧
CString m_cstypepowersignalhandle="80";
//有源信标数据帧
CString m_cstypepowersigaldata="20";
//轨道码数据
CString m_cstypetrackcode="40";
//头尾车
CString m_cstypeheadtrail="50";
*/
BOOL	m_bgSendstatus;		//是否发送
BOOL	m_bgSendNAPR;
BOOL	m_bgSendPAPR;
BOOL	m_bgSendTcode;
BOOL	m_bgSendHeadT;
BOOL	m_bgSendFStatus;
BOOL	m_bgSendFNAPR;
BOOL	m_bgSendFPAPR;
BOOL	m_bgSendFTcode;
BOOL	m_bgSendFHeadT;
//BOOL	m_bgSendTrackCode;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog 
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureDlg dialog

CTextFixtureDlg::CTextFixtureDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTextFixtureDlg::IDD, pParent),CMyDraw(&m_pListMsg,&m_pListAlarm)
{
	//{{AFX_DATA_INIT(CTextFixtureDlg)
	m_csspeed = _T("");
	m_csdirector = _T("");
	m_csdistance = _T("");
	m_cbmove = _T("");
	m_csacctimes = _T("");
	m_cswheeld = _T("");
	m_cscarriefrequency = _T("");
	m_cscodefrequency = _T("");
	m_cseditv = _T("");
	m_csreaderoffset = _T("");
	m_csautostop = _T("");
	m_csstart = _T("");
	m_csaprsenddistance = _T("");
	m_csfrusenddistance = _T("");
	m_cslogics = _T("");
	m_csworkstatus = _T("");
	m_csrestrictv = _T("");
	m_csreaddistance = _T("");
	m_csbeep = _T("");
	m_cschannel = _T("");
	m_cshealth = _T("");
	m_csantenna = _T("");
	m_csspeedlimit = _T("");
	m_csfirstbreakdistance = _T("");
	m_csfirstbreaktime = _T("");
	m_csslipbreakdistance = _T("");
	m_csslipbreaktime = _T("");
	m_cssecondbreakdistance = _T("");
	m_cssecondbreaktime = _T("");
	m_csbreakerror = _T("");
	m_cspostion = _T("");
	m_cstrackzoom = _T("");
	m_codeclearance = _T("");
	m_codeclearancedis = _T("");
	m_cspersent = _T("");
	m_csgapstoptime = _T("");
	m_cscode1 = _T("");
	m_cscode2 = _T("");
	m_cscarray1 = _T("");
	m_cscarray2 = _T("");
	m_strcom = _T("");
	m_bcheckkeepgap = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextFixtureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextFixtureDlg)
	DDX_Control(pDX, IDC_CHECKFORBIDSTATUS, m_cforbidstatus);
	DDX_Control(pDX, IDC_CHECKKEEPGAP, m_ccheckkeepgap);
	DDX_Control(pDX, IDC_CHECKFSBENABLE, m_ccheckfsbenable);
	DDX_Control(pDX, IDC_STATICATPM, m_catpm);
	DDX_Control(pDX, IDC_STATICOMPM, m_compm);
	DDX_Control(pDX, IDC_STATICCOM, m_ccomdisplay);
	DDX_Control(pDX, IDC_CHECKFSBEBENABLE, m_ccheckfsbebenble);
	DDX_Control(pDX, IDC_CODE2, m_ccode2);
	DDX_Control(pDX, IDC_CODE1, m_ccode1);
	DDX_Control(pDX, IDC_CARRAY2, m_carray2);
	DDX_Control(pDX, IDC_CARRAY1, m_carray1);
	DDX_Control(pDX, IDC_GAPSMODE, m_cgapsmode);
	DDX_Control(pDX, IDC_GAPSTOPTIME, m_cgapstoptime);
	DDX_Control(pDX, IDC_PERSENT, m_cpersent);
	DDX_Control(pDX, IDC_TACHOOVERHEAD, m_ctachooverhead);
	DDX_Control(pDX, IDC_TACHO2, m_tacho2);
	DDX_Control(pDX, IDC_TACHO1, m_tacho1);
	DDX_Control(pDX, IDC_GAPS1, m_gaps1);
	DDX_Control(pDX, IDC_FSK2, m_fsk2);
	DDX_Control(pDX, IDC_FSK1, m_fsk1);
	DDX_Control(pDX, IDC_CODECLEARANCEDIS, m_ccodeclearancedis);
	DDX_Control(pDX, IDC_CODECLEARANCE, m_ccodeclearance);
	DDX_Control(pDX, IDC_CHECKTECHOP2, m_cchecktechop2);
	DDX_Control(pDX, IDC_POSTION, m_cpostion);
	DDX_Control(pDX, IDC_EDITSPEEDLIMIT, m_cspeedlimit);
	DDX_Control(pDX, IDC_EDITANTENNA, m_cantenna);
	DDX_Control(pDX, IDC_HEALTH, m_chealth);
	DDX_Control(pDX, IDC_EUMR, m_ceumr);
	DDX_Control(pDX, IDC_TIRR, m_ctirr);
	DDX_Control(pDX, IDC_RFR2, m_crfr2);
	DDX_Control(pDX, IDC_RFR1, m_crfr1);
	DDX_Control(pDX, IDC_CHANNEL, m_cchannel);
	DDX_Control(pDX, IDC_BEEP, m_cbeep);
	DDX_Control(pDX, IDC_READDISTANCE, m_creaddistance);
	DDX_Control(pDX, IDC_RESTRICTV, m_crestrictv);
	DDX_Control(pDX, IDC_WORKSTATUS, m_cworkstatus);
	DDX_Control(pDX, IDC_READERPOWER, m_creaderpower);
	DDX_Control(pDX, IDC_READERNOPOWER, m_creadernopower);
	DDX_Control(pDX, IDC_LOGICS, m_clogics);
	DDX_Control(pDX, IDC_FRUSENDDISTANCE, m_cfrusenddistance);
	DDX_Control(pDX, IDC_NPARP2ID, m_cnpapr2id);
	DDX_Control(pDX, IDC_NPARP1ID, m_cnpapr1id);
	DDX_Control(pDX, IDC_APRSENDDISTANCE, m_caprsenddistance);
	DDX_Control(pDX, IDC_PAPR2, m_cpapr2);
	DDX_Control(pDX, IDC_PAPR1, m_cpapr1);
	DDX_Control(pDX, IDC_NPAPR2, m_cnpapr2);
	DDX_Control(pDX, IDC_NPARP1, m_cnpapr1);
	DDX_Control(pDX, IDC_TRACKSELECT, m_ctrackselect);
	DDX_Control(pDX, IDC_EDITSTART, m_cstart);
	DDX_Control(pDX, IDC_EDITSETSTOP, m_cautostop);
	DDX_Control(pDX, IDC_EDITREADEROFFSET, m_creaderoffset);
	DDX_Control(pDX, IDC_EDITSETV, m_ceditv);
	DDX_Control(pDX, IDC_STATICPIC, m_cstaticpicture);
	DDX_Control(pDX, IDC_COMBOCODEFREQUNCEY, m_ccodefrequency);
	DDX_Control(pDX, IDC_COMBOCARRIERFREQUENCY, m_ccarriefrequency);
	DDX_Control(pDX, IDC_COMBOWHEEL, m_cwheeld);
	DDX_Control(pDX, IDC_ACCTIMES, m_cacctimes);
	DDX_Control(pDX, IDC_CHECKTIR, m_checktir);
	DDX_Control(pDX, IDC_CHECKFSBRESULT, m_checkfsbresult);
	DDX_Control(pDX, IDC_CHECKZVRRESULT, m_checkzvrresult);
	DDX_Control(pDX, IDC_CHECKZVR2, m_checkzvr2);
	DDX_Control(pDX, IDC_CHECKZVR1, m_checkzvr1);
	DDX_Control(pDX, IDC_CHECKRDERRUSULT, m_checkrderrulult);
	DDX_Control(pDX, IDC_CHECKRDER2, m_checkrder2);
	DDX_Control(pDX, IDC_CHECKRDER1, m_checkrder1);
	DDX_Control(pDX, IDC_CHECKLDERRESULT, m_checklderresult);
	DDX_Control(pDX, IDC_CHECKLDER2, m_checklder2);
	DDX_Control(pDX, IDC_CHECKLDER1, m_checklder1);
	DDX_Control(pDX, IDC_CHECKFSB2, m_checkfsb2);
	DDX_Control(pDX, IDC_CHECKFSB1, m_checkfsb1);
	DDX_Control(pDX, IDC_CHECKEBRESULT, m_checkebresult);
	DDX_Control(pDX, IDC_CHECKEB2, m_checkeb2);
	DDX_Control(pDX, IDC_CHECKEB1, m_checkeb1);
	DDX_Control(pDX, IDC_CHECKTECHOP, m_checktechop);
	DDX_Control(pDX, IDC_CHECKATP2, m_checkatp2);
	DDX_Control(pDX, IDC_CHECKATP1, m_checkatp1);
	DDX_Control(pDX, IDC_CHECKATO, m_checkato);
	DDX_Control(pDX, IDC_CHECK9, m_check9);
	DDX_Control(pDX, IDC_CHECK8, m_check8);
	DDX_Control(pDX, IDC_CHECK7, m_check7);
	DDX_Control(pDX, IDC_CHECK6, m_check6);
	DDX_Control(pDX, IDC_CHECK5, m_check5);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK16, m_check16);
	DDX_Control(pDX, IDC_CHECK15, m_check15);
	DDX_Control(pDX, IDC_CHECK14, m_check14);
	DDX_Control(pDX, IDC_CHECK13, m_check13);
	DDX_Control(pDX, IDC_CHECK12, m_check12);
	DDX_Control(pDX, IDC_CHECK11, m_check11);
	DDX_Control(pDX, IDC_CHECK10, m_check10);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_RADIOTRIAL, m_radiotrial);
	DDX_Control(pDX, IDC_RADIOHEAD, m_radiohead);
	DDX_Control(pDX, IDC_COMBO_MOVE, m_ccbmove);
	DDX_Control(pDX, IDC_DISTANCE, m_cdistance);
	DDX_Control(pDX, IDC_DIRECTOR, m_cdirector);
	DDX_Control(pDX, IDC_COMBO_DIR, m_cbDir);
	DDX_Control(pDX, IDC_EDIT_SEPPD, m_eSpeed);
	DDX_Control(pDX, IDC_EDIT_ACC, m_eAcc);
	DDX_Control(pDX, IDC_LIST_MSG_2, m_pListAlarm);
	DDX_Control(pDX, IDC_LIST_MSG_1, m_pListMsg);
	DDX_Control(pDX, IDC_SLIDER_ACC, m_SldAcc);
	DDX_Text(pDX, IDC_EDIT_SEPPD, m_csspeed);
	DDX_Text(pDX, IDC_DIRECTOR, m_csdirector);
	DDX_Text(pDX, IDC_DISTANCE, m_csdistance);
	DDX_CBString(pDX, IDC_COMBO_MOVE, m_cbmove);
	DDX_Text(pDX, IDC_ACCTIMES, m_csacctimes);
	DDX_CBString(pDX, IDC_COMBOWHEEL, m_cswheeld);
	DDX_CBString(pDX, IDC_COMBOCARRIERFREQUENCY, m_cscarriefrequency);
	DDX_CBString(pDX, IDC_COMBOCODEFREQUNCEY, m_cscodefrequency);
	DDX_Text(pDX, IDC_EDITSETV, m_cseditv);
	DDX_Text(pDX, IDC_EDITREADEROFFSET, m_csreaderoffset);
	DDX_Text(pDX, IDC_EDITSETSTOP, m_csautostop);
	DDX_Text(pDX, IDC_EDITSTART, m_csstart);
	DDX_Text(pDX, IDC_APRSENDDISTANCE, m_csaprsenddistance);
	DDX_Text(pDX, IDC_FRUSENDDISTANCE, m_csfrusenddistance);
	DDX_Text(pDX, IDC_LOGICS, m_cslogics);
	DDX_Text(pDX, IDC_WORKSTATUS, m_csworkstatus);
	DDX_Text(pDX, IDC_RESTRICTV, m_csrestrictv);
	DDX_Text(pDX, IDC_READDISTANCE, m_csreaddistance);
	DDX_Text(pDX, IDC_BEEP, m_csbeep);
	DDX_Text(pDX, IDC_CHANNEL, m_cschannel);
	DDX_Text(pDX, IDC_HEALTH, m_cshealth);
	DDX_Text(pDX, IDC_EDITANTENNA, m_csantenna);
	DDX_Text(pDX, IDC_EDITSPEEDLIMIT, m_csspeedlimit);
	DDX_Text(pDX, IDC_FIRSTBREAKDISTANCE, m_csfirstbreakdistance);
	DDX_Text(pDX, IDC_FIRSTBREAKTIME, m_csfirstbreaktime);
	DDX_Text(pDX, IDC_SLIPBREAKDISTANCE, m_csslipbreakdistance);
	DDX_Text(pDX, IDC_SLIPBREAKTIME, m_csslipbreaktime);
	DDX_Text(pDX, IDC_SECONDBREAKDISTANCE, m_cssecondbreakdistance);
	DDX_Text(pDX, IDC_SECONDBREAKTIME, m_cssecondbreaktime);
	DDX_Text(pDX, IDC_BREAKERROR, m_csbreakerror);
	DDX_Text(pDX, IDC_POSTION, m_cspostion);
	DDX_Text(pDX, IDC_CODECLEARANCE, m_codeclearance);
	DDX_Text(pDX, IDC_CODECLEARANCEDIS, m_codeclearancedis);
	DDX_Text(pDX, IDC_PERSENT, m_cspersent);
	DDX_Text(pDX, IDC_GAPSTOPTIME, m_csgapstoptime);
	DDX_Text(pDX, IDC_STATICCOM, m_strcom);
	DDX_Check(pDX, IDC_CHECKKEEPGAP, m_bcheckkeepgap);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextFixtureDlg, CDialog)
	//{{AFX_MSG_MAP(CTextFixtureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SLIDER_ACC, OnOutofmemorySliderAcc)
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ROUTE, OnButtonRoute)
	ON_BN_CLICKED(IDC_RADIO_NOR, OnRadioNor)
	ON_BN_CLICKED(IDC_RADIO_TRY, OnRadioTry)
	ON_CBN_SELCHANGE(IDC_COMBO_DIR, OnCbnSelchangeComboDir)
	ON_BN_CLICKED(IDC_RADIOHEAD, OnRadiohead)
	ON_BN_CLICKED(IDC_RADIOTRIAL, OnRadiotrial)
	ON_CBN_SELCHANGE(IDC_COMBO_MOVE, OnSelchangeComboMove)
	ON_BN_CLICKED(IDC_BUTTONRELAYALLWORK, OnButtonrelayallwork)
	ON_BN_CLICKED(IDC_BUTTONRELAYALLFAIL, OnButtonrelayallfail)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECKATO, OnCheckato)
	ON_BN_CLICKED(IDC_CHECKATP1, OnCheckatp1)
	ON_BN_CLICKED(IDC_CHECKATP2, OnCheckatp2)
	ON_BN_CLICKED(IDC_CHECKTECHOP, OnChecktechop)
	ON_BN_CLICKED(IDC_NOPOWER, OnNopower)
	ON_BN_CLICKED(IDC_POWER, OnPower)
	ON_BN_CLICKED(IDC_TRACKCODE, OnTrackcode)
	ON_CBN_SELCHANGE(IDC_TRACKSELECT, OnSelchangeTrackselect)
	ON_BN_CLICKED(IDC_SAVEINIDATA, OnSaveinidata)
	ON_CBN_SELCHANGE(IDC_COMBOCARRIERFREQUENCY, OnSelchangeCombocarrierfrequency)
	ON_CBN_SELCHANGE(IDC_COMBOCODEFREQUNCEY, OnSelchangeCombocodefrequncey)
	ON_BN_CLICKED(IDC_SAVEINIDATATOFILE, OnSaveinidatatofile)
	ON_BN_CLICKED(IDC_READERPOWER, OnReaderpower)
	ON_BN_CLICKED(IDC_READERNOPOWER, OnReadernopower)
	ON_BN_CLICKED(IDC_BUTTONSETV, OnButtonsetv)
	ON_BN_CLICKED(IDC_BUTTONAUTOSTOP, OnButtonautostop)
	ON_BN_CLICKED(IDC_BUTTONREADEROFFSET, OnButtonreaderoffset)
	ON_BN_CLICKED(IDC_BUTTONSTART, OnButtonstart)
	ON_BN_CLICKED(IDC_BUTTONAPROFFSET, OnButtonaproffset)
	ON_BN_CLICKED(IDC_BUTTONCODEOFFSET, OnButtoncodeoffset)
	ON_BN_CLICKED(IDC_BUTTONLIMITV, OnButtonlimitv)
	ON_CBN_SELCHANGE(IDC_COMBOWHEEL, OnSelchangeCombowheel)
	ON_BN_CLICKED(IDC_BUTTONANTENNA, OnButtonantenna)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_CHECKTECHOP2, OnChecktechop2)
	ON_BN_CLICKED(IDC_FSK1, OnFsk1)
	ON_BN_CLICKED(IDC_FSK2, OnFsk2)
	ON_BN_CLICKED(IDC_TACHO1, OnTacho1)
	ON_BN_CLICKED(IDC_TACHO2, OnTacho2)
	ON_BN_CLICKED(IDC_SENDCODEGAPS, OnSendcodegaps)
	ON_BN_CLICKED(IDC_TACHOOVERHEAD, OnTachooverhead)
	ON_BN_CLICKED(IDC_GAPSMODE, OnGapsmode)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECKFSBEBENABLE, OnCheckfsbebenable)
	ON_BN_CLICKED(IDC_CHECKKEEPGAP, OnCheckkeepgap)
	ON_BN_CLICKED(IDC_CHECKFSBENABLE, OnCheckfsbenable)
	ON_BN_CLICKED(IDC_CHECKFORBIDSTATUS, OnCheckforbidstatus)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_COM,OnCOM)
	ON_MESSAGE(UM_COMWRITER,OnCOMWRITE)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureDlg message handlers

BOOL CTextFixtureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	ShowWindow(SW_SHOWMAXIMIZED);
	// TODO: Add extra initialization here
	//变量初始化
	m_zero="0";
	m_csframehead="FC0C0B0A";
	m_head=0;
	m_direct=0;
	m_framecounter=-1;
	m_rxlenth=0;
	m_gapoffset=0;
	LoadData();

	m_trackhead=TRUE;
	m_byteatoatp=0;
	m_bytetechop=0;
	m_bytedo1=0;
	m_bytedo2=0;

	InitProjetc();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTextFixtureDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTextFixtureDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		DrawBackGround(dc);
	}
	else
	{
		CPaintDC dc(this);
		DrawBackGround(dc);
		DrawShow(dc);
		DrawVtable(dc);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTextFixtureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTextFixtureDlg::OnOutofmemorySliderAcc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;
}

void CTextFixtureDlg::InitProjetc()
{
	m_nwheel=9;
	m_cforbidstatus.SetCheck(TRUE);
	m_bgSendstatus=FALSE;		//是否发送
	m_bgSendNAPR=FALSE;
	m_bgSendPAPR=FALSE;
	m_bgSendTcode=FALSE;
	m_bgSendHeadT=FALSE;
	m_bgSendFStatus=TRUE;
	m_bgSendFNAPR=TRUE;
	m_bgSendFPAPR=TRUE;
	m_bgSendFTcode=TRUE;
	m_bgSendFHeadT=TRUE;

	m_first=TRUE;
	m_odd=FALSE;
	m_SldAcc.EnableWindow(TRUE);
	int tosee=0;
	m_bools=FALSE;
	CString str;
	str.Format("软件启动成功!",SATS_UVER);
	AddMsg(str);
	m_codegap=FALSE;
	m_carraycode1=0;
	m_carraycode2=0;
	m_cbDir.AddString("上行");
	m_cbDir.AddString("下行");
	m_ccbmove.InsertString(0,"前进");
	m_ccbmove.InsertString(1,"后退");
	m_ccbmove.InsertString(2,"原位");
	m_ccbmove.SetCurSel(0);
	m_cbDir.SetCurSel(0);
	m_radiohead.SetCheck(1);
	m_csacctimes="1";
	m_cacctimes.SetWindowText(m_csacctimes);
	m_cwheeld.InsertString(0,"840.0");
	m_cwheeld.InsertString(1,"835.7");
	m_cwheeld.InsertString(2,"826.9");
	m_cwheeld.InsertString(3,"818.2");
	m_cwheeld.InsertString(4,"809.0");
	m_cwheeld.InsertString(5,"800.7");
	m_cwheeld.InsertString(6,"791.9");
	m_cwheeld.InsertString(7,"783.2");
	m_cwheeld.InsertString(8,"774.4");
	m_cwheeld.SetCurSel(0);
	
	m_carray1.InsertString(0,"F1:4080");
	m_carray1.InsertString(1,"F2:4320");
	m_carray1.InsertString(2,"F3:4560");
	m_carray1.InsertString(3,"F4:4800");
	m_carray1.InsertString(4,"F5:5040");
	m_carray1.InsertString(5,"F6:5280");
	m_carray1.InsertString(6,"F7:5520");
	m_carray1.InsertString(7,"F9:6000");
	m_carray1.InsertString(8,"FX:8000");
	m_carray1.SetCurSel(0);

	m_carray2.InsertString(0,"F1:4080");
	m_carray2.InsertString(1,"F2:4320");
	m_carray2.InsertString(2,"F3:4560");
	m_carray2.InsertString(3,"F4:4800");
	m_carray2.InsertString(4,"F5:5040");
	m_carray2.InsertString(5,"F6:5280");
	m_carray2.InsertString(6,"F7:5520");
	m_carray2.InsertString(7,"F9:6000");
	m_carray2.InsertString(8,"FX:8000");
	m_carray2.SetCurSel(0);

	m_ccarriefrequency.InsertString(0,"F1:4080");
	m_ccarriefrequency.InsertString(1,"F2:4320");
	m_ccarriefrequency.InsertString(2,"F3:4560");
	m_ccarriefrequency.InsertString(3,"F4:4800");
	m_ccarriefrequency.InsertString(4,"F5:5040");
	m_ccarriefrequency.InsertString(5,"F6:5280");
	m_ccarriefrequency.InsertString(6,"F7:5520");
	m_ccarriefrequency.InsertString(7,"F9:6000");
	m_ccarriefrequency.InsertString(8,"FX:8000");
	m_ccarriefrequency.InsertString(9,"错误码");
	m_ccarriefrequency.SetCurSel(0);

	m_ccode1.InsertString(0,"M1:28:74/73");
	m_ccode1.InsertString(1,"M2:32:74/58");
	m_ccode1.InsertString(2,"M3:36:59/58");
	m_ccode1.InsertString(3,"M4:40:59/37");
	m_ccode1.InsertString(4,"M5:44:38/37");
	m_ccode1.InsertString(5,"M6:48:38/27");
	m_ccode1.InsertString(6,"M7:52:NONE");
	m_ccode1.InsertString(7,"M8:56:38/0");
	m_ccode1.InsertString(8,"M9:60:28/27");
	m_ccode1.InsertString(9,"M10:64:28/19");
	m_ccode1.InsertString(10,"M11:68:28/0");
	m_ccode1.InsertString(11,"M12:72:NONE");
	m_ccode1.InsertString(12,"M13:76:20/0");
	m_ccode1.InsertString(13,"M14:80:0/0");
	m_ccode1.InsertString(14,"M15:100");
	m_ccode1.SetCurSel(0);

	m_ccode2.InsertString(0,"M1:28:74/73");
	m_ccode2.InsertString(1,"M2:32:74/58");
	m_ccode2.InsertString(2,"M3:36:59/58");
	m_ccode2.InsertString(3,"M4:40:59/37");
	m_ccode2.InsertString(4,"M5:44:38/37");
	m_ccode2.InsertString(5,"M6:48:38/27");
	m_ccode2.InsertString(6,"M7:52:NONE");
	m_ccode2.InsertString(7,"M8:56:38/0");
	m_ccode2.InsertString(8,"M9:60:28/27");
	m_ccode2.InsertString(9,"M10:64:28/19");
	m_ccode2.InsertString(10,"M11:68:28/0");
	m_ccode2.InsertString(11,"M12:72:NONE");
	m_ccode2.InsertString(12,"M13:76:20/0");
	m_ccode2.InsertString(13,"M14:80:0/0");
	m_ccode2.InsertString(14,"M15:100");
	m_ccode2.SetCurSel(0);

	m_ccodefrequency.InsertString(0,"M1:28:74/73");
	m_ccodefrequency.InsertString(1,"M2:32:74/58");
	m_ccodefrequency.InsertString(2,"M3:36:59/58");
	m_ccodefrequency.InsertString(3,"M4:40:59/37");
	m_ccodefrequency.InsertString(4,"M5:44:38/37");
	m_ccodefrequency.InsertString(5,"M6:48:38/27");
	m_ccodefrequency.InsertString(6,"M7:52:NONE");
	m_ccodefrequency.InsertString(7,"M8:56:38/0");
	m_ccodefrequency.InsertString(8,"M9:60:28/27");
	m_ccodefrequency.InsertString(9,"M10:64:28/19");
	m_ccodefrequency.InsertString(10,"M11:68:28/0");
	m_ccodefrequency.InsertString(11,"M12:72:NONE");
	m_ccodefrequency.InsertString(12,"M13:76:20/0");
	m_ccodefrequency.InsertString(13,"M14:80:0/0");
	m_ccodefrequency.InsertString(14,"M15:100");
	m_ccodefrequency.InsertString(15,"错误码");
	m_ccodefrequency.SetCurSel(0);
	
	m_checkzvr1.SetCheck(FALSE);
	m_checkzvr2.SetCheck(FALSE);
	m_checkzvrresult.SetCheck(FALSE);

	m_checkeb1.SetCheck(FALSE);
	m_checkeb2.SetCheck(FALSE);
	m_checkebresult.SetCheck(FALSE);
	
	m_checkfsb1.SetCheck(FALSE);
	m_checkfsb2.SetCheck(FALSE);
	m_checkfsbresult.SetCheck(FALSE);

	m_checklder1.SetCheck(FALSE);
	m_checklder2.SetCheck(FALSE);
	m_checklderresult.SetCheck(FALSE);

	m_checkrder1.SetCheck(FALSE);
	m_checkrder2.SetCheck(FALSE);
	m_checkrderrulult.SetCheck(FALSE);

	m_checktir.SetCheck(FALSE);


	m_bytebatter=0x00;
	m_creaderpower.SetCheck(TRUE);
	m_creadernopower.SetCheck(FALSE);

	m_SldAcc.SetRange(1,13);
	m_SldAcc.SetPos(9);
	m_direct=GOAHEAD;
	m_ebenable=FALSE;
	m_boolfsbenable=FALSE;
	m_bool=FALSE;
	m_first=TRUE;
	m_boolsetstart=TRUE;
	m_inttracklenth=m_dTrainPosition;
	m_dTrackStartPos=0;
	m_uintaprdistance=0;
	TrackStatus(m_dAcc, m_head, m_direct);

	if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFUP),_T("Gif")))
		m_cstaticpicture.Draw();

	CString m_str="";
	for (int i=1;i<TRACKNUMBER;i++)
	{
		m_str=m_stuDevs.stuDevs[0][i].szName;
		if (m_str!="NONE")
		{
			m_ctrackselect.InsertString(i-1,m_str);
		}
		else
		{
			m_ctrackselect.InsertString(i-1,"");
		}

	}

	m_cstrackzoom="5";

	m_send1or2=1;

	m_ctrackselect.SetCurSel(0);
	
	m_csreaderoffset="20000";

	m_creaderoffset.SetWindowText(m_csreaderoffset);

	m_csaprsenddistance="0";
	m_caprsenddistance.SetWindowText(m_csaprsenddistance);

	m_csantenna="0";
	m_cantenna.SetWindowText(m_csantenna);
	
	m_csstart="0";
	m_cstart.SetWindowText(m_csstart);

	m_csfrusenddistance="0";
	m_cfrusenddistance.SetWindowText(m_csfrusenddistance);
	
	m_csspeedlimit="90";
	m_cspeedlimit.SetWindowText(m_csspeedlimit);
	

	m_noverspeed=atoi(m_csspeedlimit);
	m_blimit=FALSE;

	m_ftranoffset=0;
	m_bytetechop2=0;
	m_bytetechop=0;
	//定时刷新
	SetTimer(1,2100,NULL);
	SetTimer(3,1000,NULL);
	/*
	m_bytetechop=m_bytetechop^1;
	m_bytetechop=m_bytetechop^2;
	HeadTrial();
	m_checktechop.SetCheck(TRUE);
	m_cchecktechop2.SetCheck(TRUE);
	*/
//	SetTimer(8,3000,NULL);
	InitAPRNode();
	m_gparpnode=m_gaprnode;
	OnButtonstart(); 
	/*
	CString m_strtemp;
	byte m_bytefru,m_bytecode;
	m_strtemp=m_stuDevs.stuDevs[0][1].dcarrie;
	m_strtemp=m_strtemp.Right(1);

	m_bytefru=atoi(m_strtemp);
			
	m_strtemp=m_stuDevs.stuDevs[0][1].dcode;

	m_bytecode=m_strtemp.GetLength();

	if (m_bytecode==2)
	{
		m_strtemp=m_strtemp.Right(1);
	} 
	else
	{
		m_strtemp=m_strtemp.Right(2);
	}

	m_bytecode=atoi(m_strtemp);

	TrackCode(m_bytefru,m_bytecode,m_stuDevs.stuDevs[0][1].dStartPara);
	*/
	if (OpenPort("COM5"))
	{
		setupdcb(115200);
		setuptimeout(0,0,0,0,0);
		PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT |	PURGE_TXABORT);
		//启动接收，发送线程
	//	pThread=AfxBeginThread(ThreadFunc,(LPVOID)GetSafeHwnd());
		pThreadWriter=AfxBeginThread(ThreadWrite,(LPVOID)GetSafeHwnd());
		pThreadRead=AfxBeginThread(ThreadRead,(LPVOID)GetSafeHwnd());
	} 
	else
	{
		AfxMessageBox("未找到可用串口!");
	}

}

void CTextFixtureDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	int n=m_SldAcc.GetPos();
	CString str;
	UINT n;
	n = m_SldAcc.GetPos();
		if (n!=m_accpos)
		{
			m_accpos=n;
			switch(n)
			{				
			case 1:
				str = "驾驶手柄改变 .EB";
				m_dAcc = -10;
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-10.0");
				break;
			case 2:
				str = "驾驶手柄改变 .制动7";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-7.0");
				m_dAcc = -7;
				break;
			case 3:
				str = "驾驶手柄改变 .制动6";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-6.0");
				m_dAcc = -6;
				break;
			case 4:
				str = "驾驶手柄改变 .制动5";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-5.0");
				m_dAcc = -5;
				break;
			case 5:
				str = "驾驶手柄改变 .制动4";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-4.0");
				m_dAcc = -4;
				break;
			case 6:
				str = "驾驶手柄改变 .制动3";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-3.0");
				m_dAcc = -3;
				break;
			case 7:
				str = "驾驶手柄改变 .制动2";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-2.0");
				m_dAcc = -2;
				break;
			case 8:
				str = "驾驶手柄改变 .制动1";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-1.0");
				m_dAcc = -1;
				break;
			case 9:
				str = "驾驶手柄改变 .空挡";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
				m_dAcc = 0;
				break;
			case 10:
				str = "驾驶手柄改变 .牵引1";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("1.0");
				m_dAcc = 1;
				break;
			case 11:
				str = "驾驶手柄改变 .牵引2";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("2.0");
				m_dAcc = 2;
				break;
			case 12:
				str = "驾驶手柄改变 .牵引3";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("3.0");
				m_dAcc = 3;
				break;
			case 13:
				str = "驾驶手柄改变 .牵引4";
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("4.0");
				m_dAcc = 4;
				break;
			default:
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
				m_dAcc = 0;
				break;
			}
			/*
			if (m_boolfsbenable!=FALSE)
			{
				m_dAcc=-7;
				str = "驾驶手柄改变 .制动7";
				m_SldAcc.SetPos(2);
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-7.0");
			}
			
			if (m_ebenable!=FALSE)
			{		
				str = "已启用紧急制动 .操作无效";
				m_dAcc = -10;
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-10.0");
				m_SldAcc.SetPos(1);
			} 
			*/
			AddMsg(str);
			TrackStatus(m_dAcc, m_head, m_direct);
		}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTextFixtureDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDEvent)
	{
	case 1:
		{	//列车显示状态
			CheckDisplay();
			break;
		}
	case 2:
		{
			//巡航定速功能
			SetCruise();
			break;
		}
	case 3:
		{
			CheckOverSpeed();
			break;
		}
	case 4:
		{
			AutoStop(m_ungloblespeed, m_stoppos, m_curpos);
			break;
		}
	case 5:
		{
			CheckSpeedZero();
			break;
		}
	case 6:
		{
			SencondBreak();
			break;
		}
	case 7:
		{
			CheckStop();
			break;
		}
	case 8:
		{
			m_bytetechop=m_bytetechop&0xfc;
			HeadTrial();
			m_checktechop.SetCheck(FALSE);
			m_cchecktechop2.SetCheck(FALSE);
			KillTimer(8);
			break;
		}
	case 10:
		{
			m_codegap=TRUE;
			TrackStatus(m_dAcc, m_head, m_direct);
			break;
		}
	default:
		{	
			break;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTextFixtureDlg::OnButtonRoute() 
{
	// TODO: Add your control notification handler code here

}



void CTextFixtureDlg::OnRadioNor() 
{
	// TODO: Add your control notification handler code here


}

void CTextFixtureDlg::OnRadioTry() 
{
	// TODO: Add your control notification handler code here


}

void CTextFixtureDlg::OnOK() 
{
	// TODO: Add extra validation here
	STARTUPINFO   stStartUpInfo;									//指定windows状态，标准句柄
	PROCESS_INFORMATION   procinfo;									//进程的信息 		
	memset(&stStartUpInfo,   0,   sizeof(STARTUPINFO)); 			//启动屏保 
	BOOL bRet =CreateProcess(NULL,									//该字符串指定要由已启动的进程执行的模块
								"C:\\ICS\\res\\logon.scr   -s " ,	//该字符串指定要由已启动的进程执行的命令行
								NULL,								//指向Win32 SECURITY_ATTRIBUTES结构,进程指定安全说明符
								NULL,								//指向Win32 SECURITY_ATTRIBUTES结构,进程的主线程指定安全说明符
								FALSE,								// true 调用进程中的每个可继承句柄都由已启动的进程继承
								NORMAL_PRIORITY_CLASS,				//标志控制已启动的进程的优先级类和行为
								NULL,								//指向新进程的环境块的指针
								NULL,								//该字符串指定进程当前目录的完整路径
								&stStartUpInfo,						//指向Win32 STARTUPINFOW结构，为已启动的进程的主窗口指定窗口站、桌面、标准句柄和外观。
								&procinfo);							//指向Win32 PROCESS_INFORMATION结构的指针，该结构指定有关要启动的进程的标识信息
	if(bRet)														// 不使用两个句柄，立刻关闭
	{	
		::CloseHandle (procinfo.hThread);
		::CloseHandle (procinfo.hProcess);
	}
	//CDialog::OnOK();
}

void CTextFixtureDlg::OnCbnSelchangeComboDir() 
{
	// TODO: Add your control notification handler code here

	if((m_cbDir.GetCurSel()+1)%2)			//	0下行 1 上行
	{
		m_bTrainHeadRight = FALSE;
		AddMsg("人工设置测速电机方向-下行");
		//TrackStatus(m_dAcc, m_head, m_direct);
	}
	else
	{
		m_bTrainHeadRight = TRUE;
		AddMsg("人工设置测速电机方向-上行");
		//TrackStatus(m_dAcc, m_head, m_direct);
	}	
}


void CTextFixtureDlg::TrackStatus(DOUBLE acc, int head, int direct)
{
	CString m_cstrackstatusframe;
	m_cstrackstatusframe=ComposeTrackStatus(acc, head, direct);
	SendFramebySerial(m_cstrackstatusframe);

}

CString CTextFixtureDlg::ComposeTrackStatus(double acc, int head, int direct)
{
	CString m_cstrackstatus;
	CString m_cstrackstatuscounter;
	CString m_cstrackstatusgear;
	int m_trackstatus=0;
	double	m_doubleacc=0;
	DWORD	m_dwordacc=0;
	m_cstrackstatus+=m_csframehead;

	m_trackstatus=0;
	//m_cstrackstatuscounter=itoa(m_trackstatus,&m_charcounter,16);
	m_cstrackstatuscounter.Format("%02X",m_trackstatus);
	/*
	if (m_trackstatus<=15)
	{
		m_cstrackstatuscounter=m_zero+m_cstrackstatuscounter;
	}
	*/
	m_cstrackstatus+=m_cstrackstatuscounter;

	m_cstrackstatus+=m_cstypestatus;

	m_trackstatus=(int)acc;

	switch (m_trackstatus)
	{
	case 0:
		{
			m_cstrackstatusgear="00";
			m_doubleacc=0;
			break;
		}
	case 1:
		{
			m_cstrackstatusgear="01";
			m_doubleacc=0.3;
			break;
		}
	case 2:
		{
			m_cstrackstatusgear="03";
			m_doubleacc=0.5;
			break;
		}
	case 3:
		{
			m_cstrackstatusgear="05";
			m_doubleacc=0.7;
			break;
		}
	case 4:
		{
			m_cstrackstatusgear="07";
			m_doubleacc=0.9;
			break;
		}
	case -1:
		{
			m_cstrackstatusgear="10";
			m_doubleacc=-0.1714285714285714285714285714;
			break;
		}
	case -2:
		{
			m_cstrackstatusgear="20";
			m_doubleacc=-0.3428571428571428571428571428;
			break;
		}
	case -3:
		{
			m_cstrackstatusgear="30";
			m_doubleacc=-0.5142857142857142857142857142;
			break;
		}
	case -4:
		{
			m_cstrackstatusgear="40";
			m_doubleacc=-0.6857142857142857142857142857;
			break;
		}	
	case -5:
		{
			m_cstrackstatusgear="50";
			m_doubleacc=-0.8571428571428571428571428571;
			break;
		}
	case -6:
		{
			m_cstrackstatusgear="60";
			m_doubleacc=-1.0285714285714285714285714285;
			break;
		}
	case -7:
		{
			m_cstrackstatusgear="70";
			m_doubleacc=-1.2;
			break;
		}
	case -10:
		{
			m_cstrackstatusgear="80";
			m_doubleacc=-1.4;
			break; 
		}
	default:
		{
			m_cstrackstatusgear="00";
			break;
		}
	}

	m_cstrackstatus+=m_cstrackstatusgear;

	m_cstrackstatus+=m_zero;
	m_cstrackstatus+=m_zero;


	if (head)
	{
		m_trackstatus=8;
	}
	else
	{
		m_trackstatus=16;
	}

	//方向

	m_trackstatus+=direct;
	m_cstrackstatusgear.Format("%02X",m_trackstatus);

	m_cstrackstatus+=m_cstrackstatusgear;


	//加速度

	m_cacctimes.GetWindowText(m_csacctimes);

	m_trackstatus=atoi(m_csacctimes);
	m_doubleacc=m_doubleacc*m_trackstatus;
	if (m_doubleacc>=0)
	{
		m_doubleacc=m_doubleacc*100;
		m_dwordacc=(DWORD)m_doubleacc;
		m_cstrackstatusgear.Format("%04X",m_dwordacc);
	} 
	else
	{

		m_doubleacc=m_doubleacc*100;
		m_doubleacc=-m_doubleacc;
		m_dwordacc=(DWORD)m_doubleacc;
		
		m_dwordacc=m_dwordacc|0x8000;

		m_cstrackstatusgear.Format("%04X",m_dwordacc);
	}
	

	if (m_cstrackstatusgear.GetLength())
	{
		m_cstrackstatus+=m_cstrackstatusgear.Right(2);
		m_cstrackstatusgear.Delete(2,2);
		m_cstrackstatus+=m_cstrackstatusgear;
	}
	else
	{
		AfxMessageBox("文件数据长度出错!");
	}


	//帧中间数据填充

	//即时载频码频

	if (m_cgapsmode.GetCheck())
	{
		m_cstrackstatusgear.Format("%02X",m_carraycode1);
		m_cstrackstatus+=m_cstrackstatusgear;
		
		m_cstrackstatusgear.Format("%02X",m_carraycode2);
		m_cstrackstatus+=m_cstrackstatusgear;
	}
	else
	{
		m_cstrackstatusgear="0000";
		m_cstrackstatus+=m_cstrackstatusgear;
	}



	if (m_tacho2.GetCheck())
	{
		m_cstrackstatusgear="0809";
		m_cstrackstatus+=m_cstrackstatusgear;

		
		m_cpersent.GetWindowText(m_cspersent);
		
		m_trackstatus=m_cspersent.GetLength();
		
		if (m_trackstatus>2)
		{
			AddMsg("输入百分比长度无效!");
			m_cstrackstatusgear="0000";
			m_cstrackstatus+=m_cstrackstatusgear;
		}
		else
		{
			m_trackstatus=atoi(m_cspersent);
			m_cstrackstatusgear.Format("%02X",m_trackstatus);
			if (m_ctachooverhead.GetCheck())
			{
				m_cstrackstatus+=m_cstrackstatusgear;
				m_cstrackstatusgear="00";
				m_cstrackstatus+=m_cstrackstatusgear;
			}
			else
			{
				m_cstrackstatus+="00";
				m_cstrackstatus+=m_cstrackstatusgear;
			
			}
		}
	}
	else
	{
		m_cstrackstatusgear="08091011";
		
		m_cstrackstatus+=m_cstrackstatusgear;
	}

	//码间隙

	if (m_codegap==TRUE)
	{
		if (m_bcheckkeepgap==TRUE)
		{
			m_codegap=TRUE;
			m_csgapstoptime="200";
		//	SetTimer(10,1000,NULL);
		} 
		else
		{
			m_codegap=FALSE;
			m_cgapstoptime.GetWindowText(m_csgapstoptime);
			KillTimer(10);
		}
		m_trackstatus=atoi(m_csgapstoptime);
		m_trackstatus+=m_gapoffset;
		if ((m_trackstatus>255)||(m_trackstatus<0))
		{
			m_cstrackstatusgear="00";
		} 
		else
		{
			m_cstrackstatusgear.Format("%02X",m_trackstatus);
		}
	} 
	else
	{
		m_cstrackstatusgear="00";
	}
	m_cstrackstatus+=m_cstrackstatusgear;
	//轮经值
	m_cstrackstatusgear.Format("%02X",m_nwheel);
	m_cstrackstatus+=m_cstrackstatusgear;

	m_cstrackstatusgear="14";
	
	m_cstrackstatus+=m_cstrackstatusgear;

	m_cstrackstatus+=m_cstrackstatuscounter;

	//测试状态
	m_cstrackstatus+=m_csstatustest;
	
	//CRC16
	m_cscrc16=CalculatestrCRC(m_cstrackstatus, 22);

	m_cstrackstatus+=m_cscrc16;

	return m_cstrackstatus;
}

CString CTextFixtureDlg::CalculatestrCRC(CString strtocrc, int lenth)
{
	CString m_crc16;
	//计算CRC16
	char strtochar[45]; 
	unsigned char * strchar;  //类型转换为CRC16函数可调用的类型
	unsigned short int  m_crc16result=0;      //CRC16 计算返回结果
	
	::strcpy(strtochar,strtocrc);
	
	strchar=(unsigned char *)strtochar;
	
	m_crc16result=Get_CRC16_Code(strchar, lenth);
	
	char m_itochar[4];
	
	char * m_pitochar;		//字符串数组无法直接转换为CString格式，先转换为字符串指针后直接负值即可
	
	itoa(m_crc16result,m_itochar,16);
	
	m_pitochar=m_itochar;
	
	m_crc16=m_pitochar;
	
	CString m_zero="0";
	
	int m_crclenth=m_crc16.GetLength();
	
	if (m_crclenth!=4)
	{
		switch (m_crclenth)
		{
		case 3:
			{
				m_crc16=m_zero+m_crc16;
				break;
			}
		case 2:
			{
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				break;
			}
		case 1:
			{
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				break;
			}
		case 0:
			{
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				m_crc16=m_zero+m_crc16;
				break;
			}
		default:
			{
				AfxMessageBox("CRC 计算错误!");
			}
		}
	}
	
	CString m_highcrc;
	CString m_lowcrc;
	CString m_crc;
	m_highcrc=m_crc16.Left(2);
	m_lowcrc=m_crc16.Right(2);
	
	m_crc=m_lowcrc+m_highcrc;
	
	m_crc16=m_crc;
	
	return m_crc16;
}

unsigned short int CTextFixtureDlg::Get_CRC16_Code(unsigned char* pchMsg, unsigned short int wDataLen)
{
	unsigned int CRC;
	unsigned char m_crc[22];
	BYTE m_low,m_high;
	BYTE m_word;
	//转换字符数组
	for (int i=0,j=0;i<44,j<22;i++,j++)
	{
		m_high=ConvertHexData(pchMsg[i++]);
		m_low=ConvertHexData(pchMsg[i]);
		m_word=m_high<<4;
		m_word+=m_low;
		m_crc[j]=m_word;
	}
	
	CRC  = crc16(m_crc, wDataLen);
	
	return CRC;
}

char CTextFixtureDlg::ConvertHexData(char ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30; //1的ASCII为0x31
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
    else return(-1);
}

void CTextFixtureDlg::SendFramebySerial(CString Frame)
{
	CString m_left2;
	int m_sendlenth;
	m_sendlenth=Frame.GetLength();
	if (m_sendlenth!=48)
	{
		AddMsg("发送帧错误!");
		return ;
	}
	
	CString m_strframe=Frame.GetAt(10);
	BYTE	m_byteframe=strtoul(m_strframe,NULL,16);
	switch (m_byteframe)
	{
	case 1:
		{
			m_strgsendframe2=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtxapr2[lenth]=strtoul(m_left2,NULL,16);
				
			}
			m_bgSendNAPR=TRUE;
			m_bgSendFNAPR=FALSE;
			break;
		}
	case 2:
		{
			/*
			m_strgsendframe2=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtx2[lenth]=strtoul(m_left2,NULL,16);
				
			}
			*/
			/*
			//列车状态数据帧
			CString m_cstypestatus="30";
			//无源信标握手帧
			CString	m_cstypenopowersignalhandle="70";
			//无源信标数据帧
			CString m_cstypenopowersigaldata="10";
			//有源信标握手帧
			CString m_cstypepowersignalhandle="80";
			//有源信标数据帧
			CString m_cstypepowersigaldata="20";
			//轨道码数据
			CString m_cstypetrackcode="40";
			//头尾车
			CString m_cstypeheadtrail="50";
			*/
			break;
		}
	case 3:
		{
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtxstatus[lenth]=strtoul(m_left2,NULL,16);
				
			}
			m_bgSendstatus=TRUE;
			m_bgSendFStatus=FALSE;
			break;
		}
	case 4:
		{
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtxtcode[lenth]=strtoul(m_left2,NULL,16);
				
			}
			m_bgSendTcode=TRUE;
			m_bgSendFTcode=FALSE;
			break;
		}
	case 5:
		{
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtxheadt[lenth]=strtoul(m_left2,NULL,16);
				
			}
			m_bgSendHeadT=TRUE;
			m_bgSendFHeadT=FALSE;
			break;
		}
	case 6:
		{
			
			break;
		}
	case 7:
		{
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtxapr1[lenth]=strtoul(m_left2,NULL,16);
				
			}

			break;
		}
	case 8:
		{
			/*
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtx[lenth]=strtoul(m_left2,NULL,16);
				
			}
			*/
			break;
		}
	default:
		{
			/*
			m_strgsendframe1=Frame;
			for (int lenth=0;lenth<24;lenth++)
			{
				m_left2=Frame.Left(2);
				
				Frame.Delete(0,2);
				
				m_comtx[lenth]=strtoul(m_left2,NULL,16);
				
			}
			*/
		}
	}




//	char SendOut[BUFFER];
//	int len;
//	BYTE m_array[24];

	//将CString转换为十六进制字符串
	
	

//	len=String2Hex(Frame,SendOut);



//	m_Port[0].WriteToPort(SendOut,len);

//	m_szSendArray.SetSize(24);
	//由数组的值形成发送数据
	/*
	static  bool firstSend = true;
	
	if (firstSend == true)
	{
		m_szSendArray.SetSize(array_length);
		firstSend = false;
	}
	
	for (unsigned int i = 0; i < 24; i++)
	{
		//m_szSendArray.Add((BYTE)array[i]);
		
		m_szSendArray.SetAt(i,m_array[i]);
	}
	*/
//	m_cmscom.SetOutBufferCount(0);
// 	m_cmscom.SetOutput(COleVariant(m_szSendArray));	

}



//字符转换为16进制显示方式
int CTextFixtureDlg::String2Hex(CString str, char *SendOut)
{
	int hexdata,lowhexdata; 
	int hexdatalen=0;
	int len=str.GetLength();
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr==' '||hstr=='\r'||hstr=='\n')
		{
			i++;
			
			continue;
		}
		i++;
		if (i>=len)
		{
			break;
		}
		lstr=str[i];
		hexdata=ConvertHexData(hstr);
		
		lowhexdata=ConvertHexData(lstr);
		
		if((hexdata==16)||(lowhexdata==16))
		{
			break;
		}
		else
		{
			hexdata=hexdata*16+lowhexdata;
		}
		
		i++;
		
		SendOut[hexdatalen]=(char)hexdata;
		
		hexdatalen++;
	}
	
	return hexdatalen;
}

void CTextFixtureDlg::HeadTrial()
{
	CString m_csheadtrial;
	m_csheadtrial=ComposeHeadTrial();
	SendFramebySerial(m_csheadtrial);

}

void CTextFixtureDlg::TrackCode(byte m_fru,byte m_code,unsigned int m_pos)
{
	CString m_cstrackcode;
	m_cstrackcode=ComposeTrackCode(m_fru,m_code,m_pos);
	SendFramebySerial(m_cstrackcode);

}

void CTextFixtureDlg::PowerSigal()
{
	CString m_cspsh;
	m_cspsh=ComposePowerSignalHandle();
	SendFramebySerial(m_cspsh);
	m_cspsh=ComposePowerSignalData();
	SendFramebySerial(m_cspsh);

}

void CTextFixtureDlg::NoPowerSigal(unsigned int m_pos,unsigned int m_id)
{
	CString m_csnops;
	m_csnops=ComposeNoPowerSignalHandle(m_pos);
	SendFramebySerial(m_csnops);
	m_csnops=ComposeNoPowerSignalData(m_id);
	SendFramebySerial(m_csnops);

}

CString CTextFixtureDlg::ComposeNoPowerSignalHandle(unsigned int m_pos)
{
	CString m_nopowersignalhandle;
	CString m_csnpsh;
	int m_nopsh;

	m_nopowersignalhandle=m_csframehead;
	m_nopsh=0;
	m_sendnumber=m_nopsh;

	m_csnpsh.Format("%02X",m_nopsh);

	m_nopowersignalhandle+=m_csnpsh;
	
	m_nopowersignalhandle+=m_cstypenopowersignalhandle;

	m_csnpsh.Format("%08x",m_pos);

	if (m_csnpsh.GetLength()==8)
	{
		m_nopowersignalhandle+=m_csnpsh.Right(2);
		
		m_csnpsh.Delete(6,2);
		
		m_nopowersignalhandle+=m_csnpsh.Right(2);
		
		m_csnpsh.Delete(4,2);
		
		m_nopowersignalhandle+=m_csnpsh.Right(2);
		
		m_csnpsh.Delete(2,2);
		
		m_nopowersignalhandle+=m_csnpsh.Right(2);
	}
	else
	{
		AfxMessageBox("字符串长度不为8!");
	}

	m_nopowersignalhandle+="05060708091011121314";

	m_csnpsh.Format("%02X",m_sendnumber);
	
	m_strgframecode=m_csnpsh;

	m_nopowersignalhandle+=m_csnpsh;

	m_nopowersignalhandle+="16";

	m_csnpsh=CalculatestrCRC(m_nopowersignalhandle, 22);

	m_nopowersignalhandle+=m_csnpsh;

	return m_nopowersignalhandle;
}

CString CTextFixtureDlg::ComposeNoPowerSignalData(unsigned int m_id)
{
	CString m_mark;
	CString m_nopowersignaldata;
	CString m_csnopsd;
	int		m_nopsd;
	byte	m_byte;
	m_nopowersignaldata=m_csframehead;
	m_nopsd=0;
	m_csnopsd.Format("%02X",m_nopsd);

	m_nopowersignaldata+=m_csnopsd;

	m_nopowersignaldata+=m_cstypenopowersigaldata;

	m_nopowersignaldata+="8100";

	//MARK解析

	//Mark28格式解析如下： 
	//	id = ((data[1] & 0x3f) << 22) | (data[2] << 14) | (data[3] << 6) | ((data[4] & 0xfc) >> 2);
	//m_id=0x01111111;
//	m_id=0x0c93951;
/*
	m_byte=m_id&0x000000ff;

	m_byte=m_byte<<2;

	m_csnopsd.Format("%02X",m_byte);

	m_nopowersignaldata+=m_csnopsd;

	m_id=m_id>>8;

	m_byte=m_id&0x000000ff;
	
	m_byte=m_byte<<2;
	
	m_csnopsd.Format("%02X",m_byte);
	
	m_nopowersignaldata+=m_csnopsd;
	
	m_id=m_id>>8;
	
	m_byte=m_id&0x000000ff;
	
	m_byte=m_byte<<2;
	
	m_csnopsd.Format("%02X",m_byte);
	
	m_nopowersignaldata+=m_csnopsd;

	m_id=m_id>>8;
	
	m_byte=m_id&0x000000ff;
	
	m_byte=m_byte<<2;
	
	m_csnopsd.Format("%02X",m_byte);
	
	*/
	m_byte = m_id & 0x0000003f;
	m_byte = m_byte <<2;
	m_csnopsd.Format("%02X",m_byte);
	
	m_mark=m_csnopsd; 

	m_id = m_id >> 6;
	m_byte = m_id & 0x000000ff;
	
	m_csnopsd.Format("%02X",m_byte);
	
	m_mark.Insert(0,m_csnopsd);

	m_id = m_id >> 8;
	m_byte = m_id & 0x000000ff;
	m_csnopsd.Format("%02X",m_byte);
	
		m_mark.Insert(0,m_csnopsd);

	m_id = m_id>>8;
	m_byte = m_id & 0x000000ff;	
	
	m_csnopsd.Format("%02X",m_byte);
	
	m_mark.Insert(0,m_csnopsd);

	m_nopowersignaldata+=m_mark;

	//电池状态
	m_csnopsd.Format("%02X",m_bytebatter);
	
	m_nopowersignaldata+=m_csnopsd;

	m_nopowersignaldata+="000000000000000000";

	m_csnopsd=CalculatestrCRC(m_nopowersignaldata, 22);

	m_nopowersignaldata+=m_csnopsd;

	return m_nopowersignaldata;
}

CString CTextFixtureDlg::ComposePowerSignalHandle()
{
	CString m_powersignalhandle;
	CString m_cspsh;
	int		m_psh;
	m_powersignalhandle=m_csframehead;

	m_psh=0;


	/*
	m_cspsh=itoa(m_psh,&m_charcounter,16);
	
	if (m_psh<=15)
	{
		m_cspsh=m_zero+m_cspsh;
	}
	*/

	m_cspsh.Format("%02X",m_psh);

	m_powersignalhandle+=m_cspsh;
	
	m_powersignalhandle+=m_cstypepowersignalhandle;

//	m_psh=(int)((m_dTrainPosition+50)*1000);

	if (UporDown())
	{
		m_psh=(int)((m_dTrainPosition+100)*1000);
	} 
	else
	{
		m_psh=(int)((m_dTrainPosition-100)*1000);
	}

	
	m_psh=abs(m_psh);

	m_cspsh.Format("%08x",m_psh);

	if (m_cspsh.GetLength()==8)
	{
		m_powersignalhandle+=m_cspsh.Right(2);
		
		m_cspsh.Delete(6,2);
		
		m_powersignalhandle+=m_cspsh.Right(2);
		
		m_cspsh.Delete(4,2);
		
		m_powersignalhandle+=m_cspsh.Right(2);
		
		m_cspsh.Delete(2,2);
		
		m_powersignalhandle+=m_cspsh.Right(2);
		
	}
	else
	{
		AfxMessageBox("数据长度不为8!");
	}


	
	m_powersignalhandle+="050607080910111213141516";
	
	m_cspsh=CalculatestrCRC(m_powersignalhandle, 22);
	
	m_powersignalhandle+=m_cspsh;


	return m_powersignalhandle;
}

CString CTextFixtureDlg::ComposePowerSignalData()
{
	CString m_powersignaldata;
	CString m_cspsd;
	int		m_psd;
	m_powersignaldata=m_csframehead;
	m_psd=0;

	m_cspsd.Format("%02X",m_psd);

	m_powersignaldata+=m_cspsd;
	
	m_powersignaldata+=m_cstypepowersigaldata;

	m_powersignaldata+="01020304050607080910111213141516";

	m_cspsd=CalculatestrCRC(m_powersignaldata,22);

	m_powersignaldata+=m_cspsd;

	return m_powersignaldata;
}

CString CTextFixtureDlg::ComposeHeadTrial()
{
	CString m_headtrial;
	CString m_csht;
	int		m_ht;
	m_headtrial=m_csframehead;
	m_ht=0;

	/*
	m_csht=itoa(m_ht,&m_charcounter,16);
	
	if (m_ht<=15)
	{
		m_csht=m_zero+m_csht;
	}
	*/

	m_csht.Format("%02X",m_ht);

	m_headtrial+=m_csht;
	
	m_headtrial+=m_cstypeheadtrail;

	m_csht.Format("%02X",m_byteatoatp);

	m_headtrial+=m_csht;


	m_csht.Format("%02X",m_bytetechop);

	m_headtrial+=m_csht;

	m_csht.Format("%02X",m_bytedo1);

	m_headtrial+=m_csht;

	m_csht.Format("%02X",m_bytedo2);

	m_headtrial+=m_csht;

	m_csht="05060708091011121314";
	
	m_headtrial+=m_csht;

	m_csht.Format("%02X",m_ht);
	m_headtrial+=m_csht;
	m_headtrial+="16";

	m_csht=CalculatestrCRC(m_headtrial, 22);

	m_headtrial+=m_csht;

	return m_headtrial;
}

CString CTextFixtureDlg::ComposeTrackCode(byte m_fru,byte m_code,unsigned int m_pos)
{
	CString m_trackcode;
	CString m_cstc;
	int		m_tc;
	m_trackcode=m_csframehead;
	m_tc=0;

	m_cstc.Format("%02X",m_tc);

	m_trackcode+=m_cstc;
	
	m_trackcode+=m_cstypetrackcode;
//		载频（Hz）	4080	4320	4560	4800	5040	5280	5520	6000
//		发送编号(16进制)	1	2	3	4	5	6	7	8
//		码频（Hz）	28	32	36	40	44	48	52	56	60	64	68	72	76	80
//		发送编号(16进制)	1	2	3	4	5	6	7	8	9	A	B	C	D	E
//		缺少码间隙
	
	m_cstc.Format("%02x",m_fru);

	m_trackcode+=m_cstc;

	m_cstc.Format("%02x",m_code);
	
	m_trackcode+=m_cstc;

	m_trackcode+="00";


	m_cstc.Format("%08x",m_pos);
	
	m_trackcode+=m_cstc.Right(2);
	
	m_cstc.Delete(6,2);
	
	m_trackcode+=m_cstc.Right(2);
	
	m_cstc.Delete(4,2);
	
	m_trackcode+=m_cstc.Right(2);
	
	m_cstc.Delete(2,2);
	
	m_trackcode+=m_cstc.Right(2);


	m_trackcode+="08091011121314";

	m_cstc.Format("%02X",m_tc);
	m_trackcode+=m_cstc;
	m_trackcode+="16";

	m_cstc=CalculatestrCRC(m_trackcode, 22);
	m_trackcode+=m_cstc;

	return m_trackcode;
}

void CTextFixtureDlg::OdometerVelocity(byte * frame)
{
	//里程处理部分
	int		m_nspeed;
	float m_fspeed=0;
	unsigned int m_speed=0;
	unsigned int m_speedc=0;
	double m_uintdistance=0;
	m_speed=m_comrx[9];
	m_speed = m_speed << 24;
	m_speedc=m_comrx[8];
	m_speedc = m_speedc << 16;
	m_speed+=m_speedc;
	m_speedc=m_comrx[7];
	m_speedc = m_speedc << 8;
	m_speed+=m_speedc;
	m_speed+=m_comrx[6];
	
//里程为4个字节为测速电机脉冲数寄存器表示脉冲的个数里程的计算为N*((PI*D)/130)（PI为π值D为论经值200为论经的齿数D=840mm）

	m_uintdistance=m_speed*((PI*840)/130);
	
	m_uintdistance=m_uintdistance/1000;

	if (m_bools==FALSE)
	{
		m_ddistance=m_uintdistance;
		m_bools=TRUE;
	}

	m_dTrainPosition=m_uintdistance-m_ddistance;

	//计算列车位置是否有变化
	if (Ischanged(m_dTrainPosition))
	{
		//计算是否发送APR和轨道码
		SendAPR(m_dTrainPosition+m_ftranoffset);
		if (!m_cgapsmode.GetCheck())
		{
			SendTrackCode(m_dTrainPosition+m_ftranoffset);
		}
		
	}
	
	m_cslogics.Format("%12.3lf",m_dTrainPosition);
	m_clogics.SetWindowText(m_cslogics);

	m_csdistance.Format("%12.3lf",m_uintdistance);

	m_cdistance.SetWindowText(m_csdistance);

	m_cspostion.Format("%12.3lf",m_dTrainPosition+m_ftranoffset);

	m_cpostion.SetWindowText(m_cspostion);
	//方向处理

	byte m_bytedirector=m_comrx[10];
	

	switch(m_bytedirector)
	{
	case 0xaa:
		{
			m_csdirector="前";
			m_cdirector.SetWindowText(m_csdirector);
			break;
		}
	case 0x55:
		{
			m_csdirector="后";
			m_cdirector.SetWindowText(m_csdirector);
			break;
		}
	default:
		{
			m_csdirector="无";
			m_cdirector.SetWindowText(m_csdirector);
			break;
		}
	}

	//速度处理

	m_speed=m_comrx[13];
	m_speed=m_speed << 8;
	m_speed+=m_comrx[12];

	m_fspeed=(float)m_speed;
	m_ungloblespeed=m_fspeed;
	m_fspeed=m_fspeed/100;

	m_dSpeed=m_fspeed;
	m_fglobalspeed=m_fspeed;

	m_nspeed=(int)m_fglobalspeed;
	if ((m_nspeed>=m_noverspeed)&&(m_blimit==FALSE))
	{
		m_dAcc=0;
		m_SldAcc.SetPos(9);
		TrackStatus(m_dAcc, m_head, m_direct);
		m_blimit=TRUE;
		SetTimer(3,5000,NULL);
	}

	m_csspeed.Format("%lf",m_fspeed);
	m_eSpeed.SetWindowText(m_csspeed);

		/*
		列车状态数据握手信息30
		头尾车数据握手信息50
		轨道码数据握手信息40
		无源信标握手信息70
		有源数据握手信息80
		*/

		if ((m_comtxstatus[0x4]==m_comrx[0x11])&&(m_bgSendFStatus==FALSE)&&(m_bgSendstatus==TRUE))
		{
			m_bgSendstatus=FALSE;
			m_bgSendFStatus=TRUE;

			if (!m_cforbidstatus.GetCheck())
			{
				CString m_strnum;
				CString m_strmsg;
				m_strmsg="序号:";
				m_strnum.Format("%d",m_comrx[0x11]);
				m_strmsg+=m_strnum;
				m_strmsg+="列车状态已返回!";
				AddAlarm(m_strmsg);
			}
			
		}

		if ((m_comtxheadt[0x4]==m_comrx[0x12])&&(m_bgSendFHeadT==FALSE)&&(m_bgSendHeadT==TRUE))
		{
			m_bgSendHeadT=FALSE;
			m_bgSendFHeadT=TRUE;
			
			CString m_strnum;
			CString m_strmsg;
			m_strmsg="序号:";
			m_strnum.Format("%d",m_comrx[0x12]);
			m_strmsg+=m_strnum;
			m_strmsg+="头尾车已返回!";
			AddAlarm(m_strmsg);
			
		}
		if ((m_comtxtcode[0x4]==m_comrx[0x13])&&(m_bgSendFTcode==FALSE)&&(m_bgSendTcode==TRUE))
		{
			m_bgSendTcode=FALSE;
			m_bgSendFTcode=TRUE;
			CString m_strnum;
			CString m_strmsg;
			m_strmsg="序号:";
			m_strnum.Format("%d",m_comrx[0x13]);
			m_strmsg+=m_strnum;
			m_strmsg+="轨道码已返回!";
			AddAlarm(m_strmsg);
		}
		if ((m_comtxapr1[0x4]==m_comrx[0x14])&&(m_bgSendFNAPR==FALSE)&&(m_bgSendNAPR==TRUE))
		{
			m_bgSendNAPR=FALSE;
			m_bgSendFNAPR=TRUE;
			
			CString m_strnum;
			CString m_strmsg;
			m_strmsg="序号:";
			m_strnum.Format("%d",m_comrx[0x13]);
			m_strmsg+=m_strnum;
			m_strmsg+="APR已返回!";
			AddAlarm(m_strmsg);
			
		}

	InvalidateRect(SurRect,FALSE);
	InvalidateRect(Vtable,FALSE);
}


void CTextFixtureDlg::OnRadiohead() 
{
	// TODO: Add your control notification handler code here
	m_radiohead.SetCheck(1);
	m_radiotrial.SetCheck(0);
	m_head=1;
	TrackStatus(m_dAcc, m_head, m_direct);
}

void CTextFixtureDlg::OnRadiotrial() 
{
	// TODO: Add your control notification handler code here
	m_radiohead.SetCheck(0);
	m_radiotrial.SetCheck(1);
	m_head=0;
	TrackStatus(m_dAcc, m_head, m_direct);
}

void CTextFixtureDlg::OnSelchangeComboMove() 
{
	// TODO: Add your control notification handler code here
	unsigned int	m_select=0;

	m_select=m_ccbmove.GetCurSel();
	
	switch(m_select)
	{
	case 0:
		{
			if ((1!=m_direct)&&(m_gparpnode->prior!=NULL)&&(m_gparpnode->next!=NULL))
			{
				m_gparpnode=m_gparpnode++;
			}

			m_direct=GOAHEAD;
			TrackStatus(m_dAcc, m_head, m_direct);
			AddMsg("人工设置列车运动方向-前进");
			break;
		}
	case 1:
		{
			if ((4!=m_direct)&&(m_gparpnode->prior!=NULL)&&(m_gparpnode->next->APRapos!=0))
			{
				m_gparpnode=m_gparpnode--;
			}
			m_direct=GOBACK;
			TrackStatus(m_dAcc, m_head, m_direct);
			AddMsg("人工设置列车运动方向-后退");
			break;
		}
	case 2:
		{
			m_direct=2;
			TrackStatus(m_dAcc, m_head, m_direct);
			AddMsg("人工设置列车运动方向-原地不动");
			break;
		}
	default:
		{
			m_direct=4;
			TrackStatus(m_dAcc, m_head, m_direct);
			AddMsg("无法处理的运动方向");
			break;
		}
	}


}

void CTextFixtureDlg::OnButtonrelayallwork() 
{
	// TODO: Add your control notification handler code here
//	m_byteatoatp=0;
//	m_bytetechop=0;
	m_bytedo1=0xff;
	m_bytedo2=0xff;

	m_check1.SetCheck(1);
	m_check2.SetCheck(1);
	m_check3.SetCheck(1);
	m_check4.SetCheck(1);
	m_check5.SetCheck(1);
	m_check6.SetCheck(1);
	m_check7.SetCheck(1);
	m_check8.SetCheck(1);
	m_check9.SetCheck(1);
	m_check10.SetCheck(1);
	m_check11.SetCheck(1);
	m_check12.SetCheck(1);
	m_check13.SetCheck(1);
	m_check14.SetCheck(1);
	m_check15.SetCheck(1);
	m_check16.SetCheck(1);

	AddAlarm("继电器全部励磁!");
	HeadTrial();
}

void CTextFixtureDlg::OnButtonrelayallfail() 
{
	// TODO: Add your control notification handler code here
	m_bytedo1=0;
	m_bytedo2=0;
	m_check1.SetCheck(0);
	m_check2.SetCheck(0);
	m_check3.SetCheck(0);
	m_check4.SetCheck(0);
	m_check5.SetCheck(0);
	m_check6.SetCheck(0);
	m_check7.SetCheck(0);
	m_check8.SetCheck(0);
	m_check9.SetCheck(0);
	m_check10.SetCheck(0);
	m_check11.SetCheck(0);
	m_check12.SetCheck(0);
	m_check13.SetCheck(0);
	m_check14.SetCheck(0);
	m_check15.SetCheck(0);
	m_check16.SetCheck(0);
	AddAlarm("继电器全部失磁!");
	HeadTrial();
}

void CTextFixtureDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	
	
	if (m_check1.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x01;
		AddAlarm("继电器1励磁!");

	} 
	else
	{
		m_bytedo1=m_bytedo1&0xfe;
		AddAlarm("继电器1失磁!");
	}
	
	HeadTrial();
}

void CTextFixtureDlg::OnCheck10() 
{
	// TODO: Add your control notification handler code here
	
	if (m_check10.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x02;
		AddAlarm("继电器10励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xfd;
		AddAlarm("继电器10失磁!");
	}
	
	HeadTrial();
}

void CTextFixtureDlg::OnCheck11() 
{
	// TODO: Add your control notification handler code here
	if (m_check11.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x04;
		AddAlarm("继电器11励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xfb;
		AddAlarm("继电器11失磁!");
	}

	HeadTrial();
}

void CTextFixtureDlg::OnCheck12() 
{
	// TODO: Add your control notification handler code here
	if (m_check12.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x08;
		AddAlarm("继电器12励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xf7;
		AddAlarm("继电器12失磁!");
	}

	HeadTrial();
}

void CTextFixtureDlg::OnCheck13() 
{
	// TODO: Add your control notification handler code here
	if (m_check13.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x10;
		AddAlarm("继电器13励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xef;
		AddAlarm("继电器13失磁!");
	}

	HeadTrial();
}

void CTextFixtureDlg::OnCheck14() 
{
	// TODO: Add your control notification handler code here

	if (m_check14.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x20;
		AddAlarm("继电器14励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xdf;
		AddAlarm("继电器14失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck15() 
{
	// TODO: Add your control notification handler code here

	if (m_check15.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x40;
		AddAlarm("继电器15励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xbf;
		AddAlarm("继电器15失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck16() 
{
	// TODO: Add your control notification handler code here

	if (m_check16.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x80;
		AddAlarm("继电器16励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0x7f;
		AddAlarm("继电器16失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck2() 
{
	// TODO: Add your control notification handler code here

	if (m_check2.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x02;
		AddAlarm("继电器2励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xfd;
		AddAlarm("继电器2失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck3() 
{
	// TODO: Add your control notification handler code here

	if (m_check3.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x04;
		AddAlarm("继电器3励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xfb;
		AddAlarm("继电器3失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck4() 
{
	// TODO: Add your control notification handler code here

	if (m_check4.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x08;
		AddAlarm("继电器4励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xf7;
		AddAlarm("继电器4失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck5() 
{
	// TODO: Add your control notification handler code here

	if (m_check5.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x10;
		AddAlarm("继电器5励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xef;
		AddAlarm("继电器5失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck6() 
{
	// TODO: Add your control notification handler code here

	if (m_check6.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x20;
		AddAlarm("继电器6励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xdf;
		AddAlarm("继电器6失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck7() 
{
	// TODO: Add your control notification handler code here

	if (m_check7.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x40;
		AddAlarm("继电器7励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0xbf;
		AddAlarm("继电器7失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck8() 
{
	// TODO: Add your control notification handler code here

	if (m_check8.GetCheck())
	{
		m_bytedo1=m_bytedo1|0x80;
		AddAlarm("继电器8励磁!");
	} 
	else
	{
		m_bytedo1=m_bytedo1&0x7f;
		AddAlarm("继电器8失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheck9() 
{
	// TODO: Add your control notification handler code here

	if (m_check9.GetCheck())
	{
		m_bytedo2=m_bytedo2|0x01;
		AddAlarm("继电器9励磁!");
	} 
	else
	{
		m_bytedo2=m_bytedo2&0xfe;
		AddAlarm("继电器9失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheckato() 
{
	// TODO: Add your control notification handler code here

	if (m_checkato.GetCheck())
	{
		m_byteatoatp=m_byteatoatp|0x04;
		AddAlarm("ATO继电器励磁!");
	} 
	else
	{
		m_byteatoatp=m_byteatoatp&0xfb;
		AddAlarm("ATO继电器失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheckatp1() 
{
	// TODO: Add your control notification handler code here

	if (m_checkatp1.GetCheck())
	{
		m_byteatoatp=m_byteatoatp|0x01;
		AddAlarm("ATP1继电器励磁!");
	} 
	else
	{
		m_byteatoatp=m_byteatoatp&0xfe;
		AddAlarm("ATP1继电器失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnCheckatp2() 
{
	// TODO: Add your control notification handler code here

	if (m_checkatp2.GetCheck())
	{
		m_byteatoatp=m_byteatoatp|0x02;
		AddAlarm("ATP2继电器励磁!");
	} 
	else
	{
		m_byteatoatp=m_byteatoatp&0xfd;
		AddAlarm("ATP2继电器失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnChecktechop() 
{
	// TODO: Add your control notification handler code here
	if (m_checktechop.GetCheck())
	{
		m_bytetechop=m_bytetechop^1;
		AddAlarm("测速电机1继电器励磁!");
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xfe;
		AddAlarm("测速电机1继电器失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnNopower() 
{
	// TODO: Add your control notification handler code here
	int a=1000;
	int b=10000000;

	NoPowerSigal(a,b);
}

void CTextFixtureDlg::OnPower() 
{
	// TODO: Add your control notification handler code here
	PowerSigal();
}

void CTextFixtureDlg::OnTrackcode() 
{
	// TODO: Add your control notification handler code here
//	TrackCode();
}

void CTextFixtureDlg::TrackStatusdisplay()
{
	CString str;
	BYTE m_byte;
	int		m_mclk=0;
	int		m_result=0;
	__int64	m_dresult=1000000000;

	//f= ( SIGNAL /50MCLK_NUM ) *20ns

	m_mclk=m_comrx[13];
	m_mclk=m_mclk<<8;
	m_mclk=m_mclk|m_comrx[12];
	m_mclk=m_mclk<<8;
	m_mclk=m_mclk|m_comrx[11];
	m_mclk=m_mclk<<8;
	m_mclk=m_mclk|m_comrx[10];
	if (m_mclk>0)
	{
		m_result=m_mclk*20;
		m_dresult=m_dresult*m_comrx[9];
		m_dresult=m_dresult/m_result;
	}
	else
	{
		m_dresult=0;
	}
	
	m_cshealth.Format("%d",m_dresult+1);
	m_chealth.SetWindowText(m_cshealth);

	


	//EB
	if (m_comrx[0xe]&0x10)
	{
		m_checkeb1.SetCheck(TRUE);
	} 
	else
	{
		m_checkeb1.SetCheck(FALSE);
	}
	if (m_comrx[0xe]&0x20)
	{
		m_checkeb2.SetCheck(TRUE);
	} 
	else
	{
		m_checkeb2.SetCheck(FALSE);
	}
	
	if (m_comrx[0xe]&0x40)
	{

		m_checkebresult.SetCheck(FALSE);	
		//紧急刹车
		
		if (!m_ccheckfsbebenble.GetCheck())
		{
			m_ebenable=TRUE;	

			str.Format("速度:%lf里程:%lf",m_fglobalspeed,m_dTrainPosition);
			str=str.Left(30);

			str += "启用紧急制动 .";
			
			m_dAcc = -10;
			GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-10.0");
			m_SldAcc.SetPos(1);
			
			AddMsg(str);
			TrackStatus(m_dAcc, m_head, m_direct);

		}

	} 
	else
	{
		m_checkebresult.SetCheck(TRUE);	
		if (!m_ccheckfsbebenble.GetCheck())
		{
			if(m_dAcc == -10)
			{
				m_ebenable=FALSE;	
				
				str = "已取消紧急制动 .";
				m_dAcc = 0;
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
				m_SldAcc.SetPos(9);
				
				AddMsg(str);
				TrackStatus(m_dAcc, m_head, m_direct);

			}
		}

	}

	//FSB
	if (m_comrx[0xe]&0x02)
	{
		m_checkfsb1.SetCheck(FALSE);
	} 
	else
	{
		m_checkfsb1.SetCheck(TRUE);
	}

	if (m_comrx[0xe]&0x04)
	{
		m_checkfsb2.SetCheck(FALSE);
	} 
	else
	{
		m_checkfsb2.SetCheck(TRUE);
	}

	if (m_comrx[0xe]&0x08)
	{
		m_checkfsbresult.SetCheck(TRUE);
		m_boolfsbenable=TRUE;
		if (!m_ccheckfsbenable.GetCheck())
		{
			if(m_dAcc==-7.0)
			{
				m_ebenable=FALSE;	
				
				str = "已取消制动7 .操作有效";
				m_dAcc = 0;
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
				m_SldAcc.SetPos(9);
				
				AddMsg(str);
				TrackStatus(m_dAcc, m_head, m_direct);
			}
		}
		
	}
	else
	{

		m_checkfsbresult.SetCheck(FALSE);

		if (!m_ccheckfsbenable.GetCheck())
		{
			if (m_dAcc!=-10)
			{
				m_dAcc=-7;
				str = "驾驶手柄改变 .制动7";
				m_SldAcc.SetPos(2);
				GetDlgItem(IDC_EDIT_ACC)->SetWindowText("-7.0");
				AddMsg(str);
				TrackStatus(m_dAcc, m_head, m_direct);
			}
		}

		
	}
	//TIR
	if (m_comrx[0xe]&0x01)
	{
		m_checktir.SetCheck(FALSE);
	} 
	else
	{
		m_checktir.SetCheck(TRUE);
	}
	//RDER
	if (m_comrx[0xf]&0x20)
	{
		m_checkrder1.SetCheck(TRUE);
	} 
	else
	{
		m_checkrder1.SetCheck(FALSE);
	}
	if (m_comrx[0xf]&0x40)
	{
		m_checkrder2.SetCheck(TRUE);
	} 
	else
	{
		m_checkrder2.SetCheck(FALSE);
	}
	if (m_comrx[0xf]&0x80)
	{
		m_checkrderrulult.SetCheck(FALSE);
	} 
	else
	{
		m_checkrderrulult.SetCheck(TRUE);
	}
	
	//LDER
	if (m_comrx[0xf]&0x04)
	{
		m_checklder1.SetCheck(TRUE);
	} 
	else
	{
		m_checklder1.SetCheck(FALSE);
	}
	if (m_comrx[0xf]&0x08)
	{
		m_checklder2.SetCheck(TRUE);
	} 
	else
	{
		m_checklder2.SetCheck(FALSE);
	}
	if (m_comrx[0xf]&0x10)
	{
		m_checklderresult.SetCheck(FALSE);
	} 
	else
	{
		m_checklderresult.SetCheck(TRUE);
	}
	//ZVR
	if (m_comrx[0xf]&0x01)
	{
		m_checkzvr1.SetCheck(TRUE);
	} 
	else
	{
		m_checkzvr1.SetCheck(FALSE);
	}

	if (m_comrx[0xf]&0x02)
	{
		m_checkzvr2.SetCheck(TRUE);
	} 
	else
	{
		m_checkzvr2.SetCheck(FALSE);
	}

	if (m_comrx[0xe]&0x80)
	{
		m_checkzvrresult.SetCheck(TRUE);
	} 
	else
	{
		m_checkzvrresult.SetCheck(FALSE);
	}

	//EUMR
	if (m_comrx[0x10]&0x01)
	{
		m_ceumr.SetCheck(TRUE);
	} 
	else
	{
		m_ceumr.SetCheck(FALSE);
	}
	//TIRR
	if (m_comrx[0x10]&0x02)
	{
		m_ctirr.SetCheck(TRUE);
	} 
	else
	{
		m_ctirr.SetCheck(FALSE);
	}

	if (m_comrx[0x10]&0x04)
	{
		m_crfr1.SetCheck(FALSE);
	} 
	else
	{
		m_crfr1.SetCheck(TRUE);
	}

	if (m_comrx[0x10]&0x08)
	{
		m_crfr2.SetCheck(FALSE);
	} 
	else
	{
		m_crfr2.SetCheck(TRUE);
	}




	m_byte=m_comrx[0x12]&0xc0;
	switch (m_byte)
	{
	case 0:
		{
			m_csworkstatus="无";
			m_cworkstatus.SetWindowText(m_csworkstatus);
			break;
		}
	case 0x80:
		{
			m_csworkstatus="正常工作";
			m_cworkstatus.SetWindowText(m_csworkstatus);
			break;
		}
	case 0x40:
		{
			m_csworkstatus="待机状态";
			m_cworkstatus.SetWindowText(m_csworkstatus);
			break;
		}
	case 0xc0:
		{
			m_csworkstatus="配置帧";
			m_cworkstatus.SetWindowText(m_csworkstatus);
			break;
		}
	default:
		{
			m_csworkstatus="错误信息";
			m_cworkstatus.SetWindowText(m_csworkstatus);
			break;
		}

	}
	
	m_byte=m_comrx[0x12]&0x30;
	switch (m_byte)
	{
	case 0:
		{
			m_csreaddistance="最小";
			m_creaddistance.SetWindowText(m_csreaddistance);
			break;
		}
	case 0x10:
		{
			m_csreaddistance="小";
			m_creaddistance.SetWindowText(m_csreaddistance);
			break;
		}
	case 0x20:
		{
			m_csreaddistance="大";
			m_creaddistance.SetWindowText(m_csreaddistance);
			break;
		}
	case 0x30:
		{
			m_csreaddistance="最大";
			m_creaddistance.SetWindowText(m_csreaddistance);
			break;
		}
	default:
		{
			m_csreaddistance="未知";
			m_creaddistance.SetWindowText(m_csreaddistance);
			break;
		}
	}

		m_byte=m_comrx[0x12]&0x08;

		switch (m_byte)
		{
		case 0:
			{
				m_csbeep="不蜂鸣";
				m_cbeep.SetWindowText(m_csbeep);
				break;
			}
		case 0x08:
			{
				m_csbeep="蜂鸣";
				m_cbeep.SetWindowText(m_csbeep);
				break;
			}
		default:
			{
				m_csbeep="未知";
				m_cbeep.SetWindowText(m_csbeep);
				break;
			}
		}
		

		m_cschannel.Format("%02X",m_comrx[0x13]);
		m_cchannel.SetWindowText(m_cschannel);


		str.Format("%02X",m_comrx[0x14]);
		m_csrestrictv.Format("%02X",m_comrx[0x15]);

		m_csrestrictv+=str;

		m_crestrictv.SetWindowText(m_csrestrictv);

		float m_version;

		if (m_comrx[0x11]&0x80)
		{
			m_version=(float)m_comrx[0x11]-128;
			m_version=m_version/10;
			str.Format("%f",m_version);
			m_compm.SetWindowText(str.Left(3));
		}
		else
		{
			m_version=(float)m_comrx[0x11];
			m_version=m_version/10;
			str.Format("%f",m_version);
			m_catpm.SetWindowText(str.Left(3));
		}

}

BOOL CTextFixtureDlg::UporDown()
{
	if (FALSE==m_bool)
	{
		m_bool=TRUE;
		if (m_dTrainPosition>=m_inttracklenth)
		{
			m_inttracklenth=m_dTrainPosition;
			return TRUE;
		} 
		else
		{
			m_inttracklenth=m_dTrainPosition;
			return FALSE;
		}
	} 
	else
	{
		m_bool=FALSE;

		if (m_dTrainPosition>=m_inttracklenth)
		{
			m_inttracklenth=m_dTrainPosition;
			return TRUE;
		} 
		else
		{
			m_inttracklenth=m_dTrainPosition;
			return FALSE;
		}
	}
}

void CTextFixtureDlg::CheckDisplay()
{
	//上行条件
	if (m_bTrainHeadRight==FALSE)
	{	
		//前进后退
		if (m_direct==GOAHEAD)
		{
			//速度>0?
			if (m_dSpeed>0)
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFUPMOVING),_T("Gif")))
					m_cstaticpicture.Draw();
			} 
			else
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFUP),_T("Gif")))
					m_cstaticpicture.Draw();
			}
		} 
		else if(m_direct==GOBACK)
		{
			//速度>0?
			if (m_dSpeed>0)
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFUPBACKMOVING),_T("Gif")))
					m_cstaticpicture.Draw();
			} 
			else
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFUP),_T("Gif")))
					m_cstaticpicture.Draw();
			}
		}

	} 
	else
	{
		//前进后退
		if (m_direct==GOAHEAD)
		{
			//速度>0?
			if (m_dSpeed>0)
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFDOWNMOVING),_T("Gif")))
					m_cstaticpicture.Draw();
			} 
			else
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFDOWN),_T("Gif")))
					m_cstaticpicture.Draw();
			}
		} 
		else if(m_direct==GOBACK)
		{
			//速度>0?
			if (m_dSpeed>0)
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFDOWNBACKMOVING),_T("Gif")))
					m_cstaticpicture.Draw();
			} 
			else
			{
				if (m_cstaticpicture.Load(MAKEINTRESOURCE(IDR_GIFDOWN),_T("Gif")))
					m_cstaticpicture.Draw();
			}
		}
	}

}

void CTextFixtureDlg::OnSelchangeTrackselect() 
{
	// TODO: Add your control notification handler code here
	unsigned int m_select=0;
	CString m_csselect;
	CString m_str;
	CString m_csconvert;

	m_select=m_ctrackselect.GetCurSel();
	m_ctrackselect.GetLBText(m_select,m_csselect);

	for (int i=1;i<31;i++)
	{
		m_str=m_stuDevs.stuDevs[0][i].szName;
		if (m_str==m_csselect)
		{
			m_select=m_stuDevs.stuDevs[0][i].dnp1apr;
			m_csconvert.Format("%d",m_select);
			m_cnpapr1.SetWindowText(m_csconvert);
			
			m_select=m_stuDevs.stuDevs[0][i].dnp2apr;
			m_csconvert.Format("%d",m_select);
			m_cnpapr2.SetWindowText(m_csconvert);

			m_select=m_stuDevs.stuDevs[0][i].dp1apr;
			m_csconvert.Format("%d",m_select);
			m_cpapr1.SetWindowText(m_csconvert);

			m_select=m_stuDevs.stuDevs[0][i].dp2apr;
			m_csconvert.Format("%d",m_select);
			m_cpapr2.SetWindowText(m_csconvert);

			m_select=m_stuDevs.stuDevs[0][i].np1aprid;
			m_csconvert.Format("%d",m_select);
			m_cnpapr1id.SetWindowText(m_csconvert);

			m_select=m_stuDevs.stuDevs[0][i].np2aprid;
			m_csconvert.Format("%d",m_select);
			m_cnpapr2id.SetWindowText(m_csconvert);
			
			m_csconvert=m_stuDevs.stuDevs[0][i].dcode;
			m_cscodefrequency=m_csconvert;
			m_ccodefrequency.SetWindowText(m_csconvert);

			m_csconvert=m_stuDevs.stuDevs[0][i].dcarrie;
			m_cscarriefrequency=m_csconvert;
			m_ccarriefrequency.SetWindowText(m_csconvert);



			m_csconvert=m_stuDevs.stuDevs[0][i].codeclearance;
		
			m_ccodeclearance.SetWindowText(m_csconvert);

			m_select=m_stuDevs.stuDevs[0][i].ndcodeclearance;

			m_csconvert.Format("%d",m_select);

			m_ccodeclearancedis.SetWindowText(m_csconvert);


			break;

		}	
	}

}

void CTextFixtureDlg::OnSaveinidata() 
{
	// TODO: Add your control notification handler code here
	
	unsigned int m_select=0;
	CString m_csselect;
	CString m_str;
	CString m_csconvert;
	char   m_char[5]; 
	m_select=m_ctrackselect.GetCurSel();
	m_ctrackselect.GetLBText(m_select,m_csselect);
	
	for (int i=0;i<15;i++)
	{
		m_str=m_stuDevs.stuDevs[0][i].szName;
		if (m_str==m_csselect)
		{
			/*
			BOOL WritePrivateProfileString(
				LPCTSTR lpAppName,  // pointer to section name
				LPCTSTR lpKeyName,  // pointer to key name
				LPCTSTR lpString,   // pointer to string to add
				LPCTSTR lpFileName  // pointer to initialization filename
				);
			*/

			m_cnpapr1.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].dnp1apr=m_select;
			
			m_cnpapr2.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].dnp2apr=m_select;

			m_cpapr1.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].dp1apr=m_select;

			m_cpapr2.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].dp2apr=m_select;

			m_cnpapr1id.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].np1aprid=m_select;

			m_cnpapr2id.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].np2aprid=m_select;


			m_ccodeclearancedis.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);
			m_stuDevs.stuDevs[0][i].ndcodeclearance=m_select;


			memset(m_char,0,sizeof(m_char)); 
			m_ccodeclearance.GetWindowText(m_codeclearance);

			sprintf(m_char, "%s",m_codeclearance);
			
			for (int k=0;k<5;k++)
			{
				m_stuDevs.stuDevs[0][i].codeclearance[k]=m_char[k];
			}

			memset(m_char,0,sizeof(m_char)); 

			sprintf(m_char, "%s",m_cscodefrequency);

			for (int l=0;l<5;l++)
			{
				m_stuDevs.stuDevs[0][i].dcode[l]=m_char[l];
			}
			
			memset(m_char,0,sizeof(m_char)); 
			
			sprintf(m_char, "%s",m_cscarriefrequency);

			for (int j=0;j<5;j++)
			{
				m_stuDevs.stuDevs[0][i].dcarrie[j]=m_char[j];
			}

			m_csconvert=m_stuDevs.stuDevs[0][i].dcode;
			
			m_ccodefrequency.SetWindowText(m_csconvert);
			
			m_csconvert=m_stuDevs.stuDevs[0][i].dcarrie;
			
			m_ccarriefrequency.SetWindowText(m_csconvert);


			m_csconvert=m_stuDevs.stuDevs[0][i].codeclearance;
			
			m_ccodeclearance.SetWindowText(m_csconvert);
			
			break;
			
		}	
	}

}

void CTextFixtureDlg::OnSelchangeCombocarrierfrequency() 
{
	// TODO: Add your control notification handler code here
	int m_intcarsel=0;
	m_intcarsel=m_ccarriefrequency.GetCurSel();
	m_ccarriefrequency.GetLBText(m_intcarsel,m_cscarriefrequency);
	m_cscarriefrequency=m_cscarriefrequency.Left(2);
}

void CTextFixtureDlg::OnSelchangeCombocodefrequncey() 
{
	// TODO: Add your control notification handler code here
	int m_intcodesel=0;
	m_intcodesel=m_ccodefrequency.GetCurSel();
	m_ccodefrequency.GetLBText(m_intcodesel,m_cscodefrequency);

	if (m_intcodesel<9)
	{
		m_cscodefrequency=m_cscodefrequency.Left(2);
	} 
	else
	{
		m_cscodefrequency=m_cscodefrequency.Left(3);
	}

}

void CTextFixtureDlg::SendAPR(double m_ds)
{
	unsigned int m_aprdistance=0;
	unsigned int m_curs=0; 
	unsigned int m_senddistance=0;
	float m_freader=0;
	CString m_sendframe;
	int m_nmovedirector;
	//阅读器器偏移
	m_creaderoffset.GetWindowText(m_csreaderoffset);
	m_freader=atof(m_csreaderoffset);
	m_freader=m_freader/1000;

	m_caprsenddistance.GetWindowText(m_csaprsenddistance);
	m_senddistance=atoi(m_csaprsenddistance);

	m_nmovedirector=m_ccbmove.GetCurSel();

	if (m_nmovedirector==0)
	{
		m_curs=(m_ds+m_freader+m_senddistance)*1000;
	} 
	else
	{
		m_curs=(m_ds-m_freader-m_senddistance-BACKOFFSET)*1000;
	}

	/*
	//向前
	if (m_nmovedirector==0)
	{
		if ((m_curs>=m_gparpnode->APRapos)&&(m_curs<=m_gparpnode->APRapos+1000))
		{
			//发送APR
			//APRID 计算
			NoPowerSigal(m_gparpnode->APRapos+m_ddistance*1000,m_gparpnode->APRid);
			//如果已经发送指针指向下一个，并进行运算确认，防止多次发送造成指针不确定

			if (m_gparpnode->next->APRapos>0)
			{
				m_gparpnode=m_gparpnode++;
			}
			
		
			if (m_odd==FALSE)
			{
				SecondAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				FirstAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				m_odd=TRUE;
			}
			else
			{
				SecondAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				FirstAPRd=SecondAPRd-FirstAPRd;
				CString m_str;
				FirstAPRd=FirstAPRd/1000;
				m_str.Format("APR 距离差为：%lf",FirstAPRd);
				AddMsg(m_str);
				FirstAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				
			}

		}
	}
	else
	{
		if ((m_curs<=m_gparpnode->APRapos)&&(m_curs>=m_gparpnode->APRapos-1000))
		{
			//发送APR
			//APRID 计算
			//m_aprdistance+=(m_ddistance*1000);
			NoPowerSigal(m_gparpnode->APRapos+m_ddistance*1000,m_gparpnode->APRid);
			
			if (m_gparpnode->prior!=NULL)
			{
				if (m_gparpnode->prior->APRapos>0)
				{
					m_gparpnode=m_gparpnode--;
				}
			}

			
			if (m_odd==FALSE)
			{
				
				SecondAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				FirstAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				m_odd=TRUE;
			}
			else
			{
				SecondAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				
				FirstAPRd=SecondAPRd-FirstAPRd;
				
				CString m_str;
				FirstAPRd=FirstAPRd/1000;
				m_str.Format("APR 距离差为：%lf",FirstAPRd);
				
				AddMsg(m_str);
				
				FirstAPRd=m_gparpnode->APRapos+m_ddistance*1000;
				
			}
			
		}
	}
	*/

	
	for (int i=1;i<40;i++)
	{			
		if (m_ds>=m_stuDevs.stuDevs[0][i].dStartPara)
		{
			if (m_stuDevs.stuDevs[0][i].dnp1apr!=0)
				{
					m_aprdistance=(m_stuDevs.stuDevs[0][i].dStartPara*1000)+m_stuDevs.stuDevs[0][i].dnp1apr;

					if ((m_curs>=(m_aprdistance))&&(m_aprdistance>0)&&(m_curs<=(m_aprdistance+1000))&&(m_nmovedirector==0))
					{
						//发送APR
						//APRID 计算
						m_aprdistance+=(m_ddistance*1000);
						m_senddistance=m_stuDevs.stuDevs[0][i].np1aprid;
						NoPowerSigal(m_aprdistance,m_senddistance);
						CString m_straddmess;
						m_straddmess.Format("向前发送APR信息! APR位置:%d",m_aprdistance/1000);
						AddMsg(m_straddmess);
						
						if (m_odd==FALSE)
						{
							
							SecondAPRd=m_aprdistance;
							FirstAPRd=m_aprdistance;
							m_odd=TRUE;
						}
						else
						{
							SecondAPRd=m_aprdistance;
							
							FirstAPRd=SecondAPRd-FirstAPRd;
							
							CString m_str;
							FirstAPRd=FirstAPRd/1000;
							m_str.Format("APR 距离差为:%lf",FirstAPRd);
							
							AddMsg(m_str);
							
							FirstAPRd=m_aprdistance;
							
						}


					}
					else if ((m_curs<=(m_aprdistance))&&(m_aprdistance>0)&&(m_curs>=(m_aprdistance-1000))&&(m_nmovedirector!=0))
					{
						m_aprdistance+=(m_ddistance*1000);
					//	m_aprdistance+=50000;
						m_senddistance=m_stuDevs.stuDevs[0][i].np1aprid;
						NoPowerSigal(m_aprdistance,m_senddistance);
						CString m_straddmess;
						m_straddmess.Format("向后发送APR信息! APR距离:%d",m_aprdistance/1000);
						AddMsg(m_straddmess);
						if (m_odd==FALSE)
						{
							
							SecondAPRd=m_aprdistance;
							FirstAPRd=m_aprdistance;
							m_odd=TRUE;
						}
						else
						{
							SecondAPRd=m_aprdistance;
							
							FirstAPRd=SecondAPRd-FirstAPRd;
							
							CString m_str;
							FirstAPRd=FirstAPRd/1000;
							m_str.Format("APR 距离差为：%lf",FirstAPRd);
							
							AddMsg(m_str);
							
							FirstAPRd=m_aprdistance;
							
						}

					}
				}

				if (m_stuDevs.stuDevs[0][i].dnp2apr!=0)
				{

					m_aprdistance=(m_stuDevs.stuDevs[0][i].dStartPara*1000)+m_stuDevs.stuDevs[0][i].dnp2apr;

					if  ((m_curs>=(m_aprdistance))&&(m_aprdistance>0)&&(m_curs<=(m_aprdistance+2000))&&(m_nmovedirector==0))
					{
						//发送APR
						m_aprdistance+=(m_ddistance*1000);
						m_senddistance=m_stuDevs.stuDevs[0][i].np1aprid;
						NoPowerSigal(m_aprdistance,m_senddistance);
					}

					else if ((m_curs<=(m_aprdistance))&&(m_aprdistance>0)&&(m_curs>=(m_aprdistance-2000))&&(m_nmovedirector!=0))
					{
						m_aprdistance+=(m_ddistance*1000);
						m_aprdistance+=50000;
						m_senddistance=m_stuDevs.stuDevs[0][i].np1aprid;
						NoPowerSigal(m_aprdistance,m_senddistance);
					}

				}
		}
	}
	
}

void CTextFixtureDlg::OnSaveinidatatofile() 
{
	// TODO: Add your control notification handler code here
	unsigned int m_select=0;
	CString m_csselect;
	CString m_str;
	CString m_csconvert;
	char   m_char[5]; 
	m_select=m_ctrackselect.GetCurSel();
	m_ctrackselect.GetLBText(m_select,m_csselect);
	
	for (int i=1;i<31;i++)
	{
		m_str=m_stuDevs.stuDevs[0][i].szName;
		if (m_str==m_csselect)
		{
			/*
			BOOL WritePrivateProfileString(
				LPCTSTR lpAppName,  // pointer to section name
				LPCTSTR lpKeyName,  // pointer to key name
				LPCTSTR lpString,   // pointer to string to add
				LPCTSTR lpFileName  // pointer to initialization filename
				);
			*/
			m_cnpapr1.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].dnp1apr=m_select;
			
			m_cnpapr2.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].dnp2apr=m_select;

			m_cpapr1.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].dp1apr=m_select;

			m_cpapr2.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].dp2apr=m_select;

			m_cnpapr1id.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].np1aprid=m_select;

			m_cnpapr2id.GetWindowText(m_csconvert);
			m_select=atoi(m_csconvert);

			m_stuDevs.stuDevs[0][i].np2aprid=m_select;


			memset(m_char,0,sizeof(m_char)); 

			sprintf(m_char, "%s",m_cscodefrequency);

			for (int k=0;k<5;k++)
			{
				m_stuDevs.stuDevs[0][i].dcode[k]=m_char[k];
			}
			
			memset(m_char,0,sizeof(m_char)); 
			
			sprintf(m_char, "%s",m_cscarriefrequency);

			for (int j=0;j<5;j++)
			{
				m_stuDevs.stuDevs[0][i].dcarrie[j]=m_char[j];
			}

			m_csconvert=m_stuDevs.stuDevs[0][i].dcode;
			
			m_ccodefrequency.SetWindowText(m_csconvert);
			
			m_csconvert=m_stuDevs.stuDevs[0][i].dcarrie;
			
			m_ccarriefrequency.SetWindowText(m_csconvert);


			break;
			
		}	
	}





}

void CTextFixtureDlg::SendTrackCode(unsigned int m_code)
{
	unsigned int m_trackcodeoffset=0;

	CString m_str;

	byte	m_bytefru;
	byte	m_bytecode;


//	m_cfrusenddistance.GetWindowText(m_csfrusenddistance);
	
	m_trackcodeoffset=atoi(m_csfrusenddistance);
	m_code+=m_trackcodeoffset;
	m_trackcodeoffset=atoi(m_csantenna);
	m_code+=m_trackcodeoffset;

	for (int i=1;i<40;i++)
	{
		
		if ((m_code>=m_stuDevs.stuDevs[0][i].dStartPara-8)&&(m_code<=m_stuDevs.stuDevs[0][i].dStartPara+7)&&(m_bgSendTcode==FALSE)&&(m_bgSendFTcode==TRUE))
		{
			m_str=m_stuDevs.stuDevs[0][i].dcarrie;
			m_str=m_str.Right(1);

			m_bytefru=atoi(m_str);
			
			m_str=m_stuDevs.stuDevs[0][i].dcode;

			m_bytecode=m_str.GetLength();

			if (m_bytecode==2)
			{
				m_str=m_str.Right(1);
			} 
			else
			{
				m_str=m_str.Right(2);
			}

			m_bytecode=atoi(m_str);

			TrackCode(m_bytefru,m_bytecode,m_stuDevs.stuDevs[0][i].dStartPara);
			m_bgSendTcode=TRUE;
			m_bgSendFTcode=FALSE;
			CString m_straddmess;
			
			m_straddmess.Format("发送轨道码,载频:%d,码频:%d",m_bytefru,m_bytecode);
			m_straddmess+=m_stuDevs.stuDevs[0][i].szName;
			AddAlarm(m_straddmess);

		}

	}

}

void CTextFixtureDlg::OnReaderpower() 
{
	// TODO: Add your control notification handler code here
	m_bytebatter=0x00;
	m_creadernopower.SetCheck(FALSE);
	m_creaderpower.SetCheck(TRUE);
}

void CTextFixtureDlg::OnReadernopower() 
{
	// TODO: Add your control notification handler code here
	m_bytebatter=0xfe;
	m_creadernopower.SetCheck(TRUE);
	m_creaderpower.SetCheck(FALSE);
}



void CTextFixtureDlg::OnButtonsetv() 
{
	// TODO: Add your control notification handler code here
	float m_setspeed;
	m_ceditv.GetWindowText(m_cseditv);
	m_setspeed=atof(m_cseditv);
	if (m_cseditv=="")
	{
		AddAlarm("未设定目标速度,命令未执行!");
		return;
	}
	if (m_setspeed>90)
	{
		AddAlarm("目标速度超过最大允许速度，命令未执行!");
		return;
	}

	AddAlarm("启用巡航定速!");
	bCruise=FALSE;
	SetTimer(2,2,NULL);

}

void CTextFixtureDlg::OnButtonautostop() 
{
	// TODO: Add your control notification handler code here
	//该部分可选用PID实现
	int m_nstoppos=0;
	int	m_ncurpos=0;
	unsigned int m_unstop=0;
	unsigned int m_unv2=0;
	unsigned int m_unacc=0;
	m_cautostop.GetWindowText(m_csautostop);
	m_dautostop=atof((LPCTSTR)m_csautostop);
	m_nstoppos=(int)m_dautostop*1000;
	m_ncurpos=(int)m_dTrainPosition;
	m_unstop=(unsigned int)m_dTrainPosition*1000;
	if (m_ncurpos>=m_nstoppos)
	{
		AddAlarm("设定距离处无法实现自动停车!");
		return;
	}
	AddAlarm("已设定自动停车!");
	m_stoppos=m_nstoppos;
	m_curpos=m_unstop;
	Autostopstart=TRUE;
	m_bbreaking=FALSE;
	SetTimer(4,1,NULL);

}

void CTextFixtureDlg::OnButtonreaderoffset() 
{
	// TODO: Add your control notification handler code here
	m_creaderoffset.GetWindowText(m_csreaderoffset);
	if (m_csreaderoffset!="")
	{
		m_dreaderoffset=atof((LPCTSTR)m_csreaderoffset);
		
	}
}

void CTextFixtureDlg::OnButtonstart() 
{
	// TODO: Add your control notification handler code here


	m_cstart.GetWindowText(m_csstart);
//	m_csstart="91";

	if (m_csstart!="")
	{
	//	UpdateData(TRUE);

		m_dstart=atof((LPCTSTR)m_csstart);
		
		m_ftranoffset=atof(m_cslogics);

		m_ftranoffset=m_ftranoffset+m_dstart;

		m_cspostion.Format("%lf",m_ftranoffset);

		UpdateData(FALSE);

		m_dTrackStartPos=m_dstart;

		InvalidateRect(SurRect,TRUE);
	}
}

void CTextFixtureDlg::OnButtonaproffset() 
{
	// TODO: Add your control notification handler code here

		UpdateData(TRUE);

}

void CTextFixtureDlg::OnButtoncodeoffset() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);

}

void CTextFixtureDlg::OnButtonlimitv() 
{
	// TODO: Add your control notification handler code here
	int nlimitv=0;
	m_blimit=FALSE;

	CString m_str="设定限速为:";
	UpdateData(TRUE);
	nlimitv=atoi(m_csspeedlimit);
	if (nlimitv>90)
	{
		AddAlarm("设定限速超过最大限速!");
		return;
	}
	m_str+=m_csspeedlimit;
	AddAlarm(m_str);
	m_noverspeed=atoi(m_csspeedlimit);
//	SetTimer(3,1000,NULL);
}


void CTextFixtureDlg::OnSelchangeCombowheel() 
{
	// TODO: Add your control notification handler code here
	switch (m_cwheeld.GetCurSel())
	{
	case 0:
		{	
			m_nwheel=9;
			break;
		}
	case 1:
		{
			m_nwheel=8;
			break;
		}
	case 2:
		{
			m_nwheel=7;
			break;
		}
	case 3:
		{
			m_nwheel=6;
			break;
		}
	case 4:
		{
			m_nwheel=5;
			break;
		}
	case 5:
		{
			m_nwheel=4;
			break;
		}
	case 6:
		{
			m_nwheel=3;
			break;
		}
	case 7:
		{
			m_nwheel=2;
			break;
		}
	case 8:
		{
			m_nwheel=1;
			break;
		}

	}
	TrackStatus(m_dAcc, m_head, m_direct);

}

void CTextFixtureDlg::SetCruise()
{
	float m_currv=0;
	int   m_curracc=0;
//	int m_wantv=0;
	m_ceditv.GetWindowText(m_cseditv);

	m_fsetv=atof((LPCTSTR)m_cseditv);
	
	m_currv=m_fglobalspeed;

//	m_wantv=(int)m_fsetv;
	
	if ((m_fsetv>m_currv)&&(bCruise==FALSE))
	{
	//	bCruise=TRUE;
	//	m_curracc=(m_fsetv-m_currv)/10;
		m_curracc=(int)m_fsetv-m_currv;

		if(m_curracc<10)
		{
			m_dAcc=1;
			m_SldAcc.SetPos(11);
		}
		else if((m_curracc>=10)&&(m_curracc<=20))
		{
			m_dAcc=2;
			m_SldAcc.SetPos(12);
		}
		else if((m_curracc>20)&&(m_curracc<=30))
		{
			m_dAcc=3;
			m_SldAcc.SetPos(13);
		}
		else if(m_curracc>30)
		{
			m_dAcc=4;
			m_SldAcc.SetPos(14);
		}


		TrackStatus(m_dAcc, m_head, m_direct);
		AddMsg("巡航定速加速!");
	}
	else if ((m_fsetv<m_currv)&&(bCruise==FALSE))
	{
		//bCruise=TRUE;
		m_curracc=(int)m_currv-m_fsetv;
		if(m_curracc<10)
		{
			m_dAcc=-1;
			m_SldAcc.SetPos(8);
		}
		else if((m_curracc>=10)&&(m_curracc<20))
		{
			m_dAcc=-2;
			m_SldAcc.SetPos(7);
		}
		else if((m_curracc>=20)&&(m_curracc<30))
		{
			m_dAcc=-3;
			m_SldAcc.SetPos(6);
		}
		else if((m_curracc>=30)&&(m_curracc<40))
		{
			m_dAcc=-4;
			m_SldAcc.SetPos(5);
		}
		else if((m_curracc>=50)&&(m_curracc<60))
		{
			m_dAcc=-5;
			m_SldAcc.SetPos(4);
		}
		else if((m_curracc>=60)&&(m_curracc<70))
		{
			m_dAcc=-6;
			m_SldAcc.SetPos(3);
		}
		else if(m_curracc>=70)
		{
			m_dAcc=-7;
			m_SldAcc.SetPos(2);
		}

		TrackStatus(m_dAcc, m_head, m_direct);
		AddMsg("巡航定速减速!");
	}
	else if ((m_currv+0.1>=m_fsetv)||(m_currv-0.1<=m_fsetv))
	{	
		bCruise=TRUE;
		KillTimer(2);
		m_dAcc=0;
		m_SldAcc.SetPos(9);
		TrackStatus(m_dAcc, m_head, m_direct);
		AddMsg("定速已完成,巡航中...");
	}
}

void CTextFixtureDlg::OnButtonantenna() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CTextFixtureDlg::CheckOverSpeed()
{
	m_blimit=FALSE;
	KillTimer(3);
}

void CTextFixtureDlg::AutoStop(unsigned int vt, unsigned int stoppos, int curpos)
{

	float   m_fbreaks=0;
	float	m_offsets=0;
	float	m_fbreak12km=0;
	double	m_fbreak120km=6.481469592;
	int m_unstoplenth=0;
	unsigned int m_unacc=0;
	int	m_ns=stoppos-curpos;
	curpos=(unsigned int)m_dTrainPosition;
	m_fbreak12km=BreakDistance();	//m
	m_offsets=m_fglobalspeed*120;	//mm
	m_offsets=m_offsets/1000;		//m

	m_fbreaks=m_fbreak12km+(float)m_fbreak120km+SLIPDISTANCE;


	m_unstoplenth=(int)stoppos/1000-curpos-m_fbreaks-m_offsets;
	
	if ((m_unstoplenth<0)&&(Autostopstart==TRUE))
	{
		AddAlarm("设定停车位置无法停车,命令未执行!");
		Autostopstart=FALSE;
		KillTimer(4);
		return;
	}
	
	if ((m_unstoplenth==0)&&(m_bbreaking==FALSE))
	{
		Autostopstart=FALSE;
		m_fbreakpoint=m_dTrainPosition;
	//	KillTimer(4);
		AddMsg("已启用自动停车-首次刹车");
		m_dAcc=-5;
		m_SldAcc.SetPos(4);
		TrackStatus(m_dAcc, m_head, m_direct);
		QueryPerformanceFrequency(&litmp);
		dfFreq=(double)litmp.QuadPart;		//获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1=litmp.QuadPart;				//获得初始值
		SetTimer(5,1,NULL);
		KillTimer(4);
	}
}

void CTextFixtureDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	/*
	m_csbreaktime="";
	m_csbreakdistance="";
	m_csbreakerror="";
	*/
	UpdateData(FALSE);
}

void CTextFixtureDlg::CheckSpeedZero()
{
	int nspeed;
	float m_time;
	nspeed=(int)m_fglobalspeed;
	if (nspeed==TARGETSPEED)
	{

		AddMsg("首次刹车结束");
		m_dAcc=0;
		m_SldAcc.SetPos(9);
		TrackStatus(m_dAcc, m_head, m_direct);

		QueryPerformanceCounter(&litmp);
		QPart2=litmp.QuadPart;			//获得中止值
		dfMinus=(double)(QPart2-QPart1);
		dfTim=dfMinus/dfFreq;			//获得对应的时间值，单位为秒 　　
		m_time=dfTim;

		m_csfirstbreaktime.Format("%lf",m_time);

		m_fbreakpoint=(float)m_dTrainPosition-m_fbreakpoint;
		
		m_csfirstbreakdistance.Format("%lf",m_fbreakpoint);

		UpdateData(FALSE);
		KillTimer(5);
		m_fbreakpoint=(float)m_dTrainPosition;

		QueryPerformanceFrequency(&litmp);
		dfFreq=(double)litmp.QuadPart;		//获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1=litmp.QuadPart;				//获得初始值

		SetTimer(6,1,NULL);
	}
}

float CTextFixtureDlg::BreakDistance()
{
	float m_s;
	float m_curspeed;
	m_curspeed=m_fglobalspeed/3.6;
	m_s=m_curspeed*m_curspeed-11.11111111111111111111111;
	m_s=m_s/1.714285714285714285714;
	return m_s;
}

void CTextFixtureDlg::SencondBreak()
{
	float m_time;
	int m_speed;
	float m_fs12to0;
	float m_curspeed;
	int	m_getbreakpoint;
	double	m_offset=0.1;
	m_speed=(int)m_fglobalspeed;
	m_curspeed=m_fglobalspeed/3.6;
	if (m_speed==TARGETSPEED)
	{

		m_fs12to0=m_curspeed*m_curspeed/1.71428571428571428;

		UpdateData(TRUE);

		m_getbreakpoint=atoi(m_csautostop);

		
		if (m_getbreakpoint<=((float)m_dTrainPosition+m_fs12to0+(float)m_offset))
		{


			QueryPerformanceCounter(&litmp);
			QPart2=litmp.QuadPart;			//获得中止值
			dfMinus=(double)(QPart2-QPart1);
			dfTim=dfMinus/dfFreq;			//获得对应的时间值，单位为秒 　　
			m_time=dfTim;
			
			m_csslipbreaktime.Format("%lf",m_time);
			
			m_fbreakpoint=(float)m_dTrainPosition-m_fbreakpoint;
			
			m_csslipbreakdistance.Format("%lf",m_fbreakpoint);
			
			UpdateData(FALSE);

			AddMsg("二次刹车开始");
			m_dAcc=-5;
			m_SldAcc.SetPos(4);
			TrackStatus(m_dAcc, m_head, m_direct);

			KillTimer(6);

			QueryPerformanceFrequency(&litmp);
			dfFreq=(double)litmp.QuadPart;		//获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1=litmp.QuadPart;				//获得初始值

			m_fbreakpoint=(float)m_dTrainPosition;

			SetTimer(7,100,NULL);
		}
	}
	else
	{
		AddMsg("未达到目标速度!");
		SetTimer(4,1,NULL);
	}

}

void CTextFixtureDlg::CheckStop()
{
	float m_time;
	int m_stopspeed=0;
	float	m_nstoppos=0;
	float	m_fstop=0;
	m_stopspeed=(int)m_fglobalspeed;
	if (0==m_stopspeed)
	{
		QueryPerformanceCounter(&litmp);
		QPart2=litmp.QuadPart;			//获得中止值
		dfMinus=(double)(QPart2-QPart1);
		dfTim=dfMinus/dfFreq;			//获得对应的时间值，单位为秒 　　
		m_time=dfTim;

		m_cssecondbreaktime.Format("%lf",m_time);

		m_fbreakpoint=(float)m_dTrainPosition-m_fbreakpoint;

		m_cssecondbreakdistance.Format("%lf",m_fbreakpoint);

		Sleep(1);

		m_fstop=atof(m_csautostop);

		m_nstoppos=atof(m_cslogics);

		m_time=(float)m_nstoppos-(float)m_fstop;
		m_time=m_time*1000;
		m_csbreakerror.Format("%lf",m_time);

		UpdateData(FALSE);

		AddMsg("列车已停稳!");
		KillTimer(7);
	}
}

void CTextFixtureDlg::OnChecktechop2() 
{
	// TODO: Add your control notification handler code here
	if (m_cchecktechop2.GetCheck())
	{
		m_bytetechop=m_bytetechop^2;
		AddAlarm("测速电机2继电器励磁!");
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xfd;
		AddAlarm("测速电机2继电器失磁!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnFsk1() 
{
	// TODO: Add your control notification handler code here
	if (m_fsk1.GetCheck())
	{
		m_bytetechop=m_bytetechop^0x10;
		AddAlarm("FSK1有效!");
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xef;
		AddAlarm("FSK1失效!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnFsk2() 
{
	// TODO: Add your control notification handler code here
	if (m_fsk2.GetCheck())
	{
		m_bytetechop=m_bytetechop^0x20;
		AddAlarm("FSK2有效!");
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xdf;
		AddAlarm("FSK2失效!!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnTacho1() 
{
	// TODO: Add your control notification handler code here
	if (m_tacho1.GetCheck())
	{
		m_bytetechop=m_bytetechop^0x04;
		AddAlarm("测速电机1有效!");
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xfb;
		AddAlarm("测速电机1失效!");
	}
	HeadTrial();
}

void CTextFixtureDlg::OnTacho2() 
{
	// TODO: Add your control notification handler code here
	if (m_tacho2.GetCheck())
	{
		m_bytetechop=m_bytetechop^0x08;
		AddAlarm("测速电机2有效!");
		m_cpersent.GetWindowText(m_cspersent);
		if (m_cspersent=="")
		{
			if (m_ctachooverhead.GetCheck())
			{
				AddAlarm("百分比为空!测速电机加速未执行!");
			} 
			else
			{
				AddAlarm("百分比为空!测速电机减速未执行!");
			}
		
			return;
		}
		else
		{
			HeadTrial();
			TrackStatus(m_dAcc, m_head, m_direct);
		}
	} 
	else
	{
		m_bytetechop=m_bytetechop&0xf7;
		AddAlarm("测速电机2失效!");
		HeadTrial();
		//TrackStatus(m_dAcc, m_head, m_direct);
	}

}

void CTextFixtureDlg::OnSendcodegaps() 
{
	// TODO: Add your control notification handler code here
		
		m_cgapstoptime.GetWindowText(m_csgapstoptime);
		if (m_csgapstoptime!="")
		{
			m_codegap=TRUE;
			TrackStatus(m_dAcc, m_head, m_direct);
		} 
		else
		{
			AddAlarm("码间隙时间为空，动作未执行!");
		}
		
//	HeadTrial();
}

void CTextFixtureDlg::OnTachooverhead() 
{
	// TODO: Add your control notification handler code here
	
}

void CTextFixtureDlg::OnGapsmode() 
{
	// TODO: Add your control notification handler code here
	if (m_cgapsmode.GetCheck())
	{
		AddAlarm("已启用即时模式!");
		m_bytetechop=m_bytetechop^0x40;
	}
	else
	{
		AddAlarm("已取消即时模式!");
		m_bytetechop=m_bytetechop&0xBF;
	}
	HeadTrial();
}

void CTextFixtureDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	/*
		载波频率编码表：
		载频（Hz）	4080	4320	4560	4800	5040	5280	5520	6000
		发送编号(16进制)	1	2	3	4	5	6	7	8
		调制频率编码表：
		码频（Hz）	28	32	36	40	44	48	52	56	60	64	68	72	76	80
		发送编号(16进制)	1	2	3	4	5	6	7	8	9	A	B	C	D	E
	*/
	int m_sel=0;
	if (m_cgapsmode.GetCheck())
	{
		m_sel=m_carray1.GetCurSel();
		m_carraycode1=0;
		switch(m_sel)
		{
		case 0:
			m_carraycode1=m_carraycode1^0x10;
			break;
		case 1:
			m_carraycode1=m_carraycode1^0x20;
			break;
		case 2:
			m_carraycode1=m_carraycode1^0x30;
			break;
		case 3:
			m_carraycode1=m_carraycode1^0x40;
			break;
		case 4:
			m_carraycode1=m_carraycode1^0x50;
			break;
		case 5:
			m_carraycode1=m_carraycode1^0x60;
			break;
		case 6:
			m_carraycode1=m_carraycode1^0x70;
			break;
		case 7:
			m_carraycode1=m_carraycode1^0x80;
			break;

		default:
			break;
		}

		m_sel=m_ccode1.GetCurSel();

		switch(m_sel)
		{


		case 0:
			m_carraycode1=m_carraycode1^0x01;
			break;
		case 1:
			m_carraycode1=m_carraycode1^0x02;
			break;
		case 2:
			m_carraycode1=m_carraycode1^0x03;
			break;
		case 3:
			m_carraycode1=m_carraycode1^0x04;
			break;
		case 4:
			m_carraycode1=m_carraycode1^0x05;
			break;
		case 5:
			m_carraycode1=m_carraycode1^0x06;
			break;
		case 6:
			m_carraycode1=m_carraycode1^0x07;
			break;
		case 7:
			m_carraycode1=m_carraycode1^0x08;
			break;
		case 8:
			m_carraycode1=m_carraycode1^0x09;
			break;
		case 9:
			m_carraycode1=m_carraycode1^0x0a;
			break;
		case 10:
			m_carraycode1=m_carraycode1^0x0b;
			break;
		case 11:
			m_carraycode1=m_carraycode1^0x0c;
			break;
		case 12:
			m_carraycode1=m_carraycode1^0x0d;
			break;
		case 13:
			m_carraycode1=m_carraycode1^0x0e;
				break;

		default:
			break;
		}


		m_sel=m_carray2.GetCurSel();
		m_carraycode2=0;
		switch(m_sel)
		{
		case 0:
			m_carraycode2=m_carraycode2^0x10;
			break;
		case 1:
			m_carraycode2=m_carraycode2^0x20;
			break;
		case 2:
			m_carraycode2=m_carraycode2^0x30;
			break;
		case 3:
			m_carraycode2=m_carraycode2^0x40;
			break;
		case 4:
			m_carraycode2=m_carraycode2^0x50;
			break;
		case 5:
			m_carraycode2=m_carraycode2^0x60;
			break;
		case 6:
			m_carraycode2=m_carraycode2^0x70;
			break;
		case 7:
			m_carraycode2=m_carraycode2^0x80;
			break;
			
		default:
			break;
		}
		
		m_sel=m_ccode2.GetCurSel();
		
		switch(m_sel)
		{
			
			
		case 0:
			m_carraycode2=m_carraycode2^0x01;
			break;
		case 1:
			m_carraycode2=m_carraycode2^0x02;
			break;
		case 2:
			m_carraycode2=m_carraycode2^0x03;
			break;
		case 3:
			m_carraycode2=m_carraycode2^0x04;
			break;
		case 4:
			m_carraycode2=m_carraycode2^0x05;
			break;
		case 5:
			m_carraycode2=m_carraycode2^0x06;
			break;
		case 6:
			m_carraycode2=m_carraycode2^0x07;
			break;
		case 7:
			m_carraycode2=m_carraycode2^0x08;
			break;
		case 8:
			m_carraycode2=m_carraycode2^0x09;
			break;
		case 9:
			m_carraycode2=m_carraycode2^0x0a;
			break;
		case 10:
			m_carraycode2=m_carraycode2^0x0b;
			break;
		case 11:
			m_carraycode2=m_carraycode2^0x0c;
			break;
		case 12:
			m_carraycode2=m_carraycode2^0x0d;
			break;
		case 13:
			m_carraycode2=m_carraycode2^0x0e;
			break;
			
		default:
			break;
		}

		AddAlarm("即时模式发送数据!");
		TrackStatus(m_dAcc, m_head, m_direct);
	}
	else
	{
		AddAlarm("未启用即时模式，操作无效!");
	}
}

void CTextFixtureDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CTextFixtureDlg::OnCheckfsbebenable() 
{
	// TODO: Add your control notification handler code here
	if (m_ccheckfsbebenble.GetCheck())
	{
		AddAlarm("已启用EB失效!");
	}
	else
	{
		AddAlarm("已取消EB失效!");
	}
	
}

void CTextFixtureDlg::InitAPRNode()
{
	for (int i=1;i<=TRACKNUMBER;i++)
	{
		if (m_stuDevs.stuDevs[0][i].dnp1apr!=0)
		{
			m_gaprnode[i-1].APRapos=(m_stuDevs.stuDevs[0][i].dStartPara*1000)+m_stuDevs.stuDevs[0][i].dnp1apr;
			m_gaprnode[i-1].APRid=m_stuDevs.stuDevs[0][i].np1aprid;
			if (0!=i-1)
			{
				m_gaprnode[i-1].prior=&m_gaprnode[i-2];
				m_gaprnode[i-1].next=&m_gaprnode[i];
			}
			else
			{
				m_gaprnode[i-1].prior=NULL;
				m_gaprnode[i-1].next=&m_gaprnode[i];
			}
		}
		else 
		{
			m_gaprnode[i-1].APRapos=0;
			m_gaprnode[i-1].APRid=0;
		}
	}
}

BOOL CTextFixtureDlg::Ischanged(double pos)
{
	if (m_dtrackpos==pos)
	{
		return FALSE;
	}
	else
	{
		m_dtrackpos=pos;
		return TRUE;
	}
}


UINT ThreadFunc(LPVOID lpParam)
{

	
	ReceiveChar(lpParam);
	
	return 0;
}

UINT ThreadWrite(LPVOID lpParam)
{
	//WriteChar(BYTE* m_szWriteBuffer,DWORD m_nToSend)
			/*
			//列车状态数据帧
			CString m_cstypestatus="30";
			//无源信标握手帧
			CString	m_cstypenopowersignalhandle="70";
			//无源信标数据帧
			CString m_cstypenopowersigaldata="10";
			//有源信标握手帧
			CString m_cstypepowersignalhandle="80";
			//有源信标数据帧
			CString m_cstypepowersigaldata="20";
			//轨道码数据
			CString m_cstypetrackcode="40";
			//头尾车
			CString m_cstypeheadtrail="50";
			*/
	
	for(;;)
	{
		//30
		if ((m_bgSendstatus==TRUE)&&(m_bgSendFStatus==FALSE))
		{
			WORD	m_ncrcs16;
			//填入序列号和校验
			m_comtxstatus[4]=m_framecounter++;
			if (m_framecounter==256)
			{
				m_framecounter=0;
			}
			m_comtxstatus[20]=m_comtxstatus[4];
			m_ncrcs16=crc16(m_comtxstatus,22);
			m_comtxstatus[22]=m_ncrcs16&0x00ff;
			m_ncrcs16=m_ncrcs16&0xff00;
			m_ncrcs16=m_ncrcs16>>8;
			m_comtxstatus[23]=m_ncrcs16;
			WriteChar(m_comtxstatus,24);
			if (m_comtxstatus[0x11]!=0)
			{
				m_comtxstatus[0x11]=0;
			}
			::PostMessage((HWND)lpParam, UM_COMWRITER, 0x30, m_comtxstatus[4]);
			Delayms(10);
		}
		//40
		if ((m_bgSendTcode==TRUE)&&(m_bgSendFTcode==FALSE))
		{
			int	m_ncrct16;
			//填入序列号和校验
			
			m_comtxtcode[4]=m_framecounter++;
			if (m_framecounter==256)
			{
				m_framecounter=0;
			}
			m_comtxtcode[20]=m_comtxtcode[4];
			m_ncrct16=crc16(m_comtxtcode,22);
			m_comtxtcode[22]=m_ncrct16&0x00ff;
			m_ncrct16=m_ncrct16&0xff00;
			m_ncrct16=m_ncrct16>>8;
			m_comtxtcode[23]=m_ncrct16;
			WriteChar(m_comtxtcode,24);
			::PostMessage((HWND)lpParam, UM_COMWRITER, 0x40, m_comtxtcode[4]);
			Delayms(10);
		}
		//50		
		if ((m_bgSendHeadT==TRUE)&&(m_bgSendFHeadT==FALSE))
		{
			int	m_ncrch16;
			//填入序列号和校验
			m_comtxheadt[4]=m_framecounter++;
			if (m_framecounter==256)
			{
				m_framecounter=0;
			}
			m_comtxheadt[20]=m_comtxheadt[4];
			m_ncrch16=crc16(m_comtxheadt,22);
			m_comtxheadt[22]=m_ncrch16&0x00ff;
			m_ncrch16=m_ncrch16&0xff00;
			m_ncrch16=m_ncrch16>>8;
			m_comtxheadt[23]=m_ncrch16;
			WriteChar(m_comtxheadt,24);
			::PostMessage((HWND)lpParam, UM_COMWRITER, 0x50, m_comtxheadt[4]);
			Delayms(10);
		}
		
		//10
		if ((m_bgSendNAPR==TRUE)&&(m_bgSendFNAPR==FALSE))
		{
			int	m_ncrca16;
			//填入序列号和校验
			m_comtxapr1[4]=m_framecounter++;
			if (m_framecounter==256)
			{
				m_framecounter=0;
			}
			m_comtxapr1[20]=m_comtxapr1[4];
			m_ncrca16=crc16(m_comtxapr1,22);
			m_comtxapr1[22]=m_ncrca16&0x00ff;
			m_ncrca16=m_ncrca16&0xff00;
			m_ncrca16=m_ncrca16>>8;
			m_comtxapr1[23]=m_ncrca16;
			::PostMessage((HWND)lpParam, UM_COMWRITER, 0x70, m_comtxapr1[4]);
			WriteChar(m_comtxapr1,24);
			Delayms(5);

			m_comtxapr2[4]=m_framecounter++;
			if (m_framecounter==256)
			{
				m_framecounter=0;
			}
			m_comtxapr2[20]=m_comtxapr2[4];
			m_ncrca16=crc16(m_comtxapr2,22);
			m_comtxapr2[22]=m_ncrca16&0x00ff;
			m_ncrca16=m_ncrca16&0xff00;
			m_ncrca16=m_ncrca16>>8;
			m_comtxapr2[23]=m_ncrca16;
			WriteChar(m_comtxapr2,24);
			::PostMessage((HWND)lpParam, UM_COMWRITER, 0x10, m_comtxapr2[4]);
			Delayms(10);
			m_bgSendFNAPR=TRUE;
		}

		

		Sleep(1);

		/*
		if((m_bgSend==TRUE)&&(m_bgSendFinish==FALSE))
		{
		//	m_bgSend=FALSE;

			//填入序列号和校验
			m_comtx[4]=FrameCounter(m_framecounter);
			m_framecounter++;
			m_comtx[20]=m_comtx[4];
			m_ncrc16=crc16(m_comtx,22);
			m_comtx[22]=m_ncrc16&0x00ff;
			m_ncrc16=m_ncrc16&0xff00;
			m_ncrc16=m_ncrc16>>8;
			m_comtx[23]=m_ncrc16;
			switch (m_comtx[5])
			{
			case 0x30:
				{
					
					WriteChar(m_comtx,24);

					break;
				}
			case 0x40:
				{
					
					WriteChar(m_comtx,24);
					break;
				}
			case 0x50:
				{
					
					WriteChar(m_comtx,24);
					break;
				}
			case 0x70:
				{
					
					WriteChar(m_comtx,24);

					//填入序列号和校验
					m_comtx2[4]=FrameCounter(m_framecounter);
					m_framecounter++;
					m_ncrc16=crc16(m_comtx2,22);
					m_comtx2[22]=m_ncrc16&0x00ff;
					m_ncrc16=m_ncrc16&0xff00;
					m_ncrc16=m_ncrc16>>8;
					m_comtx2[23]=m_ncrc16;
					WriteChar(m_comtx2,24);
					break;
				}
			case 0x80:
				{
					WriteChar(m_comtx,24);
					QueryPerformanceFrequency(&litmp);
					dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
					QueryPerformanceCounter(&litmp);
					QPart1 = litmp.QuadPart;// 获得初始值
					do
					{
						QueryPerformanceCounter(&litmp);
						QPart2 = litmp.QuadPart;//获得中止值
						dfMinus = (double)(QPart2-QPart1);
						dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
					}while(dfTim <0.00173575*2);
					//填入序列号和校验
					m_comtx2[4]=FrameCounter(m_framecounter);
					m_framecounter++;
					m_ncrc16=crc16(m_comtx2,22);
					m_comtx2[22]=m_ncrc16&0x00ff;
					m_ncrc16=m_ncrc16&0xff00;
					m_ncrc16=m_ncrc16>>8;
					m_comtx2[23]=m_ncrc16;
					WriteChar(m_comtx2,24);
					break;
				}
			default :
				{
					WriteChar(m_comtx,24);
					break;
				}
			}

			QueryPerformanceFrequency(&litmp);
			dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
			QueryPerformanceCounter(&litmp);
			QPart1 = litmp.QuadPart;// 获得初始值
			do
			{
				QueryPerformanceCounter(&litmp);
				QPart2 = litmp.QuadPart;//获得中止值
				dfMinus = (double)(QPart2-QPart1);
				dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
			}while(dfTim <0.00173575);

		}
		*/
	
	}
	return 0;
}
UINT ThreadRead(LPVOID lpParam)
{
	ReceiveChar(lpParam);
	return 0;
}

BOOL ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
		);
	/*
	sprintf(Temp, "WARNING: %s Failed with the following error: \n%s\nPort:
		%d\n", (char*)ErrorText, lpMsgBuf, "com2");
		MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);
		*/
	LocalFree(lpMsgBuf);
	delete[] Temp;
	return true;
}
BOOL OpenPort(char *portname)//打开一个串口
{
	/*
	hCom = CreateFile(portname,
		GENERIC_READ | GENERIC_WRITE,0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if (hCom == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return true;
	*/	
	
	//自动搜索串口算法
	CString m_strport="COM";
	CString m_strportnumber;
	
	for (int i=0;i<255;i++)
	{
		m_strport="COM";
		m_strportnumber.Format("%d",i);
		m_strport+=m_strportnumber;
		hCom = CreateFile(m_strport,
			GENERIC_READ | GENERIC_WRITE,0,
			0,
			OPEN_EXISTING,
			FILE_FLAG_OVERLAPPED,
		0);
		if (hCom == INVALID_HANDLE_VALUE)
		{
			continue;
		}
		else
		{
			return TRUE;
			break;
		}
		
	}
	
	return FALSE;

}

BOOL setupdcb(int rate_arg)
{
	DCB dcb;
	int rate= rate_arg;
	memset(&dcb,0,sizeof(dcb));
	if(!GetCommState(hCom,&dcb))//获取当前DCB 配置
	{
		ProcessErrorMessage("GetCommState()");
		return FALSE;
	}
	/* -------------------------------------------------------------------- */
// set DCB to configure the serial port
	dcb.DCBlength = sizeof(dcb);
	/* ---------- Serial Port Config ------- */
	dcb.BaudRate = rate;
	dcb.Parity = NOPARITY;
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	/* ----------------- misc parameters ----- */
	dcb.fErrorChar = 0;
	dcb.fBinary = 1;
	dcb.fNull = 0;
	dcb.fAbortOnError = 0;
	dcb.wReserved = 0;
	dcb.XonLim = 2;
	dcb.XoffLim = 4;
	dcb.XonChar = 0x13;
	dcb.XoffChar = 0x19;
	dcb.EvtChar = 0;
/* -------------------------------------------------------------------- */
// set DCB
	if(!SetCommState(hCom,&dcb))
	{
		ProcessErrorMessage("SetCommState()");
		return false;
	}
	else
	return true;
}

BOOL setuptimeout(DWORD ReadInterval,DWORD ReadTotalMultiplier,DWORD
ReadTotalconstant,DWORD WriteTotalMultiplier,DWORD WriteTotalconstant)
{
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout=ReadInterval;
	timeouts.ReadTotalTimeoutConstant=ReadTotalconstant;
	timeouts.ReadTotalTimeoutMultiplier=ReadTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant=WriteTotalconstant;
	timeouts.WriteTotalTimeoutMultiplier=WriteTotalMultiplier;
	if(!SetCommTimeouts(hCom, &timeouts))
	{
		ProcessErrorMessage("SetCommTimeouts()");
		return false;
	}
	else
	return true;
}

void ReceiveChar(LPVOID lpParam)
{
	BOOL bRead = TRUE;
	BOOL bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	for (;;)
	{
		Sleep(1);
		bResult = ClearCommError(hCom, &dwError, &comstat);
		if (comstat.cbInQue == 0)
		continue;
		if (bRead)
		{
			bResult = ReadFile(hCom, // Handle to COMM port
								&m_comrx, // RX Buffer Pointer
								24, // Read one byte
								&BytesRead, // Stores number of bytes read
								&m_overlap); // pointer to the m_ov structure
			//给主线程信息，显示
			if ((m_comrx[0]==0xfc)&&(m_comrx[1]==0x0c)&&(m_comrx[2]==0x0a)&&(m_comrx[3]==0x0b))
			{
				::PostMessage((HWND)lpParam, UM_COM, 0, 0);
			}
			else
			{
				//串口读取数据错误，终止读取，并清空缓冲区
				PurgeComm(hCom,PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT);
				Sleep(1);
				continue;
			}
			if (!bResult)
			{
				switch (dwError = GetLastError())
				{
				case ERROR_IO_PENDING:
					{
						bRead = FALSE;
						break;
					}
				default:
					{
						break;
					}
				}
			}
			else
			{
				bRead = TRUE;
			}
		} // close if (bRead)
		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(hCom, // Handle to COMM port
			&m_overlap, // Overlapped structure
			&BytesRead, // Stores number of bytes read
			TRUE); // Wait flag
		}
	}
}


BOOL WriteChar(BYTE* m_szWriteBuffer,DWORD m_nToSend)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	HANDLE m_hWriteEvent=NULL;
	ResetEvent(m_hWriteEvent);
	if (bWrite)
	{
		m_overlap.Offset = 0;
		m_overlap.OffsetHigh = 0;
		// Clear buffer
		bResult = WriteFile(hCom, // Handle to COMM Port
			m_szWriteBuffer, // Pointer to message buffer in calling finction
			m_nToSend, // Length of message to send
			&BytesSent, // Where to store the number of bytes sent
			&m_overlap ); // Overlapped structure
		if (!bResult)
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
			case ERROR_IO_PENDING:
				{
					// continue to GetOverlappedResults()
					BytesSent = 0;
					bWrite = FALSE;
					break;
				}
			default:
				{
					// all other error codes
					ProcessErrorMessage("WriteFile()");
				}
			}
		}
	} // end if(bWrite)
	if (!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult(hCom, // Handle to COMM port
			&m_overlap, // Overlapped structure
			&BytesSent, // Stores number of bytes sent
			TRUE); // Wait flag
		// deal with the error code
		if (!bResult)
		{
			ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}
	} // end if (!bWrite)
	// Verify that the data size send equals what we tried to send
	if (BytesSent != m_nToSend)
	{
		printf("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n",
			BytesSent, strlen((char*)m_szWriteBuffer));
	}
	return true;
}

LRESULT CTextFixtureDlg::OnCOM(WPARAM wParam,LPARAM lParam)
{
	CString m_strbyte;

	for (int i=0;i<24;i++)
	{
		m_strbyte.Format("%02X",m_comrx[i]);
		m_strcom+=m_strbyte;
	}
	m_ccomdisplay.SetWindowText(m_strcom);

	switch(m_comrx[5])
	{
	case 0x80:
		{
			OdometerVelocity(m_comrx);
			m_strcom="";
			break;
		}
	case 0x90:
		{
			TrackStatusdisplay();
			m_strcom="";
			break;
		}
	default:
		{
							   		AfxMessageBox("无法识别的信息!");
									break;
		}
	}
						 
	return 0;
}
void	Delayms(int m_ns)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	do
	{
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	}while(dfTim <0.00173575*m_ns);
}


void CTextFixtureDlg::OnCheckkeepgap() 
{
	// TODO: Add your control notification handler code here

	if (m_ccheckkeepgap.GetCheck())
	{
		m_bcheckkeepgap=TRUE;
		SetTimer(10,20,NULL);
	} 
	else
	{
		m_bcheckkeepgap=FALSE;
		KillTimer(10);
	}
}

void CTextFixtureDlg::OnCheckfsbenable() 
{
	// TODO: Add your control notification handler code here
	if (m_ccheckfsbenable.GetCheck())
	{
		AddAlarm("已启用FSB失效!");
	}
	else
	{
		AddAlarm("已取消FSB失效");
	}
	
}


LRESULT CTextFixtureDlg::OnCOMWRITE(WPARAM wParam,LPARAM lParam)
{
				/*
			//列车状态数据帧
			CString m_cstypestatus="30";
			//无源信标握手帧
			CString	m_cstypenopowersignalhandle="70";
			//无源信标数据帧
			CString m_cstypenopowersigaldata="10";
			//有源信标握手帧
			CString m_cstypepowersignalhandle="80";
			//有源信标数据帧
			CString m_cstypepowersigaldata="20";
			//轨道码数据
			CString m_cstypetrackcode="40";
			//头尾车
			CString m_cstypeheadtrail="50";
			*/
	switch (wParam)
	{
	case 0x30:
		{
			if (!m_cforbidstatus.GetCheck())
			{
				CString m_strcom;
				CString m_strnum;
				m_strnum.Format("%d",lParam);
				m_strcom="序列:";
				m_strcom+=m_strnum;
				m_strcom+="状态已发送!";
				AddAlarm(m_strcom);
			}
			break;
		}
	case 0x40:
		{
			CString m_strcom;
			CString m_strnum;
			m_strnum.Format("%d",lParam);
			m_strcom="序列:";
			m_strcom+=m_strnum;
			m_strcom+="轨道码已发送!";
			AddAlarm(m_strcom);
			break;
		}
	case 0x50:
		{
			CString m_strcom;
			CString m_strnum;
			m_strnum.Format("%d",lParam);
			m_strcom="序列:";
			m_strcom+=m_strnum;
			m_strcom+="头尾车已发送!";
			AddAlarm(m_strcom);
			break;
		}
	case 0x70:
		{
			CString m_strcom;
			CString m_strnum;
			m_strnum.Format("%d",lParam);
			m_strcom="序列:";
			m_strcom+=m_strnum;
			m_strcom+="APR首帧已发送!";
			AddAlarm(m_strcom);
			break;
		}
	case 0x10:
		{
			CString m_strcom;
			CString m_strnum;
			m_strnum.Format("%d",lParam);
			m_strcom="序列:";
			m_strcom+=m_strnum;
			m_strcom+="APR次帧已发送!";
			AddAlarm(m_strcom);
			break;
		}
	default:
		{
			break;
		}
	}

	return 0;
}

void CTextFixtureDlg::OnCheckforbidstatus() 
{
	// TODO: Add your control notification handler code here
	
}
