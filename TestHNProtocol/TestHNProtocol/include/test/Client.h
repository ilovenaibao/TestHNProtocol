#ifndef CLIENT_H
#define CLIENT_H

/*
ClinetNet.h��
���ڱ�ʾһ���ͻ����������
*/

#include <stdio.h>
#include<windows.h>

#pragma comment(lib, "Ws2_32.lib")

class CClientNet
{
public:
	//������ָ��������
	int Connect(int port,const char* address);
	int Run();
	int SendN(SOCKET s, const char* buf, int len);
	int RecvN(SOCKET s, char* buf, int len);

	//������Ϣ
	int SendMsg(const char* msg,int len);
	//�ر�
	void Close();

private:
	SOCKET m_sock;

public:
	char *m_send_head_buf;
	int m_send_buf_len;
	char *m_send_xml;
	char *m_send_data;

};

void client_main(char *p_send_head, int send_buf_size, char *p_xml, char *p_data);


#endif