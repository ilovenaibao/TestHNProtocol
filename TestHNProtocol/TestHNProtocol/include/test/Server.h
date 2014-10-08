#ifndef SERVER_H
#define SERVER_H

#pragma once

/*
ServerNet.h
���ڱ�ʾ����˵��������

*/

#include <stdio.h>
#include <winsock.h>

#pragma comment (lib,"ws2_32.lib")

class CServerNet
{
public:

	//��ʼ��������,����0��ʾ�ɹ�
	int Init(const char* address,int port);
	//��������
	void Run();
	int RecvN(SOCKET s, char* buf, int len);

private:
	SOCKET m_sock;

public:
	char *m_send_buf;
	int m_send_buf_len;

};

int server_main(char *p_send_buf, int send_buf_size);


#endif