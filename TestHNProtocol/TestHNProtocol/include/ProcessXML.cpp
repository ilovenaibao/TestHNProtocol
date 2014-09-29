#include "ProcessXML.h"
#include "common.h"

ProcessXML::ProcessXML()
{
	m_xml_tree = NULL;
	m_xml_root = NULL;
	m_xml_element = NULL;
	m_xml_printer = NULL;
	m_xml_writer = NULL;
}

ProcessXML::~ProcessXML()
{
	ReleaseXmlValue();
}

/*
* Method Name  : InitXml
*
* Synopsis     : int ProcessXML::InitXml(char *tree)
*
* Arguments    : char  *tree : 
*
* Description  : 初始化一个xml
* 
* Returns      : int 
*/
int ProcessXML::InitXml(char *tree)
{
	/**
	* Create an empty XML tree in memory, and add a root element
	* "scew_test".
	*/
	m_xml_tree = scew_tree_create ();
#ifdef XML_UNICODE_WCHAR_T
	scew_tree_set_xml_encoding(tree, _XT("UTF-16"));
#endif /* XML_UNICODE_WCHAR_T */
	if (NULL == m_xml_tree) {
		return 0;
	}
	if (NULL != tree) {
		m_xml_root = scew_tree_set_root (m_xml_tree, _XT(tree));
	}
	return 1;
}

/*
* Method Name  : AddInfoToXml
*
* Synopsis     : int ProcessXML::AddInfoToXml(XmlElement *pelement)
*
* Arguments    : XmlElement  *pelement : 
*
* Description  : 添加一个节点的key和info到xml中
* 
* Returns      : int 
*/
int ProcessXML::AddFrameInfoToXml(XmlElement *pelement)
{
	int ret = 0;
	scew_attribute *attribute = NULL;

	if (NULL == pelement || NULL == m_xml_tree) {
		return ret;
	}
	if (NULL == m_xml_root) {
		return ret;
	}
	/* Add an element and set element contents. */
	m_xml_element = scew_element_add (m_xml_root, _XT(pelement->element_key));
	if (NULL == m_xml_element) {
		return ret;
	}
	if (NULL != pelement->key_attrib) {
		int key_attrib_count = 0;
		for (key_attrib_count = 0; ;key_attrib_count++) {
			if (NULL == pelement->key_attrib[key_attrib_count] ||
				0 == strcmp(pelement->key_attrib[key_attrib_count], "")) {
					break;
			}
			attribute = scew_attribute_create (_XT(pelement->key_attrib[key_attrib_count]), _XT(""));
			scew_element_add_attribute (m_xml_element, attribute);
		}
	}

	scew_element_set_contents (m_xml_element, _XT(pelement->element_info));

	ret = 1;
	return ret;
}

XML_Char* ProcessXML::SaveXmlIntoBuffer(int buf_size)
{
	/* Save the XML tree to a buffer and print it to standard output. */
	XML_Char *xml_buffer = (XML_Char *) malloc (buf_size);
	m_xml_writer = scew_writer_buffer_create (xml_buffer, buf_size);
	if (NULL == m_xml_writer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create writer buffer\n");
		return xml_buffer;
	}
	m_xml_printer = scew_printer_create (m_xml_writer);
	if (NULL == m_xml_printer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create printer\n");
		return xml_buffer;
	}
	/* We should check for errors here. */
	(void) scew_printer_print_tree(m_xml_printer, m_xml_tree);
	this->ReleaseXmlValue();
	return xml_buffer;
}

static XML_Char* save_xml_into_buffer(scew_tree *xml_tree, scew_printer *xml_printer, scew_writer *xml_writer, int buf_size)
{
	/* Save the XML tree to a buffer and print it to standard output. */
	XML_Char *xml_buffer = (XML_Char *) malloc (buf_size);
	xml_writer = scew_writer_buffer_create (xml_buffer, buf_size);
	if (NULL == xml_writer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create writer buffer\n");
		return xml_buffer;
	}
	xml_printer = scew_printer_create (xml_writer);
	if (NULL == xml_printer)
	{
		if (NULL != xml_buffer) {
			free(xml_buffer);
			xml_buffer = NULL;
		}
		printf ("Unable to create printer\n");
		return xml_buffer;
	}
	/* We should check for errors here. */
	(void) scew_printer_print_tree(xml_printer, xml_tree);
	return xml_buffer;
}

void ProcessXML::ShowXmlBuffer(XML_Char *xml_buffer)
{
	// print xml buffer
	if (NULL == xml_buffer) {
		scew_printf (_XT("%s"), "null");
	} else {
		scew_printf (_XT("%s"), xml_buffer);
	}
}

int ProcessXML::AddXmlElement_(scew_element *element, XmlElement *add_element)
{
	int ret = 0;
	XML_Char const *element_key = NULL;
	XML_Char const *contents = NULL;

	unsigned int indent = 0;

	scew_list *list = NULL;

	if (element == NULL || NULL == add_element ||
		NULL == add_element->element_key || 0 == strcmp(add_element->element_key, "") ||
		NULL == add_element->parent_key || 0 == strcmp(*add_element->parent_key, ""))
	{
		return ret;
	}
	if (NULL == add_element->cur_parent_key) {
		add_element->cur_parent_key = *add_element->parent_key;
		add_element->cur_parent_key_index = 0;
	}

	/* Prints the starting element tag with its attributes. */
	element_key = scew_element_name(element);
	//print_attributes (element);
	contents = scew_element_contents(element);
	// compare key_name is right
	if (0 == strcmp(element_key, add_element->cur_parent_key)) {
		if (-1 == add_element->cur_parent_key_index) {
			ret = 1;
			return ret;
		}
		add_element->cur_parent_key_index++;
		if (NULL != add_element->parent_key[add_element->cur_parent_key_index]) {
			add_element->cur_parent_key = add_element->parent_key[add_element->cur_parent_key_index];
		} else {
			if (0 <= add_element->cur_parent_key_index) {
				add_element->cur_parent_key = add_element->element_key;
				add_element->cur_parent_key_index = -1;
			} else {
				ret = 1;
				return ret;
			}
		}
	}

	/**
	* Call print_element function again for each child of the current
	* element.
	*/
	list = scew_element_children(element);
	while (list != NULL)
	{
		scew_element *child = (scew_element *)scew_list_data(list);
		ret = AddXmlElement_(child, add_element);
		if (1 == ret) {
			// add element into buffer
			/* Add an element and set element contents. */
			if (-2 == add_element->cur_parent_key_index || NULL == child) {
				return ret;
			}
			scew_attribute *attribute = NULL;
			if (NULL != add_element->key_attrib) {
				int key_attrib_count = 0;
				for (key_attrib_count = 0; ;key_attrib_count++) {
					if (NULL == add_element->key_attrib[key_attrib_count] ||
						0 == strcmp(add_element->key_attrib[key_attrib_count], "")) {
							break;
					}
					attribute = scew_attribute_create (_XT(add_element->key_attrib[key_attrib_count]), _XT(""));
					scew_element_add_attribute (child, attribute);
				}
			}
			scew_element_set_contents (child, _XT(add_element->element_info));
			add_element->cur_parent_key_index = -2;
			break;
		}
		indent++;
		list = scew_list_next (list);
	}
	return ret;
}

XML_Char* ProcessXML::AddElementInXmlBuffer(XML_Char *buffer_in, XmlElement *add_element)
{
	scew_reader *reader = NULL;
	scew_parser *parser = NULL;
	scew_tree *tree = NULL;
	scew_writer *writer = NULL;
	scew_printer *printer = NULL;

#if defined(_MSC_VER) && defined(XML_UNICODE_WCHAR_T)
	/* Change stdout to Unicode before writing anything. */
	_setmode(_fileno(stdout), _O_U16TEXT);
#endif /* _MSC_VER && XML_UNICODE_WCHAR_T */

	/* Creates an SCEW parser. This is the first function to call. */
	parser = scew_parser_create();

	scew_parser_ignore_whitespaces (parser, SCEW_TRUE);

	/* Loads an XML file. */
	reader = scew_reader_buffer_create(buffer_in, strlen(buffer_in));
	//reader = scew_reader_file_create("testXML.xml");
	// test show xml frame
	//show_xml_buffer(buffer_in);
	if (reader == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to load file (error #%d: %s)\n"),
			code, scew_error_string (code));
	}
	tree = scew_parser_load (parser, reader);
	if (tree == NULL)
	{
		scew_error code = scew_error_code ();
		scew_printf (_XT("Unable to parse file (error #%d: %s)\n"),
			code, scew_error_string (code));
		if (code == scew_error_expat)
		{
			enum XML_Error expat_code = scew_error_expat_code (parser);
			scew_printf (_XT("Expat error #%d (line %d, column %d): %s\n"),
				expat_code,
				scew_error_expat_line (parser),
				scew_error_expat_column (parser),
				scew_error_expat_string (expat_code));
		}

		/* Frees the SCEW parser and reader. */
		scew_reader_free (reader);
		scew_parser_free (parser);

		return buffer_in;
	}
	AddXmlElement_(scew_tree_root(tree), add_element);
	writer = scew_writer_fp_create (stdout);
	printer = scew_printer_create (writer);
	// save xml to buf
	XML_Char *p_xml_buffer = NULL;
	p_xml_buffer = save_xml_into_buffer(tree, printer, writer, strlen(buffer_in) + 100);
	ShowXmlBuffer(p_xml_buffer);
	if (NULL != buffer_in) {
		free(buffer_in);
		buffer_in = NULL;
	}
	buffer_in = p_xml_buffer;
	p_xml_buffer = NULL;

	/* Remember to free tree (scew_parser_free does not free it). */
	scew_tree_free (tree);

	/* Also free the printer and writer. */
	scew_writer_free (writer);
	scew_printer_free (printer);

	/* Frees the SCEW parser and reader. */
	scew_reader_free (reader);
	scew_parser_free (parser);

	return buffer_in;
}

void ProcessXML::ReleaseXmlValue()
{
	/* Remember to free tree (scew_parser_free does not free it). */
	if (NULL != m_xml_tree) {
		scew_tree_free (m_xml_tree);
		m_xml_tree = NULL;
	}
	/* Also free the printer and writer. */
	if (NULL != m_xml_writer) {
		scew_writer_free (m_xml_writer);
		m_xml_writer = NULL;
	}
	if (NULL != m_xml_printer) {
		scew_printer_free (m_xml_printer);
		m_xml_printer = NULL;
	}
}



int ProcessXML::SendVideoReq(int version)
{

	return 1;
}


///////////////////////////SendVideoBuffer Class////////////////////////////////////

ProcessSendVideoBuffer::ProcessSendVideoBuffer()
{
	m_send_video_req_buf = NULL;
	m_xml_buffer = NULL;
}

ProcessSendVideoBuffer::~ProcessSendVideoBuffer()
{
	if (NULL != m_send_video_req_buf) {
		free(m_send_video_req_buf);
		m_send_video_req_buf = NULL;
	}

	if (NULL != m_xml_buffer) {
		free(m_xml_buffer);
		m_xml_buffer = NULL;
	}
}

/*
* Method Name  : InitSendVideoBuffer
*
* Synopsis     : int ProcessXML::InitSendVideoBuffer()
*
* Description  : 初始化客户端发送VideoBuffer的请求 协议
* 
* Returns      : int (sucess : fail -> 1 : 0)
*/
int ProcessSendVideoBuffer::InitSendVideoBuffer()
{
	if (NULL != m_send_video_req_buf) {
		free(m_send_video_req_buf);
		m_send_video_req_buf = NULL;
	}
	m_send_video_req_header.SendVideoReqData();

	m_send_video_req_buf = (char *)malloc(sizeof(SendVideoReqData));
	if (NULL == m_send_video_req_buf) {
		return 0; // fail to malloc
	}
	memset(m_send_video_req_buf, 0, sizeof(SendVideoReqData));

	return 1;
}

int ProcessSendVideoBuffer::SetSendVideoBufferXml(XML_Char *xml_buffer)
{
	if (NULL == xml_buffer) {
		return 0;
	}
	m_xml_buffer = xml_buffer;
	return 1;
}

/*
* Method Name  : UpdateSendVideoBuffer
*
* Synopsis     : int ProcessXML::UpdateSendVideoBuffer()
*
* Description  : 更新发送VideoBuffer请求协议
* 
* Returns      : int (sucess : fail -> 1 : 0)
*/
int ProcessSendVideoBuffer::UpdateSendVideoBuffer()
{
	// update xml len
	if (NULL == m_send_video_req_buf) {
		return 0;
	}
	if (NULL == m_xml_buffer) {
		m_send_video_req_header.xml_len = 0;
	} else {
		m_send_video_req_header.xml_len = strlen(m_xml_buffer);
	}
	// update block data len
	int tmp_data_len = 0;
	m_send_video_req_header.block_data_len = (SEND_CUSTOMER_KIND + 1) * sizeof(int) + m_send_video_req_header.xml_len;
	if (NULL != m_send_video_req_header.data) {
		tmp_data_len = strlen(m_send_video_req_header.data);
		m_send_video_req_header.block_data_len += tmp_data_len;
	}

	// adjust send buffer
	m_send_video_req_buf = adjust_buffer_size(m_send_video_req_buf, m_send_video_req_header.block_data_len);

	// update send buffer
	if (NULL == m_send_video_req_buf) {
		return 0;
	}
	memcpy(m_send_video_req_buf + SEND_BLOCK_DATA_LEN * sizeof(int), (char *)&m_send_video_req_header.block_data_len, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_VERSION * sizeof(int), (char *)&m_send_video_req_header.version, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_CMD_CODE * sizeof(int), (char *)&m_send_video_req_header.cmd_code, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_XML_LEN * sizeof(int), (char *)&m_send_video_req_header.xml_len, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_SOURCE_ID * sizeof(int), (char *)&m_send_video_req_header.source_id, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_DEST_ID * sizeof(int), (char *)&m_send_video_req_header.dest_id, sizeof(int));
	memcpy(m_send_video_req_buf + SEND_CUSTOMER_KIND * sizeof(int), (char *)&m_send_video_req_header.customer_kind, sizeof(int));
	if (0 != m_send_video_req_header.xml_len) {
		memcpy(m_send_video_req_buf + (SEND_CUSTOMER_KIND + 1) * sizeof(int), m_xml_buffer, m_send_video_req_header.xml_len);
	}
	if (NULL != m_send_video_req_header.data) {
		memcpy(m_send_video_req_buf + (SEND_CUSTOMER_KIND + 1) * sizeof(int) + m_send_video_req_header.xml_len, m_send_video_req_header.data, tmp_data_len);
	}

#ifdef _DEBUG_FLAG_
	save_buffer_to_file(m_send_video_req_buf, "m_send_video_req_buf.txt", "w+b", MEMORY_DYNAMIC);
#endif

	return 1;
}

/*
* Method Name  : GetSendVideoBuffer
*
* Synopsis     : int ProcessXML::GetSendVideoBuffer(char *p_in)
*
* Arguments    : char  *p_in : 
*
* Description  : 服务端使用的解析客户端发送的SendVideoBuffer的协议
* 
* Returns      : int (sucess : fail -> 1 : 0)
*/
int ProcessSendVideoBuffer::GetSendVideoBuffer(char *p_in)
{
	int tmp_len = 0;

#ifdef _DEBUG_FLAG_
	p_in = read_file_to_buffer("m_send_video_req_buf.txt");

#endif
	m_send_video_req_header.SendVideoReqData();
	if (NULL == p_in) {
		return 0;
	}
	m_send_video_req_header.block_data_len = _msize(p_in);
	if (m_send_video_req_header.block_data_len < (SEND_CUSTOMER_KIND + 1) * sizeof(int)) {
		m_send_video_req_header.block_data_len = 0;
		return 0;
	}

	if (NULL != m_send_video_req_buf) {
		free(m_send_video_req_buf);
		m_send_video_req_buf = NULL;
	}
	m_send_video_req_buf = (char *)malloc(m_send_video_req_header.block_data_len);
	if (NULL == m_send_video_req_buf) {
		m_send_video_req_header.block_data_len = 0;
		return 0;
	}
	// copy receive buf
	memcpy(m_send_video_req_buf, p_in, m_send_video_req_header.block_data_len);
	// parser receive buf
	m_send_video_req_header.version = (int)*(m_send_video_req_buf + SEND_VERSION * sizeof(int));
	m_send_video_req_header.version = m_send_video_req_header.version < 0 ? 
		256 + m_send_video_req_header.version : m_send_video_req_header.version;
	m_send_video_req_header.cmd_code = (int)*(m_send_video_req_buf + SEND_CMD_CODE * sizeof(int));
	m_send_video_req_header.cmd_code = m_send_video_req_header.cmd_code < 0 ?
		256 + m_send_video_req_header.cmd_code : m_send_video_req_header.cmd_code;
	m_send_video_req_header.xml_len = (int)*(m_send_video_req_buf + SEND_XML_LEN * sizeof(int));
	m_send_video_req_header.xml_len = m_send_video_req_header.xml_len < 0 ?
		256 + m_send_video_req_header.xml_len : m_send_video_req_header.xml_len;
	m_send_video_req_header.source_id = (int)*(m_send_video_req_buf + SEND_SOURCE_ID * sizeof(int));
	m_send_video_req_header.source_id = m_send_video_req_header.source_id < 0 ?
		256 + m_send_video_req_header.source_id : m_send_video_req_header.source_id;
	m_send_video_req_header.dest_id = (int)*(m_send_video_req_buf + SEND_DEST_ID * sizeof(int));
	m_send_video_req_header.dest_id = m_send_video_req_header.dest_id < 0 ?
		256 + m_send_video_req_header.dest_id : m_send_video_req_header.dest_id;
	m_send_video_req_header.customer_kind = (int)*(m_send_video_req_buf + SEND_CUSTOMER_KIND * sizeof(int));
	m_send_video_req_header.customer_kind = m_send_video_req_header.customer_kind < 0 ?
		256 + m_send_video_req_header.customer_kind : m_send_video_req_header.customer_kind;
	if (0 < m_send_video_req_header.xml_len) {
		m_send_video_req_header.xml_data = (char *)malloc(sizeof(char) * (m_send_video_req_header.xml_len + 1));
		memcpy(m_send_video_req_header.xml_data, 
			m_send_video_req_buf + (SEND_CUSTOMER_KIND + 1) * sizeof(int),
			m_send_video_req_header.xml_len);
		m_send_video_req_header.xml_data[m_send_video_req_header.xml_len] = '\0';
	}
	int tmp_data_len = m_send_video_req_header.block_data_len - m_send_video_req_header.xml_len - (SEND_CUSTOMER_KIND + 1) * sizeof(int);
	if (0 < tmp_data_len) {
		m_send_video_req_header.data = (char *)malloc(sizeof(char) * tmp_data_len);
		memcpy(m_send_video_req_header.data, 
			m_send_video_req_buf + m_send_video_req_header.block_data_len - tmp_data_len, tmp_data_len);
	}

	return 1;
}

/*
* Method Name  : GetSendVideoBufferHeader
*
* Synopsis     : SendVideoReqData ProcessXML::GetSendVideoBufferHeader()
*
* Description  : 服务端用来获得SendVideoBuffer各项信息的解析结果
* 
* Returns      : SendVideoReqData 
*/
SendVideoReqData ProcessSendVideoBuffer::GetSendVideoBufferHeader()
{
	return m_send_video_req_header;
}

//////////////////////SendVideoBuffer Class End/////////////////////////////////////////

//////////////////////ReceiveVideoBuffer Class//////////////////////////////////////////

ProcessReceiveVideoBuffer::ProcessReceiveVideoBuffer()
{
	m_recv_video_req_buf = NULL;
	m_xml_buffer = NULL;
}

ProcessReceiveVideoBuffer::~ProcessReceiveVideoBuffer()
{
	if (NULL != m_recv_video_req_buf) {
		free(m_recv_video_req_buf);
		m_recv_video_req_buf = NULL;
	}
	if (NULL != m_xml_buffer) {
		free(m_xml_buffer);
		m_xml_buffer = NULL;
	}
}


/*
* Method Name  : InitReceiveVideoBuffer
*
* Synopsis     : int ProcessXML::InitReceiveVideoBuffer()
*
* Description  : 初始化服务端反馈给客户端SendVideoBuffer的协议
* 
* Returns      : int 
*/
int ProcessReceiveVideoBuffer::InitReceiveVideoBuffer()
{
	if (NULL != m_recv_video_req_buf) {
		free(m_recv_video_req_buf);
		m_recv_video_req_buf = NULL;
	}
	m_recv_video_req_header.ReceiveVideoReqData();
	m_recv_video_req_buf = (char *)malloc(sizeof(ReceiveVideoReqData));
	if (NULL == m_recv_video_req_buf) {
		return 0; // fail to malloc
	}
	memset(m_recv_video_req_buf, 0, sizeof(ReceiveVideoReqData));

	return 1;
}

/*
* Method Name  : UpdateReceiveVideoBuffer
*
* Synopsis     : int ProcessXML::UpdateReceiveVideoBuffer()
*
* Description  : 服务端使用，更新SendVideoBuffer的反馈协议
* 
* Returns      : int 
*/
int ProcessReceiveVideoBuffer::UpdateReceiveVideoBuffer()
{
	// update xml len
	if (NULL == m_recv_video_req_buf) {
		return 0;
	}
	if (NULL == m_xml_buffer) {
		m_recv_video_req_header.xml_len = 0;
	} else {
		m_recv_video_req_header.xml_len = strlen(m_xml_buffer);
	}
	// update block data len
	int tmp_data_len = 0;
	m_recv_video_req_header.block_data_len = (RECV_CUR_TIME + 1) * sizeof(int) + m_recv_video_req_header.xml_len;
	if (NULL != m_recv_video_req_header.data) {
		tmp_data_len = strlen(m_recv_video_req_header.data);
		m_recv_video_req_header.block_data_len += tmp_data_len;
	}

	// adjust send buffer
	m_recv_video_req_buf = adjust_buffer_size(m_recv_video_req_buf, m_recv_video_req_header.block_data_len);

	// update send buffer
	if (NULL == m_recv_video_req_buf) {
		return 0;
	}
	memcpy(m_recv_video_req_buf + RECV_BLOCK_DATA_LEN * sizeof(int), (char *)&m_recv_video_req_header.block_data_len, sizeof(int));
	memcpy(m_recv_video_req_buf + RECV_VERSION * sizeof(int), (char *)&m_recv_video_req_header.version, sizeof(int));
	memcpy(m_recv_video_req_buf + RECV_CMD_CODE * sizeof(int), (char *)&m_recv_video_req_header.cmd_code, sizeof(int));
	memcpy(m_recv_video_req_buf + RECV_XML_LEN * sizeof(int), (char *)&m_recv_video_req_header.xml_len, sizeof(int));
	memcpy(m_recv_video_req_buf + RECV_SUCCESS_FLAG * sizeof(int), (char *)&m_recv_video_req_header.success_flag, sizeof(int));
	memcpy(m_recv_video_req_buf + RECV_CUR_TIME * sizeof(int), (char *)&m_recv_video_req_header.cur_time, sizeof(int));
	if (0 < m_recv_video_req_header.xml_len) {
		memcpy(m_recv_video_req_buf + (RECV_CUR_TIME + 1) * sizeof(int), m_xml_buffer, m_recv_video_req_header.xml_len);
	}
	if (NULL != m_recv_video_req_header.data) {
		memcpy(m_recv_video_req_buf + (RECV_CUR_TIME + 1) * sizeof(int) + m_recv_video_req_header.xml_len, m_recv_video_req_header.data, tmp_data_len);
	}

#ifdef _DEBUG_FLAG_
	save_buffer_to_file(m_recv_video_req_buf, "m_recv_video_req_buf.txt", "w+b", MEMORY_DYNAMIC);
#endif

	return 1;
}

/*
* Method Name  : GetReceiveVideoBuffer
*
* Synopsis     : int ProcessXML::GetReceiveVideoBuffer(char *p_in)
*
* Arguments    : char  *p_in : 
*
* Description  : 客户端使用，获取详细SendVideoBuffer的反馈信息
* 
* Returns      : int 
*/
int ProcessReceiveVideoBuffer::GetReceiveVideoBuffer(char *p_in)
{
	int tmp_len = 0;

#ifdef _DEBUG_FLAG_
	p_in = read_file_to_buffer("m_recv_video_req_buf.txt");

#endif
	m_recv_video_req_header.ReceiveVideoReqData();
	if (NULL == p_in) {
		return 0;
	}
	m_recv_video_req_header.block_data_len = _msize(p_in);
	if (m_recv_video_req_header.block_data_len < (RECV_CUR_TIME + 1) * sizeof(int)) {
		m_recv_video_req_header.block_data_len = 0;
		return 0;
	}

	if (NULL != m_recv_video_req_buf) {
		free(m_recv_video_req_buf);
		m_recv_video_req_buf = NULL;
	}
	m_recv_video_req_buf = (char *)malloc(m_recv_video_req_header.block_data_len);
	if (NULL == m_recv_video_req_buf) {
		m_recv_video_req_header.block_data_len = 0;
		return 0;
	}
	// copy receive buf
	memcpy(m_recv_video_req_buf, p_in, m_recv_video_req_header.block_data_len);
	// parser receive buf
	m_recv_video_req_header.version = (int)*(m_recv_video_req_buf + RECV_VERSION * sizeof(int));
	m_recv_video_req_header.version = m_recv_video_req_header.version < 0 ? 
		256 + m_recv_video_req_header.version : m_recv_video_req_header.version;
	m_recv_video_req_header.cmd_code = (int)*(m_recv_video_req_buf + RECV_CMD_CODE * sizeof(int));
	m_recv_video_req_header.cmd_code = m_recv_video_req_header.cmd_code < 0 ?
		256 + m_recv_video_req_header.cmd_code : m_recv_video_req_header.cmd_code;
	m_recv_video_req_header.xml_len = (int)*(m_recv_video_req_buf + RECV_XML_LEN * sizeof(int));
	m_recv_video_req_header.xml_len = m_recv_video_req_header.xml_len < 0 ?
		256 + m_recv_video_req_header.xml_len : m_recv_video_req_header.xml_len;
	m_recv_video_req_header.success_flag = (int)*(m_recv_video_req_buf + RECV_SUCCESS_FLAG * sizeof(int));
	m_recv_video_req_header.success_flag = m_recv_video_req_header.success_flag < 0 ?
		256 + m_recv_video_req_header.success_flag : m_recv_video_req_header.success_flag;
	m_recv_video_req_header.cur_time = (int)*(m_recv_video_req_buf + RECV_CUR_TIME * sizeof(int));
	m_recv_video_req_header.cur_time = m_recv_video_req_header.cur_time < 0 ?
		256 + m_recv_video_req_header.cur_time : m_recv_video_req_header.cur_time;
	if (0 < m_recv_video_req_header.xml_len) {
		m_recv_video_req_header.xml_data = (char *)malloc(sizeof(char) * (m_recv_video_req_header.xml_len + 1));
		memcpy(m_recv_video_req_header.xml_data, 
			m_recv_video_req_buf + (RECV_CUR_TIME + 1) * sizeof(int),
			m_recv_video_req_header.xml_len);
		m_recv_video_req_header.xml_data[m_recv_video_req_header.xml_len] = '\0';
	}
	int tmp_data_len = m_recv_video_req_header.block_data_len - m_recv_video_req_header.xml_len - (RECV_CUR_TIME + 1) * sizeof(int);
	if (0 < tmp_data_len) {
		m_recv_video_req_header.data = (char *)malloc(sizeof(char) * tmp_data_len);
		memcpy(m_recv_video_req_header.data, 
			m_recv_video_req_buf + m_recv_video_req_header.block_data_len - tmp_data_len, tmp_data_len);
	}

	return 1;
}

/*
* Method Name  : GetReceiveVideoBufferHeader
*
* Synopsis     : ReceiveVideoReqData ProcessXML::GetReceiveVideoBufferHeader()
*
* Description  : 客户端使用，获取SendVideoBuffer的反馈各项信息
* 
* Returns      : ReceiveVideoReqData 
*/
ReceiveVideoReqData ProcessReceiveVideoBuffer::GetReceiveVideoBufferHeader()
{
	return m_recv_video_req_header;
}

//////////////////////ReceiveVideoBuffer Class End//////////////////////////////////////