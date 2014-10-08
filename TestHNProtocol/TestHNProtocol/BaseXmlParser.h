#ifndef BASE_XML_PARSER_H
#define BASE_XML_PARSER_H

#include <stdio.h>
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

class BaseXmlParser 
{
public:
	BaseXmlParser();
	~BaseXmlParser();

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


static XML_Char* save_xml_into_buffer(scew_tree *xml_tree, int buf_size);







#endif