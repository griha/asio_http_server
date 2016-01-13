#ifndef ASIO_SERVER_HTTP_FWD_H_
#define ASIO_SERVER_HTTP_FWD_H_

#include <memory>
#include <map>

namespace server
{
namespace asio
{

using HttpRequestPtr = std::shared_ptr<class HttpRequest>;
using HttpResponsePtr = std::shared_ptr<class HttpResponse>;
using HttpRequestHandlerPtr = std::shared_ptr<class IHttpRequestHandler>;
using IHttpRequestHandlerFactoryPtr = std::shared_ptr<class IHttpRequestHandlerFactory>;
using HttpHeader = std::map<std::string, std::string>;

} // namespace asio
} // namespace server

#endif /* ASIO_SERVER_HTTP_FWD_H_ */
