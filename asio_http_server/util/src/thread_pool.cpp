#include <asio_http_server/util/thread_pool.h>

#include <memory>
#include <thread>
#include <functional>

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
: next_io_service_(0),
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
					//bindToCpu(cpuCores_);
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
	boost::asio::io_service& io_service = *iOServices_[next_io_service_];
	  ++next_io_service_;
	  if (next_io_service_ == iOServices_.size())
	    next_io_service_ = 0;
	  return io_service;
}

} // namespace asio
} // namespace server
