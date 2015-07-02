/*
 * http_response.h
 *
 *  Created on: 25 марта 2015 г.
 *      Author: griha
 */

#ifndef ASIO_SERVER_HTTP_HTTP_RESPONSE_H_
#define ASIO_SERVER_HTTP_HTTP_RESPONSE_H_

#include <string>
#include <boost/asio.hpp>

namespace server
{
namespace asio
{

class HttpResponse: private boost::noncopyable
{
public:

	enum class Version
	{
		HTTP_UNKNOWN,
		HTTP10,
		HTTP11
	};

	static std::string versionToString(Version version);

	static std::string statusToString(int status);

	HttpResponse(Version version,
			int httpStatus);

	virtual ~HttpResponse(){}

	void setHttpVersion(Version version);

	void setStatus(int status)
	{
		httpStatus_ = status;
	}

	void setBody(std::unique_ptr<boost::asio::streambuf> body);

	void addHeader(const std::string& headerName,
			const std::string& headerValue);

	friend std::ostream& operator<<(std::ostream& out, const HttpResponse& response);

private:

	Version version_;
	int httpStatus_;
	// header -> value
	std::map<std::string, std::string> headers_;
	std::unique_ptr<boost::asio::streambuf> body_;
	boost::asio::streambuf outBuf_;

};

//std::ostream& operator<<(std::ostream& out, HttpResponsePtr response);

} // namespace asio
} // namespace server



#endif /* ASIO_SERVER_HTTP_HTTP_RESPONSE_H_ */
