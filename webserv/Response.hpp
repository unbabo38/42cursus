#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "ConfigServer.hpp"
class Client;
class Response {
  private:
    std::string _response;
	std::map<int, std::string> _errorMap;
  public:
    Response();
	~Response();
    Location longestPrefixMatch(std::string requestTarget, const std::vector<Location> &locations);
	void createResponse(Client *clinet, std::vector<ConfigServer> servers);
	std::string errorResponse(Client *client);
	std::string regularResponse(Client *client, std::vector<ConfigServer> servers);
	const std::string &getResponseStr() const;
	void setResponseStr(std::string response);
	void initErrorMap();

};

#endif
