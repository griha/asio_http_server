#ifndef ASIO_HTTP_SERVER_NET_REQUEST_HANDLER_H_
#define ASIO_HTTP_SERVER_NET_REQUEST_HANDLER_H_

#include <functional>
#include <boost/noncopyable.hpp>

#include <asio_http_server/net/http/fwd.h>

namespace server
{
namespace asio
{

struct RequestHandler: boost::noncopyable
{
public:

	virtual ~RequestHandler()
	{
	}

	virtual void onHeader(const HttpHeader& httpHeader) = 0;

	virtual void onRequest(const HttpRequest& request, HttpResponsePtr response) = 0;
};

} // namespace asio
} // namespace server

#endif /* ASIO_HTTP_SERVER_NET_REQUEST_HANDLER_H_ */
