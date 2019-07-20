#ifndef _SERVER_H_
#define _SERVER_H_

#include "../sysHeader.h"
#include "struct_data.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
#define BUFFSIZE 4096

//客户端状态结构体
typedef struct _STATUS
{
	//私有fifo的fd
	int pri_fifo;
	//客户端标识
	char name[4];
	//客户端是否在线
	bool online;
}status,*pstatus;

class server
{
	private:
		//保存客户端状态的链表
		vector<pstatus> cli_statlist;
		//服务端公共fifo的fd，用来接收所有客户端的数据
		int ser_fifo;
		//接收客户端连接数据buf
		char recvbuf[BUFFSIZE];
		//发送数据buf
		char sendbuf[BUFFSIZE];
		//接受客户端数据buf
		char datarecv[BUFFSIZE];

		//启动函数
		void start();
		//解析客户端连接数据包
		void parse(int len);
		//在vector中找到特定的客户端，并返回在链表中的位置
		pstatus find(const char* name,int* index);
		//解析客户端发送的数据
		void parsedata(int len);
	public:
		server();
		~server();
		//返回公有fifo的fd，如果请求的客户端没有登录,返回-1
		int getSerFifo(const char* name);
};

#endif
