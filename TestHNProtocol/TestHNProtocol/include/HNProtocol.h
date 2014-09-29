
#ifndef _HNPROTOCOL_H
#define _HNPROTOCOL_H

#include <scew.h>

class HNProtocol
{
public:
	HNProtocol();
	~HNProtocol();
public:
	virtual void GetHeartBeat(int version)  = 0;
	virtual void SendHeartBeat(int version) = 0;
	virtual int  SendVideoReq(int version)  = 0;


public:
};
#endif