#include <asio_http_server/net/src/acceptor.h>
#include <asio_http_server/net/tcp_connection.h>

namespace server
{
namespace asio
{

namespace ip = boost::asio::ip;

Acceptor::Acceptor(const std::string& host,
		unsigned int port,
		boost::asio::io_service& ioService)
: acceptor_(ioService),
  ioService_(ioService)
{

	ip::tcp::endpoint endpoint = host.empty() ?
	    		ip::tcp::endpoint(ip::tcp::v4(), port) :
				ip::tcp::endpoint(ip::address().from_string(host), port);

	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(tcp::acceptor::reuse_address(true)); // SO_REUSEADDR
	acceptor_.set_option(boost::asio::socket_base::keep_alive(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();
}

void Acceptor::asyncAccept(TcpConnectionPtr connection,
		std::function<void(const boost::system::error_code& error)>&& acceptCallback)
{
	acceptor_.async_accept(connection->socket(), [connection, callback = std::move(acceptCallback)](const boost::system::error_code& error)
	{
		callback(error);
	});
}

} // namespace asio
} // namespace server
