#include <string>
#include <vector>
#include "ConfigServer.hpp"
#include <iostream>
#include <sstream>
#include <sys/stat.h>

static size_t ft_stoi(std::string str);
static std::string ft_to_string(int num);
static int is_file_exist_posix(const std::string& filename);

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
