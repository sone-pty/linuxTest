#include "client.h"
#include "common.h"

client::client(const string& serfifoname,const string& name)
{
	memcpy(this -> name,name.c_str(),sizeof(this -> name));
	//获得serfifo的fd，设置非阻塞并保存
	int temp = open(serfifoname.c_str(),O_RDWR);

	if(temp == -1)
		sys_error("client-open",-1);

	//获得私有fifo的fd
	int fd;
	while((fd = open(this -> name,O_RDWR)) == -1)
		mkfifo(this -> name,0777);

	this -> pri_fifo = fd; 
	//私有fifo设置非阻塞
	int flag = fcntl(fd,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd,F_SETFL,flag);
	
	//终端读非阻塞
	flag = fcntl(STDIN_FILENO,F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(STDIN_FILENO,F_SETFL,flag);

	this -> serfifo = temp;

	//创建私有fifo文件
	//登录
	login(true);

	cout << "登录成功,按w进行回复,按q退出" << endl;

	//开始收发数据
	start();
}

client::~client()
{

}

void client::start()
{
	char req;
	while(1)
	{
		//读取终端输入
		req = getchar();
		//读取服务端输入
		memset(ser_recv,0,BUFFSIZE);
		int len = read(this -> pri_fifo,ser_recv,BUFFSIZE);
		parse(len);

		switch(req)
		{
			case 'q':
				{
					//发送登出数据包
					login(false);
					exit(1);
				}
			case 'w':
				{
					cout << "请输入要发送的内容" << endl;
					char data[BUFFSIZE];
					int len = read(STDIN_FILENO,std_recv,BUFFSIZE);

					while(len == -1)
					{
						len = read(STDIN_FILENO,std_recv,BUFFSIZE);
						//sleep(1);
					}
					
					
					//构造translate数据包
					translate t;
					t.id = TRANSLATE;
					memcpy(t.src,this -> name,sizeof(this -> name));
					memcpy(t.data,this -> std_recv,sizeof(this -> std_recv));

					//从私有fifo发送数据
					int res = write(this -> pri_fifo,&t,TRANSIZE);
					sleep(1);

					if(res == -1)
						sys_error("client::start-write",-1);

					cout << name << ":" << std_recv << endl;
					break;
				}
			default:continue;
		}
	}
}

void client::parse(int len)
{
	if(len == -1)
		return;

	PROTOCOL_ID id;
	int fin = 0;
	while(fin != len)
	{
		memcpy(&id,ser_recv + fin,sizeof(id));

		switch(id)
		{
			case TRANSLATE:
				{
					translate t;
					memcpy(&t,ser_recv + fin,TRANSIZE);

					cout << t.src << ":" << t.data << endl;

					fin += TRANSIZE;
					break;
				}
			default:continue;
		}
	}
}

bool client::login(bool status)
{
	//构造登录数据包并发送到serfifo
	log l;
	memcpy(l.identity,this -> name,sizeof(this -> name));
	l.id = LOG;
	l.log_status = status;

	int res = write(this -> serfifo,&l,LOGSIZE);

	if(res == -1)
		return false;

	if(res == LOGSIZE)
	{
		online = status;
		return true;
	}

	return false;
}
