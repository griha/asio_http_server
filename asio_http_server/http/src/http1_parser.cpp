#include <asio_http_server/http/src/http1_parser.h>

#include <boost/throw_exception.hpp>
#include <boost/algorithm/string.hpp>

#include <externals/http_parser/http_parser.h>

#include <glog/logging.h>
#include <asio_http_server/http/http_request.h>

namespace server
{
namespace asio
{

HttpParser::HttpParser():
		httpRequest_(std::make_shared<HttpRequest>())
{
	http_parser_init(httpParser_.get(), HTTP_REQUEST);
	parserSettings_->on_message_begin = HttpParser::messageBeginCallback;
	parserSettings_->on_url = HttpParser::urlCallback;
	parserSettings_->on_body = HttpParser::bodyCallback;
	parserSettings_->on_headers_complete = HttpParser::headersCompleteCallback;
	parserSettings_->on_message_complete = HttpParser::messageCompleteCallback;
	parserSettings_->on_header_field = HttpParser::headerFieldCallback;
	parserSettings_->on_header_value = HttpParser::HttpParser::headerValueCallback;
	httpParser_->data = this;
}

HttpParser::~HttpParser()
{
}

int HttpParser::bodyCallback(http_parser* parser, const char* buf, size_t len)
{
	LOG(INFO) << "bodyCallback";

	HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	httpParser->httpRequest_->setInputBuffer(buf, len);

	return 0;
}

int HttpParser::urlCallback(http_parser* parser, const char* buf, size_t len)
{
	LOG(INFO) << "urlCallback";
	HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	httpParser->httpRequest_->setUrl(std::string(buf, len));

	return 0;
}

int HttpParser::messageBeginCallback(http_parser* /*parser*/)
{
	LOG(INFO) << "message begin callback";
	return 0;
}

int HttpParser::messageCompleteCallback(http_parser* /*parser*/)
{
	LOG(INFO) << "message complete callback";

	return 0;
}

int HttpParser::statusCallback(http_parser* /*parser*/,
		const char* /*buf*/,
		size_t /*len*/)
{
	LOG(INFO) << "statusCallback";

	//HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	return 0;
}

int HttpParser::headerFieldCallback(http_parser* parser, const char* buf, size_t len)
{
	LOG(INFO) << "headerFieldCallback";

	HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	httpParser->httpRequest_->setCurrentHeaderKey(std::string(buf, len));

	return 0;
}

int HttpParser::headerValueCallback(http_parser* parser, const char* buf, size_t len)
{
	LOG(INFO) << "headerValueCallback";
	HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	const std::string value(buf, len);

	httpParser->httpRequest_->addHeader(httpParser->httpRequest_->getCurrentHeaderKey(), value);

	if (httpParser->httpRequest_->getCurrentHeaderKey() == "Content-length")
	{
		httpParser->httpRequest_->setContentLength(std::stoull(value));
	}

	if ((httpParser->httpRequest_->getCurrentHeaderKey() == "Connection") &&
			(boost::iequals(value, "keep-alive")))
	{
		httpParser->httpRequest_->setKeepAlive(true);
	}

	LOG(INFO) << "header: " << httpParser->httpRequest_->getCurrentHeaderKey() << ", value: " << value;

	return 0;
}

int HttpParser::headersCompleteCallback(http_parser* parser)
{
	LOG(INFO) << "headersCompleteCallback";

	HttpParser* httpParser = static_cast<HttpParser*>(parser->data);

	httpParser->httpRequest_->setHttpVersion(parser->http_major, parser->http_minor);

	if (parser->content_length)
		httpParser->httpRequest_->setContentLength(parser->content_length);

	return 0;
}

size_t HttpParser::getContentLength() const
{
	return httpRequest_->getContentLength();
}

void HttpParser::parse(const boost::asio::streambuf& inputBuffer, size_t bytes)
{
	const char* buff = boost::asio::buffer_cast<const char*>(inputBuffer.data());

	size_t nparsed = http_parser_execute(httpParser_.get(), parserSettings_.get(), buff, bytes);

	if (nparsed != bytes)
	{
		const char* error = http_errno_description(HTTP_PARSER_ERRNO(httpParser_.get()));

		BOOST_THROW_EXCEPTION(std::runtime_error("failed to read from socket: " + std::string(error)));
	}
}

} // namespace asio
} // namespace server
