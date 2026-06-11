#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include "ConfigServer.hpp"

class Client {
  private:
	ConfigServer _server;
	int			 _socket;
	int			 _statusCode;
	bool		 _sentRequest;
	bool 		 _isParseCompleted;
	bool 		 _isChunked;

	std::string	 _request;
	time_t		 _lastRequest;
	std::string	 _requestTarget;
	std::string	 _method;
	std::string  _resource;
	std::string  _httpVersion;
	std::string  _body;

	std::map<std::string, std::string> _fields;
	size_t		 _contentLength;

  public:
    Client();
    ~Client();
	void setRequest(const char* buf, const int &len);
	const std::string &getRequest() const;
	const std::string inspectRequest();
	void parseHeader(std::string &header);
    void checkRequestTarget();
    void checkHttpVersion();
	void divideKeyAndValue(std::string line);
	void parseFields(std::string request, size_t i);
	void parseCompleted();
	void checkMethod();

};

#endif
