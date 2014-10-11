/**************************************************************************
/*
* Copyright (c) 2014
* All rights reserved.
*
* File Name： ALANThrea.h
* Summary：   音视频基础平台协议基础类
* 
* Current version： 1.0
* Author：Alan.zhang(张建中)
* Start： 2014/09/30/
* End：   2014/09/30/
*
* Revise version：  1.0
* Author：Alan.zhang(张建中)
* Start： 2014/09/30/
* End：   2014/09/30/

* Introduction:
* Base Lock Class
* Base Thread Class
*
**************************************************************************/


#ifndef _HNPROTOCOL_H
#define _HNPROTOCOL_H

#include <scew.h>

enum { MAX_OUTPUT_BUFFER_ = 2000 };

enum {
	XML_EOF = -1,
	XML_ROOT = 0,
	XML_ELEMENT,
	XML_SUB_ELEMENT
};

typedef struct {
	int element_deep; // 数组结束标志，如果-1则表示结束
	char *element_key; // key标志
	char *key_attrib[100]; // key的属性
	char *key_attrib_content[100]; // key属性内容
	char *element_info; // content内容
	char *parent_key[100]; // 父类节点集合
	char *cur_parent_key; // 当前父节点位置
	int cur_parent_key_index; // 当前父节点计数

	void XmlElement()
	{
		element_deep = 0;
		element_key = NULL;
		*key_attrib = NULL;
		*key_attrib_content = NULL;
		element_info = NULL;
		*parent_key = NULL;
		cur_parent_key = *parent_key;
	}

} XmlElement;


enum {
	CMD_CODE_SEND_VIDEO_REQUEST	= 2004,
	CMD_CODE_RECV_VIDEO_REQUEST,
};


// 发送data
typedef struct _SendMegHead {
	_SendMegHead(void)
	{
		data_len = 0;
		version = 100;
		cmd_code = 0;
		xml_len = 0;
		source_id = 0;
		dest_id = 0;
	}

	int data_len; // 数据长度
	int version; // 版本号
	int cmd_code; // 指令编号
	int xml_len; // xml长度
	int source_id; // 源业务ID
	int dest_id; // 目的业务ID

} SendMegHead,*PSendMegHead;

// 接收data
typedef struct _RecvMegHead {
	_RecvMegHead(void) {
		data_len = 0;
		version = 0;
		cmd_code = 0;
		xml_len = 0;
		success_flag = 0;
	}

	int data_len; // 数据长度
	int version; // 版本号
	int cmd_code; // 指令编号
	int xml_len; // xml长度
	int success_flag; // 成功标志
} RecvMegHead, *PRecvMegHead;


class HNProtocol
{
public:
	HNProtocol();
	~HNProtocol();

public:	
	virtual void SendHeartBeat(int version) = 0;
	virtual void GetHeartBeat(int version)  = 0;
	virtual int  SendVideoReq(int version)  = 0;


public:
	XML_Char* CreateXmlBuffer(XmlElement *all_elements);
	XML_Char* AddElementInXmlBuffer(XML_Char *buffer_in, XmlElement *add_element, int add_kind = SET_ELEMENT);
	void GetElementInXmlBuffer(XML_Char *buffer_in, XmlElement *in_element);
	void PrintXmlBuffer(XML_Char *xml_buffer);

private:
	int AddFrameInfoToXml(scew_tree *xml_tree, XmlElement *pelement);
	int AddXmlElement_(scew_element *element, XmlElement *add_element, int add_kind);
	int AddXmlElement_2(scew_element *element, XmlElement *add_element, int add_kind);
	int GetXmlElement_(scew_element *element, XmlElement *in_element);
	void GetXmlElement_2(scew_element *element, XmlElement *out_element);
	XML_Char* SaveXmlIntoBuffer(scew_tree *m_xml_tree, int buf_size = MAX_OUTPUT_BUFFER_);	

public:
	enum {
		SET_ELEMENT		= 0x00000001,	// 设置element相关content
		ADD_ELEMENT		= 0x00000010,	// 添加element
		ALL_OF_KIND		= 0x00000011	// 添加element并设置相关content
	};

};
#endif