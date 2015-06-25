#include <asio_http_server/http/http_request.h>

#include <iostream>

namespace server
{
namespace asio
{

HttpRequest::HttpRequest()
: contentLength_(0),
  keepAlive_(false)
{
}

void HttpRequest::addHeader(const std::string& header,
			const std::string& value)
{
	headers_.emplace(header, value);
}

} // namespace asio
} // namespace server



