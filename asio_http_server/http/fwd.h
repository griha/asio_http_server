/*
 * fwd.h
 *
 *  Created on: 14 апр. 2015 г.
 *      Author: griha
 */

#ifndef ASIO_SERVER_HTTP_FWD_H_
#define ASIO_SERVER_HTTP_FWD_H_

#include <memory>

namespace server
{
namespace asio
{

using HttpRequestPtr = std::shared_ptr<class HttpRequest>;
using HttpResponsePtr = std::shared_ptr<class HttpResponse>;
using HttpRequestHandlerPtr = std::shared_ptr<class IHttpRequestHandler>;
using IHttpRequestHandlerFactoryPtr = std::shared_ptr<class IHttpRequestHandlerFactory>;

} // namespace asio
} // namespace server

#endif /* ASIO_SERVER_HTTP_FWD_H_ */
