#ifndef PROCESS_XML_H
#define PROCESS_XML_H


//#ifndef  _SCEWPRC_H
//#define  _SCEWPRC_H

// for debug
#ifndef _DEBUG_FLAG_
#define _DEBUG_FLAG_	0
#endif


#include <iostream>
#include "HNProtocol.h"

using namespace std;



class ProcessXML:public HNProtocol
{
public:
	ProcessXML();
	~ProcessXML();

public:
	void GetHeartBeat(int version) {}
	void SendHeartBeat(int version) {}
	int  SendVideoReq(int version);


};

//#endif

#endif // PROCESS_XML_H