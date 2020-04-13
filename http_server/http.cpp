#include "http.h"

namespace sone
{
	static std::map<int, std::string> RESPSTATES =
	{
		{ 100, "CONTINUE" },
		{ 200, "OK" },
		{ 400, "Bad Request" },
		{ 403, "Forbidden" },
		{ 404, "Not Found" },
		{ 500, "Internal Server Error" },
		{ 505, "HTTP_Version_Not_Supported" }
	};

	static std::map<std::string, http_method, CaseInsensitiveLess> METHODS = 
	{
		{ "GET", http_method::GET },
		{ "POST", http_method::POST },
		{ "HEAD", http_method::HEAD },
		{ "PUT", http_method::PUT },
		{ "TRACE", http_method::TRACE },
		{ "OPTIONS", http_method::OPTIONS },
		{ "DELETE", http_method::DELETE }
	};

	static std::map<std::string, http_headers, CaseInsensitiveLess>  HEADERS = 
	{
		#define X(s) \
			{ ConvertHeaderToString(http_headers::s), http_headers::s },
			
			X(Connection)
			X(Date)
			X(MIME_Version)
			X(Trailer)
			X(Transfer_Encoding)
			X(Update)
			X(Via)
			X(Cache_Control)
			X(Pragma)
			X(Client_Ip)
			X(Host)
			X(User_Agent)
			X(Accept)
			X(Accept_Charset)
			X(Accept_Encoding)
			X(Accept_Language)
			X(Authorization)
			X(Cookie)
			X(Upgrade_Insecure_Requests)
			X(Sec_Fetch_User)
			X(Sec_Fetch_Site)
			X(Sec_Fetch_Mode)
			X(Purpose)
			X(If_Modified_Since)
			X(Age)
			X(Public)
			X(Server)
			X(Title)
			X(Set_Cookie)
			X(Expires)
			X(Location)
			X(Content_Base)
			X(Content_Encoding)
			X(Content_Language)
			X(Content_Length)
			X(Content_Location)
			X(Content_MD5)
			X(Content_Range)
			X(Content_Type)
		#undef X
			{ "Last-Modified", http_headers::Last_Modified }
	};

	std::string ConvertRespStateToString(http_resp_state state)
	{
		std::string res;
		switch(state)
		{
			case http_resp_state::UNKNOW:
				res = "UNKNOW";break;
			case http_resp_state::OK:
				res = "OK";break;
			case http_resp_state::Not_Modified:
				res = "Not Modified";break;
			case http_resp_state::Bad_Request:
				res = "Bad Request";break;
			case http_resp_state::Continue:
				res = "Continue";break;
			case http_resp_state::Forbidden:
				res = "Forbidden";break;
			case http_resp_state::HTTP_Version_Not_Supported:
				res = "HTTP Version Not Supported";break;
			case http_resp_state::Internal_Server_Error:
				res = "Internal Server Error";break;
			case http_resp_state::Not_Found:
				res = "Not Found";break;
		}

		return res;
	}

	std::string ConvertMethodToString(http_method method)
	{
		std::string res;
		switch(method)
		{
			case http_method::GET:
				res = "GET";break;
			case http_method::DELETE:
				res = "DELETE";break;
			case http_method::HEAD:
				res = "HEAD";break;
			case http_method::OPTIONS:
				res = "OPTIONS";break;
			case http_method::POST:
				res = "POST";break;
			case http_method::PUT:
				res = "PUT";break;
			case http_method::TRACE:
				res = "TRACE";break;
			default:
				res = "UNKNOW";break;
		}

		return res;
	}

	http_method ConvertStringToMethod(const std::string& s)
	{
		if(METHODS.find(s) == METHODS.end())
			return http_method::UNKNOW;
		else
			return METHODS[s];
	}

	std::string ConvertHeaderToString(http_headers header)
	{
		std::string res;
		switch(header)
		{
			case http_headers::UNKNOW:
				res = "UNKNOW";break;
			case http_headers::Connection:
				res = "Connection";break;
			case http_headers::Date:
				res = "Date";break;
			case http_headers::MIME_Version:
				res = "MIME-Version";break;
			case http_headers::Trailer:
				res = "Trailer";break;
			case http_headers::Transfer_Encoding:
				res = "Transfer-Encoding";break;
			case http_headers::Update:
				res = "Update";break;
			case http_headers::Via:
				res = "Via";break;
			case http_headers::Cache_Control:
				res = "Cache-Control";break;
			case http_headers::Pragma:
				res = "Pragma";break;
			case http_headers::Client_Ip:
				res = "Client-Ip";break;
			case http_headers::Host:
				res = "Host";break;
			case http_headers::User_Agent:
				res = "User-Agent";break;
			case http_headers::Accept:
				res = "Accept";break;
			case http_headers::Accept_Charset:
				res = "Accept-Charset";break;
			case http_headers::Accept_Encoding:
				res = "Accept-Encoding";break;
			case http_headers::Accept_Language:
				res = "Accept-Language";break;
			case http_headers::Authorization:
				res = "Authorization";break;
			case http_headers::Cookie:
				res = "Cookie";break;
			case http_headers::Upgrade_Insecure_Requests:
				res = "Upgrade-Insecure-Requests";break;
			case http_headers::Sec_Fetch_Mode:
				res = "Sec-Fetch-Mode";break;
			case http_headers::Sec_Fetch_Site:
				res = "Sec-Fetch-Site";break;
			case http_headers::Sec_Fetch_User:
				res = "Sec-Fetch-User";break;
			case http_headers::Purpose:
				res = "Purpose";break;
			case http_headers::If_Modified_Since:
				res = "If-Modified-Since";break;
			case http_headers::Age:
				res = "Age";break;
			case http_headers::Public:
				res = "Public";break;
			case http_headers::Server:
				res = "Server";break;
			case http_headers::Title:
				res = "Title";break;
			case http_headers::Set_Cookie:
				res = "Set-Cookie";break;
			case http_headers::Expires:
				res = "Expires";break;
			case http_headers::Location:
				res = "Location";break;
			case http_headers::Content_Base:
				res = "Content-Base";break;
			case http_headers::Content_Encoding:
				res = "Content-Encoding";break;
			case http_headers::Content_Language:
				res = "Content-Language";break;
			case http_headers::Content_Length:
				res = "Content-Length";break;
			case http_headers::Content_Location:
				res = "Content-Location";break;
			case http_headers::Content_MD5:
				res = "Content-MD5";break;
			case http_headers::Content_Range:
				res = "Content-Range";break;
			case http_headers::Content_Type:
				res = "Content-Type";break;
			case http_headers::Last_Modified:
				res = "Last-Modified";break;
		}
		return res;
	}

	http_headers ConvertStringToHeader(const std::string& s)
	{
		auto iter = HEADERS.find(s);
		if(iter != HEADERS.end())
			return iter->second;
		else
			return http_headers::UNKNOW;
	}

	/* HttpRequest */
	HttpRequest::HttpRequest()
		:req_func(http_method::UNKNOW), _scheme(http_scheme::UNKNOW), _version(http_version::UNKNOW)
	{

	}

	HttpRequest::~HttpRequest()
	{

	}

	http_scheme HttpRequest::getScheme()
	{
		return _scheme;
	}

	http_method HttpRequest::getMethod()
	{
		return req_func;
	}

	http_version HttpRequest::getVersion()
	{
		return _version;
	}

	std::string HttpRequest::getRequestUrl()
	{
		return _url;
	}

	std::string HttpRequest::getParam(const std::string& key)
	{
		return _params[key];
	}

	std::string HttpRequest::getCookie(const std::string& key)
	{
		return _cookies[key];
	}

	std::string HttpRequest::getHeader(const std::string& header)
	{
		auto iter = HEADERS.find(header);
		if(iter == HEADERS.end())
			return "UNKNOW";
		else
			return _headers[iter->first];
	}

	std::string HttpRequest::getContent()
	{
		return _content;
	}

	bool HttpRequest::setHeader(const std::string& key, const std::string& val)
	{
		auto iter = HEADERS.find(key);
		if(iter == HEADERS.end())
			return false;
		else
		{
			_headers[ConvertHeaderToString(iter->second)] = val;
			return true;
		}
	}

	void HttpRequest::setScheme(http_scheme scheme)
	{
		_scheme = scheme;
	}

	void HttpRequest::setMethod(http_method method)
	{
		req_func = method;
	}

	void HttpRequest::setVersion(http_version ver)
	{
		_version = ver;
	}

	void HttpRequest::setRequestUrl(const std::string& url)
	{
		_url = url;
	}

	void HttpRequest::setParam(const std::string& key, const std::string& val)
	{
		_params[key] = val;
	}

	void HttpRequest::setCookie(const std::string& key, const std::string& val)
	{
		_cookies[key] = val;
	}

	void HttpRequest::setContent(const std::string& content)
	{
		_content = content;
	}

	/* HttpResponse */
	HttpResponse::HttpResponse()
		:_state(http_resp_state::UNKNOW), _version(http_version::UNKNOW) {}

	HttpResponse::~HttpResponse() {}

	void HttpResponse::setCookie(const std::string& key, const std::string& val)
	{
		_cookies[key] = val;
	}

	void HttpResponse::setContent(const std::string& content)
	{
		_content = content;
	}

	void HttpResponse::setContent(const std::string&& content)
	{
		_content = std::move(content);
	}

	bool HttpResponse::setHeader(const std::string& key, const std::string& val)
	{
		auto iter = HEADERS.find(key);
		if(iter == HEADERS.end())
			return false;
		else
		{
			_headers[ConvertHeaderToString(iter->second)] = val;
			return true;
		}
	}

	void HttpResponse::setVersion(http_version ver)
	{
		_version = ver;
	}
		
	http_resp_state HttpResponse::getRestState()
	{
		return _state;
	}

	void HttpResponse::setRespState(http_resp_state state)
	{
		_state = state;
	}

	std::string HttpResponse::getCookie(const std::string& key)
	{
		return _cookies[key];
	}

	std::string HttpResponse::getHeader(const std::string& header)
	{
		auto iter = HEADERS.find(header);
		if(iter == HEADERS.end())
			return "UNKNOW";
		else
			return _headers[iter->first];
	}

	std::string HttpResponse::getContent()
	{
		return _content;
	}
	
	http_version HttpResponse::getVersion()
	{
		return _version;
	}

	std::string HttpResponse::toString()
	{
		std::string res;
		res.reserve(512);
		//状态行
		if(_version == http_version::HTTP10)
			res.append("HTTP/1.0 ");
		else if(_version == http_version::HTTP11)
			res.append("HTTP/1.1 ");
		switch(_state)
		{
			case http_resp_state::Continue:
				res.append("100 Continue\r\n");break;
			case http_resp_state::OK:
				res.append("200 OK\r\n");break;
			case http_resp_state::Not_Modified:
				res.append("304 Not Modified\r\n");break;
			case http_resp_state::Not_Found:
				res.append("404 Not Found\r\n");break;
			case http_resp_state::Bad_Request:
				res.append("400 Bad Request\r\n");break;
			case http_resp_state::Forbidden:
				res.append("403 Forbidden\r\n");break;
			case http_resp_state::Internal_Server_Error:
				res.append("500 Internal Server Error\r\n");break;
			case http_resp_state::HTTP_Version_Not_Supported:
				res.append("505 HTTP Version Not Supported\r\n");break;
			default:
				break;
		}
		//首部
		for(auto iter = _headers.begin();iter != _headers.end();++iter)
			res.append(iter->first + ": " + iter->second + "\r\n");
		//主体
		res.append("\r\n" + _content);
		_len = res.length();
		return res;
	}
}
