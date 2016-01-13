#include <asio_http_server/net/response_handler.h>

#include <boost/asio/streambuf.hpp>

#include <asio_http_server/net/http/http_response.h>

namespace server
{
namespace asio
{

void ResponseHandler::onError(HttpResponsePtr response, const std::string& error)
{
	response->setStatus(500);
	std::unique_ptr<boost::asio::streambuf> outBuf;
	std::ostream out(outBuf.get());
	out << error;

	response->setBody(std::move(outBuf));
}

} // namespace asio
} // namespace server

