#include <glog/logging.h>

#include <asio_http_server/net/tcp_server.h>

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
	option.threads_count = 1;

	server::asio::TcpServer tcpServer(option);
	tcpServer.setRequestCallback([&tcpServer](boost::asio::ip::tcp::socket& socket,
								 boost::asio::streambuf& inputBuffer)
						    {
							    tcpServer.OnRequest(socket, inputBuffer);
						    });

	PLOG(INFO) << "starting server";
	VLOG(6) << "-----------";

	tcpServer.run();

	return 0;
}
