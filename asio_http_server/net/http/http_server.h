#ifndef ASIO_HTTP_SERVER_NET_HTTP_HTTP_SERVER_H_
#define ASIO_HTTP_SERVER_NET_HTTP_HTTP_SERVER_H_

#include <asio_http_server/net/tcp_server.h>
#include <asio_http_server/net/http/fwd.h>
#include <asio_http_server/net/http/callbacks.h>

namespace server
{
namespace asio
{

class HttpServer: public TcpServer
{
public:
	HttpServer(const ServerOption& serverOption);

	void setHttpBodyCallback(const HttpBodyCallback& bodyCallback)
	{
		httpBodyCallback_ = bodyCallback;
	}

	void onHttpRequest(boost::asio::ip::tcp::socket& socket,
	                    boost::asio::streambuf& inputBuffer);

private:

	void onHttpResponse(const HttpResponsePtr& response,
					boost::asio::streambuf& outBuffer,
					boost::asio::ip::tcp::socket& socket);


	void readRequest();

	void writeRespone();

	HttpBodyCallback httpBodyCallback_;

};

} // namespace asio
} // namespace server


#endif /* ASIO_HTTP_SERVER_NET_HTTP_HTTP_SERVER_H_ */
