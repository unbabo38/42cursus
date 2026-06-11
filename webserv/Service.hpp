#include <string>
#include "ConfigServer.hpp"
#include "Client.hpp"

class Service {
  private:
	ConfigServer _server;
	int			 _socket;
	bool		 _sentRequest;
	std::string	 _request;
	time_t		 _lastRequest;
	std::string	 _requestPayload;
	std::string	 _method;
	std::string  _resource;
	//stringMap	 _headers;
	size_t		 _contentLength;
    std::map<int, Client> _client;

  public:
    Service(){};
	~Service(){};
	std::map<int, Client> getClient() const;

};
