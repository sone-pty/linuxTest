#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "struct_data.h"
#include "../sysHeader.h"
#include <string>
#include <iostream>

using namespace std;
#define BUFFSIZE 4096

class server;

class client
{
	private:
		//私有fifo的fd
		int pri_fifo;
		//客户端标识
		char name[4];
		//当前是否在线
		bool online = false;
		//服务端公有fifo的fd
		int serfifo;
		//接收服务端数据buf
		char ser_recv[BUFFSIZE];
		//接收终端数据buf
		char std_recv[100];

		//登录或者登出函数
		bool login(bool status);
		//和其他客户端通信的函数，target是目标的标识，data是数据
		bool talk(char* destname,string data);
		//收发数据
		void start();
		//解析数据
		void parse(int len);
	public:
		client(const string& serfifoname,const string& name);
		~client();
};

#endif
