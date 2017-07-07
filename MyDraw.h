// MyDraw.h: interface for the CMyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include <math.h>
#if !defined(AFX_MYDRAW_H__D79F35F2_C305_42AC_A9CA_E960383DEA9F__INCLUDED_)
#define AFX_MYDRAW_H__D79F35F2_C305_42AC_A9CA_E960383DEA9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define TRAIN_X		720
#define TRAIN_Y1	100
#define TRAIN_Y2	5
#define TRAIN_Y3	250
#define TRAIN_Y4	180
#define TRAIN_Y5	400
#define APRXOFFSET	20
#define APRYOFFSET	3
#define APRIDYOFFSET 35
#define MILLIMETER	1000
#define OFFSET		16
#define SPEEDOFFSET 155
#define VYMETTER	158
class CMyDraw 
{
public:
	CMyDraw(CListBox *m_pListMsg,CListBox *m_pListAlarm);
	virtual ~CMyDraw();

public:

	
	POINT m_speeddisplay[720];
	POINT m_speeddisplays[720];
	struct _stuDev
	{
		UINT nType[5];					//	վ���
		CHAR szName[7];				//	���������
		float dStartPara;				//	���
		float dEndPara;					//	�յ�
		float dPara;						//	����
		UINT dnp1apr;
		UINT dnp2apr;
		UINT np1aprid;
		UINT np2aprid;
		UINT dp1apr;
		UINT dp2apr;
		CHAR dcode[5];
		CHAR dcarrie[5];
		UINT nDir;						//	���з��� 1����, 2����, 0˫��
		BOOL bStation;					//	false��, TRUEվ̨
		UINT nInOut;					//	����� 
		int speed;
		UINT ndcodeclearance;
		CHAR codeclearance[5];
 
		unsigned char ucValue[100];
	};
	struct _stuDevs
	{
		int nDevSum;
		struct _stuDev stuDevs[2][150];
	};

public:
	void DrawShow(CPaintDC &dc);
	void DrawTrain(CDC &MemDC);
	void DrawStation(CDC &MemDC);
	void DrawZone(CDC &MemDC, UINT x, UINT y, UINT tab);

	void AddMsg(CString szMsg);
	void AddAlarm(CString szMsg);
	void GetSysTime(CString *m_szTime);
	void LoadData();
	void *CreateFont(int nSize,char *szFacename="������",int nWeight=FW_NORMAL,bool bItalic=false,bool bUnderline=false);

public:
	void DrawSpeedStep(CPaintDC &dc);
	void DrawVtable(CPaintDC &dc);
	void DrawBackGround(CPaintDC &dc);
	CRect SurRect;						//	��ʾ����
	CRect BackGround;					//��������
	CRect Vtable;						//�ٶȱ��
	CListBox *pListMsg;
	CListBox *pListAlarm;
	struct _stuDevs m_stuDevs;
	BOOL m_bTrainHeadRight;				//	�г����з���
	int  m_nlasttrainpos;
	double	m_dStartPos;				//	��ʼλ��
	double	m_dEndPos;					//	��ֹλ��
	double  m_dTrackStartPos;
	BOOL	m_DrawTab;
	double	m_dAcc;
	double	m_dSpeed;
	double	m_dTrainPosition;
	UINT x1,x2,x3,x4;
	BOOL m_first;
	POINT pRx1,pRx2,pGx1,pGx2,pBx1,pBx2;
	CString m_stralarm;
	CString m_strinfo;
};

#endif // !defined(AFX_MYDRAW_H__D79F35F2_C305_42AC_A9CA_E960383DEA9F__INCLUDED_)
