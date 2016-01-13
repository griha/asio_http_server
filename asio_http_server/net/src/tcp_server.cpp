#include <asio_http_server/net/tcp_server.h>

#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <glog/logging.h>

#include <asio_http_server/net/tcp_connection.h>

namespace server
{
namespace asio
{

TcpServer::TcpServer(const ServerOption& serverOption)
:serverOption_(serverOption),
  threadPool_(serverOption_.threads_count, std::thread::hardware_concurrency(), "worker"),
  acceptor_(serverOption.host, serverOption.port, getIoService()),
  signals_(getIoService())
{
	signals_.add(SIGINT);
	signals_.add(SIGTERM);

	signals_.async_wait([this](boost::system::error_code, int signum)
	{
		signalHandler(signum);
	});
}

TcpServer::~TcpServer()
{
}

void TcpServer::accept()
{
	auto connectionPtr = std::make_shared<TcpConnection>(getIoService(), requestCallback_);
	acceptor_.asyncAccept(connectionPtr,
			[this, connectionPtr](const boost::system::error_code& error)
			{
				onAccept(connectionPtr, error);
			});
}

void TcpServer::onAccept(TcpConnectionPtr connectionPtr,
			const boost::system::error_code& error)
{
	if (!error)
	{
		auto socketFd = connectionPtr->socket().native_handle();

		VLOG(6) << "accepting connection:" << socketFd;

		threadPool_.addTask([connectionPtr]()
		{
			connectionPtr->process();
		});

		accept();
	}
	else
	{
		LOG(ERROR) << "accept failed with error: " << error.message();
	}
}

void TcpServer::OnRequest( boost::asio::ip::tcp::socket& socket,
		boost::asio::streambuf& inputBuffer)
{
	boost::system::error_code error;
	std::size_t alreadyRead = boost::asio::read_until(socket, inputBuffer, "\r\n\r\n", error);

	if (error == boost::asio::error::eof)
	{
		LOG(WARNING) << "Connection closed cleanly by peer.";

		throw boost::system::system_error(error);
	}
	else if (error)
	{
		throw boost::system::system_error(error);
	}

}

void TcpServer::run()
{
	accept();
	threadPool_.run();
}

} // namespace asio
} // namespace server


