#ifndef ASIO_HTTP_SERVER_NET_SRC_ACCEPTOR_H_
#define ASIO_HTTP_SERVER_NET_SRC_ACCEPTOR_H_

#include <boost/asio.hpp>
#include <asio_http_server/net/fwd.h>

namespace server
{
namespace asio
{

typedef boost::asio::ip::tcp tcp;


class Acceptor: private boost::noncopyable
{
public:
	Acceptor(const std::string& host,
			unsigned int port,
			boost::asio::io_service& ioService);

	void asyncAccept(TcpConnectionPtr connection,
			std::function<void(const boost::system::error_code& error)>&& acceptCallback);

private:

	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_service& ioService_;
};

} // namespace asio
} // namespace server



#endif /* ASIO_HTTP_SERVER_NET_SRC_ACCEPTOR_H_ */
