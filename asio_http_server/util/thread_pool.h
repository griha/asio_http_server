#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <memory>

#include <boost/asio.hpp>
#include <vector>
#include <boost/noncopyable.hpp>

namespace server
{
namespace asio
{

class ThreadPool: private boost::noncopyable
{
public:

	explicit ThreadPool(std::size_t poolSize,
			std::size_t cpuCores,
			const std::string& threadName);

	void run();

	void stop();

	template<typename Task>
	void addTask(Task&& task)
	{
		boost::asio::io_service& ioService = nextIoService();
		ioService.post(std::forward<Task>(task));
	}

	boost::asio::io_service& nextIoService();

private:
	using IoServerPtr = std::shared_ptr<boost::asio::io_service>;
	using WorkPtr = std::shared_ptr<boost::asio::io_service::work>;

	std::vector<IoServerPtr> iOServices_;
	std::vector<WorkPtr> works_;
	std::size_t nextIoService_;
	std::size_t cpuCores_;


};

} // namespace asio
} // namespace server

#endif /* THREAD_POOL_H_ */
