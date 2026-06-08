#include "ConfigServer.hpp"
#include <iostream>

ConfigServer::ConfigServer()
{
	this->_port = 0;
}

ConfigServer::~ConfigServer() { }

void ConfigServer::printServerConfig() const {
    std::cout << "========= SERVER CONFIG =========" << std::endl;
    // ※ ゲッターの名前（getPort等）は、ご自身のConfigServerの関数名に合わせて調整してください
    std::cout << "  Port:        " << this->getPort() << std::endl;
    std::cout << "  Server Name: " << this->getServerName() << std::endl;
    std::cout << "  Root:        " << this->getRoot() << std::endl;

    std::cout << "=================================" << std::endl;

    // 配下の Location ブロックをすべてループで回して出力
    const std::vector<Location>& locs = this->getLocation(); // Locationのvectorを返すゲッター
    std::cout << "  [Locations Count: " << locs.size() << "]" << std::endl;

    for (size_t i = 0; i < locs.size(); ++i) {
        const Location& loc = locs[i];
        std::cout << "  -------------------------------" << std::endl;
        std::cout << "  Location [" << i << "]" << std::endl;
        std::cout << "    Root:           " << loc.getRoot() << std::endl;
        std::cout << "    Autoindex:      " << (loc.getAutoindex() ? "on" : "off") << std::endl;
        std::cout << "    Max Body Size:  " << loc.getClientMaxBodySize() << std::endl;
        std::cout << "    Upload Store:   " << loc.getUploadStore() << std::endl;

        // return (リダイレクト) のデバッグ
        std::pair<int, std::string> ret = loc.getReturn();
        std::cout << "    Return:         Code: " << ret.first << ", URL: " << ret.second << std::endl;

        // index_files_vec (vector) の中身を全出力
        std::cout << "    Index Files:    [ ";
        const std::vector<std::string>& idxs = loc.getIndexFilesVec();
        for (size_t j = 0; j < idxs.size(); ++j) {
            std::cout << idxs[j] << " ";
        }
        std::cout << "]" << std::endl;

        // limit_except (set) の中身を全出力
        std::cout << "    Allowed Methods:[ ";
        const std::set<std::string>& methods = loc.getLimitExcept();
        for (std::set<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
            std::cout << *it << " "; // C++98互換のイテレータ回し
        }
        std::cout << "]" << std::endl;

        // cgi_handlers (map) の中身を全出力
        std::cout << "    CGI Handlers:   " << std::endl;
        const std::map<std::string, std::string>& cgis = loc.getCgiHandlersMap();
        for (std::map<std::string, std::string>::const_iterator it = cgis.begin(); it != cgis.end(); ++it) {
            std::cout << "      " << it->first << " -> " << it->second << std::endl;
        }
    }
    std::cout << "=================================\n" << std::endl;
}

void	ConfigServer::setPort(uint16_t portNum) {
  this->_port = portNum;
}

void	ConfigServer::setServerName(std::string serverName)
{
	this->_server_name = serverName;
}

void	ConfigServer::setRoot(std::string root)
{
	this->_root = root;
}

const uint16_t	&ConfigServer::getPort() const {
  return this->_port;
}

const std::string	&ConfigServer::getServerName() const {
  return this->_server_name;
}

const std::string	&ConfigServer::getRoot() const {
  return this->_root;
}

void ConfigServer::addLocation(const Location& loc) {
    this->_locations.push_back(loc);
}

const std::vector<Location> &ConfigServer::getLocation() const {
	return this->_locations;
}

void ConfigServer::addErrorPage(const short &code, const std::string &path) {
    this->_error_pages[code] = path;
}
