
#ifndef ASIO_HTTP_SERVER_NET_TCP_CONNECTION_H_
#define ASIO_HTTP_SERVER_NET_TCP_CONNECTION_H_

#include <boost/asio.hpp>
#include <asio_http_server/net/callbacks.h>

namespace server
{
namespace asio
{

using tcp = boost::asio::ip::tcp;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>,
					 private boost::noncopyable
{
public:
	TcpConnection(boost::asio::io_service& ioService,
			    const RequestCallback& requestCallback);

	void setRequestCallback(const RequestCallback& requestCallback)
	{
		requestCallback_ = requestCallback;
	}

	tcp::socket& socket() noexcept
	{
		return serverSocket_;
	}

	void process();

private:

	void readRequest();
	void writeResponse();

	tcp::socket serverSocket_;

	boost::asio::streambuf inBuf_;
	boost::asio::streambuf outBuf_;

	RequestCallback requestCallback_;


};

} // namespace asio
} // namespace server



#endif /* ASIO_HTTP_SERVER_NET_TCP_CONNECTION_H_ */
