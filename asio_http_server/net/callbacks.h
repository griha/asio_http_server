#ifndef ASIO_HTTP_SERVER_NET_CALLBACKS_H_
#define ASIO_HTTP_SERVER_NET_CALLBACKS_H_

#include <functional>
#include <boost/asio/streambuf.hpp>
#include <asio_http_server/net/fwd.h>

namespace server
{
namespace asio
{

// callback для чтения данных
using RequestCallback = std::function<void(boost::asio::ip::tcp::socket& ,
		boost::asio::streambuf& inputBuffer)>;

// callback для парсинга протокола
using ProtocolCallback = std::function<void(const boost::asio::streambuf& inputBuffer)>;

} // namespace asio
} // namespace server

#endif /* ASIO_HTTP_SERVER_NET_CALLBACKS_H_ */
