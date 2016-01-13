#ifndef ASIO_HTTP_SERVER_CALLBACKS_H_H
#define ASIO_HTTP_SERVER_CALLBACKS_H_H

#include <asio_http_server/net/http/fwd.h>

namespace server
{
namespace asio
{

// callback для парсинга протокола
using HttpRequestCallback = std::function<HttpRequestPtr(boost::asio::streambuf& inputBuffer,
                boost::asio::ip::tcp::socket& socket)>;

using HttpBodyCallback = std::function<HttpResponsePtr(HttpRequestPtr)>;

using HttpResponseCallback = std::function<void(const HttpResponsePtr& response,
                boost::asio::streambuf& outBuffer,
        boost::asio::ip::tcp::socket& socket)>;

} // namespace asio
} // namespace server

#endif //ASIO_HTTP_SERVER_CALLBACKS_H_H
