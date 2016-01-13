#include <asio_http_server/net/tcp_connection.h>
#include <glog/logging.h>

namespace server
{
namespace asio
{

TcpConnection::TcpConnection(boost::asio::io_service& ioService,
					    const RequestCallback& requestCallback)
: serverSocket_(ioService),
  requestCallback_(requestCallback)
{

}

void TcpConnection::readRequest()
{

}

void TcpConnection::writeResponse()
{

}

void TcpConnection::process()
{
	if (requestCallback_)
	{
		VLOG(6) << "executing callback";
		requestCallback_(serverSocket_, inBuf_);
	}
	else
	{
		VLOG(6) << "request callback is empty";
	}
}

} // namespace asio
} // namespace server




