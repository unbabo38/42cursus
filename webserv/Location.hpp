#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <sstream>
#include <map>
#include <set>
#include <ostream>
class ConfigServer;

class Location{

private:
	std::string						   _location_path;
    std::string                        _root;
    std::string				           _index;
    std::size_t                        _client_max_body_size;
    bool                               _autoindex;
    std::map<int, std::string>         _error_pages_map;
    std::map<std::string, std::string> _cgi_handlers_map;
    std::set<std::string>              _limit_except;
    std::string                        _upload_store;
    std::pair<int, std::string>        _return;
  public:
    Location();
	~Location();
	Location(const Location& other);
    Location& operator=(const Location& other);
	static void    parseLocation(const std::vector<std::string>& tokens, size_t start, size_t len, ConfigServer *confserv);

  public:
	const std::string&					getLocationPath() const;
    const std::string&                  getRoot() const;
    const std::string&			        getIndex() const;
    std::size_t                         getClientMaxBodySize() const; // 値渡し
    bool                                getAutoindex() const;         // 値渡し
    // const std::map<int, std::string>&   getErrorPagesMap() const;
    const std::map<std::string, std::string>& getCgiHandlersMap() const;
    const std::set<std::string>&        getLimitExcept() const;
    const std::string&                  getUploadStore() const;
    std::pair<int, std::string>         getReturn() const;            // 値渡し（std::pairは基本値渡しでOK）

	void setLocationPath(const std::string& location_path);
    void setRoot(const std::string& root);
	void setIndex(const std::string& index);
    void setClientMaxBodySize(std::size_t size);
    void setAutoindex(bool autoindex);
    void setErrorPagesMap(const std::map<int, std::string>& error_pages);
    void setCgiHandlersMap(const std::map<std::string, std::string>& cgi_handlers);
    void setLimitExcept(const std::set<std::string>& limit_except);
    void setUploadStore(const std::string& upload_store);
    void setReturn(int code, const std::string& url);
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
