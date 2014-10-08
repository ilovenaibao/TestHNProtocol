#ifndef PROCESS_XML_H
#define PROCESS_XML_H


#ifndef  _SCEWPRC_H
#define  _SCEWPRC_H

// for debug
#ifndef _DEBUG_FLAG_
#define _DEBUG_FLAG_
#endif


#include <iostream>
#include "HNProtocol.h"
#include "../BaseXmlParser.h"
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

///////////////////////////SendVideoBuffer Class////////////////////////////////////

// 发送data
enum {
	SEND_BLOCK_DATA_LEN = 0,
	SEND_VERSION,
	SEND_CMD_CODE,
	SEND_XML_LEN,
	SEND_SOURCE_ID,
	SEND_DEST_ID,
	SEND_CUSTOMER_KIND
};

typedef struct {
	int block_data_len; // 数据长度
	int version; // 版本号
	int cmd_code; // 指令编号
	int xml_len; // xml长度
	int source_id; // 源业务ID
	int dest_id; // 目的业务ID
	int customer_kind; // 客户类型
	int data_len; // 发送数据的长度
	char *xml_data; // xml数据
	char *data; // 发送的数据

	void SendVideoReqData()
	{
		block_data_len = 0;
		version = 0;
		cmd_code = 0;
		xml_len = 0;
		source_id = 0;
		dest_id = 0;
		customer_kind = 0;

		data_len = 0;
		xml_data = NULL;
		data = NULL;
	}

} SendVideoReqData;

class ProcessSendVideoBuffer:public ProcessXML
{
public:
	ProcessSendVideoBuffer();
	~ProcessSendVideoBuffer();

	// SendVideoBuffer
	int InitSendVideoBuffer();
	int SetSendVideoBufferXml(XML_Char *xml_buffer);
	int ProcessSendVideoBuffer::SetSendVideoBufferData(char *data, int data_size);
	int UpdateSendVideoBuffer();
	int GetSendVideoBuffer(char *p_in = NULL);
	SendVideoReqData GetSendVideoBufferHeader();

public:
	// block send data used
	SendVideoReqData m_send_video_req_header;
	char *m_send_video_req_buf;
	XML_Char *m_xml_buffer;

};
//////////////////////SendVideoBuffer Class End/////////////////////////////////////////


//////////////////////ReceiveVideoBuffer Class//////////////////////////////////////////

// 接收data
enum {
	RECV_BLOCK_DATA_LEN = 0,
	RECV_VERSION,
	RECV_CMD_CODE,
	RECV_XML_LEN,
	RECV_SUCCESS_FLAG,
	RECV_CUR_TIME
};

typedef struct {
	int block_data_len; // 数据长度
	int version; // 版本号
	int cmd_code; // 指令编号
	int xml_len; // xml长度
	int success_flag; // 成功标志
	int cur_time; // 当前时间
	char *xml_data; // xml数据
	char *data; // 接收数据

	void ReceiveVideoReqData()
	{
		block_data_len = 0;
		version = 0;
		cmd_code = 0;
		xml_len = 0;
		success_flag = 0;
		cur_time = 0;
		xml_data = NULL;
		data = NULL;
	}

} ReceiveVideoReqData;

class ProcessReceiveVideoBuffer:public ProcessXML
{
public: 
	ProcessReceiveVideoBuffer();
	~ProcessReceiveVideoBuffer();

public:
	int InitReceiveVideoBuffer();
	int UpdateReceiveVideoBuffer();
	int GetReceiveVideoBuffer(char *p_in = NULL);
	ReceiveVideoReqData GetReceiveVideoBufferHeader();

public:
	// block receive data used
	ReceiveVideoReqData m_recv_video_req_header;
	char *m_recv_video_req_buf;
	XML_Char *m_xml_buffer;

};
//////////////////////ReceiveVideoBuffer Class End//////////////////////////////////////


#endif

#endif // PROCESS_XML_H