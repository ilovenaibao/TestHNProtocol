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
		xml_data = NULL;
		data = NULL;
	}

} SendVideoReqData;

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

// xml data
typedef struct {
	int element_deep; // 数组结束标志，如果-1则表示结束
	char *element_key; // key标志
	char *key_attrib[100]; // key的属性
	char *element_info; // content内容
	char *parent_key[100]; // 父类节点集合
	char *cur_parent_key; // 当前父节点位置
	int cur_parent_key_index; // 当前父节点计数

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