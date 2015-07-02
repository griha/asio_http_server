#ifndef ASIO_HTTP_SERVER_NET_SRC_CONNECTION_H_
#define ASIO_HTTP_SERVER_NET_SRC_CONNECTION_H_

#include <memory>
#include <boost/asio.hpp>

#include <asio_http_server/net/http/fwd.h>

namespace server
{
namespace asio
{

class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection>,
					private boost::noncopyable
{
public:

	enum class State
	{
		LISTEN,
		READ,
		WRITE,
		CLOSE
	};

    using tcp = boost::asio::ip::tcp;

    Connection(boost::asio::io_service& ioService,
    		IHttpRequestHandlerFactoryPtr requestFactory,
			ConnectionManager& connectionManager);

    ~Connection();

    tcp::socket& socket() noexcept
    {
        return serverSocket_;
    }

    void start();

private:

    HttpRequestPtr readRequest();

    void writeResponse(const HttpResponsePtr& response);

    tcp::socket serverSocket_;
    ConnectionManager& connectionManager_;

    boost::asio::streambuf inBuf_;
    boost::asio::streambuf outBuf_;

    IHttpRequestHandlerFactoryPtr requestFactory_;

    State state_;

};

} // namespace asio
} // namespace server

#endif /* ASIO_HTTP_SERVER_NET_SRC_CONNECTION_H_ */
