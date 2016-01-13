#ifndef ASIO_HTTP_SERVER_NET_SERVER_OPTION_H_
#define ASIO_HTTP_SERVER_NET_SERVER_OPTION_H_

#include <string>

namespace server
{
namespace asio
{

struct ServerOption
{
	std::string host;

	unsigned int port;

	size_t threads_count;
};

} // namespace asio
} // namespace server

#endif /* ASIO_HTTP_SERVER_NET_SERVER_OPTION_H_ */
