#include<stdio.h>
#include<Windows.h>
#include "ftplib.h"
#include "ftptest.h"
#include<string>
#include "iostream.h"
using namespace std;

typedef void (*pFtpInit)();  //
typedef int  (*pFtpConnect)(const char *host, netbuf **nControl);
typedef int  (*pFtpLogin)(const char *user, const char *pass, netbuf *nControl);
typedef void (*pFtpQuit)(netbuf *nControl);
typedef int  (*pFtpNlst)(const char *output, const char *path, netbuf *nControl);
typedef int  (*pFtpDir)(const char *output, const char *path, netbuf *nControl);
typedef int  (*pFtpGet)(const char *output, const char *path, char mode,netbuf *nControl);//
typedef int  (*pFtpMkdir)(const char *path, netbuf *nControl);
typedef int  (*pFtpRename)(const char *src, const char *dst, netbuf *nControl);
//HINSTANCE m_hInstince;	// 
 HMODULE pHandle;	// 

/******************************************************************************
* Purpose : ��ȡ���ִ�
* Returns : ��ȡ���ʳ���
* Argument: ais_in:�����ִ�;aos_out:����ִ�;aii_start:��ʼ��ȡλ��,��0��ʼ;aii_sublen:��ȡ����
* Author  : jiangjg 2016/12/02
*****************************************************************************/
int str_substr(char *aos_out,char *ais_in,int aii_start,int aii_sublen)
{
   int i,li_len;
   char *lsp_p;

   if (aii_sublen == 0)
   {
      aos_out[0]=0;
      return(0);
   }
   
   li_len = strlen(ais_in);

   lsp_p = ais_in+aii_start;
   for (i=0;i<aii_sublen && i<li_len;i++) aos_out[i] = *(lsp_p+i);
   aos_out[i] = 0;
   
   return(i);
}
/******************************************************************************
 * -rw-r--r--   1 jiangjg      root           0 Sep 10  2012 CMB_20120905_downloaded.TXT
 * Purpose : ��ls�Ľ���з�����ļ���,����
 * Returns : 0:�ɹ�;-1:����Ϊ��;-2:δ�ҵ�Ŀ¼��־
 * Argument: ais_filename:�����ļ���;aos_filename:����ļ���
 * Author  : jiangjg 2016/12/02
 *****************************************************************************/
int str_get_filename_from_ls(char *ais_str,char *aos_filename)
{
   int i,li_len;
   
   li_len=strlen(ais_str);
   if(li_len<=0) return(-1);
   
   for(i=0; i<li_len; i++) if (ais_str[i]=='/') break;	/*��Ŀ¼*/
   if (i>=li_len)	/*�Ҳ���Ŀ¼���ҿո�*/
   {
	   for(i=li_len-1;i>=0;i--) if (ais_str[i]==9 || ais_str[i]==32) break;
  	 if (i<0) return(-2);
	 }else i=i-1;
   
   str_substr(aos_filename,ais_str,i+1,li_len-i-1);

   return 0;
} 
/******************************************************************************
 * Purpose : �Ӱ���Ŀ¼���ļ����з�����ļ���
 * Returns : 0:�ɹ�;-1:����Ϊ��;-2:δ�ҵ�Ŀ¼��־
 * Argument: ais_filename:�����ļ���;aos_filename:����ļ���
 * Author  : jiangjg 2016/12/02
 *****************************************************************************/
int str_get_filename(char *ais_filename,char *aos_filename)
{
   int i,li_len;
   
   li_len=strlen(ais_filename);
   if(li_len<=0) return(-1);
   
   for(i=li_len-1;i>=0;i--) if (ais_filename[i]=='/') break;
   
   if (i<0) 
   {
   	 strcpy(aos_filename,ais_filename);
   	 return(-2);
   }
   
   str_substr(aos_filename,ais_filename,i+1,li_len-i-1);

   return 0;
}
/******************************************************************************
 * Purpose : �Ӱ���Ŀ¼���ļ����з�����ļ�����׺
 * Returns : 0:�ɹ�;-1:����Ϊ��;-2:δ�ҵ�Ŀ¼��־
 * Argument: ais_filename:�����ļ���;aos_fileext:����ļ�����׺
 * Author  : jiangjg 2012/12/07
 *****************************************************************************/
int str_get_fileext(char *ais_filename,char *aos_fileext)
{
   int i,li_len;
   
   li_len=strlen(ais_filename);
   if(li_len<=0) return(-1);
   
   for(i=li_len-1;i>=0;i--) if (ais_filename[i]=='.') break;
   
   if (i<0) return(-2);
   
   str_substr(aos_fileext,ais_filename,i+1,li_len-i-1);

   return(0);
}

int main()
{
	int xx = WinExec("telnet.exe 127.0.0.1",SW_SHOW);
    int i;
	cin>>i;

	/*char remoteListFileName[256],line[1024+1],lsTmp[1024+1],lsLocalTmpFile[256],lsLocalFile[256],remoteMatchFileList[256];
	char lsFileName[256],lsRemoteFile[256],lsRemoteBakFile[256];

	int ret = 0;
	netbuf *nControl;
	FILE *fp;
	nControl = (netbuf *)NULL;

	pHandle = ::LoadLibrary("ftplib.dll");

	pFtpQuit ppFtpQuit = NULL;
	ppFtpQuit = (pFtpQuit)GetProcAddress(pHandle,"FtpQuit");
	pFtpInit ppFtpInit = NULL;
	ppFtpInit = (pFtpInit)GetProcAddress(pHandle,"FtpInit");
	pFtpConnect ppFtpConnect = NULL;
	ppFtpConnect = (pFtpConnect)GetProcAddress(pHandle,"FtpConnect");
	pFtpLogin ppFtpLogin = NULL;
	ppFtpLogin = (pFtpLogin)GetProcAddress(pHandle,"FtpLogin");
	pFtpNlst ppFtpNlst = NULL;
	ppFtpNlst = (pFtpNlst)GetProcAddress(pHandle,"FtpNlst");
    pFtpDir ppFtpDir = NULL;
	ppFtpDir = (pFtpDir)GetProcAddress(pHandle,"FtpDir");
	pFtpGet ppFtpGet = NULL;
	ppFtpGet = (pFtpGet)GetProcAddress(pHandle,"FtpGet");//
	pFtpMkdir ppFtpMkdir = NULL;
	ppFtpMkdir = (pFtpMkdir)GetProcAddress(pHandle,"FtpMkdir");
	pFtpRename ppFtpRename = NULL;
	ppFtpRename = (pFtpRename)GetProcAddress(pHandle,"FtpRename");
    //��ʼ������
	ppFtpInit();

	string strMatchList = "csv,bin";
    while(true)
	{
		//Sleep(10000);
		memset(remoteListFileName,0,sizeof(remoteListFileName));
		memset(line,0,sizeof(line));
		memset(lsTmp,0,sizeof(lsTmp));
		memset(lsLocalTmpFile,0,sizeof(lsLocalTmpFile));
		memset(lsLocalFile,0,sizeof(lsLocalFile));
		memset(remoteMatchFileList,0,sizeof(remoteMatchFileList));
		memset(lsFileName,0,sizeof(lsFileName));
		memset(lsRemoteFile,0,sizeof(lsRemoteFile));
		memset(lsRemoteBakFile,0,sizeof(lsRemoteBakFile));

		char tmp[256];
		strcpy(tmp,"127.0.0.1:21");
		//����FTP������
		ret =  ppFtpConnect(tmp,&nControl);

		if(ret == 0)
		{
			printf("FtpConnect to [%s] failed.","127.0.0.1:21");
			
			if(nControl)
			{
				free(nControl);
				nControl=NULL;				
			}
			
			continue;
		}		
		printf("FtpConnect to %s succeed.","127.0.0.1:21");
		//��¼FTP������
		ret=ppFtpLogin("ftptest","ftptest",nControl);
		if(ret == 0)
		{
			printf("FtpLogin user=[%s] failed.","ftptest");
			ppFtpQuit(nControl);
			nControl = NULL;
			continue;
		}
		printf("FtpLogin succeed.");

		sprintf(remoteListFileName,"%s\\%s.filelist",".\\","DTS_FTP_����");	//Զ��ƥ���ļ��б����ļ���
		sprintf(remoteMatchFileList,"%s\\%s",".\\data\\","*.bin");	//Զ���ļ�ƥ�����

		//��ȡԶ���ļ��б�д������remoteListFileName�ļ�
		//ret=ppFtpDir(remoteListFileName,remoteMatchFileList,nControl);//
		ret=ppFtpNlst(remoteListFileName,remoteMatchFileList,nControl);//
		if(ret == 0)
		{
			printf("No match files found! [%s] [%s] ret[%d].",remoteListFileName,remoteMatchFileList,ret);
			ppFtpQuit(nControl);
			nControl = NULL;
			continue;
		}
		printf("match files succeed.");
		
		fp=fopen(remoteListFileName,"r");
		if (!fp)
		{
			printf("fopen [%s] failed.",remoteListFileName);
			ppFtpQuit(nControl);
			nControl = NULL;
			continue;
		}

		while(fgets(line,sizeof(line),fp))
		{
			line[strlen(line) - 1]=0;		
			printf("line [%s].",line);

			ret = str_get_filename(line,lsFileName);
			if (ret == -2) strcpy(lsFileName,line);	//���ϼ�Ŀ¼��ֱ��ȡ�ļ�
			
			printf("filename [%s],matchstr[%s].",lsFileName,".bin");
            
			bool bFind = false;
			string strFileName = lsFileName;
			char sfileext[20];
			memset(sfileext,0,sizeof(sfileext));
			str_get_fileext(lsFileName,sfileext);
			ret = strMatchList.find(sfileext);
			if(ret >= 0)bFind = true;
			if (bFind)	//ƥ����ļ�
			{
				sprintf(lsLocalTmpFile,"%s%s","D:\\jjg\\download\\tmp\\", lsFileName);	//������ʱ�ļ�
				sprintf(lsLocalFile,"%s%s%s%s","D:\\jjg\\download\\data\\", "",lsFileName,"");//���������ļ�//
				sprintf(lsRemoteFile,"%s%s",".\\data\\",lsFileName);//Զ��Դ�ļ�
				sprintf(lsRemoteBakFile,"%s%s%s%s",".\\data\\","", lsFileName, "bak");//Զ�̱����ļ�
				
				//�����ļ�
				ret=ppFtpGet(lsLocalTmpFile,lsRemoteFile,'I',nControl);
				if(ret == 0)
				{
					printf("FtpGet [%s] to [%s] failed.",lsRemoteFile,lsLocalTmpFile);
					break;	//������һ���ڵ�
				}
				printf("FtpGet %s succeed.",lsRemoteFile);
                //Զ�̽�������Ŀ¼
				//ret=ppFtpMkdir(".\\bak\\",nControl);
				//if(ret < 0)
				//{
				//	printf("ppFtpMkdir [%s] failed. ret[%d]",".\\bak\\",ret);
				//	break;	//������һ���ڵ�
				//}
				//printf("ppFtpMkdir [%s] succeed.",".\\bak\\");
				//Զ���ļ�ת�Ƶ�����
				ret=ppFtpRename(lsRemoteFile,lsRemoteBakFile,nControl);
				if(ret == 0)
				{
					printf("FtpRename [%s] to [%s] failed. ret[%d]",lsRemoteFile,lsRemoteBakFile,ret);
					break;	//������һ���ڵ�
				}
				printf("FtpRename [%s] to [%s] succeed.",lsRemoteFile,lsRemoteBakFile);
				
				//�����ļ��Ƶ���ʽĿ¼
				ret=rename(lsLocalTmpFile,lsLocalFile);
				if(ret != 0)
				{
					printf("rename [%s] to [%s] failed.errno[%d] msg[%s]",lsLocalTmpFile,lsLocalFile,errno,strerror(errno));
					break;	//������һ���ڵ�
				}
				printf("rename [%s] to [%s] succeed.",lsLocalTmpFile,lsLocalFile);
			}
		
		}
		fclose(fp);
		ppFtpQuit(nControl);
		nControl = NULL;
		Sleep(10000);
	}

    ::FreeLibrary(pHandle);
	pHandle = NULL;*/
	return 0;
}