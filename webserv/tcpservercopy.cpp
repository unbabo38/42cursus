#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "ConfigParser.hpp"
#include "ConfigServer.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Response.hpp"
#define PORT 8080
#define MAX_EVENTS 10



int main(int argc, char const *argv[])
{
	Server serv;

	std::map<int, Client> client;

	ConfigParser conf_parse;
	ConfigServer conf_serv;
	Response response;
  	std::ifstream istrm(argv[1], std::ios::binary);
	if (!istrm.is_open()) {
    	std::cout << "failed to open " << argv[1] << '\n';
	} else {
		double d;
		std::string line;
		std::stringstream stream_binding;
		stream_binding << istrm.rdbuf();
		std::string file = stream_binding.str();
		conf_parse.splitConfToServers(file, &conf_serv);
		std::vector<Location> loc;
		loc = conf_serv.getLocation();
	}
	int read_result = 0;
	std::cout << conf_parse.getServers().size() << std::endl;
	serv.createSocketAndEpoll(conf_serv);
	serv.run(conf_parse.getServers());

    return 0;
}
