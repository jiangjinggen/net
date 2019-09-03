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
* Purpose : 截取子字串
* Returns : 截取单词长度
* Argument: ais_in:输入字串;aos_out:输出字串;aii_start:起始截取位置,从0开始;aii_sublen:截取长度
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
 * Purpose : 从ls的结果中分离出文件名,如上
 * Returns : 0:成功;-1:输入为空;-2:未找到目录标志
 * Argument: ais_filename:输入文件名;aos_filename:输出文件名
 * Author  : jiangjg 2016/12/02
 *****************************************************************************/
int str_get_filename_from_ls(char *ais_str,char *aos_filename)
{
   int i,li_len;
   
   li_len=strlen(ais_str);
   if(li_len<=0) return(-1);
   
   for(i=0; i<li_len; i++) if (ais_str[i]=='/') break;	/*找目录*/
   if (i>=li_len)	/*找不到目录，找空格*/
   {
	   for(i=li_len-1;i>=0;i--) if (ais_str[i]==9 || ais_str[i]==32) break;
  	 if (i<0) return(-2);
	 }else i=i-1;
   
   str_substr(aos_filename,ais_str,i+1,li_len-i-1);

   return 0;
} 
/******************************************************************************
 * Purpose : 从包含目录的文件名中分离出文件名
 * Returns : 0:成功;-1:输入为空;-2:未找到目录标志
 * Argument: ais_filename:输入文件名;aos_filename:输出文件名
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
 * Purpose : 从包含目录的文件名中分离出文件名后缀
 * Returns : 0:成功;-1:输入为空;-2:未找到目录标志
 * Argument: ais_filename:输入文件名;aos_fileext:输出文件名后缀
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
    //初始化网络
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
		//连接FTP服务器
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
		//登录FTP服务器
		ret=ppFtpLogin("ftptest","ftptest",nControl);
		if(ret == 0)
		{
			printf("FtpLogin user=[%s] failed.","ftptest");
			ppFtpQuit(nControl);
			nControl = NULL;
			continue;
		}
		printf("FtpLogin succeed.");

		sprintf(remoteListFileName,"%s\\%s.filelist",".\\","DTS_FTP_配置");	//远程匹配文件列表结果文件名
		sprintf(remoteMatchFileList,"%s\\%s",".\\data\\","*.bin");	//远程文件匹配参数

		//获取远程文件列表，写到本地remoteListFileName文件
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
			if (ret == -2) strcpy(lsFileName,line);	//无上级目录，直接取文件
			
			printf("filename [%s],matchstr[%s].",lsFileName,".bin");
            
			bool bFind = false;
			string strFileName = lsFileName;
			char sfileext[20];
			memset(sfileext,0,sizeof(sfileext));
			str_get_fileext(lsFileName,sfileext);
			ret = strMatchList.find(sfileext);
			if(ret >= 0)bFind = true;
			if (bFind)	//匹配的文件
			{
				sprintf(lsLocalTmpFile,"%s%s","D:\\jjg\\download\\tmp\\", lsFileName);	//本地临时文件
				sprintf(lsLocalFile,"%s%s%s%s","D:\\jjg\\download\\data\\", "",lsFileName,"");//本地最终文件//
				sprintf(lsRemoteFile,"%s%s",".\\data\\",lsFileName);//远程源文件
				sprintf(lsRemoteBakFile,"%s%s%s%s",".\\data\\","", lsFileName, "bak");//远程备份文件
				
				//下载文件
				ret=ppFtpGet(lsLocalTmpFile,lsRemoteFile,'I',nControl);
				if(ret == 0)
				{
					printf("FtpGet [%s] to [%s] failed.",lsRemoteFile,lsLocalTmpFile);
					break;	//处理下一个节点
				}
				printf("FtpGet %s succeed.",lsRemoteFile);
                //远程建立备份目录
				//ret=ppFtpMkdir(".\\bak\\",nControl);
				//if(ret < 0)
				//{
				//	printf("ppFtpMkdir [%s] failed. ret[%d]",".\\bak\\",ret);
				//	break;	//处理下一个节点
				//}
				//printf("ppFtpMkdir [%s] succeed.",".\\bak\\");
				//远程文件转移到备份
				ret=ppFtpRename(lsRemoteFile,lsRemoteBakFile,nControl);
				if(ret == 0)
				{
					printf("FtpRename [%s] to [%s] failed. ret[%d]",lsRemoteFile,lsRemoteBakFile,ret);
					break;	//处理下一个节点
				}
				printf("FtpRename [%s] to [%s] succeed.",lsRemoteFile,lsRemoteBakFile);
				
				//本地文件移到正式目录
				ret=rename(lsLocalTmpFile,lsLocalFile);
				if(ret != 0)
				{
					printf("rename [%s] to [%s] failed.errno[%d] msg[%s]",lsLocalTmpFile,lsLocalFile,errno,strerror(errno));
					break;	//处理下一个节点
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