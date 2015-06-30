
#include <asio_http_server/http/http_response.h>

#include <boost/noncopyable.hpp>

#include <glog/logging.h>

namespace server
{
namespace asio
{

std::string HttpResponse::versionToString(Version version)
{
	switch(version)
	{
		case Version::HTTP11: return "HTTP/1.1";
		case Version::HTTP10: return "HTTP/1.0";
		default: return "HTTP/1.0";
	}
}

std::string HttpResponse::statusToString(int status)
{
	switch(status)
	{
		case 100: return "100 Continue";
		case 101: return "101 Switching Protocols";
		case 200: return "200 OK";
		case 201: return "201 Created";
		case 202: return "202 Accepted";
		case 203: return "203 Non-Authoritative Information";
		case 204: return "204 No Content";
		case 205: return "205 Reset Content";
		case 206: return "206 Partial Content";
		case 300: return "300 Multiple Choices";
		case 301: return "301 Moved Permanently";
		case 302: return "302 Found";
		case 303: return "303 See Other";
		case 304: return "304 Not Modified";
		case 305: return "305 Use Proxy";
		case 307: return "307 Temporary Redirect";
		case 400: return "400 Bad Request";
		case 401: return "401 Unauthorized";
		case 402: return "402 Payment Required";
		case 403: return "403 Forbidden";
		case 404: return "404 Not Found";
		case 405: return "405 Method Not Allowed";
		case 406: return "406 Not Acceptable";
		case 407: return "407 Proxy Authentication Required";
		case 408: return "408 Request Timeout";
		case 409: return "409 Conflict";
		case 410: return "410 Gone";
		case 411: return "411 Length Required";
		case 412: return "412 Precondition Failed";
		case 413: return "413 Request Entity Too Large";
		case 414: return "414 Request-URI Too Long";
		case 415: return "415 Unsupported Media Type";
		case 416: return "416 Requested Range Not Satisfiable";
		case 417: return "417 Expectation Failed";
		case 418: return "418 I'm a teapot";
		case 422: return "422 Unprocessable Entity (WebDAV) (RFC 4918)";
		case 423: return "423 Locked (WebDAV) (RFC 4918)";
		case 424: return "424 Failed Dependency (WebDAV) (RFC 4918)";
		case 425: return "425 Unordered Collection (RFC 3648)";
		case 426: return "426 Upgrade Required (RFC 2817)";
		case 449: return "449 Retry With";
		case 450: return "450 Blocked by Windows Parental Controls";
		case 500: return "500 Internal Server Error";
		case 501: return "501 Not Implemented";
		case 502: return "502 Bad Gateway";
		case 503: return "503 Service Unavailable";
		case 504: return "504 Gateway Timeout";
		case 505: return "505 HTTP Version Not Supported";
		case 506: return "506 Variant Also Negotiates (RFC 2295)";
		case 507: return "507 Insufficient Storage (WebDAV) (RFC 4918)";
		case 509: return "509 Bandwidth Limit Exceeded (Apache bw/limited extension)";
		case 510: return "510 Not Extended (RFC 2774)";
		case 600: return "600 Malformed URI";
		case 601: return "601 Connection Timed";
		case 602: return "602 Unknown Error";
		case 603: return "603 Could Not Parse Reply";
		case 604: return "604 Protocol Not Supported";
		default: return std::string();
	}

}

HttpResponse::HttpResponse(
		Version version,
		int httpStatus)
: version_(version),
  httpStatus_(httpStatus),
  body_(nullptr)
{

}

void HttpResponse::setHttpVersion(Version version)
{
	version_ = version;
}

void HttpResponse::setBody(std::unique_ptr<boost::asio::streambuf> body)
{
	body_ = std::move(body);
}

void HttpResponse::addHeader(const std::string& headerName,
		const std::string& headerValue)
{
	headers_.emplace(headerName, headerValue);
}

std::ostream& operator<<(std::ostream& out, const HttpResponse& response)
{
	LOG(INFO) << "building response ...";

	out << HttpResponse::versionToString(response.version_) << " ";
	out << HttpResponse::statusToString(response.httpStatus_) << "\r\n";

	for(const auto pair: response.headers_)
	{
		out << pair.first << ": " << pair.second << "\r\n";
	}

	out << "\r\n";
	out << response.body_.get();
	out << "\r\n\r";
	out << std::flush;

	return out;
}

} // namespace asio
} // namespace server



