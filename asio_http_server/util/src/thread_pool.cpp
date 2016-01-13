#include <asio_http_server/util/thread_pool.h>

#include <thread>

namespace server
{
namespace asio
{

namespace
{

void bindToCpu(size_t cpuCores)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	CPU_ZERO(&cpuset);
	CPU_SET(cpuCores , &cpuset);

	sched_setaffinity(0, sizeof(cpuset), &cpuset);

	 while (1);
}

}

ThreadPool::ThreadPool(std::size_t poolSize,
		std::size_t cpuCores,
		const std::string& /*threadName*/)
: nextIoService_(0),
  cpuCores_(cpuCores)
{
	for (std::size_t i = 0; i < poolSize; ++i)
	{
		IoServerPtr io_service = std::make_shared<boost::asio::io_service>();
		WorkPtr work = std::make_shared<boost::asio::io_service::work>(*io_service);
		iOServices_.push_back(io_service);
		works_.push_back(work);
	}
}

void ThreadPool::run()
{
	std::vector<std::unique_ptr<std::thread>> threads;
	for (std::size_t i = 0; i < iOServices_.size(); ++i)
	{
		std::unique_ptr<std::thread> thread = std::make_unique<std::thread>([this, i]
				{
					iOServices_[i]->run();
				}
		);

		threads.push_back(std::move(thread));
	}

	for(std::size_t i = 0; i < threads.size(); ++i)
	    threads[i]->join();
}

void ThreadPool::stop()
{
	for (std::size_t i = 0; i < iOServices_.size(); ++i)
	{
		iOServices_[i]->stop();
	}
}

boost::asio::io_service& ThreadPool::nextIoService()
{
	boost::asio::io_service& io_service = *iOServices_[nextIoService_];
	  ++nextIoService_;
	  if (nextIoService_ == iOServices_.size())
	    nextIoService_ = 0;
	  return io_service;
}

} // namespace asio
} // namespace server
