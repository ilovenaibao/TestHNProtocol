/*
ServerNet.cpp
*/
#include "Server.h"

int CServerNet::Init( const char* address,int port )
{
	int rlt = 0;

	//���ڼ�¼������Ϣ�������
	int iErrorMsg;

	//��ʼ��WinSock
	WSAData wsaData;
	iErrorMsg = WSAStartup(MAKEWORD(1,1),&wsaData);

	if (iErrorMsg != NO_ERROR)
	{
		//��ʼ��WinSockʧ��
		printf("wsastartup failed with error : %d\n",iErrorMsg);

		rlt = 1;
		return rlt;
	}

	//���������Socket
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)

	{
		//����Socket�쳣
		printf("socket failed with error : %d\n",WSAGetLastError());

		rlt = 2;
		return rlt;
	}

	//������Ϣ
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = inet_addr(address);

	//��
	iErrorMsg = bind(m_sock,(sockaddr*)&serverAddr,sizeof(serverAddr));
	if (iErrorMsg < 0)
	{
		//��ʧ��
		printf("bind failed with error : %d\n",iErrorMsg);
		rlt = 3;
		return rlt;
	}



	return rlt;
}

void CServerNet::Run()
{
	//��������
	listen(m_sock,5);

	sockaddr_in tcpAddr;
	int len = sizeof(sockaddr);
	SOCKET newSocket;
	char buf[1024];
	int rval;

	do 
	{
		//������Ϣ
		printf("server: wait client connect...\n");
		newSocket = accept(m_sock, (sockaddr*)&tcpAddr, &len);
		printf("server: client connected\n");

		if (newSocket == INVALID_SOCKET)
		{
			//�ǿ���socket
		}
		else
		{
			//��Ϣ����
			do
			{
				//��������
				memset(buf,0,sizeof(buf));
				rval = recv(newSocket, buf, 1024, 0);

				if (rval == SOCKET_ERROR) {
					//��Ӧ���Ǹ��쳣�����ͻ���û�е���closeSocket��ֱ���˳���Ϸ��ʱ�򣬽����������
					printf("server: client exit, socket error\n");
					break;
				}
				if (rval == 0) {
					//recv����0��ʾ�����˳�
					printf("server: ending connection");
					break;
				} else {
					//��ʾ���յ�������
					printf("server: recv\n%s\n",buf);

					int iErrMsg = 0;
					if (NULL != m_send_buf) {
						iErrMsg = send(newSocket, m_send_buf, max(strlen(m_send_buf), m_send_buf_len), 0);
						if (iErrMsg < 0)
							//����ʧ��
						{
							printf("server: send msg failed with error : %d\n",iErrMsg);
						}
					}
				}

			}while(rval != 0);

			//�رն�ӦAccept��socket
			closesocket(newSocket);
		}

	} while (1);

	//�ر������Socket
	closesocket(m_sock);
}

int CServerNet::RecvN(SOCKET s, char* buf, int len)
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
int server_main(char *p_send_buf, int send_buf_size)
{
	CServerNet serverNet;
	int iRlt = serverNet.Init("127.0.0.1", 8888);
	if (iRlt == 0)
	{
		printf("init ok...\n");
		serverNet.m_send_buf = p_send_buf;
		serverNet.m_send_buf_len = send_buf_size;
		serverNet.Run();
	} else {
		printf("serverNet init failed with error : %d\n",iRlt);
	}
	system("pause");

	return 1;
}