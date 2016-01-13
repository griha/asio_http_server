#ifndef ASIO_HTTP_SERVER_NET_RESPONSE_HANDLER_H_
#define ASIO_HTTP_SERVER_NET_RESPONSE_HANDLER_H_

#include <boost/noncopyable.hpp>

#include <asio_http_server/net/http/fwd.h>

namespace server
{
namespace asio
{

struct ResponseHandler: private boost::noncopyable
{
public:

	virtual ~ResponseHandler()
	{}

	void onError(HttpResponsePtr response, const std::string& error);
};

} // namespace asio
} // namespace server

#endif /* ASIO_HTTP_SERVER_NET_RESPONSE_HANDLER_H_ */
