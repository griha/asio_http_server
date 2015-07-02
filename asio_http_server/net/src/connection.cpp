#include <asio_server/connection.h>

#include <glog/logging.h>
#include <boost/asio/read.hpp>

#include <asio_server/connection_manager.h>

#include <asio_server/http/http1_parser.h>
#include <asio_server/http/http_request_factory.h>
#include <asio_server/http/http_request_handler.h>

#include <asio_server/http/http_response.h>
#include <asio_server/http/http_request.h>

namespace server
{
namespace asio
{

Connection::Connection(boost::asio::io_service& ioService,
		IHttpRequestHandlerFactoryPtr requestFactory,
		ConnectionManager& connectionManager)
: serverSocket_(ioService),
  connectionManager_(connectionManager),
  requestFactory_(requestFactory),
  state_(State::LISTEN)
{
}

Connection::~Connection()
{
	LOG(INFO) << "Connection::~Connection";
	serverSocket_.close();
}

void Connection::start()
{
	try
	{
		HttpRequestPtr httpRequest = readRequest();
		HttpRequestHandlerPtr requestHandler =
				requestFactory_->createHttpRequestHandler(httpRequest);

		const HttpResponsePtr& response = requestHandler->process(httpRequest);

		writeResponse(response);

		if (httpRequest->isKeepAlive())
		{
			auto self = shared_from_this();
			//connectionManager_.add(shared_from_this());
			//boost::asio::io_service& ioService = serverSocket_.get_io_service();
			//ioService.post([self](){ self->start(); });
		}
	}
	catch(const std::exception& ex)
	{
		LOG(ERROR) << "exception: " << ex.what();
	}
}

// for post request
HttpRequestPtr Connection::readRequest()
{
	state_ = State::READ;

	boost::system::error_code error;
	std::size_t alreadyRead = boost::asio::read_until(serverSocket_, inBuf_, "\r\n\r\n", error);

	if (error == boost::asio::error::eof)
	{
		LOG(WARNING) << "Connection closed cleanly by peer.";
		//return HttpRequestPtr();
		throw boost::system::system_error(error);
	}
	else if (error)
	{
		throw boost::system::system_error(error);
	}

	HttpParser parser;
	parser.parse(inBuf_, alreadyRead);

	inBuf_.consume(alreadyRead);

	// дочитываем, что не дочитали в read_until
	size_t contentLength = parser.getContentLength();
	if (contentLength > inBuf_.size() && contentLength != -1ULL)
	{
		auto remainingBytes = contentLength - inBuf_.size();

		boost::asio::read(serverSocket_, inBuf_, boost::asio::transfer_at_least(remainingBytes));

		parser.parse(inBuf_, contentLength);
	}

	return parser.getHttpRequest();
}

void Connection::writeResponse(const HttpResponsePtr& response)
{
	state_ = State::WRITE;

	std::ostream out(&outBuf_);
	out.rdbuf()->pubsetbuf(0, 0);
	out << *response.get() << std::endl;

	boost::system::error_code ignored_error;

	size_t cn = boost::asio::write(serverSocket_, outBuf_, boost::asio::transfer_all(), ignored_error);

	LOG(INFO) << "writing response: " << cn;
}

} // namespace asio
} // namespace server



