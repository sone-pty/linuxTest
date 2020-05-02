#include "FcgiConnection.h"
#include "consts.h"

namespace sone
{
	FcgiConnection::FcgiConnection(eventloop* loop, int fd, const InetAddress& local_addr, const InetAddress& peer_addr, int requestId)
		:TcpConnection(loop, fd, local_addr, peer_addr), _requestId(requestId)
	{
		_socket->setNoBlock(false);
	}

	FcgiConnection::~FcgiConnection()
	{

	}

	void FcgiConnection::connecionEstablished()
	{

	}

	void FcgiConnection::connecionDestroyed()
	{
		
	}

	void FcgiConnection::handleRead()
	{

	}

	void FcgiConnection::handleWrite()
	{

	}

	void FcgiConnection::handleClose()
	{
		
	}

	void FcgiConnection::readContent(Buffer* buf)
	{
		FCGI_Header resp_header;
		int contentLen;
		char tmp[8];

		while(read(_socket->getFd(), &resp_header, FCGI_HEADER_LEN) > 0)
		{
			if(resp_header.type == FCGI_STDOUT)
			{
				contentLen = (resp_header.contentLengthB1 << 8) + (resp_header.contentLengthB0);
				buf->read(_socket->getFd(), contentLen);
				//跳过填充部分
				if(resp_header.paddingLength > 0)
					::read(_socket->getFd(), tmp, resp_header.paddingLength);
			}
			else if(resp_header.type == FCGI_STDERR)
			{
				contentLen = (resp_header.contentLengthB1 << 8) + (resp_header.contentLengthB0);
				char error_content[contentLen];
				::read(_socket->getFd(), error_content, contentLen);
				//跳过填充部分
				if(resp_header.paddingLength > 0)
					::read(_socket->getFd(), tmp, resp_header.paddingLength);
			}
			else if(resp_header.type == FCGI_END_REQUEST)
			{
				FCGI_EndRequestBody endRequest;
				::read(_socket->getFd(), &endRequest, sizeof(endRequest));
			}
		}
	}

	void FcgiConnection::sendBeginRequest()
	{
		FCGI_BeginRequestRecord begin_record;
		//设置头部
		begin_record.header.version = FCGI_VERSION_1;
		begin_record.header.type = FCGI_BEGIN_REQUEST;
		begin_record.header.requestIdB1 = (unsigned char)((_requestId >> 8) & 0xff);
		begin_record.header.requestIdB0 = (unsigned char)(_requestId & 0xff);
		begin_record.header.contentLengthB1 = (unsigned char)((sizeof(FCGI_BeginRequestBody) >> 8) & 0xff);
		begin_record.header.contentLengthB0 = (unsigned char)(sizeof(FCGI_BeginRequestBody) & 0xff);
		begin_record.header.paddingLength = 0;
		begin_record.header.reserved = 0;
		//设置体
		begin_record.body.roleB1 = (unsigned char)((FCGI_RESPONDER >> 8) & 0xff);
		begin_record.body.roleB0 = (unsigned char)(FCGI_RESPONDER & 0xff);
		begin_record.body.flags = 0;
		memset(&begin_record.body.reserved, 0, sizeof(begin_record.body.reserved));
		output_buffer.append((char*)&begin_record, sizeof(begin_record));
	}

	void FcgiConnection::sendEndRequest()
	{
		FCGI_Header end_header;
		end_header.version = FCGI_VERSION_1;
		end_header.type = FCGI_PARAMS;
		end_header.requestIdB1 = (unsigned char)((_requestId >> 8) & 0xff);
		end_header.requestIdB0 = (unsigned char)(_requestId & 0xff);
		end_header.contentLengthB1 = 0;
		end_header.contentLengthB0 = 0;
		end_header.paddingLength = 0;
		end_header.reserved = 0;
		output_buffer.append((char*)&end_header, sizeof(end_header));
		
		ssize_t len = ::write(_socket->getFd(), output_buffer.peek(), output_buffer.dataLen());
		output_buffer.moveLow(len);
	}

	void FcgiConnection::makeValueBody(const std::string& key, const std::string& value, unsigned char*	bodybuffer, int* bodylen)
	{
		unsigned char *startBodyBuffPtr = bodybuffer;  //记录body的开始位置
		size_t nameLen = key.length(), valueLen = value.length();
		
		if(nameLen < 128)//如果nameLen长度小于128字节
		{
			*bodybuffer++ = (unsigned char)nameLen;    //nameLen用一个字节保存
		}
		else
    	{
    	    //nameLen用4个字节保存
    	    *bodybuffer++ = (unsigned char)((nameLen >> 24) | 0x80);
    	    *bodybuffer++ = (unsigned char)(nameLen >> 16);
    	    *bodybuffer++ = (unsigned char)(nameLen >> 8);
    	    *bodybuffer++ = (unsigned char)nameLen;
    	}

    	if(valueLen < 128)  //valueLen小于128就用一个字节保存
    	{
    	    *bodybuffer++ = (unsigned char)valueLen;
    	}
    	else
    	{
    	    //valueLen用4个字节保存
    	    *bodybuffer++ = (unsigned char)((valueLen >> 24) | 0x80);
    	    *bodybuffer++ = (unsigned char)(valueLen >> 16);
    	    *bodybuffer++ = (unsigned char)(valueLen >> 8);
    	    *bodybuffer++ = (unsigned char)valueLen;
    	}

    	for(auto ch : key)
    	{
    	    *bodybuffer++ = ch;
    	}

    	//将value中的值逐一加入body的buffer中
    	for(auto ch : value)
    	{
    	    *bodybuffer++ = ch;
    	}

    	//计算出body的长度
    	*bodylen = bodybuffer - startBodyBuffPtr;
	}

	void FcgiConnection::sendParams(const std::string& key, const std::string& value)
	{
		unsigned char buff[BUFSIZE];
		memset(buff, 0, BUFSIZE);
		int bodylen;

		makeValueBody(key, value, buff, &bodylen);
		FCGI_Header nameValueHeader;

		nameValueHeader.version = FCGI_VERSION_1;
		nameValueHeader.type = FCGI_PARAMS;
		nameValueHeader.requestIdB1 = (unsigned char)((_requestId >> 8) & 0xff);
		nameValueHeader.requestIdB0 = (unsigned char)(_requestId & 0xff);
		nameValueHeader.contentLengthB1 = (unsigned char)((bodylen >> 8) & 0xff);
		nameValueHeader.contentLengthB0 = (unsigned char)(bodylen & 0xff);
		nameValueHeader.paddingLength = 0;
		nameValueHeader.reserved = 0;

		output_buffer.append((char*)&nameValueHeader, sizeof(nameValueHeader));
		output_buffer.append((char*)buff, bodylen);
	}
}
