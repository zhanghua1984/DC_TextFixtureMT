// MyDraw.cpp: implementation of the CMyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TextFixture.h"
#include "MyDraw.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern double m_dstart;
extern float	m_fglobalspeed;
extern int		m_gapoffset;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMyDraw::CMyDraw(CListBox *m_pListMsg,CListBox *m_pListAlarm)
{
	pListMsg	=m_pListMsg;
	pListAlarm	=m_pListAlarm;

	SurRect.SetRect(0,490,1440,550);										//	显示区域
	BackGround.SetRect(0,468,1444,490);
	Vtable.SetRect(0,292,1440,468);
	m_dStartPos	= 0;	//	起始位置
	m_dEndPos	= 0;														//	终止位置
	m_bTrainHeadRight = FALSE;												// 运行方向 true 下行 false 上行
	m_DrawTab = FALSE;
	m_dAcc = 0;
	m_dSpeed = 0;
	m_dTrainPosition = 0;												//列车当前位置
	m_nlasttrainpos=0;
}

CMyDraw::~CMyDraw()
{

}

void CMyDraw::DrawShow(CPaintDC &dc)
{
	CDC MemDC;																										//内存绘图设备
	CBitmap MemBit;
	CString m_str;
//	int		m_id;

	CPen penGreen(PS_SOLID,1,RGB( 0, 204, 0));
	
	CPen penWhite(PS_SOLID,1,RGB( 255, 255, 255));
	
	CPen penBlue(PS_SOLID,1,RGB(100,100,215));
	
	CPen penRed(PS_SOLID,1,RGB( 221, 0, 0));
	
	CPen penR(PS_SOLID,1,RGB(250,0,0));
	CPen penG(PS_SOLID,1,RGB(0,250,0));
	CPen PenB(PS_SOLID,1,RGB(0,0,250));

	CBrush bruGreen(RGB(  0, 204, 0));
	
	CBrush bruBlack=(RGB(  0, 0, 0));

//	CFont *pFont15=(CFont *)CreateFont(15);
//	CFont *pFont10=(CFont *)CreateFont(1);
	

	MemDC.CreateCompatibleDC(&dc);	
	MemBit.CreateCompatibleBitmap(&dc,SurRect.Width(),SurRect.Height());
	MemDC.SelectObject(&MemBit);
	MemDC.FillSolidRect(&SurRect,RGB(0,0,0));																		//设定背景颜色
	MemDC.SetBkMode(TRANSPARENT);																					//设置背景模式


	if (m_DrawTab == TRUE)
	{	
		MemDC.SelectObject((CPen * )&penWhite);
		MemDC.SelectObject((CBrush * )&bruGreen);	

		for (UINT i=1;i<40; i++)
		{
			//绘制轨道
			x1=(int)((TRAIN_X-m_dstart)-m_dTrainPosition+m_stuDevs.stuDevs[0][i].dStartPara);
			x2=(int)((TRAIN_X-m_dstart)-m_dTrainPosition+m_stuDevs.stuDevs[0][i].dEndPara);
			MemDC.MoveTo(x1,TRAIN_Y2);
			MemDC.LineTo(x2,TRAIN_Y2);
			//绘制速度台阶

			DrawZone(MemDC, x2, TRAIN_Y2, 3);
			
			MemDC.SetTextColor(RGB(255, 255, 0));

//			MemDC.SelectObject(pFont15);
	
			MemDC.TextOut(x1+10,TRAIN_Y2+10,m_stuDevs.stuDevs[0][i].szName);

			m_str=m_stuDevs.stuDevs[0][i].dcarrie;

			if (m_str!="")
			{
				MemDC.TextOut(x1+50,TRAIN_Y2+25,m_stuDevs.stuDevs[0][i].dcarrie);
			}
			m_str=m_stuDevs.stuDevs[0][i].dcode;
			if (m_str!="")
			{
				MemDC.TextOut(x1+80,TRAIN_Y2+25,m_stuDevs.stuDevs[0][i].dcode);
			}
			
			
			MemDC.SelectObject((CPen * )&penGreen);

			MemDC.SelectObject((CBrush * )&bruBlack);

			//绘制APR位置
			
			if (m_stuDevs.stuDevs[0][i].dnp1apr!=0)
			{
				float	m_fapr;
				CRect	m_rectnp1;
				POINT	m_pTri[4];
				m_fapr=(((float)m_stuDevs.stuDevs[0][i].dnp1apr)/1000);

				//m_rectnp1.SetRect(x1+(int)m_fapr,TRAIN_Y2+APRYOFFSET,x1+(int)m_fapr+70,TRAIN_Y2+APRYOFFSET+20);
				//MemDC.Rectangle(m_rectnp1);
				m_pTri[0].x=x1+(int)m_fapr;
				m_pTri[0].y=TRAIN_Y2+APRYOFFSET;
				m_pTri[1].x=x1+(int)m_fapr-5;
				m_pTri[1].y=TRAIN_Y2+APRYOFFSET+5;
				m_pTri[2].x=x1+(int)m_fapr+5;
				m_pTri[2].y=TRAIN_Y2+APRYOFFSET+5;
				m_pTri[3].x=x1+(int)m_fapr;
				m_pTri[3].y=TRAIN_Y2+APRYOFFSET;
				MemDC.Polyline(m_pTri,4);
				m_str.Format("%.3f",m_fapr);

				MemDC.TextOut(x1+(int)m_fapr+APRXOFFSET,TRAIN_Y2+APRYOFFSET+APRIDYOFFSET,m_str);
				/*
				m_id=m_stuDevs.stuDevs[0][i].np1aprid;
				m_str.Format("%X",m_id);
				MemDC.TextOut(x1+(int)m_fapr+APRXOFFSET+1,TRAIN_Y2+APRYOFFSET+APRIDYOFFSET-32,m_str);
				*/

			}
			/*
			if (m_stuDevs.stuDevs[0][i].dnp2apr!=0)
			{
				float	m_fapr;
				CRect	m_rectnp2;

				m_fapr=(((float)m_stuDevs.stuDevs[0][i].dnp2apr)/1000);

				m_rectnp2.SetRect(x1+(int)m_fapr+APRXOFFSET,TRAIN_Y2+APRYOFFSET,x1+(int)m_fapr+APRXOFFSET+70,TRAIN_Y2+APRYOFFSET+20);

				MemDC.Rectangle(m_rectnp2);
				m_str.Format("%.3f",m_fapr);
				MemDC.TextOut(x1+(int)m_fapr+APRXOFFSET,TRAIN_Y2+APRYOFFSET+APRIDYOFFSET,m_str);
				m_id=m_stuDevs.stuDevs[0][i].np2aprid;
				m_str.Format("%X",m_id);
				
				MemDC.TextOut(x1+(int)m_fapr+APRXOFFSET+1,TRAIN_Y2+APRYOFFSET+APRIDYOFFSET-32,m_str);

			}
			*/
			MemDC.SelectObject((CPen * )&penRed);

			MemDC.SelectObject((CBrush * )&bruBlack);

			if (m_stuDevs.stuDevs[0][i].dp1apr!=0)
			{
				MemDC.Rectangle(x1+m_stuDevs.stuDevs[0][i].dp1apr+APRXOFFSET,TRAIN_Y2+APRYOFFSET,x1+m_stuDevs.stuDevs[0][i].dp1apr+APRXOFFSET+30,TRAIN_Y2+APRYOFFSET+20);
			}
			if (m_stuDevs.stuDevs[0][i].dp2apr!=0)
			{
				MemDC.Rectangle(x1+m_stuDevs.stuDevs[0][i].dp2apr+APRXOFFSET,TRAIN_Y2+APRYOFFSET,x1+m_stuDevs.stuDevs[0][i].dp2apr+APRXOFFSET+30,TRAIN_Y2+APRYOFFSET+20);
			}
		

			//恢复
			MemDC.SelectObject((CPen * )&penWhite);
			MemDC.SelectObject((CBrush * )&bruGreen);
		}
	}

	dc.BitBlt(SurRect.left,SurRect.top,SurRect.right,SurRect.bottom,&MemDC,0,0,SRCCOPY);	
	//复制到屏幕上去。
	DeleteObject(m_speeddisplay);
	penGreen.DeleteObject();
	penWhite.DeleteObject();
	penRed.DeleteObject();
	penBlue.DeleteObject();
	bruGreen.DeleteObject();
	bruBlack.DeleteObject();
//	pFont15->DeleteObject();
//	pFont10->DeleteObject();
	MemBit.DeleteObject();
	MemDC.DeleteDC();
	
}

void CMyDraw::DrawZone(CDC &MemDC, UINT X, UINT Y, UINT Tab)
{
	CPen penWhite(PS_SOLID,1,RGB( 255, 245, 252));
	MemDC.SelectObject((CPen * )&penWhite);
	
	if(Tab == 1){
		POINT Up[4]		={{X+4,Y-5},{X,Y-5},{X,Y+5},{X+5,Y+5}};
		MemDC.Polyline(Up,4);
		DeleteObject(Up);
	}
	else if(Tab == 2){
		POINT Down[4]	={{X-4,Y-5},{X,Y-5},{X,Y+5},{X-5,Y+5}};
		MemDC.Polyline(Down,4);
		DeleteObject(Down);
	}
	else if(Tab == 3){
		POINT TwoWay[6]	={{X-4,Y-5},{X+5,Y-5},{X,Y-5},{X,Y+5},{X+5,Y+5},{X-5,Y+5}};
		MemDC.Polyline(TwoWay,6);
		DeleteObject(TwoWay);
	}
	penWhite.DeleteObject();

}

void CMyDraw::DrawStation(CDC &MemDC)
{
//	CFont *pFont20=(CFont *)CreateFont(25);
	MemDC.SetTextColor(RGB(255, 102, 0));
//	MemDC.SelectObject(pFont20);
	MemDC.TextOut(0,TRAIN_Y1-90,"《西直门》");
	CPen penWhite(PS_SOLID,1,RGB( 255, 255, 255));
	CBrush bruWhite(RGB( 255, 255, 255));
	MemDC.SelectObject((CPen * )&penWhite);
	MemDC.SelectObject((CBrush * )	&bruWhite);
	for (UINT i=0; i<45; i++)
	{
		MemDC.MoveTo(TRAIN_X,TRAIN_Y1-100+10*i);
		MemDC.LineTo(TRAIN_X,TRAIN_Y1-100+10*i+5);
	}
	POINT train_Station_Among[4]	={{TRAIN_X-100,TRAIN_Y4-20},{TRAIN_X-100,TRAIN_Y4-130},{TRAIN_X+100,TRAIN_Y4-130},{TRAIN_X+100,TRAIN_Y4-20}};
	POINT train_Station_Up[4]		={{TRAIN_X-100,TRAIN_Y2-20},{TRAIN_X-100,TRAIN_Y2-55},{TRAIN_X+100,TRAIN_Y2-55},{TRAIN_X+100,TRAIN_Y2-20}};
	POINT train_Station_Down[4]		={{TRAIN_X-100,TRAIN_Y5-20},{TRAIN_X-100,TRAIN_Y5-55},{TRAIN_X+100,TRAIN_Y5-55},{TRAIN_X+100,TRAIN_Y5-20}};
	UINT A;
	A=1;
	if(A)
	{
		MemDC.Polygon(train_Station_Among,4);
		MemDC.Polygon(train_Station_Up,4);
		MemDC.Polygon(train_Station_Down,4);
	}
	else
	{
		MemDC.Polygon(train_Station_Up,4);
		MemDC.Polygon(train_Station_Down,4);
	}
	bruWhite.DeleteObject();
	penWhite.DeleteObject();
//	delete pFont20;
}

void CMyDraw::DrawTrain(CDC &MemDC)
{

	CPen penGreen(PS_SOLID,1,RGB( 0, 204, 0));
	CBrush bruGreen(RGB(  155, 25, 60));
	MemDC.SelectObject((CPen * )&penGreen);
	MemDC.SelectObject((CBrush * )	&bruGreen);	
	UINT Y1, a;
	a=2;
	switch(a)
	{
	case 1:
		Y1 = TRAIN_Y1;
		break;
	case 2:
		Y1 = TRAIN_Y2;
		break;
	case 3:
		Y1 = TRAIN_Y3;
		break;
	case 4:
		Y1 = TRAIN_Y4;
		break;
	case 5:
		Y1 = TRAIN_Y5;
		break;
	default:
		break;
	}
	if (m_bTrainHeadRight == TRUE)
	{
		POINT train_DOWN[7]	={{TRAIN_X-30,Y1-10},{TRAIN_X-10,Y1-20},{TRAIN_X-10,Y1-15},{TRAIN_X+30,Y1-15},{TRAIN_X+30,Y1-5},{TRAIN_X-10,Y1-5},{TRAIN_X-10,Y1-0}};
		MemDC.Polygon(train_DOWN,7);		
	}
	else
	{	
		POINT train_UP[7]	={{TRAIN_X+30,Y1-10},{TRAIN_X+10,Y1-20},{TRAIN_X+10,Y1-15},{TRAIN_X-30,Y1-15},{TRAIN_X-30,Y1-5},{TRAIN_X+10,Y1-5},{TRAIN_X+10,Y1-0}};
		MemDC.Polygon(train_UP,7);
	}
	bruGreen.DeleteObject();
	penGreen.DeleteObject();	
}

void CMyDraw::AddAlarm(CString szMsg)
{
	if (m_stralarm!=szMsg)
	{
		CString str,m_szTime;
		GetSysTime(&m_szTime);
		str.Format("[%s] %s",m_szTime,szMsg);	
		pListAlarm->AddString(str);	
		pListAlarm->SetCurSel(pListAlarm->GetCount()-1);

		str+="\r\n";

		CStdioFile sf;
		sf.Open("D:/operation.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
		//	sf.Seekg();
		//	sf.Seekp(0,ios::end);
		sf.SeekToEnd();
		sf.WriteString(str);//CStdioFile 的方法
		sf.Flush();
		sf.Close();
		m_stralarm=szMsg;
	}
	

}

void CMyDraw::AddMsg(CString szMsg)
{
	if (m_strinfo!=szMsg)
	{
		CString str,m_szTime;
		GetSysTime(&m_szTime);
		str.Format("[%s] %s",m_szTime,szMsg);	
		pListMsg->AddString(str);
		pListMsg->SetCurSel(pListMsg->GetCount()-1);
		str+="\r\n";
		CStdioFile sf;
		sf.Open("D:/running.txt",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate);
		sf.SeekToEnd();
		sf.WriteString(str);//CStdioFile 的方法
		sf.Flush();
		sf.Close();
		m_strinfo=szMsg;
	}



}

void CMyDraw::GetSysTime(CString *m_szTime)						//获取系统时间
{
	CTime ct(CTime::GetCurrentTime());
	SYSTEMTIME lSysTime;		
	GetSystemTime(&lSysTime);		
	m_szTime->Format("%s",ct.Format("%Y-%m-%d %H:%M:%S"));
}

void CMyDraw::LoadData()
{
	CString strFilePath = CONFIG_FILE_PATH;
	CString strDevNum;
	CString strDevType;
	CString strDevName;
	POINT	m_inip;
	for (int len=0;len<720;len++)
	{	
		m_inip.x=len;
		m_inip.y=SPEEDOFFSET;
		m_speeddisplay[len]=m_inip;
		if (len>600)
		{
			m_inip.x=600;
			m_speeddisplays[len]=m_inip;
		}
		else
		{
			m_speeddisplays[len]=m_inip;
		}

	}
	

	for(UINT j=0; j<2; j++)
	{
		for(UINT i=1; i<40; i++)
		{
			strDevNum.Format("LOOP01%02d%02d",j+1,i);
			GetPrivateProfileString(strDevNum,"NAME","NONE",m_stuDevs.stuDevs[j][i].szName,7,LPCSTR(strFilePath));
			m_stuDevs.stuDevs[j][i].dStartPara=(float)((GetPrivateProfileInt(strDevNum,"START",0,LPCSTR(strFilePath))-10000000));
			m_stuDevs.stuDevs[j][i].dEndPara=(float)((GetPrivateProfileInt(strDevNum,"END",0,LPCSTR(strFilePath))-10000000));
			m_stuDevs.stuDevs[j][i].dStartPara=m_stuDevs.stuDevs[j][i].dStartPara/100;
			m_stuDevs.stuDevs[j][i].dEndPara=m_stuDevs.stuDevs[j][i].dEndPara/100;
			m_stuDevs.stuDevs[j][i].dPara = m_stuDevs.stuDevs[j][i].dEndPara -m_stuDevs.stuDevs[j][i].dStartPara;
			GetPrivateProfileString(strDevNum,"CARRIER","",m_stuDevs.stuDevs[j][i].dcarrie,7,LPCSTR(strFilePath));
			GetPrivateProfileString(strDevNum,"CODE","",m_stuDevs.stuDevs[j][i].dcode,7,LPCSTR(strFilePath));
			GetPrivateProfileString(strDevNum,"CODECLEARANCEC","",m_stuDevs.stuDevs[j][i].codeclearance,7,LPCSTR(strFilePath));
			m_stuDevs.stuDevs[j][i].dnp1apr=((GetPrivateProfileInt(strDevNum,"NPAPR1",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].dnp2apr=((GetPrivateProfileInt(strDevNum,"NPARP2",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].dp1apr=((GetPrivateProfileInt(strDevNum,"PAPR1",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].dp2apr=((GetPrivateProfileInt(strDevNum,"PARP2",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].np1aprid=((GetPrivateProfileInt(strDevNum,"NPAPR1ID",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].np2aprid=((GetPrivateProfileInt(strDevNum,"NPAPR2ID",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].speed=((GetPrivateProfileInt(strDevNum,"SPEED",0,LPCSTR(strFilePath))));
			m_stuDevs.stuDevs[j][i].ndcodeclearance=((GetPrivateProfileInt(strDevNum,"CODECLEARANCED",0,LPCSTR(strFilePath))));

		}
	}
	m_DrawTab = TRUE;
	/*
	UINT GetPrivateProfileInt(
		LPCTSTR lpAppName,  // address of section name
		LPCTSTR lpKeyName,  // address of key name
		INT nDefault,       // return value if key name is not found
		LPCTSTR lpFileName  // address of initialization filename
		);
	*/
	m_gapoffset=GetPrivateProfileInt("GAPOFFSET","offset",0,LPCSTR(strFilePath));
}

void *CMyDraw::CreateFont(int nSize,char *szFacename,int nWeight,bool bItalic,bool bUnderline)
{
	CFont *pFont=new CFont;
	pFont->CreateFont( nSize,					       // nHeight
							0,                         // nWidth
							0,                         // nEscapement
							0,                         // nOrientation
							nWeight,		           // nWeight
							bItalic,                   // bItalic
							bUnderline,                // bUnderline
							0,                         // cStrikeOut
							ANSI_CHARSET,              // nCharSet
							OUT_DEFAULT_PRECIS,        // nOutPrecision
							CLIP_DEFAULT_PRECIS,       // nClipPrecision
							DEFAULT_QUALITY,           // nQuality
							DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
							szFacename);               // lpszFacename
	return (void *)pFont;
}

void CMyDraw::DrawBackGround(CPaintDC &dc)
{
	CDC MemDC2;																										//内存绘图设备
	CBitmap MemBit2;
//	CFont *pFont15=(CFont *)CreateFont(15);

	CPen penGreen(PS_SOLID,1,RGB( 0, 204, 0));
	CPen penWhite(PS_SOLID,1,RGB( 255, 255, 255));
	CBrush bruGreen(RGB(  0, 204, 0));

	MemDC2.CreateCompatibleDC(&dc);	
	MemBit2.CreateCompatibleBitmap(&dc,BackGround.Width(),BackGround.Height());
	MemDC2.SelectObject(&MemBit2);
	MemDC2.FillSolidRect(&BackGround,RGB(0,0,0));																		//设定背景颜色
	MemDC2.SetBkMode(TRANSPARENT);																					//设置背景模式

	dc.BitBlt(BackGround.left,BackGround.top,BackGround.right,BackGround.bottom,&MemDC2,0,0,SRCCOPY);	

	penGreen.DeleteObject();
	penWhite.DeleteObject();
	bruGreen.DeleteObject();
//	pFont15->DeleteObject();
	MemBit2.DeleteObject();
	MemDC2.DeleteDC();
	
}

void CMyDraw::DrawVtable(CPaintDC &dc)
{
	CDC MemDC2;																										//内存绘图设备
	CBitmap MemBit2;
	CString m_str;
	int m_ncode,m_nfsb,m_ntir,m_nesi;
//	CFont *pFont15=(CFont *)CreateFont(15);
	CPen penBlue(PS_SOLID,1,RGB(100,100,215));
	CPen penGreen(PS_SOLID,1,RGB( 0, 120, 0));
	CPen penWhite(PS_SOLID,1,RGB( 255, 255, 255));
	CPen penRed(PS_SOLID,1,RGB( 50, 240, 80));
	CPen penYellow(PS_SOLID,1,RGB(12,240,240));
	CPen penR(PS_SOLID,1,RGB(240,0,0));
	CPen penG(PS_SOLID,1,RGB(0,240,0));
	CPen penB(PS_SOLID,1,RGB(240,240,0));
	CBrush bruGreen(RGB(  242, 204, 0));

	MemDC2.CreateCompatibleDC(&dc);	
	MemBit2.CreateCompatibleBitmap(&dc,Vtable.Width(),Vtable.Height());
	MemDC2.SelectObject(&MemBit2);
	MemDC2.FillSolidRect(&Vtable,RGB(0,0,0));																		//设定背景颜色
	MemDC2.SetBkMode(TRANSPARENT);																					//设置背景模式
	
	//文字说明
	m_ncode=950;
	MemDC2.SetTextColor(RGB(240, 0, 0));
				
//	MemDC2.SelectObject(pFont15);
				
	MemDC2.TextOut(m_ncode,2,"EBR速度");
	
	
	MemDC2.SetTextColor(RGB(240, 240, 0));
				
//	MemDC2.SelectObject(pFont15);
				
	MemDC2.TextOut(m_ncode+100,2,"最大安全速度");


	MemDC2.SetTextColor(RGB(0, 240, 0));
				
//	MemDC2.SelectObject(pFont15);
				
	MemDC2.TextOut(m_ncode+200,2,"目标速度");
	
	
	
	//速度曲线
	
	MemDC2.SelectObject((CPen * )&penBlue);
	MemDC2.SelectObject((CBrush * )&bruGreen);	
//	MemDC2.SelectObject(pFont15);
	MemDC2.SetTextColor(RGB(240, 240, 20));
	//绘制速度表格
	
	int x=0,y=10,k=0;
	for (k=0;k<=9;k++)
	{
		MemDC2.MoveTo(x,y+k*OFFSET);
		MemDC2.LineTo(x+720,y+k*OFFSET);
		
		m_str.Format("%d",90-k*10);
		MemDC2.TextOut(x+720,y+k*OFFSET-10,m_str);
		
		MemDC2.MoveTo(x,y+k*OFFSET);
		MemDC2.LineTo(x,y+OFFSET);
		
	}
	x=20;
	y=10;
	for (k=0;k<=7;k++)
	{

		MemDC2.MoveTo(x,y);
		MemDC2.LineTo(x,VYMETTER);
		m_str.Format("%d",720-x);
		MemDC2.TextOut(x,VYMETTER,m_str);
		x+=100;
	}

	MemDC2.SelectObject((CPen * )&penRed);
	MemDC2.SelectObject((CBrush * )&bruGreen);	
	if (m_nlasttrainpos!=(int)m_dTrainPosition)
	{
		
		for (k=0;k<719;k++)
		{
			m_speeddisplay[k].y=m_speeddisplay[k+1].y;
			m_speeddisplay[k].x=m_speeddisplay[k+1].x-1;
			m_speeddisplays[k].y=m_speeddisplays[k+1].y;
			m_speeddisplays[k].x=m_speeddisplays[k+1].x-1;
		}
		
		m_speeddisplay[719].x=720;
		m_speeddisplay[719].y=+SPEEDOFFSET-(int)(m_fglobalspeed*1.6);
		
		m_speeddisplays[719].x=600;
		m_speeddisplays[719].y=+SPEEDOFFSET-(int)(m_fglobalspeed*1.6);


		m_nlasttrainpos=(int)m_dTrainPosition;
	}
	
	MemDC2.Polyline(m_speeddisplay,720);	
	MemDC2.SelectObject((CPen * )&penYellow);
	MemDC2.Polyline(m_speeddisplays,720);

	//速度台阶

	for (UINT i=1;i<31; i++)
	{
		//绘制轨道
		x1=(int)((TRAIN_X-m_dstart)-m_dTrainPosition+m_stuDevs.stuDevs[0][i].dStartPara);
		x2=(int)((TRAIN_X-m_dstart)-m_dTrainPosition+m_stuDevs.stuDevs[0][i].dEndPara);


		m_str=m_stuDevs.stuDevs[0][i].dcode;
			
			if(m_str!="")
			{

				m_str.Delete(0,1);
				m_ncode=atoi(m_str);
				switch(m_ncode)
				{
				case 1:
					{
						m_nfsb=74;
						m_ntir=73;
						m_nesi=75;
						break;
					}
				case 2:
					{
						m_nfsb=74;
						m_ntir=58;
						m_nesi=75;
						break;
					}
				case 3:
					{
						m_nfsb=59;
						m_ntir=58;
						m_nesi=60;
						break;
					}
				case 4:
					{
						m_nfsb=59;
						m_ntir=37;
						m_nesi=60;
						break;
					}
				case 5:
					{
						m_nfsb=38;
						m_ntir=37;
						m_nesi=39;
						break;
					}
				case 6:
					{
						m_nfsb=38;
						m_ntir=27;
						m_nesi=39;
						break;
					}
				case 7:
					{
						m_nfsb=74;
						m_ntir=73;
						m_nesi=75;
						break;
					}
				case 8:
					{
						m_nfsb=38;
						m_ntir=0;
						m_nesi=39;
						break;
					}
				case 9:
					{
						m_nfsb=28;
						m_ntir=27;
						m_nesi=29;
						break;
					}
				case 10:
					{
						m_nfsb=28;
						m_ntir=19;
						m_nesi=29;
						break;
					}
				case 11:
					{
						m_nfsb=28;
						m_ntir=0;
						m_nesi=29;
						break;
					}
				case 12:
					{
						m_nfsb=74;
						m_ntir=73;
						m_nesi=75;
						break;
					}
				case 13:
					{
						m_nfsb=20;
						m_ntir=0;
						m_nesi=21;
						break;
					}
				case 14:
					{
						m_nfsb=0;
						m_ntir=0;
						m_nesi=0;
						break;
					}
				default:
					{
						m_nfsb=74;
						m_ntir=73;
						m_nesi=75;
						break;
					}
				}

				MemDC2.SelectObject((CPen * )&penB);

				MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_nfsb*1.6));
				MemDC2.LineTo(x2,SPEEDOFFSET-(int)(m_nfsb*1.6));
				

				
				MemDC2.SelectObject((CPen * )&penG);
				MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_ntir*1.6));
				MemDC2.LineTo(x2,SPEEDOFFSET-(int)(m_ntir*1.6));
				

				
				MemDC2.SelectObject((CPen * )&penR);
				MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_nesi*1.6));
				MemDC2.LineTo(x2,SPEEDOFFSET-(int)(m_nesi*1.6));
				


				if (m_first!=TRUE)
				{


					if ((i==1)||(i==30))
					{
						pBx2.x=x2;
						pBx2.y=SPEEDOFFSET-(int)(m_nfsb*1.6);
						
						pGx2.x=x2;
						pGx2.y=SPEEDOFFSET-(int)(m_ntir*1.6);
						
						pRx2.x=x2;
						pRx2.y=SPEEDOFFSET-(int)(m_nesi*1.6);
					}
					else
					{
						MemDC2.SelectObject((CPen * )&penB);
						MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_nfsb*1.6));
						MemDC2.LineTo(pBx2);
						
						MemDC2.SelectObject((CPen * )&penG);
						MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_ntir*1.6));
						MemDC2.LineTo(pGx2);
						
						
						MemDC2.SelectObject((CPen * )&penR);
						MemDC2.MoveTo(x1,SPEEDOFFSET-(int)(m_nesi*1.6));
						MemDC2.LineTo(pRx2);
						
						
						pBx2.x=x2;
						pBx2.y=SPEEDOFFSET-(int)(m_nfsb*1.6);
						
						pGx2.x=x2;
						pGx2.y=SPEEDOFFSET-(int)(m_ntir*1.6);
						
						pRx2.x=x2;
						pRx2.y=SPEEDOFFSET-(int)(m_nesi*1.6);
					}

				}
				else
				{
					m_first=FALSE;
				}

			}

	}

	dc.BitBlt(Vtable.left,Vtable.top,Vtable.right,Vtable.bottom,&MemDC2,0,0,SRCCOPY);	

	penRed.DeleteObject();
	penBlue.DeleteObject();
	penGreen.DeleteObject();
	penWhite.DeleteObject();
	bruGreen.DeleteObject();
//	pFont15->DeleteObject();
	MemBit2.DeleteObject();
	MemDC2.DeleteDC();
}


void CMyDraw::DrawSpeedStep(CPaintDC &dc)
{

}
