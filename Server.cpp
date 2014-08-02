#include <stdio.h>
#include<WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#define  BUF_SZIE    256
int main(int argc, char* argv[]){
	/*char ch=255;
	printf("%d",(unsigned char)ch);*/
	char buf_rec[BUF_SZIE];
	int retVal;
	SOCKET sClient;

	/*初始化套结字动态库*/
	WSADATA  wsd;
	if (WSAStartup(MAKEWORD(2,2), &wsd) != 0){
		printf("WSAStartup failed!\n");
		return 1;
	}

	/*创建套接字*/
	SOCKET sServer;
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if(INVALID_SOCKET == sServer){
		printf("socket failed!\n");
		WSACleanup();
		return  -1;
	}

	/*服务器套接字地址*/
	SOCKADDR_IN  addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(4999);
	addrServ.sin_addr.s_addr = INADDR_ANY;      

	/*绑定套接字*/
	retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal){   
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	/*开始监听*/
	retVal = listen(sServer, 1);
	if(SOCKET_ERROR == retVal){       
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	/*接受客户端请求*/
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	sClient = accept(sServer,(sockaddr FAR*)&addrClient, &addrClientlen);
	if(INVALID_SOCKET == sClient){
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	/*接收客户端数据*/
	while(1){
		ZeroMemory(buf_rec, BUF_SZIE);
		retVal = recv(sClient, buf_rec, BUF_SZIE, 0);
		FILE* pf=fopen(buf_rec,"wb");	
		while(1){
			recv(sClient, buf_rec, BUF_SZIE, 0);
			if(buf_rec[0]=='#'&&buf_rec[1]=='!')
				break;
			fwrite(buf_rec+1,1,(unsigned char)buf_rec[0],pf);
			ZeroMemory(buf_rec, BUF_SZIE);
			//recv(sClient, buf_rec, BUF_SZIE, 0);
		}
		fclose(pf);
	}		
	/*退出*/
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	getchar();
	return 0;
}
