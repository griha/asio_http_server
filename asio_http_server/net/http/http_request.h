#ifndef ASIO_SERVER_SOURCE_HTTP_REQUEST_H_
#define ASIO_SERVER_SOURCE_HTTP_REQUEST_H_

#include <boost/asio/streambuf.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <string>
#include <fstream>
#include <map>

#include <glog/logging.h>

namespace server
{
namespace asio
{

class HttpRequest: private boost::noncopyable
{
public:

	enum class Version
	{
		HTTP_UNKNOWN,
		HTTP10,
		HTTP11
	};

	HttpRequest();

	~HttpRequest(){}

	void addHeader(const std::string& header,
			const std::string& value);

	const std::string& getMethod() const noexcept
	{
		return method_;
	}

	void setCurrentHeaderKey(const std::string& currentHeaderKey)
	{
		currentHeaderKey_ = currentHeaderKey;
	}

	std::string getCurrentHeaderKey() const noexcept
	{
		return currentHeaderKey_;
	}

	void setHttpVersion(unsigned short httpMajor,
			unsigned short httpMinor)
	{
		if (httpMajor == 1 && httpMinor == 0)
		{
			version_ = Version::HTTP10;
		}
		else if (httpMajor == 1 && httpMinor ==1 )
		{
			version_ = Version::HTTP11;
		}
		else
		{
			version_ = Version::HTTP_UNKNOWN;
		}

	}

	HttpRequest::Version getHttpVersion() const
	{
		return version_;
	}

	void setUrl(const std::string& url)
	{
		url_.append(url);
	}

	void setContentLength(long long contentLength)
	{
		contentLength_ = contentLength;
	}

	long long getContentLength() const noexcept
	{
		return contentLength_;
	}

	void setInputBuffer(const char* buffer, size_t size)
	{
		std::ostream out(&inBuf_);
		out.write(buffer, size);

		VLOG(6) << "size of stream: " << inBuf_.size();

#if 0
		std::ofstream tmp;
		tmp.open("/tmp/1.log");
		tmp << out.rdbuf();
		tmp.close();
#endif

	}

	const boost::asio::streambuf& getInputBuffer() const
	{
		return inBuf_;
	}

	void setKeepAlive(bool value)
	{
		keepAlive_ = value;
	}

	bool isKeepAlive() const
	{
		return keepAlive_;
	}

	std::string getHeader(const std::string& header) const
	{
	     auto it = headers_.find(header);
             if (it != headers_.end())
             {
                 return it->second;
             }
             else
             {
                 return std::string();
             }
	}

private:

	Version version_;
	const std::string method_;
	std::string url_;
	std::string currentHeaderKey_;
	long long contentLength_;
	bool keepAlive_;

	// header -> value
	std::map<std::string, std::string> headers_;

	boost::asio::streambuf inBuf_;
};

} // namespace asio
} // namespace server

#endif /* ASIO_SERVER_SOURCE_HTTP_REQUEST_H_ */
