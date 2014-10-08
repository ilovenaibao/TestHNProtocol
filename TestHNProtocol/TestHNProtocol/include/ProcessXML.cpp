#include "ProcessXML.h"
#include "common.h"
#include "../leak_detector/leak_detector_c.h"

ProcessXML::ProcessXML()
{

}

ProcessXML::~ProcessXML()
{

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

int ProcessSendVideoBuffer::SetSendVideoBufferData(char *data, int data_size)
{
	if (0 >= data_size || NULL == data) {
		return 0;
	}

	m_send_video_req_header.data = data;
	m_send_video_req_header.data_len = data_size;
	return 1;
}

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
	m_send_video_req_header.block_data_len = (SEND_CUSTOMER_KIND + 1) * sizeof(int) + m_send_video_req_header.xml_len;
	if (0 < m_send_video_req_header.data_len && NULL != m_send_video_req_header.data) {
		m_send_video_req_header.block_data_len += m_send_video_req_header.data_len;
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
		memcpy(m_send_video_req_buf + (SEND_CUSTOMER_KIND + 1) * sizeof(int) + m_send_video_req_header.xml_len, 
			m_send_video_req_header.data, m_send_video_req_header.data_len);
	}

#ifdef _DEBUG_FLAG_
	save_buffer_to_file(m_send_video_req_buf, "m_send_video_req_buf.txt", "a+b", MEMORY_DYNAMIC);
#endif

	return 1;
}

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
	save_buffer_to_file(m_recv_video_req_buf, "m_recv_video_req_buf.txt", "a+b", MEMORY_DYNAMIC);
#endif

	return 1;
}

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

ReceiveVideoReqData ProcessReceiveVideoBuffer::GetReceiveVideoBufferHeader()
{
	return m_recv_video_req_header;
}

//////////////////////ReceiveVideoBuffer Class End//////////////////////////////////////