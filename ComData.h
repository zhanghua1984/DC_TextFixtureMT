
//帧类型定义
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
//列车状态
//测试
CString m_csstatustest="AA";
//模拟
CString m_csstatuscontrol="55";

int FrameCounter(int currentcounter);

int crc16(unsigned char *pchMsg,unsigned int wDataLen);

