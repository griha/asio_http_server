#include <asio_http_server/net/http/http_server.h>
#include <asio_http_server/net/server_option.h>
#include <glog/logging.h>
#include <asio_http_server/net/http/src/http1_parser.h>
#include <asio_http_server/net/http/http_request.h>
#include <asio_http_server/net/http/http_response.h>

namespace server
{
namespace asio
{

HttpServer::HttpServer(const ServerOption& serverOption)
: TcpServer(serverOption)
{
}

void HttpServer::onHttpRequest(boost::asio::ip::tcp::socket& socket,
		boost::asio::streambuf& inputBuffer)
{
    boost::system::error_code error;
    std::size_t alreadyRead = boost::asio::read_until(socket, inputBuffer, "\r\n\r\n", error);

    if (error == boost::asio::error::eof)
    {
        LOG(WARNING) << "Connection closed cleanly by peer.";
        return;

        //throw boost::system::system_error(error);
    }
    else if (error)
    {
        throw boost::system::system_error(error);
    }

    HttpParser parser;
    parser.parse(inputBuffer, alreadyRead);

    inputBuffer.consume(alreadyRead);

    // дочитываем, что не дочитали в read_until
    size_t contentLength = parser.getContentLength();
    if (contentLength > inputBuffer.size() && contentLength != -1ULL)
    {
        auto remainingBytes = contentLength - inputBuffer.size();

        boost::asio::read(socket, inputBuffer, boost::asio::transfer_at_least(remainingBytes));

        parser.parse(inputBuffer, contentLength);
    }

    HttpRequestPtr requestPtr = parser.getHttpRequest();
    auto version = requestPtr->getHttpVersion();
    const std::string& connectionStatus = requestPtr->getHeader("Connection");
    bool isConnectionClose = (connectionStatus == "close" || (version == HttpRequest::Version::HTTP10 && connectionStatus != "Keep-Alive")) ;

    HttpResponsePtr responsePtr = std::make_shared<server::asio::HttpResponse>(static_cast<HttpResponse::Version>(requestPtr->getHttpVersion()), 200, true);

    if (httpBodyCallback_)
    {
    	httpBodyCallback_(requestPtr, responsePtr);
    }

    boost::asio::streambuf outBuffer;
    onHttpResponse(responsePtr, outBuffer, socket);

}

void HttpServer::onHttpResponse(const HttpResponsePtr& response,
                            boost::asio::streambuf& outBuffer,
                            boost::asio::ip::tcp::socket& socket)
{
    std::ostream out(&outBuffer);
    out.rdbuf()->pubsetbuf(0, 0);
    out << *response << std::endl;

    boost::system::error_code ignored_error;

    size_t cn = boost::asio::write(socket, outBuffer, boost::asio::transfer_all(), ignored_error);

    LOG(INFO) << "writing response: " << cn;
}

} // namespace asio
} // namespace server

