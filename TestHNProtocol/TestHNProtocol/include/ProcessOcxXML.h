
#ifndef  _SCEWPRC_H
#define  _SCEWPRC_H

#include <iostream>
#include "HNProtocol.h"
using namespace std;


class ProcessOcxXML:public HNProtocol
{
public:
	ProcessOcxXML();
	~ProcessOcxXML();

public:
	void GetHeartBeat(int version) {}
	void SendHeartBeat(int version) {}
	int  SendVideoReq(int version);
private:
};

#endif