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
using namespace std;

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
		xml_data = NULL;
		data = NULL;
	}

} SendVideoReqData;

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

// xml data
typedef struct {
	int element_deep; // ���������־�����-1���ʾ����
	char *element_key; // key��־
	char *key_attrib[100]; // key������
	char *element_info; // content����
	char *parent_key[100]; // ����ڵ㼯��
	char *cur_parent_key; // ��ǰ���ڵ�λ��
	int cur_parent_key_index; // ��ǰ���ڵ����

	void XmlElement()
	{
		element_deep = 0;
		element_key = NULL;
		*key_attrib = NULL;
		element_info = NULL;
		*parent_key = NULL;
		cur_parent_key = *parent_key;
	}
} XmlElement;


class ProcessXML:public HNProtocol
{
public:
	ProcessXML();
	~ProcessXML();

public:
	void GetHeartBeat(int version) {}
	void SendHeartBeat(int version) {}
	int  SendVideoReq(int version);

	// init xml tree...
	int InitXml(char *tree = "null");
	int AddFrameInfoToXml(XmlElement *pelement);
	XML_Char* SaveXmlIntoBuffer(int buf_size);
	void ShowXmlBuffer(XML_Char *xml_buffer);
	int AddXmlElement_(scew_element *element, XmlElement *add_element);
	XML_Char* AddElementInXmlBuffer(XML_Char *buffer_in, XmlElement *add_element);
	void ReleaseXmlValue();

private:

public:
	// xml used
	scew_tree		*m_xml_tree;
	scew_element	*m_xml_root;
	scew_element	*m_xml_element;
	scew_printer	*m_xml_printer;
	scew_writer		*m_xml_writer; 
	//XML_Char		*m_xml_buffer;
};

static XML_Char* save_xml_into_buffer(scew_tree *xml_tree, scew_printer *xml_printer, scew_writer *xml_writer, int buf_size);

///////////////////////////SendVideoBuffer Class////////////////////////////////////

class ProcessSendVideoBuffer:public ProcessXML
{
public:
	ProcessSendVideoBuffer();
	~ProcessSendVideoBuffer();

	// SendVideoBuffer
	int InitSendVideoBuffer();
	int SetSendVideoBufferXml(XML_Char *xml_buffer);
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