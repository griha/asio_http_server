#ifndef SOURCE_HTTP1_PARSER_H_
#define SOURCE_HTTP1_PARSER_H_

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>

#include <asio_http_server/net/http/fwd.h>

struct http_parser;
struct http_parser_settings;

namespace server
{
namespace asio
{

class HttpParser: private boost::noncopyable
{
public:

	HttpParser();

	~HttpParser();

	void parse(const boost::asio::streambuf& inputBuffer,
			size_t bytes);

	size_t getContentLength() const;

	HttpRequestPtr getHttpRequest() const
	{
		return httpRequest_;
	}

private:

	static int bodyCallback(http_parser* parser, const char* buf, size_t len);
	static int urlCallback(http_parser* parser, const char* buf, size_t len);
	static int messageBeginCallback(http_parser* parser);
	static int messageCompleteCallback(http_parser* parser);
	static int statusCallback(http_parser* parser, const char* buf, size_t len);
	static int headerFieldCallback(http_parser* parser, const char* buf, size_t len);
	static int headerValueCallback(http_parser* parser, const char* buf, size_t len);
	static int headersCompleteCallback(http_parser* parser);

	std::shared_ptr<http_parser> httpParser_;
	std::shared_ptr<http_parser_settings> parserSettings_;
	HttpRequestPtr httpRequest_;
};

} // namespace asio
} // namespace server


#endif /* SOURCE_HTTP1_PARSER_H_ */
