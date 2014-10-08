/*
CLientNet.cpp
*/
#include "Client.h"
#include <conio.h>


int CClientNet::Connect( int port,const char* address )
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�����
	int iErrMsg;
	//����WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1,1),&wsaData);
	if (iErrMsg != NO_ERROR)
		//�д���
	{
		printf("client: failed with wsaStartup error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}

	//����Socket
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)
		//����Socketʧ��
	{
		printf("client: socket failed with error : %d\n",WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	//Ŀ�����������
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = port;
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//����,sock��Ŀ�����������
	iErrMsg = connect(m_sock,(sockaddr*)&sockaddrServer,sizeof(sockaddrServer));
	if (iErrMsg < 0)
	{
		printf("client: connect failed with error : %d\n",iErrMsg);

		rlt = 3;
		return rlt;
	}

	return rlt;
}

int CClientNet::Run()
{
	int rlt = 0;
	int iErrMsg = 0;

	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;
	char buf[1024];
	int rval;

	do 
	{
		if (NULL != m_send_buf) {
			printf("client: send data\n");
			iErrMsg = send(m_sock, m_send_buf, max(strlen(m_send_buf), m_send_buf_len), 0);
			if (iErrMsg < 0)
				//����ʧ��
			{
				printf("client: send msg failed with error : %d\n",iErrMsg);

				rlt = 1;
				return rlt;
			}
		}
		
		//��Ϣ����
		do
		{
			//��������
			memset(buf,0,sizeof(buf));
			rval = recv(m_sock, buf, 1024, 0);

			if (rval == SOCKET_ERROR) {
				//��Ӧ���Ǹ��쳣�����ͻ���û�е���closeSocket��ֱ���˳���Ϸ��ʱ�򣬽����������
				printf("client: recv socket error\n");
				break;
			}
			if (rval == 0) {
				//recv����0��ʾ�����˳�
				printf("client: ending connection");
				break;
			} else {
				//��ʾ���յ�������
				printf("client: recv \n%s\n",buf);
				getch();
				if (NULL != m_send_buf) {
					iErrMsg = send(m_sock, m_send_buf, max(strlen(m_send_buf), m_send_buf_len), 0);
					if (iErrMsg < 0)
						//����ʧ��
					{
						printf("client: send msg failed with error : %d\n",iErrMsg);
					}
				}
			}

		}while(rval != 0);

		//�رն�ӦAccept��socket
		closesocket(newSocket);

	} while (1);


	return rlt;
}


int CClientNet::SendMsg(const char* msg,int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//������Ϣ��ָ��sock������Ϣ
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//����ʧ��
	{
		printf("send msg failed with error : %d\n",iErrMsg);

		rlt = 1;
		return rlt;
	}

	return rlt;
}

void CClientNet::Close()
{
	closesocket(m_sock);
}

int CClientNet::RecvN(SOCKET s, char* buf, int len)
{
	int cut = len;
	int cutlen = 0;

	while(cut > 0)
	{
		cutlen = recv(s, (char *)buf, cut, 0 );
		if(cutlen<0)
		{
			return -1;
		}
		if(cutlen == 0)
		{
			return len-cut;
		}

		buf += cutlen;
		cut -= cutlen;
	}
	return len;
}

// test main
void client_main(char *p_send, int send_buf_size)
{
	CClientNet client;

	//���ӵ�127.0.0.1�������أ�,�˿ں�Ϊ8888�ķ����
	printf("Connect\n");
	int connect_flag = client.Connect(8888,"127.0.0.1");
	if (0 != connect_flag) {
		return;
	}
	client.m_send_buf = p_send;
	client.m_send_buf_len = send_buf_size;
	client.Run();

	////�ر�socket
	//printf("close\n");
	//client.Close();
}