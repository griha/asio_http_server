#ifndef ASIO_HTTP_SERVER_NET_TCP_SERVER_H_
#define ASIO_HTTP_SERVER_NET_TCP_SERVER_H_

#include <boost/noncopyable.hpp>

#include <boost/asio.hpp>

#include <asio_http_server/net/server_option.h>
#include <asio_http_server/net/src/acceptor.h>
#include <asio_http_server/util/thread_pool.h>
#include <asio_http_server/net/fwd.h>
#include <asio_http_server/net/callbacks.h>

namespace server
{
namespace asio
{

class TcpServer: private boost::noncopyable
{

public:

	TcpServer(const ServerOption& serverOption);

	virtual ~TcpServer();

	void run();

	void setRequestCallback(const RequestCallback& callback)
	{
		requestCallback_ = callback;
	}

	void OnRequest( boost::asio::ip::tcp::socket& socket,
				 boost::asio::streambuf& inputBuffer);

protected:

	RequestCallback requestCallback_;


private:

	void onAccept(TcpConnectionPtr connectionPtr,
			const boost::system::error_code& error);

	boost::asio::io_service& getIoService()
	{
		return threadPool_.nextIoService();
	}

	void accept();

	void signalHandler(int signum)
	{
		threadPool_.stop();
	}

	const ServerOption serverOption_;
	ThreadPool threadPool_;

	Acceptor acceptor_;
	boost::asio::signal_set signals_;


};

} // namespace asio
} // namespace server



#endif /* ASIO_HTTP_SERVER_NET_TCP_SERVER_H_ */
