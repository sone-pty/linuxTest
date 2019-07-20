#include "server.h"
#include "common.h"

void printa(const char* str,int len)
{
	char c;
	for(int i = 0;i < len;i++)
	{
		c = str[i];
		cout << (int)c << " ";
	}
	cout << endl;
}

server::server()
{
	cout << "server start!" << endl;

	memset(recvbuf,0,BUFFSIZE);
	memset(sendbuf,0,BUFFSIZE);
	
	start();
}

server::~server()
{
	cout << "server stop!" << endl;
}

void server::start()
{
	/*
	//创建fifo
	int res = mkfifo("serfifo",0777);
	int fd;

	if(res == -1)
		sys_error("mkfifo",-1);
	*/
	int res;
	int fd = open("serfifo",O_RDWR);
	this -> ser_fifo = fd;

	//设置非阻塞
	int flag = fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);

	//获取客户端传过来的数据，并且分别解析
	while(1)
	{
		//接受客户端发送的连接数据
		res = read(fd,recvbuf,BUFFSIZE);

		//解析连接数据包
		if(res != -1)	
			parse(res);

		//接受每一个客户端发送的数据，并解析处理
		for(int i = 0;i < cli_statlist.size();i++)
		{	
			auto ptr = cli_statlist[i];

			int flag = fcntl(ptr -> pri_fifo,F_GETFL);
			flag |= O_NONBLOCK;
			fcntl(ptr -> pri_fifo,F_SETFL,flag);
			
			int len = read(ptr -> pri_fifo,datarecv,BUFFSIZE);

			if(len == -1)
				continue;
			else
				parsedata(len);
		}

		memset(recvbuf,0,BUFFSIZE);
		memset(datarecv,0,BUFFSIZE);
	}
}

void server::parsedata(int len)
{
	int fin = 0;
	PROTOCOL_ID id;
	translate t;
	memset(&t,0,TRANSIZE);

	while(fin != len)
	{
		memcpy(&id,datarecv + fin,sizeof(id));

		switch(id)
		{
			case TRANSLATE:
				{
					memcpy(&t,datarecv + fin,TRANSIZE);
					
					int src_index;
					auto cli_src = find(t.src,&src_index);
					//发送给其余客户端
					for(int i = 0;i < cli_statlist.size();i++)
					{
						auto ptr = cli_statlist[i];
						if(cli_src != ptr)
							write(ptr -> pri_fifo,&t,TRANSIZE);
						else
							continue;
					}
					
					fin += TRANSIZE;
					break;
				}
			default:return;
		}
	}
}

void server::parse(int len)
{
	int fin = 0;
	PROTOCOL_ID id;
	log l;
	translate t;
	
	memset(&l,0,LOGSIZE);
	memset(&t,0,TRANSIZE);

	while(fin != len)
	{
		//获得协议id
		memcpy(&id,recvbuf + fin,sizeof(id));

		switch(id)
		{
			case LOG://登录数据包
				{
					memcpy(&l,recvbuf + fin,LOGSIZE);
					//找到特定的client
					int index;
					auto cli  = find(l.identity,&index);
					
					//如果没找到，说明该客户端还没登录，设置status结构体，并放入vector
					if(!cli)
					{
						cout << l.identity << " login" << endl;

						status newcli;
						
						int fd = open(l.identity,O_RDWR);
						newcli.pri_fifo = fd;
						memcpy(newcli.name,l.identity,sizeof(l.identity));
						newcli.online = true;

						cli_statlist.push_back(&newcli);
					}
					//如果该客户端已经登录，那么查看数据包中的登录状态；如果状态是false，从vector中删除该客户端记录，否则，根据数据包中的值修改记录
					else
					{
						if(!l.log_status)
						{
							auto beg = cli_statlist.begin();
							cli_statlist.erase(beg + index);
							cout << "用户" << l.identity << "已经退出" << endl;
						}
						else
						{	
							cli -> online = l.log_status;
							memcpy(cli -> name,l.identity,sizeof(l.identity));
						}
					}
					
					//修改fin
					fin += LOGSIZE;
					break;
				}
				/*
			case TRANSLATE:
				{
					
					memcpy(&t,recvbuf + fin,TRANSIZE);
					
					int src_index;
					auto cli_src = find(t.src,&src_index);

					//发送给其余客户端
					for(int i = 0;i < cli_statlist.size();i++)
					{
						auto ptr = cli_statlist[i];
						if(i != src_index)
							write(ptr -> pri_fifo,&t,TRANSIZE);
						else
							continue;
					}
					
					fin += TRANSIZE;
					break;
				}*/
			default:return;
		}
	}
}

pstatus server::find(const char* name,int* index)
{
	for(int i = 0;i < cli_statlist.size();i++)
	{
		auto ptr = cli_statlist[i];
		printa(name,4);
		printa(ptr -> name,4);
		if(strcmp(name,ptr -> name) == 0)
		{
			*index = i;
			return ptr;
		}
	}
	//如果没找到index置-1，返回0
	*index = -1;
	return 0;
}

int server::getSerFifo(const char* name)
{
	int index;
	if(!find(name,&index))
		return -1;
	else
		return ser_fifo;
}
