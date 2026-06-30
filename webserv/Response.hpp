#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <ctime>
#include "ConfigServer.hpp"
#include "cgi.hpp"

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
	std::string errorResponse(Client *client, int errorNum);
	std::string regularResponse(Client *client);
	const std::string &getResponseStr() const;
	void setResponseStr(const std::string &response);
	void initErrorMap();
	std::string getContentType(const std::string& filepath);
	std::string postResponse(Client *client);
	std::string deleteResponse(Client *client);
	std::string errorResponse405(Client *client, std::set<std::string> &methods);
	std::string getFileType(const std::string &filepath);
	std::string redirect301Response(Client *client, std::string url);
	std::string responseBodyTooLong(Client *client);
	bool checkFileName(std::string filename);
	CGI cgi;
	bool isDirectory(const std::string& filename);
	std::string Autoindex(const std::string& filename, Location &loc);
	std::string generateSessionId();
};

#endif
