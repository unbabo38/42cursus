#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <iostream>
#include <map>
#include "Location.hpp"
struct Session {
    int visitCount;
    std::string username;
};
class ConfigServer{
  private:
	uint16_t 						_port;
	std::string						_server_name;
	std::string						_root;
	unsigned long					_client_max_body_size;
	std::string						_index;
	bool							_autoindex;
	std::map<short, std::string>	_error_pages;
	std::vector<Location> 			_locations;
    //struct sockaddr_in 				_server_address;
    int     						_listen_fd;
	Session session;
	std::map<std::string, Session> _sessions;
  public:
    ConfigServer();
	~ConfigServer();
	const uint16_t	&getPort() const;
	void	setPort(uint16_t portNum);
	const std::string &getServerName() const;
	void	setServerName(std::string serverName);
	const std::string &getRoot() const;
	void	setRoot(std::string root);
	void addLocation(const Location& loc);
	void addErrorPage(const short &code, const std::string &path);

	const std::vector<Location>  &getLocation() const;
	void printServerConfig() const;
	Session &getSession(std::string sessionId);
};

// typedef struct {
//     size_t      	len;
//     std::string     data;
// ‌} wserv_str_t;

// typedef struct {
//     wserv_str_t   key;
//     wserv_str_t   value;
// ‌} wserv_keyval_t;


// typedef struct {
//     unsigned    	len:28;

//     unsigned    	valid:1;
//     unsigned    	no_cacheable:1;
//     unsigned    	not_found:1;
//     unsigned    	escape:1;

//     std::string     data;
// ‌} wserv_variable_value_t;


// typedef struct {
//     wserv_str_t                 url;
//     wserv_str_t                 host;
//     wserv_str_t                 port_text;
//     wserv_str_t                 uri;

//     in_port_t                 port;
//     in_port_t                 default_port;
//     int                       family;

//     unsigned                  listen:1;
//     unsigned                  uri_part:1;
//     unsigned                  no_resolve:1;
//     unsigned                  one_addr:1;  /* compatibility */

//     unsigned                  no_port:1;
//     unsigned                  wildcard:1;

//     socklen_t                 socklen;
//     u_char                    sockaddr[wserv_SOCKADDRLEN];

//     wserv_addr_t               *addrs;
//     wserv_uint_t                naddrs;

//     char                     *err;
// ‌} wserv_url_t;

#endif
