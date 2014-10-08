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
	int RecvN(SOCKET s, char* buf, int len);

	//������Ϣ
	int SendMsg(const char* msg,int len);
	//�ر�
	void Close();

private:
	SOCKET m_sock;

public:
	char *m_send_buf;
	int m_send_buf_len;

};

void client_main(char *p_send, int send_buf_size);


#endif