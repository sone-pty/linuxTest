#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <map>
#include <string>
#include <strings.h>
#include <functional>
#include <memory>

namespace sone
{

//请求方法
enum class http_method
{
	UNKNOW,
	GET,
	HEAD,
	POST,
	PUT,
	TRACE,
	OPTIONS,
	DELETE
};

//schemes
enum class http_scheme
{
	UNKNOW,
	HTTP,
	HTTPS
};

//versions
enum class http_version
{
	UNKNOW,
	HTTP10,
	HTTP11
};

//headers
enum class http_headers
{
	UNKNOW,
	//通用首部
	Connection,
	Date,
	MIME_Version,
	Trailer,
	Transfer_Encoding,
	Update,
	Via,
	Cache_Control,
	Pragma,
	//请求首部
	Client_Ip,
	Host,
	User_Agent,
	Accept,
	Accept_Charset,
	Accept_Encoding,
	Accept_Language,
	Authorization,
	Cookie,
	Upgrade_Insecure_Requests,
	Sec_Fetch_User,
	Sec_Fetch_Site,
	Sec_Fetch_Mode,
	Purpose,
	If_Modified_Since,
	Referer,
	Origin,
	//响应首部
	Age,
	Public,
	Server,
	Title,
	Set_Cookie,
	Expires,
	//实体首部
	Location,
	Content_Base,
	Content_Encoding,
	Content_Language,
	Content_Length,
	Content_Location,
	Content_MD5,
	Content_Range,
	Content_Type,
	Last_Modified
};

//响应状态码
enum class http_resp_state
{
	UNKNOW = 1,
	Continue = 100,
	OK = 200,
	Not_Modified = 304,
	Not_Found = 404,
	Bad_Request = 400,
	Forbidden = 403,
	Internal_Server_Error = 500,
	HTTP_Version_Not_Supported = 505
};

//HTTP报文每一行的解析状态
enum class http_line_state
{
	//行完整，行出错，行不完整
	LINE_OK, LINE_ERROR, LINE_MORE
};

//HTTP解析请求的状态
enum class req_check_state
{
	CHECK_REQUESTLINE,
	CHECK_HEADER,
	CHECK_CONTENT
};

std::string ConvertHeaderToString(http_headers header);
http_headers ConvertStringToHeader(const std::string& s);
std::string ConvertMethodToString(http_method method);
http_method ConvertStringToMethod(const std::string& s);
std::string ConvertRespStateToString(http_resp_state state);

struct CaseInsensitiveLess : public std::binary_function<std::string, std::string, bool>
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
	{
		return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
	}
};

struct CaseInsensitiveEqual : public std::binary_function<std::string, std::string, bool>
{
	bool operator()(const std::string& s1, const std::string& s2) const
	{
		return strcasecmp(s1.c_str(), s2.c_str()) == 0;
	}
};

class HttpRequest{
public:
	typedef std::map<std::string, std::string, CaseInsensitiveLess> Map;

	HttpRequest();
	~HttpRequest();
	//get-set
	http_scheme getScheme();
	void setScheme(http_scheme scheme);
	http_method getMethod();
	void setMethod(http_method method);
	http_version getVersion();
	void setVersion(http_version ver);
	std::string getRequestUrl();
	void setRequestUrl(const std::string& url);
	std::string getParam(const std::string& key);
	void setParam(const std::string& key, const std::string& val);
	std::string getCookie(const std::string& key);
	void setCookie(const std::string& key, const std::string& val);
	std::string getHeader(const std::string& header);
	bool setHeader(const std::string& key, const std::string& val);
	std::string getContent();
	void setContent(const std::string& content);
	std::string getQueryString();
	void setQueryString(const std::string &query_str);
private:
	//请求方法
	http_method req_func;
	//方案
	http_scheme _scheme;
	//请求头部
	Map _headers;
	//请求参数
	Map _params;
	//cookies
	std::map<std::string, std::string> _cookies;
	//请求URL
	std::string _url;
	//版本号
	http_version _version;
	//报文实体
	std::string _content;
	//请求字符串
	std::string query_string;
};

class HttpResponse{
public:
	typedef std::map<std::string, std::string, CaseInsensitiveLess> Map;

	HttpResponse();
	~HttpResponse();
	http_resp_state getRestState();
	void setRespState(http_resp_state state);
	http_version getVersion();
	void setVersion(http_version version);
	std::string getHeader(const std::string& header);
	bool setHeader(const std::string& key, const std::string& val);
	std::string getCookie(const std::string& key);
	void setCookie(const std::string& key, const std::string& val);
	std::string getContent();
	void setContent(const std::string& content);
	void setContent(const std::string&& content);
	//需要确保响应报文中填充的各个部分符合规则以及主体需要被编码的情况下调用
	std::string toString();
private:
	//响应码
	http_resp_state _state;
	//版本
	http_version _version;
	//首部
	Map _headers;
	//set-cookies
	std::map<std::string, std::string> _cookies;
	//主体
	std::string _content;
	//响应报文长度
	size_t _len;
};

}

#endif
