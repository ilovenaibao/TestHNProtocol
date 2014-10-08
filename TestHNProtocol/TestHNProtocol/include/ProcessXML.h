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

// ����data
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
	int block_data_len; // ���ݳ���
	int version; // �汾��
	int cmd_code; // ָ����
	int xml_len; // xml����
	int source_id; // Դҵ��ID
	int dest_id; // Ŀ��ҵ��ID
	int customer_kind; // �ͻ�����
	int data_len; // �������ݵĳ���
	char *xml_data; // xml����
	char *data; // ���͵�����

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

// ����data
enum {
	RECV_BLOCK_DATA_LEN = 0,
	RECV_VERSION,
	RECV_CMD_CODE,
	RECV_XML_LEN,
	RECV_SUCCESS_FLAG,
	RECV_CUR_TIME
};

typedef struct {
	int block_data_len; // ���ݳ���
	int version; // �汾��
	int cmd_code; // ָ����
	int xml_len; // xml����
	int success_flag; // �ɹ���־
	int cur_time; // ��ǰʱ��
	char *xml_data; // xml����
	char *data; // ��������

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