#include <winsock.h>
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include"UDPServer.h"

#define SERV_PORT 8088
#define MAXLINE 2048
#define SA struct sockaddr  
#define SOCKET_TIMEOUT 5000

void netInit(void)
{
#if defined(_WIN32)
    WORD wVersionRequested;
    WSADATA wsadata;
    int err = 0;
    wVersionRequested = MAKEWORD(1,1);
    if ((err = WSAStartup(wVersionRequested,&wsadata)) != 0)
	fprintf(stderr,"Network failed to start: %d\n",err);
#endif
}

void dg_echo(int sockfd,SA *pclient,int clilen)
{
    int n = 0;
	int len = 0;
	int err = 0;
	time_t timenow;
	struct tm *timeinfo;
	time(&timenow);
	timeinfo = localtime(&timenow);
	char stime[20];
	memset(stime,0,sizeof(stime));
	strcpy(stime,asctime(timeinfo));

    printf("time:%s,udpserver start!",stime);
	char mesg[MAXLINE];
	memset(mesg,0,sizeof(mesg));
  
	for(;;)
	{
	    len = clilen;

		time(&timenow);
		timeinfo = localtime(&timenow);
		memset(stime,0,sizeof(stime));
		strcpy(stime,asctime(timeinfo));

        memset(mesg,0,sizeof(mesg));
		n = recvfrom(sockfd,mesg,MAXLINE,0,pclient,&len);
		printf("time:%s,recv:%s\n",stime,mesg);
		sendto(sockfd,mesg,n,0,pclient,len);
		//Sleep(5000);

	}
}

int main()
{
	int err = 0;
	int sockfd = 0;
	struct sockaddr_in server,client;
	//³õÊ¼»¯ÍøÂç
    netInit();

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd <= 0)
	{
		err = WSAGetLastError();
		fprintf(stderr,"Network failed to socket: %d\n",err);
		return -1;
	}
			
	int timeout = SOCKET_TIMEOUT; //
	setsockopt(sockfd,SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int));

	memset(&server,0,sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERV_PORT);
	if(SOCKET_ERROR == bind(sockfd,(SA*)&server,sizeof(sockaddr_in)))
	{
		err = WSAGetLastError();
		fprintf(stderr,"Network failed to bind: %d\n",err);
		closesocket(sockfd);
        return -1;
	}
	
	dg_echo(sockfd,(SA*)&client,sizeof(sockaddr_in));
	closesocket(sockfd);
    return 0;
}
