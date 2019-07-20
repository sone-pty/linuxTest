#ifndef _CHAT_H_
#define _CHAT_H_


//定义协议id
enum PROTOCOL_ID
{
	LOG,TRANSLATE,ERROR
};

//登录数据包
typedef struct _LOG_
{
	//协议id
	PROTOCOL_ID id;
	//登录状态，false表示登出，true表示登入
	bool log_status;
	//身份标识
	char identity[4];
}log,*plog;

//发送数据数据包
typedef struct _TRANSLATE_
{
	//协议id
	PROTOCOL_ID id;
	//源客户端
	char src[4];
	//目标客户端
	//char dest[4];
	//发送的数据，上限100字节
	char data[100];
}translate,*ptranslate;

//错误数据包
typedef struct _ERROR_
{
	//协议id
	PROTOCOL_ID id;
	//错误信息
	char info[50];
}error,*Perror;

//结构体大小常量
enum
{
	LOGSIZE = sizeof(log),
	TRANSIZE = sizeof(translate),
	ERRORSIZE = sizeof(error)
};

#endif
