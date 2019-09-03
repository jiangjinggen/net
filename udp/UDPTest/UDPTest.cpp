#include <winsock.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "CRC.h"
#include "CRCCheck.h"
#include "UDPTest.h"
#include "iostream.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 8888
#define MAXLINE 2048
#define SA struct sockaddr 

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

void dg_cli(int sockfd,SA *server,int servlen)
{
    int n = 0;
	int err = 0;
	char sendline[MAXLINE]; 
	char recvline[MAXLINE];
	memset(sendline,0,sizeof(sendline));
	memset(recvline,0,sizeof(recvline));
	time_t timenow;
	struct tm *timeinfo;
	time(&timenow);
	timeinfo = localtime(&timenow);
	char stime[20];
	memset(stime,0,sizeof(stime));
	strcpy(stime,asctime(timeinfo));
    printf("time:%s,udpclient start!",stime);

	for(;;)
	{	
		memset(sendline,0,sizeof(sendline));
		strcpy(sendline,"Hello Server!");
		sendto(sockfd,sendline,MAXLINE,0,server,servlen);
        Sleep(5000);

		time(&timenow);
		timeinfo = localtime(&timenow);
		memset(stime,0,sizeof(stime));
		strcpy(stime,asctime(timeinfo));

        memset(recvline,0,sizeof(recvline));
		n = recvfrom(sockfd,recvline,MAXLINE,0,server,&servlen);
		printf("time:%s,recv:%s\n",stime,recvline);
	}
}
unsigned char databuf[] = {0xa9,0x0d,0x08,0xb4,0xe9,0x01,0x08,0x3c,0x20,0x01,0x82,0xe9,0x01,0x08,0x3c,0xa9,0x0d,0x08,0xb4,0x00,0x2c,0x00,0x01,
0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x15,0x01,0x00,0x00,0x00,0xe6,0xdb};
unsigned char databuf1[] = {0xa9,0x0d,0x08,0xb4,0xe9,0x01,0x08,0x3c,0x20,0x01,0x82,0xe9,0x01,0x08,0x3c,0xa9,0x0d,0x08,0xb4,0x00,0x2c,0x00,0x01,
0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x07,0x01,0x00,0x07,0x00,0x5c,0xe8};
unsigned char databuf2[] = {0xa9,0x0d,0x08,0xb4,0xe9,0x01,0x08,0x3c,0x20,0x01,0x82,0xe9,0x01,0x08,0x3c,0xa9,0x0d,0x08,0xb4,0x00,0x2c,0x00,0x01,
0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x04,0x01,0x00,0x0c,0x00,0x1f,0xd8};
int main()
{
 	//测试CRC
	/*int size0 = sizeof(ITCSAddr);
	int size1 = sizeof(ITCS_L2);
	int size2 = sizeof(ITCS_NVL5);
	unsigned char tmpbuf[42];
	memset(tmpbuf,0,sizeof(tmpbuf));
	FILE *fp = NULL;
	fp = fopen("D:\\jjg\\test\\OBC_4.log","rb");
	if(NULL == fp)
	{
		printf("%d,%s\n",errno,strerror(errno));
	}
	else
	{
		fread(tmpbuf,41,1,fp);
		fclose(fp);
	}
    unsigned short cc = tmpbuf[39] + tmpbuf[40]*256;
	unsigned short us = 0;
	memcpy(&us,&tmpbuf[39],2);
	//
	CCRCCheck *m_pOBCCRC = new CCRCCheck(16, 0x8005, true, true, 0xffff, 0);
	unsigned char crcbuf[41];
	memset(crcbuf,0,sizeof(crcbuf));
	memcpy(crcbuf,tmpbuf+10,29);
	unsigned short aa = CRC16(crcbuf,29);
    unsigned short bb = m_pOBCCRC->GetCRC(crcbuf,29);
	//测试取列车号
	unsigned char srcdata[5];
	memset(srcdata,0,sizeof(srcdata));
	memcpy(srcdata,&databuf[11],4);
	unsigned int uitmp = 0;
	memcpy(&uitmp,&databuf[11],4);
	uitmp = uitmp<<13;
	uitmp = uitmp>>13+5;
	
    //反转
	unsigned char resdata[5];
	memset(resdata,0,sizeof(resdata));
	int n = 3;
	int i = 0;
	for(i = 0;i < 4; i++)
	{
	   resdata[i] = srcdata[n];
	   n--;
	}
	//转成比特串,取中间12位
	char resstr[17];
	memset(resstr,0,sizeof(resstr));
	unsigned char utmp = 0;
	utmp = resdata[1];
	for(i = 0; i < 3; i++)
	{
	    utmp = utmp << 5+i;
	    utmp = utmp >> 5+i;
		int itmp = utmp >> (2-i);
		if(itmp)resstr[i] = '1';
		else resstr[i] = '0';

	}
	utmp = resdata[2];
	for(i = 0; i < 8; i++)
	{		
		int itmp = utmp >> (7-i);
		if(itmp)resstr[i+3] = '1';
		else resstr[i+3] = '0';
		utmp = utmp << i+1;
	    utmp = utmp >> i+1;
	}
	utmp = resdata[3];
	utmp = utmp >> 5;
	for(i = 0; i < 3; i++)
	{
		int itmp = utmp >> (2-i);
		if(itmp)resstr[i+11] = '1';
		else resstr[i+11] = '0';
		utmp = utmp << 6+i;
	    utmp = utmp >> 6+i;
	}
    //
	int err = 0;
	int sockfd = 0;
	struct sockaddr_in server;

	memset(&server,0,sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	//server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_addr.S_un.S_addr =  inet_addr(SERV_IP);
	server.sin_port = htons(SERV_PORT);
	//初始化网络
    netInit();

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd <= 0)
	{
		err = WSAGetLastError();
		fprintf(stderr,"Network failed to socket: %d\n",err);
		return -1;
	}

	dg_cli(sockfd,(SA*)&server,sizeof(sockaddr_in));
	closesocket(sockfd);*/
	int err = 0;
	int sockfd = 0;
	struct sockaddr_in server;

	memset(&server,0,sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	//server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_addr.S_un.S_addr =  inet_addr(SERV_IP);
	server.sin_port = htons(SERV_PORT);
	//初始化网络
    netInit();
	for(int i = 0; i < 200 ; i++)
	{
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		if(sockfd <= 0)
		{
			err = WSAGetLastError();
			fprintf(stderr,"Network failed to socket: %d\n",err);
			return -1;
		}
		Sleep(100);
        err = connect(sockfd,(SOCKADDR*)&server,sizeof(SOCKADDR));//连接服务器
	}
	//dg_cli(sockfd,(SA*)&server,sizeof(sockaddr_in));
	cin>>err;
	closesocket(sockfd);
    return 0;
}
