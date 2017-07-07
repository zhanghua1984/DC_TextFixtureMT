// TextFixtureDlg.h : header file
//
//{{AFX_INCLUDES()
//}}AFX_INCLUDES

#if !defined(AFX_TEXTFIXTUREDLG_H__1E8052B5_BC5A_464F_ACA7_42ABEB41C64B__INCLUDED_)
#define AFX_TEXTFIXTUREDLG_H__1E8052B5_BC5A_464F_ACA7_42ABEB41C64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTextFixtureDlg dialog
#include "afxwin.h"
#include "MyDraw.h"
#include "comdef.h"
#include "TextFixtureDlg.h"
#include "PictureEx.h"
#define BUFFER 960000
#define PI	3.1415926
#define GOAHEAD	1
#define	GOBACK	4
#define TRACKLENTH 80
#define BACKOFFSET 0
#define SPEEDLIMIT 2
#define AUTOSTOPOFFSET 3
#define SLIPDISTANCE	0
#define TARGETSPEED		5
#define TRACKNUMBER		31
#define UM_COM WM_USER+1
#define UM_COMWRITER WM_USER+2

	typedef struct APRNode 
	{
		float APRapos;
		UINT  APRid;
		struct APRNode * prior;
		struct APRNode * next;
	}APRNode,* pAPRNode;

	UINT	ThreadFunc(LPVOID lpParam);
	UINT	ThreadWrite(LPVOID lpParam);
	UINT	ThreadRead(LPVOID lpParam);
	BOOL	ProcessErrorMessage(char* ErrorText);
	BOOL	OpenPort(char *portname);
	BOOL	setupdcb(int rate_arg);
	BOOL	setuptimeout(DWORD ReadInterval,DWORD ReadTotalMultiplier,DWORD
		ReadTotalconstant,DWORD WriteTotalMultiplier,DWORD WriteTotalconstant);
	void ReceiveChar(LPVOID lpParam);
BOOL WriteChar(BYTE* m_szWriteBuffer,DWORD m_nToSend);
void	Delayms(int m_ns);

class CTextFixtureDlg : public CDialog , public CMyDraw
{
// Construction
public:
	CTextFixtureDlg(CWnd* pParent = NULL);	// standard constructor
	LRESULT OnCOM(WPARAM wParam,LPARAM lParam);
	LRESULT OnCOMWRITE(WPARAM wParam,LPARAM lParam);
	int m_nBaud;       //波特率
	int m_nCom;         //串口号
	char m_cParity;    //校验
	int m_nDatabits;    //数据位
	int m_nStopbits;    //停止位
	DWORD m_dwCommEvents;
	CString	m_csframehead;
	//头尾车方向
	int m_head;
	int m_direct;
	char m_charcounter;
	CString m_zero;
	CString m_cscrc16;
	int m_rxlenth;
	bool	m_trackhead;
	byte	m_byteatoatp;
	byte	m_bytetechop;
	byte	m_bytetechop2;
	byte	m_bytedo1;
	byte	m_bytedo2;
	bool	m_ebenable;
	bool	m_boolfsbenable;
	bool	m_bool;
	double		m_inttracklenth;
	bool	m_first;
	bool	m_boolsetstart;
	double  m_dautostop;
	double	m_dreaderoffset;
	float	m_fsetv;
	double  m_ddistance;
	bool	m_bools;
	byte	m_bytebatter;
	unsigned int m_uintaprdistance;
	unsigned int m_uintaprid;
	int		m_sendnumber;
	int		m_send1or2;
	CString m_strReceive;
	CByteArray m_szSendArray;
	unsigned int m_vt;
	unsigned int m_stoppos;
	unsigned int m_curpos;
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus,dfFreq,dfTim;
	float	m_ftranoffset;
	byte	m_carraycode1;
	byte	m_carraycode2;
	BOOL	m_codegap;
	int		m_nwheel;
//	CWinThread* pThread;

// Dialog Data
	//{{AFX_DATA(CTextFixtureDlg)
	enum { IDD = IDD_TEXTFIXTURE_DIALOG };
	CButton	m_cforbidstatus;
	CButton	m_ccheckkeepgap;
	CButton	m_ccheckfsbenable;
	CStatic	m_catpm;
	CStatic	m_compm;
	CStatic	m_ccomdisplay;
	CButton	m_ccheckfsbebenble;
	CComboBox	m_ccode2;
	CComboBox	m_ccode1;
	CComboBox	m_carray2;
	CComboBox	m_carray1;
	CButton	m_cgapsmode;
	CEdit	m_cgapstoptime;
	CEdit	m_cpersent;
	CButton	m_ctachooverhead;
	CButton	m_tacho2;
	CButton	m_tacho1;
	CEdit	m_gaps2;
	CEdit	m_gaps1;
	CButton	m_fsk2;
	CButton	m_fsk1;
	CEdit	m_ccodeclearancedis;
	CEdit	m_ccodeclearance;
	CButton	m_cchecktechop2;
	CEdit	m_cpostion;
	CEdit	m_cspeedlimit;
	CEdit	m_cantenna;
	CEdit	m_chealth;
	CButton	m_ceumr;
	CButton	m_ctirr;
	CButton	m_crfr2;
	CButton	m_crfr1;
	CEdit	m_cchannel;
	CEdit	m_cbeep;
	CEdit	m_creaddistance;
	CEdit	m_crestrictv;
	CEdit	m_cworkstatus;
	CButton	m_creaderpower;
	CButton	m_creadernopower;
	CEdit	m_clogics;
	CEdit	m_cfrusenddistance;
	CEdit	m_cnpapr2id;
	CEdit	m_cnpapr1id;
	CEdit	m_caprsenddistance;
	CEdit	m_cpapr2;
	CEdit	m_cpapr1;
	CEdit	m_cnpapr2;
	CEdit	m_cnpapr1;
	CComboBox	m_ctrackselect;
	CEdit	m_cstart;
	CEdit	m_cautostop;
	CEdit	m_creaderoffset;
	CEdit	m_ceditv;
	CPictureEx 	m_cstaticpicture;
	CStatic	m_cstaticpic;
	CComboBox	m_ccodefrequency;
	CComboBox	m_ccarriefrequency;
	CComboBox	m_cwheeld;
	CEdit	m_cacctimes;
	CButton	m_checktir;
	CButton	m_checkfsbresult;
	CButton	m_checkzvrresult;
	CButton	m_checkzvr2;
	CButton	m_checkzvr1;
	CButton	m_checkrderrulult;
	CButton	m_checkrder2;
	CButton	m_checkrder1;
	CButton	m_checklderresult;
	CButton	m_checklder2;
	CButton	m_checklder1;
	CButton	m_checkfsb2;
	CButton	m_checkfsb1;
	CButton	m_checkebresult;
	CButton	m_checkeb2;
	CButton	m_checkeb1;
	CButton	m_checktechop;
	CButton	m_checkatp2;
	CButton	m_checkatp1;
	CButton	m_checkato;
	CButton	m_check9;
	CButton	m_check8;
	CButton	m_check7;
	CButton	m_check6;
	CButton	m_check5;
	CButton	m_check4;
	CButton	m_check3;
	CButton	m_check2;
	CButton	m_check16;
	CButton	m_check15;
	CButton	m_check14;
	CButton	m_check13;
	CButton	m_check12;
	CButton	m_check11;
	CButton	m_check10;
	CButton	m_check1;
	CButton	m_radiotrial;
	CButton	m_radiohead;
	CComboBox	m_ccbmove;
	CEdit	m_cdistance;
	CEdit	m_cdirector;
	CComboBox	m_cbStation;
	CComboBox	m_cbDir;
	CEdit	m_eSpeed;
	CEdit	m_eSpeedAto;
	CEdit	m_eAccAto;
	CEdit	m_eAcc;
	CListBox	m_pListAlarm;
	CListBox	m_pListMsg;
	CButton	m_rTry;
	CButton	m_rNor;
	CSliderCtrl	m_SldAcc;
	CString	m_csspeed;
	CString	m_csdirector;
	CString	m_csdistance;
	CString	m_cbmove;
	CString	m_csacctimes;
	CString	m_cswheeld;
	CString	m_cscarriefrequency;
	CString	m_cscodefrequency;
	CString	m_cseditv;
	CString	m_csreaderoffset;
	CString	m_csautostop;
	CString	m_csstart;
	CString	m_csaprsenddistance;
	CString	m_csfrusenddistance;
	CString	m_cslogics;
	CString	m_csworkstatus;
	CString	m_csrestrictv;
	CString	m_csreaddistance;
	CString	m_csbeep;
	CString	m_cschannel;
	CString	m_cshealth;
	CString	m_csantenna;
	CString	m_csspeedlimit;
	CString	m_csfirstbreakdistance;
	CString	m_csfirstbreaktime;
	CString	m_csslipbreakdistance;
	CString	m_csslipbreaktime;
	CString	m_cssecondbreakdistance;
	CString	m_cssecondbreaktime;
	CString	m_csbreakerror;
	CString	m_cspostion;
	CString	m_cstrackzoom;
	CString	m_codeclearance;
	CString	m_codeclearancedis;
	CString	m_cspersent;
	CString	m_csgapstoptime;
	CString	m_cscode1;
	CString	m_cscode2;
	CString	m_cscarray1;
	CString	m_cscarray2;
	CString	m_strcom;
	BOOL	m_bcheckkeepgap;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextFixtureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTextFixtureDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOutofmemorySliderAcc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRoute();
	afx_msg void OnRadioNor();
	afx_msg void OnRadioTry();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeComboDir();
	afx_msg void OnRadiohead();
	afx_msg void OnRadiotrial();
	afx_msg void OnSelchangeComboMove();
	afx_msg void OnButtonrelayallwork();
	afx_msg void OnButtonrelayallfail();
	afx_msg void OnCheck1();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnCheck16();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnCheck7();
	afx_msg void OnCheck8();
	afx_msg void OnCheck9();
	afx_msg void OnCheckato();
	afx_msg void OnCheckatp1();
	afx_msg void OnCheckatp2();
	afx_msg void OnChecktechop();
	afx_msg void OnNopower();
	afx_msg void OnPower();
	afx_msg void OnTrackcode();
	afx_msg void OnSelchangeTrackselect();
	afx_msg void OnSaveinidata();
	afx_msg void OnSelchangeCombocarrierfrequency();
	afx_msg void OnSelchangeCombocodefrequncey();
	afx_msg void OnSaveinidatatofile();
	afx_msg void OnReaderpower();
	afx_msg void OnReadernopower();
	afx_msg void OnButtonsetv();
	afx_msg void OnButtonautostop();
	afx_msg void OnButtonreaderoffset();
	afx_msg void OnButtonstart();
	afx_msg void OnButtonaproffset();
	afx_msg void OnButtoncodeoffset();
	afx_msg void OnButtonlimitv();
	afx_msg void OnSelchangeCombowheel();
	afx_msg void OnButtonantenna();
	afx_msg void OnClear();
	afx_msg void OnChecktechop2();
	afx_msg void OnFsk1();
	afx_msg void OnFsk2();
	afx_msg void OnTacho1();
	afx_msg void OnTacho2();
	afx_msg void OnSendcodegaps();
	afx_msg void OnTachooverhead();
	afx_msg void OnGapsmode();
	afx_msg void OnSend();
	afx_msg void OnDestroy();
	afx_msg void OnCheckfsbebenable();
	afx_msg void OnCheckkeepgap();
	afx_msg void OnCheckfsbenable();
	afx_msg void OnCheckforbidstatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
	void InitProjetc();	

public:
	BOOL Ischanged(double pos);
	void InitAPRNode();
	BOOL m_odd;
	float SecondAPRd;
	float SecondAPR;
	float FirstAPRd;
	float FirstAPR;
	void CheckStop();
	void SencondBreak();
	float BreakDistance();
	BOOL m_bbreaking;
	float m_addoffset;
	float m_fbreakpoint;
	void CheckSpeedZero();
	BOOL bCruise;
	BOOL Autostopstart;
	void AutoStop(unsigned int vt, unsigned int stoppos,int curpos);
	unsigned int m_ungloblespeed;
	BOOL m_blimit;
	int m_noverspeed;
	void CheckOverSpeed();
	void SetCruise();
	int m_accpos;
	void ContinueSendAPR();
	void SendTrackCode(unsigned int m_code);
	void SendAPR(double m_ds);
	void CheckDisplay();
	BOOL UporDown();
	void TrackStatusdisplay();
	void OdometerVelocity(byte * frame);
	CString ComposeTrackCode(byte m_fru,byte m_code,unsigned int m_pos);
	CString ComposeHeadTrial();
	CString ComposePowerSignalData();
	CString ComposePowerSignalHandle();
	CString ComposeNoPowerSignalData(unsigned int m_id);
	CString ComposeNoPowerSignalHandle(unsigned int m_pos);
	void NoPowerSigal(unsigned int m_pos,unsigned int m_id);
	void PowerSigal();
	void TrackCode(byte m_fru,byte m_code,unsigned int m_pos);
	void HeadTrial();
	void SendFramebySerial(CString Frame);
	CString ComposeTrackStatus(double acc, int head , int direct);
	void TrackStatus(DOUBLE acc, int head , int direct);
	CString CalculatestrCRC(CString strtocrc,int lenth);
	unsigned short int Get_CRC16_Code(unsigned char* pchMsg, unsigned short int wDataLen);
	char ConvertHexData(char ch);
	int String2Hex(CString str, char *SendOut);

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTFIXTUREDLG_H__1E8052B5_BC5A_464F_ACA7_42ABEB41C64B__INCLUDED_)
