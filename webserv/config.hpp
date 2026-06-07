#include <string>
#include <vector>

class ConfigParser{
  private:
	std::vector<std::string>	_server_config;
	size_t						_nb_server;
  public:
    ConfigParser();
	~ConfigParser();
	size_t findStartServer(size_t start, std::string &content);
	size_t findEndServer(size_t start, std::string &content);
	void splitServers(std::string &content);

};
