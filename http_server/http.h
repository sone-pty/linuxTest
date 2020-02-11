#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include <map>
#include <string>
#include <strings.h>
#include <functional>

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
	//响应首部
	Age,
	Public,
	Server,
	Title,
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

std::string ConvertHeaderToString(http_headers header);
http_headers ConvertStringToHeader(const std::string& s);

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
	Map _cookies;
	//请求URL
	std::string _url;
	//版本号
	http_version _version;
};

}

#endif
