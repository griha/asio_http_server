#include <glog/logging.h>
#include <asio_http_server/net/http/http_server.h>
#include <asio_http_server/net/http/fwd.h>
#include <asio_http_server/net/http/http_request.h>
#include <asio_http_server/net/http/http_response.h>

int main(int argc, char* argv[])
{

     google::InitGoogleLogging(argv[0]);
     google::InstallFailureSignalHandler();

     //FLAGS_logtostderr = 1;
     FLAGS_colorlogtostderr = 1;
     //FLAGS_stderrthreshold = 2;
     FLAGS_v = 6;

     //ServerOption
     server::asio::ServerOption option;
     option.host = "127.0.0.1";
     option.port = 9999;
     option.threads_count = 20;

     server::asio::HttpServer httpServer(option);
     httpServer.setRequestCallback([&httpServer](boost::asio::ip::tcp::socket& socket,
     								 boost::asio::streambuf& inputBuffer)
     						    {
    	 	 	 	 	 	 	 	 httpServer.onHttpRequest(socket, inputBuffer);
     						    });

    httpServer.setHttpBodyCallback([&](server::asio::HttpRequestPtr httpRequest)
    		 {
    	 	 	 auto response = std::make_shared<server::asio::HttpResponse>(server::asio::HttpResponse::Version::HTTP11, 200);
    	 	 	 response->addHeader("Content-Type", "text/html");

				if ((httpRequest->getHttpVersion() == server::asio::HttpRequest::Version::HTTP11) ||
						httpRequest->isKeepAlive())
				{
					response->addHeader("Connection", "keep-alive");
					//response->addHeader("Connection", "close");
				}


				auto body = std::make_unique<boost::asio::streambuf>();
				std::ostream out(body.get());

				const std::string str = "<html><head><title>test</title></head>"
							"<body><h1>Test</h1><p>This is a test!</p></body></html>";

				out << str;

				response->addHeader("Content-Length: ", std::to_string(str.length()));


				response->setBody(std::move(body));

				return response;
    		 });

     PLOG(INFO) << "starting server";
     VLOG(6) << "-----------";

     httpServer.run();

     return 0;
}

